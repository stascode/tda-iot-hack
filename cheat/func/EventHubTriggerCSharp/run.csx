#r "Microsoft.ServiceBus"
#r "Newtonsoft.Json"

using System;
using System.IO;
using ProtoBuf;
using Microsoft.ServiceBus.Messaging;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;

[ProtoContract]
class EventMessage 
{
    [ProtoMember(1)]
    public uint Version {get; set;}

    [ProtoMember(2)]
    public double WindSpeed {get; set;}
}

public static void Run(EventData[] eventData, ICollector<string> outEvents, TraceWriter log)
{
    foreach (var ed in eventData)
    {
        using (var ms = new MemoryStream(ed.GetBytes()))
        {
            try 
            {
                EventMessage message = ProtoBuf.Serializer.Deserialize<EventMessage>(ms);
                log.Info($"Message received. WindSpeed: '{message.WindSpeed}'");

                string deviceId = ed.SystemProperties["iothub-connection-device-id"].ToString();
                string messageId = ed.SystemProperties["message-id"].ToString();
                DateTime enqueuedTime = (DateTime)ed.SystemProperties["iothub-enqueuedtime"];

                JObject jo = new JObject();
                jo.Add("SpeedOfWind", message.WindSpeed);
                jo.Add("Version", message.Version);
                jo.Add("DeviceId", deviceId);
                jo.Add("MessageId", messageId);
                jo.Add("EnqueuedTime", enqueuedTime);

                string mout = jo.ToString();

                outEvents.Add(mout);
            }
            catch (Exception ex)
            { 
                log.Error($"Error processing message:: '{ex.Message}'");
            }
        }
    }
}
