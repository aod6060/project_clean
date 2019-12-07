#include "sys.h"


static InputState keys[KB_SIZE];

void input_init() {
	for (uint32_t i = 0; i < KB_SIZE; i++) {
		keys[i] = IS_RELEASE;
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

		switch (e.type) {
		case SDL_KEYDOWN:
			_doKeyboardDown(e.key);
			break;
		case SDL_KEYUP:
			_doKeyboardUp(e.key);
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
}

bool input_isKeyRelease(const Keyboard& key) {
	return keys[key] == IS_RELEASE;
}

bool input_isKeyDown(const Keyboard& key) {
	return keys[key] == IS_DOWN;
}

bool input_isKeyPressed(const Keyboard& key) {
	return keys[key] == IS_PRESSED;
}

bool input_isKeyUp(const Keyboard& key) {
	return keys[key] == IS_UP;
}

