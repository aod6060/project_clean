#pragma once


#include "sys.h"


class GameWindowCallback : public IWindowCallback {
private:
	Camera camera;
	RenderPassManager renderPassManager;
	MainRenderPass mainRenderPass;

	bool isWire = false;

	SceneGeometry multiMeshTest;
	//StaticTerrainGeometry terrain;
	ProcTerrainGeometry terrain;

	Texture2D angry;
	// Terrain Stuff
	Texture2D grass1;
	Texture2D grass2;
	Texture2D dirt1;
	Texture2D dirt2;
	Texture2D sand1;

	float yrot = 0.0f;

public:
	virtual void init();
	virtual void update(float delta);
	virtual void fixedUpdate();
	virtual void render();
	virtual void release();
};