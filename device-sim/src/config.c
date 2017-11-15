#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"

const SIM_CONFIG * g_simConfig;

static bool read_value_bool(const char *env, const bool def)
{
	const char *val = getenv(env);
	if (val && (0 == strcmp(val, "On") || 0 == strcmp(val, "on") || 0 == strcmp(val, "True") || 0 == strcmp(val, "true")))
		return true;
	return def;
}

static int read_value_int(const char *env, const int def)
{
	const char *val = getenv(env);
	if (val)
		return atoi(val);
	return def;
}

const char *HUB_SUFFIX_DEFAULT = "azure-devices.net";

SIM_CONFIG* SimLoadConfig() 
{
	SIM_CONFIG * simConfig = malloc(sizeof(SIM_CONFIG));
	memset(simConfig, 0, sizeof(SIM_CONFIG));

	simConfig->hubName = getenv("HUB_NAME");
	simConfig->hubSuffix = getenv("HUB_SUFFIX");
	if (NULL == simConfig->hubSuffix)
		simConfig->hubSuffix = HUB_SUFFIX_DEFAULT;

	if (!simConfig->hubName) {
		printf("HUB_NAME is not set\n");
		return NULL;
	}
	
	simConfig->deviceId = getenv("DEVICE_ID");
	simConfig->deviceKey = getenv("DEVICE_KEY");

	if (!simConfig->deviceId || !simConfig->deviceKey) {
        printf("DEVICE_ID or DEVICE_KEY is not set\n");
        return NULL;
	}
	
	simConfig->logLevel = read_value_int("LOG_LEVEL", 3);
	simConfig->mqttKeepAlive = read_value_int("MQTT_KEEP_ALIVE", 0);

	simConfig->deviceTrace = read_value_bool("DEVICE_TRACE", true);
	simConfig->D2C_PacketInterval = read_value_int("D2C_PACKET_INTERVAL", 5);

	size_t sz1 = strlen(simConfig->hubName);
	size_t sz = sz1 + strlen(simConfig->hubSuffix) + 2;
	char *hubHostname = malloc(sz);
	strcpy(hubHostname, simConfig->hubName);
	hubHostname[sz1] = '.';
	strcpy(hubHostname + sz1 + 1, simConfig->hubSuffix);
	simConfig->hubHostname = hubHostname;

	printf(
		"Load config:\n hubHostname: %s\n deviceId: %s\n", 
		simConfig->hubHostname, 
		simConfig->deviceId
	);
	
	return simConfig;
}

// Environment variables
// # Sample file for configuration of a load agent
// HubName <name of IoT Hub>
// HubSuffix azure-devices.net

// MQTT_KeepAlive 0

// # logging levels: 0 - err, 1 - inf, 2 - verbose
// LogLevel 2
// DeviceTrace On

// # Scenario to simulate
// Scenario D2C
// # Time window for establishing all connections
// D2C_ConnectionsWindow 5
// # Interval for each device (in seconds) to send message
// D2C_PacketInterval 5
// # Delta diff for the interval (in seconds)
// D2C_PacketIntervalDiff 5
// # Reset connection after each number of packets (0 to not reset the connection)
// D2C_ResetConnectionAfter 0
