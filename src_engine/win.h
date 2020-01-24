#pragma once

// win.h
// Window
class IWindowCallback {
public:
	virtual void init() = 0;
	virtual void update(float delta) = 0;
	virtual void fixedUpdate() = 0;
	virtual void render() = 0;
	virtual void release() = 0;
};

void win_register_event(std::function<void(SDL_Event&)> eventCB);

void win_init(IWindowCallback* appCB);
void win_update();
void win_release();

void win_exit();

IWindowCallback* win_getCallback();

template<typename T>
T* win_getCallbackTemp() {
	return (T*)win_getCallback();
}