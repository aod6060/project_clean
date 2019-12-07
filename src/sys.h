#pragma once



// Standard Libraries
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>

// 3rd Party
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <GL/glew.h>
#include <json/json.h>
#include <btBulletDynamicsCommon.h>

// My Stuff

// Configuration
void conf_init();
uint32_t conf_getWidth();
uint32_t conf_getHeight();
bool conf_isFullscreen();

// Window