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
	if (input_isInputMappingRelease(conf_getInputMapping("left-key"))) {
		std::cout << "left-key" << std::endl;
	}

	if (input_isInputMappingRelease(conf_getInputMapping("left-button"))) {
		std::cout << "left-button" << std::endl;
	}
	*/

	/*
	if (input_isInputMappingDown(conf_getInputMapping("left-key"))) {
		std::cout << "left-key" << std::endl;
	}
	
	if (input_isInputMappingDown(conf_getInputMapping("left-button"))) {
		std::cout << "left-button" << std::endl;
	}
	*/

	/*
	if (input_isInputMappingPress(conf_getInputMapping("left-key"))) {
		std::cout << "left-key" << std::endl;
	}

	if (input_isInputMappingPress(conf_getInputMapping("left-button"))) {
		std::cout << "left-button" << std::endl;
	}
	*/

	/*
	if (input_isInputMappingUp(conf_getInputMapping("left-key"))) {
		std::cout << "left-key" << std::endl;
	}

	if (input_isInputMappingUp(conf_getInputMapping("left-button"))) {
		std::cout << "left-button" << std::endl;
	}
	*/

	/*
	if (input_isIMFromConfRelease("left-key")) {
		std::cout << "left-key" << std::endl;
	}

	if (input_isIMFromConfRelease("left-button")) {
		std::cout << "left-button" << std::endl;
	}
	*/

	/*
	if (input_isIMFromConfDown("left-key")) {
		std::cout << "left-key" << std::endl;
	}

	if (input_isIMFromConfDown("left-button")) {
		std::cout << "left-button" << std::endl;
	}
	*/

	/*
	if (input_isIMFromConfPress("left-key")) {
		std::cout << "left-key" << std::endl;
	}

	if (input_isIMFromConfPress("left-button")) {
		std::cout << "left-button" << std::endl;
	}
	*/

	/*
	if (input_isIMFromConfUp("left-key")) {
		std::cout << "left-key" << std::endl;
	}

	if (input_isIMFromConfUp("left-button")) {
		std::cout << "left-button" << std::endl;
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

