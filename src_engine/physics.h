#pragma once

struct PhysicsData {
	int id;
	void* data;
};

struct PhysicsManager {
	btBroadphaseInterface* broadphase = nullptr;
	btCollisionDispatcher* disp = nullptr;
	btConstraintSolver* solver = nullptr;
	btDefaultCollisionConfiguration* collisionConf = nullptr;
	btDiscreteDynamicsWorld* world = nullptr;

	void init();

	void update(float delta, int subpasses);

	void release();

	btDiscreteDynamicsWorld* getWorld();

	void stepSimulation();

	btBoxShape* createBoxShape(const btVector3& halfExtends);

	btSphereShape* createSphereShape(const btScalar& radius);

	btStaticPlaneShape* createStaticPlaneShape(const btVector3& planeNormal, const btScalar planeConstant);

	btCapsuleShape* createCapsuleShape(const btScalar& radius, const btScalar& height);

	btHeightfieldTerrainShape* createHeightfiledCollisionShape(ProcTerrain& procTerrain);

	btRigidBody* createRigidBody(float mass, const btTransform& startTransform, btCollisionShape* shape);

	void removeRigidBody(btRigidBody* body);
};

struct PhysicsCamera : public Camera {
	PhysicsManager* phyManager;

	btRigidBody* body = nullptr;
	btCollisionShape* shape = nullptr;

	bool isRunning = false;
	bool isMoving = false;

	float jumpSpeed = 32.0f;

	void setJumpSpeed(float jumpSpeed);

	void setPhysicsManager(PhysicsManager* man);

	virtual void init(
		glm::vec3 pos,
		glm::vec2 rot,
		float fov,
		float aspect,
		float znear,
		float zfar,
		float rotSpeed = 64.0f,
		float walkingSpeed = 32.0f);

	virtual void update(float delta);

	void fixedUpdate();

	virtual glm::mat4 getView();

	void release();
};