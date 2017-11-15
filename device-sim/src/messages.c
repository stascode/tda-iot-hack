#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "messages.h"
#include "pb.h"
#include "pb_encode.h"
#include "contoso.pb.h"

// TODO Place some base value for the wind speed
static const double windSpeedBase = ...;

size_t DeviceMessage_GenerateTelemetryMessage(uint32_t *randSeed, uint8_t *buffer, size_t bufferSize) 
{
	// TODO Generate a random but realistic delta using rand_r(randSeed), that is make the wind oscilate within a range of from 0 to +/-1 mph around the base value.  
	double delta = ...

	double avgWindSpeed = windSpeedBase + delta;

	// Serialize it to Protobuf
	pb_ostream_t stream = pb_ostream_from_buffer(buffer, bufferSize);

	EventMessage message = EventMessage_init_zero;
	message.WindSpeed = avgWindSpeed;

	bool status = pb_encode(&stream, EventMessage_fields, &message);
	if (!status) {
		printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
		exit(-1);
	}

	return stream.bytes_written;
}
