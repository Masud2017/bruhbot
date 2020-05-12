/******************************************************/
/* includes */
/******************************************************/
#include "log.h"
#include <stdio.h>

/******************************************************/
/* functions */
/******************************************************/

/******************************************************/
void log_printf(const char *format, ...)
{
	va_list argptr;
	va_start(argptr, format);
	vprintf(format, argptr);
	va_end(argptr);
}

/******************************************************/
void log_print(const char *file, int line,
	       const char *zone, const char *format, ...)
{
	const char bold_on[] = { 0x1B, '[', '1', 'm', '\0' };
	//   const char thin_on[] = { 0x1B, '[', '2', 'm' , '\0'};
	const char attrib_off[] = { 0x1B, '[', '0', 'm', '\0' };

	char buffer[256];
	snprintf(buffer, 256, "%s[%s] %s:%u%s", bold_on, zone, file, line,
		 attrib_off);

	printf("%-38s %s%s: ", buffer, bold_on, attrib_off);
	va_list argptr;
	va_start(argptr, format);
	vprintf(format, argptr);
	va_end(argptr);

	printf("\n");
}
