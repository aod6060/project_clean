#pragma once



#include "sys.h"


class ProcTerrainTestWindowCallback : public IWindowCallback {
private:

public:
	virtual void init();
	virtual void update(float delta);
	virtual void fixedUpdate();
	virtual void render();
	virtual void release();
};