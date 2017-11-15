#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SIM_CONFIG_TAG {
	const char * hubHostname;
	const char * hubName;
	const char * hubSuffix;

	const char * deviceId;
	const char * deviceKey;

	int logLevel;

	int mqttKeepAlive;

	const char * scenario;
	bool deviceTrace;

	int D2C_PacketInterval;
} SIM_CONFIG;

SIM_CONFIG* SimLoadConfig();

extern const SIM_CONFIG * g_simConfig;

#ifdef __cplusplus
}
#endif

#endif // COMMON_H