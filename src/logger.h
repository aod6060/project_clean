#pragma once

// logger.h
enum LoggerType {
	LT_CONSOLE = 0,
	LT_FILE,
	LT_BOTH,
	LT_SIZE
};

// Logger Interface

void logger_init();
void logger_release();

void logger_output(const char* format, ...);