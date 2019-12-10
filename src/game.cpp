#include "game.h"


void GameWindowCallback::init() {
	glEnable(GL_DEPTH_TEST);
}

void GameWindowCallback::update(float delta) {
	//std::cout << delta << std::endl;

	if (input_isKeyDown(Keyboard::KB_ESCAPE)) {
		win_exit();
	}
}

void GameWindowCallback::fixedUpdate() {
}

void GameWindowCallback::render() {
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


}

void GameWindowCallback::release() {
}

