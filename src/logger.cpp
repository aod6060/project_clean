#include "sys.h"


static FILE* fp = nullptr;

void logger_init() {
	if (conf_getLoggerType() == LT_FILE || conf_getLoggerType() == LT_BOTH) {
		fp = fopen("output.txt", "w");
	}
}

void logger_release() {
	if (conf_getLoggerType() == LT_FILE || conf_getLoggerType() == LT_BOTH) {
		fclose(fp);
	}
}

void logger_output(const char* format, ...) {
	va_list args;
	va_start(args, format);

	if (
		conf_getLoggerType() == LT_CONSOLE || 
		conf_getLoggerType() == LT_BOTH) {
		vprintf(format, args);
	}

	if (
		conf_getLoggerType() == LT_FILE || 
		conf_getLoggerType() == LT_BOTH) {
		vfprintf(fp, format, args);
	}

	va_end(args);
}