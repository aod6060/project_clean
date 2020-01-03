#pragma once

#include <sys.h>

struct Logo1State : public AbstractState {
	//Texture2D logo;

	RenderPassManager renderPassManager;
	RenderPass mainRenderPass;

	QuadHUBGeometry hubGeom;

	float time;
	float maxTime;

	virtual void init();
	virtual void update(float delta);
	virtual void fixedUpdate();
	virtual void render();
	virtual void release();
};

struct Logo2State : public AbstractState {
	//Texture2D logo;

	RenderPassManager renderPassManager;
	RenderPass mainRenderPass;

	QuadHUBGeometry hubGeom;

	float time;
	float maxTime;

	virtual void init();
	virtual void update(float delta);
	virtual void fixedUpdate();
	virtual void render();
	virtual void release();
};

struct StartMenuState : public AbstractState {
	//Texture2D startMenu;

	RenderPassManager renderPassManager;
	RenderPass mainRenderPass;

	QuadHUBGeometry hubGeom;

	UIManager manager;
	UIButtonComponent startButton;
	UIButtonComponent exitButton;

	float time;
	float maxTime;

	float uiTime;
	float uiMaxTime;

	void _uiInit();

	virtual void init();
	virtual void update(float delta);
	virtual void fixedUpdate();
	virtual void render();
	virtual void release();
};

struct GameState : public AbstractState {
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

	//Texture2D angry;
	// Terrain Stuff
	//Texture2D grass1;
	//Texture2D grass2;
	//Texture2D dirt1;
	//Texture2D dirt2;
	//Texture2D sand1;

	//Texture2D water;

	float yrot = 0.0f;

	// UI Section
	UIButtonComponent continueButton;
	UIButtonComponent exitButton;
	UIManager uiManager;

	void _initUI();

	virtual void init();
	virtual void update(float delta);
	virtual void fixedUpdate();
	virtual void render();
	virtual void release();
};
