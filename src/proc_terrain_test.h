#pragma once



#include "sys.h"


class ProcTerrainTestWindowCallback : public IWindowCallback {
private:
	ProcTerrain procTerrain;
	RenderPassManager renderPassManager;
	HUBRenderPass hubRenderPass;
	QuadHUBGeometry quad;

	Texture2D* current;

public:
	virtual void init();
	virtual void update(float delta);
	virtual void fixedUpdate();
	virtual void render();
	virtual void release();
};