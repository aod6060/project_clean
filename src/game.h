#pragma once


#include "sys.h"


class GameWindowCallback : public IWindowCallback {
private:

	InputMapping leftKey;
	InputMapping leftMB;
public:
	virtual void init();
	virtual void update(float delta);
	virtual void fixedUpdate();
	virtual void render();
	virtual void release();
};