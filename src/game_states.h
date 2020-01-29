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

enum PhysicsType {
	PT_PLAYER,
	PT_COLLECTABLE,
	PT_TERRAIN,
	PT_WATER,
	PT_MAX_SIZE
};

struct GameCamera : public Camera {
	glm::vec3 zoom;

	void init(
		glm::vec3 pos,
		glm::vec2 rot,
		glm::vec3 zoom
	);

	virtual void update(float delta);

	virtual glm::mat4 getView();

};

struct LevelManager {
	// Physics Manager
	PhysicsManager* phyManager = nullptr;
	GameState* state = nullptr;

	// Terrain
	ProcTerrainGeometry terrain;
	btHeightfieldTerrainShape* terrainShape;
	btRigidBody* terrainBody;
	PhysicsData terrainData;

	// Water
	float waterAnim = 0.0f;
	WaterGeometry waterGeom;
	btCollisionShape* waterShape;
	btRigidBody* waterBody;
	PhysicsData waterData;

	void init(GameState* state);
	void render();
	void update(float delta);
	void release();

};

struct PlayerManager {
	PhysicsManager* phyManager = nullptr;
	GameState* state = nullptr;
	PhysicsData data;

	SceneGeometry mesh;

	//glm::vec3 position;
	float yrot;

	float speed = 512.0f;

	btCollisionShape* shape = nullptr;
	btRigidBody* body = nullptr;

	glm::vec3 position;

	void init(GameState* state);

	void update(float delta);
	void fixedUpdate();
	void render();
	void release();

	glm::vec3 getPos();

};

enum CollectableType {
	CT_NORM_STICK = 0,
	CT_NORM_TRASHBAG,
	CT_NORM_TV,
	CT_NORM_FRIGORATOR,
	CT_SILVER_STICK,
	CT_SILVER_TRASHBAG,
	CT_SILVER_TV,
	CT_SILVER_FRIGORATOR,
	CT_GOLD_STICK,
	CT_GOLD_TRASHBAG,
	CT_GOLD_TV,
	CT_GOLD_FRIGORATOR,
	CT_SUPRISE_CRATE,
	CT_MAX_SIZE
};

struct CollectableManager;

struct CollectableObject {
	CollectableManager* manager;
	CollectableType type;
	btRigidBody* body;
	PhysicsData data;
	bool playerCollide = false;
	float time = 0.0f;
	float maxTime = 1.0f;
};

struct CollectableManager {
	GameState* state;
	PhysicsManager* phyManager;

	SceneGeometry collectableMesh;
	btCollisionShape* shape;

	std::vector<std::string> textures;
	std::vector<std::function<int()>> callbacks;
	
	int score = 0;

	// Table of values...
	std::vector<CollectableType> table;

	std::vector<CollectableObject> colObjs;

	void init(GameState* state);

	void render();

	void update(float delta);

	void release();

	void buildTable();

	CollectableType getTypeFromTable();


};

struct GameState : public AbstractState {
	RenderPassManager renderPassMan;
	RenderPass mainRenderPass;
	RenderPass hubRenderPass;

	PhysicsManager phyManager;

	//Camera testCamera;
	GameCamera camera;

	LevelManager levelManager;
	PlayerManager playerManager;
	CollectableManager collectableManager;

	virtual void init();
	virtual void update(float delta);
	virtual void fixedUpdate();
	virtual void render();
	virtual void release();
};


struct ClassModeGameState : public GameState {};