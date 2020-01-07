#include "sys.h"

// Physics Manager
void PhysicsManager::init() {
	this->collisionConf = new btDefaultCollisionConfiguration();
	this->disp = new btCollisionDispatcher(this->collisionConf);
	this->broadphase = new btDbvtBroadphase();
	this->solver = new btSequentialImpulseConstraintSolver();
	this->world = new btDiscreteDynamicsWorld(
		disp,
		broadphase,
		solver,
		collisionConf);
	world->setGravity(btVector3(0, -10, 0));
}

void PhysicsManager::release() {
	delete this->world;
	delete this->solver;
	delete this->broadphase;
	delete this->disp;
	delete this->collisionConf;
}

btDiscreteDynamicsWorld* PhysicsManager::getWorld() {
	return this->world;
}

void PhysicsManager::stepSimulation() {
	this->getWorld()->stepSimulation(FF60);
}

btBoxShape* PhysicsManager::createBoxShape(const btVector3& halfExtends) {
	return new btBoxShape(halfExtends);
}

btSphereShape* PhysicsManager::createSphereShape(const btScalar& radius) {
	return new btSphereShape(radius);
}

btStaticPlaneShape* PhysicsManager::createStaticPlaneShape(const btVector3& planeNormal, const btScalar planeConstant) {
	return new btStaticPlaneShape(planeNormal, planeConstant);
}

btCapsuleShape* PhysicsManager::createCapsuleShape(const btScalar& radius, const btScalar& height) {
	return new btCapsuleShape(radius, height);
}

btHeightfieldTerrainShape* PhysicsManager::createHeightfiledCollisionShape(ProcTerrain& procTerrain) {
	return new btHeightfieldTerrainShape(
		procTerrain.size,
		procTerrain.size,
		procTerrain.maskedElevationScaled.data(),
		procTerrain.heightScale,
		0.0f,
		procTerrain.heightScale,
		1,
		PHY_FLOAT,
		false);
}

btRigidBody* PhysicsManager::createRigidBody(float mass, const btTransform& startTransform, btCollisionShape* shape) {
	bool isDynamic = (mass > 0.f);

	btVector3 localInertia(0, 0, 0);

	if (isDynamic) {
		shape->calculateLocalInertia(mass, localInertia);
	}

	btDefaultMotionState* ms = new btDefaultMotionState(
		startTransform
	);

	btRigidBody::btRigidBodyConstructionInfo info(mass, ms, shape, localInertia);

	btRigidBody* body = new btRigidBody(info);

	body->setUserIndex(-1);

	this->getWorld()->addRigidBody(body);

	return body;
}

void PhysicsManager::removeRigidBody(btRigidBody* body) {
	// Do nothing for now
	this->getWorld()->removeRigidBody(body);
	btMotionState* ms = body->getMotionState();
	delete body;
	delete ms;
}