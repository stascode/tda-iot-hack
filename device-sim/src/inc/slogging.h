#ifndef SLOGGING_H
#define SLOGGING_H

#include "azure_c_shared_utility/xlogging.h"

#define LOG_LEVEL_ERROR 0
#define LOG_LEVEL_INFO  1
#define LOG_LEVEL_TRACE 2
#define LogTrace(FORMAT, ...) do{LOG(AZ_LOG_TRACE, LOG_LINE, FORMAT, ##__VA_ARGS__); }while((void)0,0)

void SLogging_Init();

#endif // SLOGGING_H