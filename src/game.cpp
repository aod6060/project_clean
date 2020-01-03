#include "game.h"



void GameStateWindowCallback::init() {

	// Adding gamewindow states 
	this->logo1State.setStateWindowCallback(this);
	this->logo2State.setStateWindowCallback(this);
	this->startMenuState.setStateWindowCallback(this);
	this->gameState.setStateWindowCallback(this);

	this->addState("logo1", &this->logo1State);
	this->addState("logo2", &this->logo2State);
	this->addState("start-menu", &this->startMenuState);
	this->addState("game", &this->gameState);

	this->setDefaultState("logo1");

	StateWindowCallback::init();
}