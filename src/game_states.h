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
	RenderPassManager renderPassManager;
	RenderPass mainRenderPass;

	QuadHUBGeometry hubGeom;

	float time;
	float maxTime;

	float uiTime;
	float uiMaxTime;

	virtual void init();
	virtual void update(float delta);
	virtual void fixedUpdate();
	virtual void render();
	virtual void release();
};


struct GameState;

struct LevelManager {
	// Physics Manager
	PhysicsManager* phyManager = nullptr;
	GameState* state = nullptr;

	// Terrain
	ProcTerrainGeometry terrain;
	btHeightfieldTerrainShape* terrainShape;
	btRigidBody* terrainBody;

	// Water
	float waterAnim = 0.0f;
	WaterGeometry waterGeom;
	btCollisionShape* waterShape;
	btRigidBody* waterBody;

	void init(GameState* state);
	void render();
	void update(float delta);
	void release();

};

struct GameState : public AbstractState {
	RenderPassManager renderPassMan;
	RenderPass mainRenderPass;
	RenderPass hubRenderPass;

	PhysicsManager phyManager;

	Camera testCamera;

	LevelManager levelManager;


	virtual void init();
	virtual void update(float delta);
	virtual void fixedUpdate();
	virtual void render();
	virtual void release();
};


struct ClassModeGameState : public GameState {};

/*
enum PhysicObjectType {
	POT_TERRAIN = 0,
	POT_CRATE,
	POT_PLAYER
};

struct GameState;

struct LevelManager {
	PhysicsManager* phyManager = nullptr;
	GameState* state;

	ProcTerrainGeometry terrain;
	btHeightfieldTerrainShape* shape;
	btRigidBody* body = nullptr;
	PhysicsData data;

	void init(GameState* state);

	void render();

	void release();
};

struct CratesManager {
	PhysicsManager* phyManager = nullptr;
	GameState* state = nullptr;

	SceneGeometry crappyCrate;

	btCollisionShape* shape = nullptr;
	PhysicsData data;

	std::vector<btRigidBody*> bodies;
	//std::vector<PhysicsData> physicsData;

	void init(GameState* state);

	void render();

	void fixedUpdate();

	void release();

};
*/

/*
struct GameState : public AbstractState {
	PhysicsManager phyManager;
	btCollisionShape* shape;
	btRigidBody* body;

	//Camera camera;
	PhysicsCamera camera;
	PhysicsData cameraData;

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
	// Continue Button
	UIButtonComponent continueButton;

	// Master Sound Control
	UILabelComponent masterSliderLabel;
	UISliderComponent masterSlider;

	// Music
	UILabelComponent musicSliderLabel;
	UISliderComponent musicSlider;

	// Ambient
	UILabelComponent ambientSliderLabel;
	UISliderComponent ambientSlider;

	// Sound FX
	UILabelComponent soundfxSliderLabel;
	UISliderComponent soundfxSlider;

	// Exit Button
	UIButtonComponent exitButton;
	UIManager uiManager;

	void _initUI();

	virtual void init();
	virtual void update(float delta);
	virtual void fixedUpdate();
	virtual void render();
	virtual void release();
};
*/