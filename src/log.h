#pragma once

/******************************************************/
/* includes */
/******************************************************/
#include <stdarg.h>
#include <string.h>

/******************************************************/
/* defines */
/******************************************************/
#define __FILENAME__                                                           \
        (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define LINFO __FILENAME__, __LINE__,  " INFO"
#define LDEBUG __FILENAME__, __LINE__, "DEBUG"
#define LERROR __FILENAME__, __LINE__, "ERROR"

/******************************************************/
/* functions */
/******************************************************/
void log_printf(const char *format, ...);
void log_print(const char *file, int line,
	       const char *zone, const char *format, ...);

