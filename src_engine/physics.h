#pragma once



struct PhysicsObject {
	btRigidBody* body;
	int group;
	int mask;
};

struct PhysicsManager {
	btBroadphaseInterface* broadphase = nullptr;
	btCollisionDispatcher* disp = nullptr;
	btConstraintSolver* solver = nullptr;
	btDefaultCollisionConfiguration* collisionConf = nullptr;
	btDiscreteDynamicsWorld* world = nullptr;

	void init();

	void release();

	btDiscreteDynamicsWorld* getWorld();


};