#include "game.h"


void GameWindowCallback::init() {
}

void GameWindowCallback::update(float delta) {
	//std::cout << delta << std::endl;

	if (input_isKeyDown(Keyboard::KB_ESCAPE)) {
		win_exit();
	}

	logger_output("delta: %f\n", delta);
}

void GameWindowCallback::fixedUpdate() {
}

void GameWindowCallback::render() {

}

void GameWindowCallback::release() {
}

