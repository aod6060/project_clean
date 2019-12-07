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
std::string conf_getCaption();
uint32_t conf_getWidth();
uint32_t conf_getHeight();
bool conf_isFullscreen();

// Window
class IWindowCallback {
public:
	virtual void init() = 0;
	virtual void update(float delta) = 0;
	virtual void fixedUpdate() = 0;
	virtual void render() = 0;
	virtual void release() = 0;
};

void win_register_event(std::function<void(SDL_Event&)>& eventCB);

void win_init(IWindowCallback* appCB);
void win_update();
void win_release();

void win_exit();
