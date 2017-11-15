#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#include "azure_c_shared_utility/platform.h"

#include "sim_all.h"
#include "slogging.h"

int main(int argc, char **argv)
{
    g_simConfig = SimLoadConfig();

    if (!g_simConfig)
        return -1;

    SLogging_Init(NULL);

    if (platform_init() != 0) {
        LogError("Failed to initialize the platform.");
        return 1;
    }

    fflush(stdout);

    D2C_SimulateDevice();

    // sleep forever
    while (true) 
        sleep(10000);

    return 0;
}
