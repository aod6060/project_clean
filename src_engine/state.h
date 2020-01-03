#pragma once

struct IState;

struct StateWindowCallback : public IWindowCallback {
	std::string current;

	std::map<std::string, IState*> states;

	virtual void init();
	virtual void update(float delta);
	virtual void fixedUpdate();
	virtual void render();
	virtual void release();

	virtual void setDefaultState(std::string state);
	virtual void addState(std::string name, IState* state);
	virtual void changeState(std::string state);
};

struct IState {
	virtual void setStateWindowCallback(StateWindowCallback* callback) = 0;
	virtual void init() = 0;
	virtual void update(float delta) = 0;
	virtual void fixedUpdate() = 0;
	virtual void render() = 0;
	virtual void release() = 0;
};

struct AbstractState : public IState {
	StateWindowCallback* callback = nullptr;

	virtual void setStateWindowCallback(StateWindowCallback* callback);

};