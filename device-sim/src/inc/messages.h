#ifndef MESSAGES_H
#define MESSAGES_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

size_t DeviceMessage_GenerateTelemetryMessage(uint32_t *randSeed, uint8_t *buffer, size_t bufferSize);


#ifdef __cplusplus
}
#endif

#endif // MESSAGES_H