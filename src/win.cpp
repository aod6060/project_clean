#include "sys.h"

const static float g_fixed_fps = 1.0f / 60.0f;

static IWindowCallback* g_appCB = nullptr;
static std::vector <std::function<void(SDL_Event&)>> g_eventCBs;
static SDL_Window* g_window = nullptr;
static SDL_GLContext g_context = nullptr;
// Controlled Loop
static bool g_isRunning = true;
// Timing
static uint32_t g_preTime = 0;
static uint32_t g_currTime = 0;
static float g_delta = 0.0f;
static float g_fixedTime = 0.0f;

void win_init(IWindowCallback* appCB) {
	g_appCB = appCB;

	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	if (conf_isFullscreen()) {
		g_window = SDL_CreateWindow(
			conf_getCaption().c_str(),
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			conf_getWidth(),
			conf_getHeight(),
			SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN
		);
	}
	else {
		g_window = SDL_CreateWindow(
			conf_getCaption().c_str(),
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			conf_getWidth(),
			conf_getHeight(),
			SDL_WINDOW_OPENGL
		);
	}

	g_context = SDL_GL_CreateContext(g_window);

	glewInit();
	logger_init();
	input_init();
	ShaderManager::init();
	PreProcessorManager::init();

	if (g_appCB) {
		g_appCB->init();
	}
}

void win_update() {

	SDL_Event e;

	g_preTime = SDL_GetTicks();

	while (g_isRunning) {

		// Handle Timing
		g_currTime = SDL_GetTicks();
		g_delta = (g_currTime - g_preTime) / 1000.0f;
		g_fixedTime += g_delta;
		g_preTime = g_currTime;

		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				win_exit();
			}

			std::for_each(g_eventCBs.begin(), g_eventCBs.end(), [&](std::function<void(SDL_Event&)>& func) {
				func(e);
			});

		}

		if (g_appCB) {

			g_appCB->update(g_delta);

			if (g_fixedTime >= g_fixed_fps) {
				g_appCB->fixedUpdate();
			}

			g_appCB->render();
		}

		if (g_fixedTime >= g_fixed_fps) {
			g_fixedTime = 0.0f;
		}

		input_update();

		SDL_GL_SwapWindow(g_window);
	}
}

void win_release() {
	
	if (g_appCB) {
		g_appCB->release();
	}
	g_appCB = nullptr;

	g_eventCBs.clear();

	PreProcessorManager::release();
	ShaderManager::release();

	logger_release();

	SDL_GL_DeleteContext(g_context);
	SDL_DestroyWindow(g_window);
	SDL_Quit();
}

void win_register_event(std::function<void(SDL_Event&)> eventCB) {
	g_eventCBs.push_back(eventCB);
}

void win_exit() {
	g_isRunning = false;
}