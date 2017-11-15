#ifndef SIM_ALL_H
#define SIM_ALL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "iothub_client.h"
#include "iothub_message.h"
#include "iothubtransportmqtt.h"

#include "config.h"
#include "sched.h"

// Auxiliary functions
void IoTHubClient_Aux_SetTracing(IOTHUB_CLIENT_HANDLE clientHandle);
void IoTHubClient_Aux_LL_SetTracing(IOTHUB_CLIENT_LL_HANDLE clientHandle);

void D2C_SimulateDevice();

#ifdef __cplusplus
}
#endif

#endif // SIM_ALL_H