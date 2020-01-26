#include "sys.h"


struct WaitExecute {
	float time = 0.0f;
	float max = 0.5f;

	WaitExecute() { time = max; }

	void call(float delta, std::function<void()> func) {
		if (time >= max) {
			reset();
			func();
		}
		else {
			time += delta;
		}
	}

	void reset() {
		time = 0.0f;
	}
};
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

void PhysicsManager::update(float delta, int subpasses) {
	this->getWorld()->stepSimulation(delta, subpasses);
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
	this->getWorld()->stepSimulation(FF60, 10);
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



WaitExecute runningExecute;
WaitExecute walkingExecute;
WaitExecute jumpExecute;

// PhysicsCamera
void PhysicsCamera::init(
	glm::vec3 pos,
	glm::vec2 rot,
	float fov,
	float aspect,
	float znear,
	float zfar,
	float rotSpeed,
	float walkingSpeed) {

	this->shape = phyManager->createCapsuleShape(1.0f, 2.0f);

	btVector3 p = btVector3(pos.x, pos.y, pos.z);

	btTransform transform = btTransform(btQuaternion(0, 0, 0, 1), p);

	this->body = phyManager->createRigidBody(1.0f, transform, this->shape);
	this->body->setSleepingThresholds(0.0f, 0.0f);
	this->body->setAngularFactor(0.0f);

	this->rot = rot;

	this->fov = fov;
	this->apsect = aspect;
	this->znear = znear;
	this->zfar = zfar;

	this->rotSpeed = rotSpeed;
	this->walkingSpeed = walkingSpeed;
	
	runningExecute.time = runningExecute.max = SoundManager::getSound("running")->getTimeInSeconds();
	walkingExecute.time = walkingExecute.max = SoundManager::getSound("walking")->getTimeInSeconds();
	jumpExecute.time = jumpExecute.max = SoundManager::getSound("jumping")->getTimeInSeconds();

}


void PhysicsCamera::update(float delta) {
	if (input_getGrab()) {

		this->pos.x = body->getCenterOfMassPosition().x();
		this->pos.y = body->getCenterOfMassPosition().y();
		this->pos.z = body->getCenterOfMassPosition().z();

		int x, y;

		input_getMousePos(x, y);

		rot.x += this->rotSpeed * y * ((delta - 0.001f < 0) ? 0.001f : delta);
		rot.y += this->rotSpeed * x * ((delta - 0.001f < 0) ? 0.001f : delta);

		if (rot.y <= -360.0f) {
			rot.y += 360.0f;
		}

		if (rot.y >= 360.0f) {
			rot.y -= 360.0f;
		}

		if (rot.x < -90.0f) {
			rot.x = -90.0f;
		}

		if (rot.x > 90.0f) {
			rot.x = 90.0f;
		}

		float yrad = glm::radians(rot.y);

		float sp = this->walkingSpeed;

		if (input_isIMFromConfPress("move-run")) {
			sp *= 3.0f;

			this->isRunning = true;
		}

		btVector3 vel = body->getLinearVelocity();

		vel[0] = 0;
		vel[2] = 0;

		if (input_isIMFromConfDown("move-jump")) {
			vel[1] = jumpSpeed;
			jumpExecute.call(delta, [&]() {
				SoundManager::getSound("jumping")->play();
				jumpExecute.time = jumpExecute.max;
			});
		}

		if (input_isIMFromConfPress("move-forward")) {
			//pos.x += sp * glm::sin(yrad) * delta;
			//pos.z -= sp * glm::cos(yrad) * delta;
			vel[0] += sp * btSin(yrad) * FF60;
			vel[2] -= sp * btCos(yrad) * FF60;
			isMoving = true;
		}

		if (input_isIMFromConfPress("move-backward")) {
			//pos.x -= sp * glm::sin(yrad) * delta;
			//pos.z += sp * glm::cos(yrad) * delta;
			vel[0] -= sp * btSin(yrad) * FF60;
			vel[2] += sp * btCos(yrad) * FF60;
			isMoving = true;
		}

		if (input_isIMFromConfPress("strafe-left")) {
			//pos.x -= sp * glm::cos(yrad) * delta;
			//pos.z -= sp * glm::sin(yrad) * delta;
			vel[0] -= sp * btCos(yrad) * FF60;
			vel[2] -= sp * btSin(yrad) * FF60;
			isMoving = true;
		}

		if (input_isIMFromConfPress("strafe-right")) {
			//pos.x += sp * glm::cos(yrad) * delta;
			//pos.z += sp * glm::sin(yrad) * delta;
			vel[0] += sp * btCos(yrad) * FF60;
			vel[2] += sp * btSin(yrad) * FF60;
			isMoving = true;
		}

		body->setLinearVelocity(vel);


		if (this->isMoving) {
			if (this->isRunning) {
				
				runningExecute.call(delta, [&]() {
					SoundManager::getSound("running")->play();
				});
			}
			else {
				walkingExecute.call(delta, [&]() {
					SoundManager::getSound("walking")->play();
				});
				
			}
		}
		else {
			runningExecute.reset();
			walkingExecute.reset();
		}

		this->isMoving = false;
		this->isRunning = false;
	}
}


void PhysicsCamera::fixedUpdate() {}

glm::mat4 PhysicsCamera::getView() {
	btVector3 pos = this->body->getCenterOfMassPosition();

	glm::vec3 p = glm::vec3(
		pos.x(),
		pos.y() + 1.0f,
		pos.z()
	);

	return
		glm::rotate(glm::mat4(1.0f), glm::radians(this->rot.x), glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::rotate(glm::mat4(1.0f), glm::radians(this->rot.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::translate(glm::mat4(1.0f), -p);
}

void PhysicsCamera::release() {
	phyManager->removeRigidBody(this->body);
	delete this->shape;
	phyManager = nullptr;
}

void PhysicsCamera::setPhysicsManager(PhysicsManager* man) {
	this->phyManager = man;
}

void PhysicsCamera::setJumpSpeed(float jumpSpeed) {
	this->jumpSpeed = jumpSpeed;
}
