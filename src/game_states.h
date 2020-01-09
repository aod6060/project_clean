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

struct GameState;

struct LevelManager {
	PhysicsManager* phyManager = nullptr;
	GameState* state;

	ProcTerrainGeometry terrain;
	btHeightfieldTerrainShape* shape;
	btRigidBody* body = nullptr;

	void init(GameState* state);

	void render();

	void release();
};

struct CratesManager {
	PhysicsManager* phyManager = nullptr;
	GameState* state = nullptr;

	SceneGeometry crappyCrate;

	btCollisionShape* shape = nullptr;

	std::vector<btRigidBody*> bodies;

	void init(GameState* state);

	void render();

	void release();

};

struct GameState : public AbstractState {
	PhysicsManager phyManager;
	btCollisionShape* shape;
	btRigidBody* body;

	//Camera camera;
	PhysicsCamera camera;

	RenderPassManager renderPassManager;
	RenderPass mainRenderPass;

	QuadHUBGeometry hubGeom;
	RenderPass hubRenderPass;

	bool isWire = false;

	//SceneGeometry multiMeshTest;
	//ProcTerrainGeometry terrain;

	LevelManager levelManager;
	CratesManager cratesManager;

	float waterAnim = 0.0f;
	WaterGeometry waterGeom;

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
