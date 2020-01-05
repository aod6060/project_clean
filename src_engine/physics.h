#pragma once


struct PhysicsManager {
	btBroadphaseInterface* broadphase = nullptr;
	btCollisionDispatcher* disp = nullptr;
	btConstraintSolver* solver = nullptr;
	btDefaultCollisionConfiguration* collisionConf = nullptr;
	btDiscreteDynamicsWorld* world = nullptr;

	void init();

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