#include "game.h"
#include "menu_manager.h"


void GameWindowCallback::init() {
	MenuManager::init();

	// Adding gamewindow states 
	this->logo1State.setStateWindowCallback(this);
	this->logo2State.setStateWindowCallback(this);
	this->startMenuState.setStateWindowCallback(this);
	//this->gameState.setStateWindowCallback(this);
	this->classModeGameState.setStateWindowCallback(this);

	this->addState("logo1", &this->logo1State);
	this->addState("logo2", &this->logo2State);
	this->addState("start-menu", &this->startMenuState);
	//this->addState("game", &this->gameState);
	this->addState("classic-mode", &this->classModeGameState);

	this->setDefaultState("logo1");

	StateWindowCallback::init();
}

void GameWindowCallback::release() {
	StateWindowCallback::release();
	MenuManager::release();
}