#pragma once


#include "sys.h"


class GameWindowCallback : public IWindowCallback {
private:
	Camera camera;
	RenderPassManager renderPassManager;
	MainRenderPass mainRenderPass;

	bool isWire = false;

	SceneGeometry multiMeshTest;

	Texture2D angry;

	float yrot = 0.0f;

public:
	virtual void init();
	virtual void update(float delta);
	virtual void fixedUpdate();
	virtual void render();
	virtual void release();
};