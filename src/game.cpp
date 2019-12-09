#include "game.h"


void GameWindowCallback::init() {
	std::cout << "Init" << std::endl;

	this->leftKey.isMouseButton = false;
	this->leftKey.key = Keyboard::KB_LEFT;

	this->leftMB.isMouseButton = true;
	this->leftMB.mb = MouseButtons::MBS_LEFT;
}

void GameWindowCallback::update(float delta) {
	//std::cout << delta << std::endl;

	if (input_isKeyDown(Keyboard::KB_ESCAPE)) {
		win_exit();
	}

	// Testing Input Mapping
	/*
	if (input_isInputMappingRelease(&this->leftKey)) {
		std::cout << "LEFT KEY" << std::endl;
	}

	if (input_isInputMappingRelease(&this->leftMB)) {
		std::cout << "LEFT MOUSE BUTTON" << std::endl;
	}
	*/

	/*
	if (input_isInputMappingDown(&this->leftKey)) {
		std::cout << "LEFT KEY" << std::endl;
	}

	if (input_isInputMappingDown(&this->leftMB)) {
		std::cout << "LEFT MOUSE BUTTON" << std::endl;
	}
	*/

	/*
	if (input_isInputMappingPress(&this->leftKey)) {
		std::cout << "LEFT KEY" << std::endl;
	}

	if (input_isInputMappingPress(&this->leftMB)) {
		std::cout << "LEFT MOUSE BUTTON" << std::endl;
	}
	*/

	/*
	if (input_isInputMappingUp(&this->leftKey)) {
		std::cout << "LEFT KEY" << std::endl;
	}

	if (input_isInputMappingUp(&this->leftMB)) {
		std::cout << "LEFT MOUSE BUTTON" << std::endl;
	}
	*/
}

void GameWindowCallback::fixedUpdate() {
}

void GameWindowCallback::render() {

}

void GameWindowCallback::release() {
	std::cout << "Release" << std::endl;
}

