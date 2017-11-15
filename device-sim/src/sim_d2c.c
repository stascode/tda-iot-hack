#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "azure_c_shared_utility/threadapi.h"

#include "sim_all.h"
#include "slogging.h"
#include "messages.h"


//
// Device connection helper context. Use this as a context to pass 
// in IoTHubClient_LL_SetConnectionStatusCallback and IoTHubClient_LL_SendEventAsync
//
typedef struct DEVICE_SIMULATE_CONTEXT_TAG {
    const char *deviceId; // Device Id passed from env variable
    const char *deviceKey; // Device key passed from env variable
    unsigned int randSeed; // use this to pass in rand_r function

    // Configuration of the device connection for C SDK
    IOTHUB_CLIENT_CONFIG config;

    // Client handle for C SDK
    IOTHUB_CLIENT_LL_HANDLE clientHandle;

    // Set this flag to true as you succesfully connect to the Hub
    bool connected; 
    // Set this flag to true when the callback function passed in IoTHubClient_LL_SetConnectionStatusCallback completes
    bool connectComplete; 

    // Handle for the message
    IOTHUB_MESSAGE_HANDLE message;
    // Set this flag to true when the callback function passed in IoTHubClient_LL_SendEventAsync completes
    bool sendComplete;
    int event;

} DEVICE_SIMULATE_CONTEXT;


//
// Callback for SendEventAsync
//
static void sendConfirmationCallback(
    IOTHUB_CLIENT_CONFIRMATION_RESULT result, 
    void* context)
{
    DEVICE_SIMULATE_CONTEXT *ctx = (DEVICE_SIMULATE_CONTEXT *)context;

    if (
        // TODO Check if result contains success or not and emit a log entry
        ...
    ) {
        LogError("FW: Device %s: confirmation callback error %s", ctx->deviceId,
            ENUM_TO_STRING(IOTHUB_CLIENT_CONFIRMATION_RESULT, result)
        );
    } 
    
    // TODO Write the code to properly destroy the message (you don't want memory leaks and set ctx->sendComplete to true)
    ...
}

//
// Schedules a task to send an event 
//
static bool sendDeviceEvent(DEVICE_SIMULATE_CONTEXT *ctx)
{
    uint8_t msgBuf[1024];
    char    msgId[64];
    char    corId[64];

    ctx->event++;

    IOTHUB_MESSAGE_RESULT mresult;

    sprintf_s(msgId, sizeof(msgId), "msg_%i", ctx->event);
    sprintf_s(corId, sizeof(corId), "cor_%i", ctx->event);

    size_t msgSize = DeviceMessage_GenerateTelemetryMessage(&ctx->randSeed, msgBuf, sizeof(msgBuf));


    IOTHUB_MESSAGE_HANDLE message = IoTHubMessage_CreateFromByteArray(msgBuf, msgSize);


    mresult = IoTHubMessage_SetMessageId(message, msgId);
    mresult = IoTHubMessage_SetCorrelationId(message, corId);

    ctx->message = message;
    ctx->sendComplete = false;

    LogTrace("FW: Device %s. Sending event %u", ctx->deviceId, ctx->event);

    // TODO Write the code to send the message to IoT Hub using a _LL layer function
    IOTHUB_CLIENT_RESULT result = ...

    // TODO Write the code till the sendConfirmationCallback callback completes, i.e. till ctx->sendComplete is set to true 
    ...

    return true;
}

//
// Callback for the device connection status
//
static void onDeviceConnectionStatus(
    IOTHUB_CLIENT_CONNECTION_STATUS result, 
    IOTHUB_CLIENT_CONNECTION_STATUS_REASON reason, 
    void* argctx) 
{
    DEVICE_SIMULATE_CONTEXT *ctx = (DEVICE_SIMULATE_CONTEXT *)argctx;

    LogTrace("FW: In onDeviceConnectionStatus for %s. Result: %u. Reason: %u.", ctx->deviceId, result, reason);

    // TODO check that result and reason conform to succesful result codes and set ctx->connected appropriately
    ...



    // TODO do not forget to set connectComplete as you exit this function
    ...
}

//
// Main function to connect to the device to device
//
static bool connectDevice(DEVICE_SIMULATE_CONTEXT *ctx)
{
    ctx->config.deviceId = ctx->deviceId;
    ctx->config.deviceKey = ctx->deviceKey;
    ctx->config.deviceSasToken = NULL;
    ctx->config.iotHubName = g_simConfig->hubName;
    ctx->config.iotHubSuffix = g_simConfig->hubSuffix;
    ctx->config.protocol = MQTT_Protocol;
    ctx->config.protocolGatewayHostName = NULL;

    ctx->connectComplete = false;
    ctx->connected = false;
    
    // TODO Write the code to establish connection to IoT Hub using _LL functions
    // TODO pass onDeviceConnectionStatus function as a callback for the connection status 
    ctx->clientHandle = ... ;
    
    ...

    IoTHubClient_LL_SetOption(ctx->clientHandle, "logtrace", &g_simConfig->deviceTrace);
    if (g_simConfig->mqttKeepAlive > 0) 
        IoTHubClient_LL_SetOption(ctx->clientHandle, "keepalive", &g_simConfig->mqttKeepAlive);

    // TODO Write the code to wait till the callback completes, that is till ctx->connectComplete becomes true
    ...
    
    if (!ctx->connected) {
        IoTHubClient_LL_Destroy(ctx->clientHandle);
        return false;
    }

    return true;
}

//
// Simulates Device to Cloud telemetry
// 
void D2C_SimulateDevice() 
{
    // needs to be allocated on the heap  
    // since the ConnectionStatusCallback is invoked during lifetime of the device
    DEVICE_SIMULATE_CONTEXT *ctx = malloc (sizeof(DEVICE_SIMULATE_CONTEXT));
    memset(ctx, 0, sizeof(DEVICE_SIMULATE_CONTEXT));

    ctx->deviceId = g_simConfig->deviceId;
    ctx->deviceKey = g_simConfig->deviceKey;

    // This helps in getting a good enough random seed between different devices
    const char *deviceKey = ctx->deviceKey;
    ctx->randSeed = time(NULL) * deviceKey[0] * deviceKey[1] * deviceKey[2] * deviceKey[3] * deviceKey[4];

    if (!connectDevice(ctx))
        return;

    LogTrace("FW: Device connected. Start sending messages");
    while (1) {
        sendDeviceEvent(ctx);

        // Sleep for 5 seconds
        int delay = 1000 * g_simConfig->D2C_PacketInterval;
        ThreadAPI_Sleep(delay);
    }
}