#pragma once

// config.h
// Configuration
struct InputMapping;

void conf_init();
std::string conf_getCaption();
uint32_t conf_getWidth();
uint32_t conf_getHeight();
bool conf_isFullscreen();

InputMapping* conf_getInputMapping(std::string name);

LoggerType conf_getLoggerType();

// Graphics Section
float conf_getFOV();