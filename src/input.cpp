#include "sys.h"


static InputState keys[KB_SIZE];

static InputState mouseButtons[MBS_SIZE];

static int g_mx = 0;
static int g_my = 0;

static int g_mouseWheelX = 0;
static int g_mouseWheelY = 0;

void input_init() {
	for (uint32_t i = 0; i < KB_SIZE; i++) {
		keys[i] = IS_RELEASE;
	}

	for (uint32_t i = 0; i < MBS_SIZE; i++) {
		mouseButtons[i] = IS_RELEASE;
	}

	// Setting Up Events

	
	win_register_event([&](SDL_Event& e) {

		std::function<void(SDL_KeyboardEvent& k)> _doKeyboardDown = [&](SDL_KeyboardEvent& k) {
			if (keys[k.keysym.scancode] == IS_RELEASE) {
				keys[k.keysym.scancode] = IS_DOWN;
			}
		};

		std::function<void(SDL_KeyboardEvent& k)> _doKeyboardUp = [&](SDL_KeyboardEvent& k) {
			if (keys[k.keysym.scancode] == IS_PRESSED) {
				keys[k.keysym.scancode] = IS_UP;
			}
		};

		std::function<void(SDL_MouseButtonEvent&)> _doMouseDown = [&](SDL_MouseButtonEvent& m) {
			if (mouseButtons[m.button - 1] == IS_RELEASE) {
				mouseButtons[m.button - 1] = IS_DOWN;
			}
		};

		std::function<void(SDL_MouseButtonEvent&)> _doMouseUp = [&](SDL_MouseButtonEvent& m) {
			if (mouseButtons[m.button - 1] == IS_PRESSED) {
				mouseButtons[m.button - 1] = IS_UP;
			}
		};

		std::function<void(SDL_MouseMotionEvent&)> _doMouseMotion = [&](SDL_MouseMotionEvent& m) {
			if (input_getGrab()) {
				g_mx = m.xrel;
				g_my = m.yrel;
			}
			else {
				g_mx = m.x;
				g_my = m.y;
			}
		};

		std::function<void(SDL_MouseWheelEvent&)> _doMouseWheel = [&](SDL_MouseWheelEvent& wheel) {
			g_mouseWheelX = wheel.x;
			g_mouseWheelY = wheel.y;
		};

		switch (e.type) {
		case SDL_KEYDOWN:
			_doKeyboardDown(e.key);
			break;
		case SDL_KEYUP:
			_doKeyboardUp(e.key);
			break;
		case SDL_MOUSEBUTTONDOWN:
			_doMouseDown(e.button);
			break;
		case SDL_MOUSEBUTTONUP:
			_doMouseUp(e.button);
			break;
		case SDL_MOUSEMOTION:
			_doMouseMotion(e.motion);
			break;
		case SDL_MOUSEWHEEL:
			_doMouseWheel(e.wheel);
			break;
		}
	});
}

void input_update() {

	for (uint32_t i = 0; i < KB_SIZE; i++) {
		if (keys[i] == IS_DOWN) {
			keys[i] = IS_PRESSED;
		}

		if (keys[i] == IS_UP) {
			keys[i] = IS_RELEASE;
		}
	}

	for (uint32_t i = 0; i < MBS_SIZE; i++) {
		if (mouseButtons[i] == IS_DOWN) {
			mouseButtons[i] = IS_PRESSED;
		}

		if (mouseButtons[i] == IS_UP) {
			mouseButtons[i] = IS_RELEASE;
		}
	}

	if (input_getGrab()) {
		g_mx = 0;
		g_my = 0;
	}

	g_mouseWheelX = 0;
	g_mouseWheelY = 0;
}

bool input_isKeyRelease(const Keyboard& key) {
	return keys[key] == IS_RELEASE;
}

bool input_isKeyDown(const Keyboard& key) {
	return keys[key] == IS_DOWN;
}

bool input_isKeyPress(const Keyboard& key) {
	return keys[key] == IS_PRESSED;
}

bool input_isKeyUp(const Keyboard& key) {
	return keys[key] == IS_UP;
}

bool input_isMouseButtonRelease(const MouseButtons& mb) {
	return mouseButtons[mb] == IS_RELEASE;
}

bool input_isMouseButtonDown(const MouseButtons& mb) {
	return mouseButtons[mb] == IS_DOWN;
}

bool input_isMouseButtonPress(const MouseButtons& mb) {
	return mouseButtons[mb] == IS_PRESSED;
}

bool input_isMouseButtonUp(const MouseButtons& mb) {
	return mouseButtons[mb] == IS_UP;
}

void input_getMousePos(int& mx, int& my) {
	mx = g_mx;
	my = g_my;
}

bool input_getGrab() {
	return SDL_GetRelativeMouseMode() == SDL_TRUE ? true : false;
}

void input_setGrab(bool grab) {
	SDL_SetRelativeMouseMode(grab ? SDL_TRUE : SDL_FALSE);
}

void input_toggleGrab() {
	input_setGrab(!input_getGrab());
}

void input_getMouseWheelCoord(int& x, int& y) {
	x = g_mouseWheelX;
	y = g_mouseWheelY;
}

bool input_isInputMappingRelease(InputMapping* mapping) {
	bool b = false;

	if (mapping->isMouseButton) {
		b = input_isMouseButtonRelease(mapping->mb);
	}
	else {
		b = input_isKeyRelease(mapping->key);
	}

	return b;
}

bool input_isInputMappingDown(InputMapping* mapping) {
	bool b = false;

	if (mapping->isMouseButton) {
		b = input_isMouseButtonDown(mapping->mb);
	}
	else {
		b = input_isKeyDown(mapping->key);
	}

	return b;
}

bool input_isInputMappingPress(InputMapping* mapping) {
	bool b = false;

	if (mapping->isMouseButton) {
		b = input_isMouseButtonPress(mapping->mb);
	}
	else {
		b = input_isKeyPress(mapping->key);
	}

	return b;
}

bool input_isInputMappingUp(InputMapping* mapping) {
	bool b = false;

	if (mapping->isMouseButton) {
		b = input_isMouseButtonUp(mapping->mb);
	}
	else {
		b = input_isKeyUp(mapping->key);
	}

	return b;
}

