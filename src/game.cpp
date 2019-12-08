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

	/*
	if (input_isKeyUp(Keyboard::KB_LEFT)) {
		std::cout << "KEYBOARD UP[LEFT]" << std::endl;
	}
	*/

	/*
	if (input_isMouseButtonRelease(MouseButtons::MBS_LEFT)) {
		std::cout << "MOUSE BUTTON RELEASE[LEFT]" << std::endl;
	}
	*/

	/*
	if (input_isMouseButtonDown(MouseButtons::MBS_LEFT)) {
		std::cout << "MOUSE BUTTON DOWN[LEFT]" << std::endl;
	}
	*/

	/*
	if (input_isMouseButtonPress(MouseButtons::MBS_LEFT)) {
		std::cout << "MOUSE BUTTON PRESS[LEFT]" << std::endl;
	}
	*/

	/*
	if (input_isMouseButtonUp(MouseButtons::MBS_LEFT)) {
		std::cout << "MOUSE BUTTON UP[LEFT]" << std::endl;
	}
	*/

	/*
	if (input_isKeyDown(Keyboard::KB_TAB)) {
		input_toggleGrab();
	}

	int x;
	int y;

	input_getMousePos(x, y);

	std::cout << x << ", " << y << std::endl;
	*/

	int x, y;

	input_getMouseWheelCoord(x, y);

	std::cout << x << ", " << y << std::endl;
}

void GameWindowCallback::fixedUpdate() {
	//std::cout << "Hello, from fixed update..." << std::endl;
}

void GameWindowCallback::render() {

}

void GameWindowCallback::release() {
	std::cout << "Release" << std::endl;
}

