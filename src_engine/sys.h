#pragma once


#define FF60 1.0f / 60.0f

// Standard Libraries
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <cstdarg>
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
#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>
#include <fmod/fmod.hpp>

// My Stuff
#include "logger.h"
#include "config.h"
#include "win.h"
#include "state.h"
#include "input.h"
#include "graphics.h"
#include "texture_manager.h"
#include "shader.h"
#include "geometry.h"
#include "render_pass.h"
#include "pre_process.h"
#include "font_render.h"
#include "ui.h"
#include "camera.h"
#include "proc_terrain.h"
#include "physics.h"
#include "sound.h"
