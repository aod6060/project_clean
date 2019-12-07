#include "game.h"


void GameWindowCallback::init() {
	std::cout << "Init" << std::endl;
}

void GameWindowCallback::update(float delta) {
	//std::cout << delta << std::endl;

	if (input_isKeyDown(Keyboard::KB_ESCAPE)) {
		win_exit();
	}

	/*
	if (input_isKeyRelease(Keyboard::KB_LEFT)) {
		std::cout << "KEYBOARD RELEASE [LEFT]" << std::endl;
	}
	*/

	/*
	if (input_isKeyDown(Keyboard::KB_LEFT)) {
		std::cout << "KEYBOARD DOWN [LEFT]" << std::endl;
	}
	*/

	/*
	if (input_isKeyPressed(Keyboard::KB_LEFT)) {
		std::cout << "KEYBOARD PRESSED [LEFT]" << std::endl;
	}
	*/

	
	if (input_isKeyUp(Keyboard::KB_LEFT)) {
		std::cout << "KEYBOARD UP[LEFT]" << std::endl;
	}
	
}

void GameWindowCallback::fixedUpdate() {
	//std::cout << "Hello, from fixed update..." << std::endl;
}

void GameWindowCallback::render() {

}

void GameWindowCallback::release() {
	std::cout << "Release" << std::endl;
}

