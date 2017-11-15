#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "messages.h"
#include "pb.h"
#include "pb_encode.h"
#include "contoso.pb.h"

static const double windSpeedBase = 10.0;

size_t DeviceMessage_GenerateTelemetryMessage(uint32_t *randSeed, uint8_t *buffer, size_t bufferSize) 
{
	double delta = (double)(rand_r(randSeed) % 10 + 1) / 10;
	if (rand_r(randSeed) % 2)
		delta *= -1;

	double avgWindSpeed = windSpeedBase + delta;

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
