#include "game.h"


void GameWindowCallback::init() {
	std::cout << "Init" << std::endl;
}

void GameWindowCallback::update(float delta) {
	//std::cout << delta << std::endl;
}

void GameWindowCallback::fixedUpdate() {
	std::cout << "Hello, from fixed update..." << std::endl;
}

void GameWindowCallback::render() {

}

void GameWindowCallback::release() {
	std::cout << "Release" << std::endl;
}

