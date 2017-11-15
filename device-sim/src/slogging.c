#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <pthread.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>

#include "azure_c_shared_utility/xlogging.h"

#include "slogging.h"
#include "config.h"

static void defLogSink(LOG_CATEGORY log_category, const char* file, const char* func, int line, unsigned int options, const char* format, ...);

void SLogging_Init()
{
	// hook up xlogging from the iothub_client sdk
	xlogging_set_log_function(defLogSink);
}

void defLogSink(
	LOG_CATEGORY log_category,
	const char* file, const char* func, int line, 
	unsigned int options,const char* format, 
	...) 
{
    time_t t;
    va_list args;
    va_start(args, format);

    t = time(NULL); 

	char buf[2048];
	char *sbuf = buf;

    switch (log_category)
    {
    case AZ_LOG_INFO:
		if (g_simConfig->logLevel < LOG_LEVEL_INFO) {
			va_end(args);
			return;
		}
		sprintf(sbuf, "INF: ");
        break;
    case AZ_LOG_ERROR:
		{
		char * ct = ctime(&t);
		sprintf(sbuf, "ERR: Time:%.24s File:%s Func:%s Line:%d\n", ct, file, func, line);
        break;
		}
    default:
		if (g_simConfig->logLevel < LOG_LEVEL_TRACE) {
			va_end(args);
			return;
		}
		sprintf(sbuf, "TRC: ");
        break;
    }

	sbuf = buf + strlen(buf);

	vsprintf(sbuf, format, args);
    va_end(args);

	sbuf = buf + strlen(buf);

    if (options & LOG_LINE)
		sprintf(sbuf, "\r\n");

	printf("%s", buf);
	fflush(stdout);
}
