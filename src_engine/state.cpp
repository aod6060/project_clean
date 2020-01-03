#include "sys.h"


// StateWindowCallback

void StateWindowCallback::init() {
	this->states[this->current]->init();
}

void StateWindowCallback::update(float delta) {
	this->states[this->current]->update(delta);
}

void StateWindowCallback::fixedUpdate() {
	this->states[this->current]->fixedUpdate();
}

void StateWindowCallback::render() {
	this->states[this->current]->render();
}

void StateWindowCallback::release() {
	this->states[this->current]->release();
}

void StateWindowCallback::setDefaultState(std::string state) {
	this->current = state;
}

void StateWindowCallback::addState(std::string name, IState* state) {
	this->states[name] = state;
}

void StateWindowCallback::changeState(std::string state) {
	this->states[this->current]->release();
	this->current = state;
	this->states[this->current]->init();
}

// AbstractState
void AbstractState::setStateWindowCallback(StateWindowCallback* callback) {
	this->callback = callback;
}