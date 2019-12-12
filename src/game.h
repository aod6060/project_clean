#pragma once


#include "sys.h"


class GameWindowCallback : public IWindowCallback {
private:
	RenderPassManager renderPassManager;
	MainRenderPass mainRenderPass;
	Texture2D angry;
	QuadTestShaderGeomentry quad;

	float yrot = 0.0f;

public:
	virtual void init();
	virtual void update(float delta);
	virtual void fixedUpdate();
	virtual void render();
	virtual void release();
};