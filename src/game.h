#pragma once


#include <sys.h>



class GameWindowCallback : public IWindowCallback {
private:
	Camera camera;
	RenderPassManager renderPassManager;
	RenderPass mainRenderPass;

	QuadHUBGeometry hubGeom;
	RenderPass hubRenderPass;

	bool isWire = false;

	SceneGeometry multiMeshTest;
	//StaticTerrainGeometry terrain;
	ProcTerrainGeometry terrain;

	float waterAnim = 0.0f;
	WaterGeometry waterGeom;

	Texture2D angry;
	// Terrain Stuff
	Texture2D grass1;
	Texture2D grass2;
	Texture2D dirt1;
	Texture2D dirt2;
	Texture2D sand1;

	Texture2D water;

	float yrot = 0.0f;

	// UI Section
	UIButtonComponent continueButton;
	UIButtonComponent exitButton;
	UIManager uiManager;

	void _initUI();

public:
	virtual void init();
	virtual void update(float delta);
	virtual void fixedUpdate();
	virtual void render();
	virtual void release();
};