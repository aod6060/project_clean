#include "game_states.h"
#include "menu_manager.h"
#include <chrono>
#include <random>

// Game Camera
void GameCamera::init(
	glm::vec3 pos,
	glm::vec2 rot,
	glm::vec3 zoom
) {
	this->zoom = zoom;
	Camera::init(
		pos,
		rot,
		conf_getFOV(),
		(float)conf_getWidth() / (float)conf_getHeight(),
		1.0f,
		1024.0f);
}

void GameCamera::update(float delta) {}

glm::mat4 GameCamera::getView() {
	return
		glm::translate(glm::mat4(1.0f), -this->zoom) *
		glm::rotate(glm::mat4(1.0f), glm::radians(rot.x), glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::rotate(glm::mat4(1.0f), glm::radians(rot.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::translate(glm::mat4(1.0f), -this->pos);
}


// Level Manager
void LevelManager::init(GameState* state) {
	this->state = state;
	this->phyManager = &this->state->phyManager;

	terrain.init();

	terrain.setBlackChannel(TextureManager::getTex("terrain:dirt1"));
	terrain.setRedChannel(TextureManager::getTex("terrain:sand1"));
	terrain.setGreenChannel(TextureManager::getTex("terrain:grass1"));
	terrain.setBlueChannel(TextureManager::getTex("terrain:dirt2"));

	this->terrainShape = this->phyManager->createHeightfiledCollisionShape(
		terrain.data
	);

	float offset = terrain.data.heightScale * 0.5f;
	btVector3 v(0.0f, offset, 0.0f);

	this->terrainBody = this->phyManager->createRigidBody(0, btTransform(btQuaternion(0, 0, 0, 1), v), this->terrainShape);
	this->terrainData.id = PT_TERRAIN;
	this->terrainBody->setUserPointer(&this->terrainData);

	this->waterGeom.init();

	float y = terrain.data.beachLevel * terrain.data.heightScale - 2.0f;

	this->waterShape = phyManager->createStaticPlaneShape(btVector3(0, 1, 0), 0.0f);
	this->waterBody = phyManager->createRigidBody(0.0f, btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, y, 0)), waterShape);
	this->waterData.id = PT_WATER;
	this->waterBody->setUserPointer(&this->waterData);
	
}

void LevelManager::render() {
	glm::mat4 model;

	float m[16];

	this->terrainBody->getWorldTransform().getOpenGLMatrix(m);

	model = glm::make_mat4(m);

	// Terrain
	ShaderManager::terrainShader.bind();
	ShaderManager::terrainShader.setCamera(&this->state->camera);
	ShaderManager::terrainShader.setTexScale(128.0f);
	ShaderManager::terrainShader.setModel(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f)));
	terrain.render(&ShaderManager::terrainShader);
	ShaderManager::terrainShader.unbind();

	// Water
	ShaderManager::waterShader.bind();
	ShaderManager::waterShader.setCamera(&this->state->camera);

	float y = terrain.data.beachLevel * terrain.data.heightScale;
	ShaderManager::waterShader.setModel(
		glm::translate(glm::mat4(1.0f), glm::vec3(0, y, 0)) *
		glm::scale(glm::mat4(1.0f), glm::vec3(1024.0f, 0.0f, 1024.0f))
	);
	ShaderManager::waterShader.setTexScale(64.0f);
	ShaderManager::waterShader.setTimeDelta(this->waterAnim);

	TextureManager::getTex("water:water1")->bind();
	waterGeom.render(&ShaderManager::waterShader);
	TextureManager::getTex("water:water1")->unbind();

	ShaderManager::waterShader.unbind();
}

void LevelManager::update(float delta) {

	this->waterAnim += delta * 0.001f;

	if (this->waterAnim >= 1.0f) {
		this->waterAnim -= 1.0f;
	}
}

void LevelManager::release() {
	waterBody->setUserPointer(nullptr);
	phyManager->removeRigidBody(this->waterBody);
	delete waterShape;
	waterGeom.release();
	this->terrainBody->setUserPointer(nullptr);
	phyManager->removeRigidBody(this->terrainBody);
	delete terrainShape;
	terrain.release();
	phyManager = nullptr;
	state = nullptr;
}

// Player Manager
void PlayerManager::init(GameState* state) {
	this->state = state;
	this->phyManager = &this->state->phyManager;

	this->mesh.setFilePath("data/meshes/objects/temp_player/temp_player.json");
	this->mesh.init();

	float x = (rand() % 512) - 256.0f;
	float z = (rand() % 512) - 256.0f;


	this->shape = this->phyManager->createCapsuleShape(1.0f, 2.0f);
	
	btVector3 pos = btVector3(
		x,
		state->levelManager.terrain.data.getY(x, z) + 2.0f,
		z);

	this->body = this->phyManager->createRigidBody(
		1.0f,
		btTransform(btQuaternion(0, 0, 0, 1), pos),
		this->shape);

	this->body->setSleepingThresholds(0, 0);
	this->body->setAngularFactor(0);
	
	data.id = PT_PLAYER;
	
	this->body->setUserPointer(&data);

	yrot = (float)(rand() % 360);

	this->speed = 1024.0f;

}

void PlayerManager::update(float delta) {
	// Do Nothing for now...
	if (input_getGrab()) {
		int x, y;
		input_getMousePos(x, y);

		glm::vec2 rot = state->camera.rot;
		float rotSpeed = 128;

		float threshold = 0.0006f;

		float d = ((delta > threshold) ? delta : threshold);

		rot.x += rotSpeed * y * d;
		rot.y += rotSpeed * x * d;

		if (rot.y <= -360.0f) {
			rot.y += 360.0f;
		}

		if (rot.y >= 360.0f) {
			rot.y -= 360.0f;
		}

		if (rot.x < 0.0f) {
			rot.x = 0.0f;
		}

		if (rot.x > 90.0f) {
			rot.x = 90.0f;
		}


		float zoom = state->camera.zoom.z;

		int cx, cy;

		input_getMouseWheelCoord(cx, cy);

		zoom += -(cy * 256.0f * d);

		if (zoom < 4) {
			zoom = 4;
		}

		if (zoom > 32) {
			zoom = 32;
		}

		state->camera.zoom.z = zoom;
		glm::vec3 p = this->getPos();
		p.y -= 2.0f;
		state->camera.pos = p;
		state->camera.rot = rot;

		float yrad = glm::radians(rot.y);

		float speed = this->speed;

		btVector3 vel = body->getLinearVelocity();

		vel[0] = 0;
		vel[2] = 0;

		if (input_isIMFromConfDown("move-jump")) {
			vel[1] = 10.0f;
		}

		if (input_isIMFromConfPress("move-run")) {
			speed *= 2.0f;
		}

		float modif = 16.0f;
		if (input_isIMFromConfPress("move-forward")) {
			this->yrot = rot.y;
			vel[0] += btSin(yrad) * speed * FF60;
			vel[2] -= btCos(yrad) * speed * FF60;
		}

		if (input_isIMFromConfPress("move-backward")) {
			this->yrot = rot.y;
			vel[0] -= btSin(yrad) * speed * FF60;
			vel[2] += btCos(yrad) * speed * FF60;
		}

		if (input_isIMFromConfPress("strafe-left")) {
			this->yrot = rot.y;
			vel[0] -= btCos(yrad) * speed * FF60;
			vel[2] -= btSin(yrad) * speed * FF60;
		}

		if (input_isIMFromConfPress("strafe-right")) {
			this->yrot = rot.y;
			vel[0] += btCos(yrad) * speed * FF60;
			vel[2] += btSin(yrad) * speed * FF60;
		}

		body->setLinearVelocity(vel);
		
	}
}

void PlayerManager::fixedUpdate() {
}

void PlayerManager::render() {

	ShaderManager::sceneShader.bind();

	ShaderManager::sceneShader.setCamera(&this->state->camera);

	//float m[16];

	glm::vec3 p = this->getPos();

	p.y -= 2.0f;

	/*
	glm::vec3 p = glm::vec3(
		this->position.x,
		this->position.y - 2.0f,
		this->position.z);
	*/
	mesh.setModel(
		glm::translate(glm::mat4(1.0f), p) *
		glm::rotate(glm::mat4(1.0f), glm::radians(-this->yrot), glm::vec3(0, 1, 0))
	);

	TextureManager::getTex("obj:temp_player")->bind();
	mesh.render(&ShaderManager::sceneShader);
	TextureManager::getTex("obj:temp_player")->unbind();

	ShaderManager::sceneShader.unbind();

}

void PlayerManager::release() {

	this->phyManager->removeRigidBody(this->body);
	delete this->shape;
	this->mesh.release();
	this->phyManager = nullptr;
	this->state = nullptr;
}

glm::vec3 PlayerManager::getPos() {
	btVector3 v = body->getCenterOfMassPosition();
	return glm::vec3(
		v.x(),
		v.y(),
		v.z()
	);
}

// collectable manager

void add_v(std::vector<CollectableType>& c, CollectableType type, uint32_t max) {
	for (int i = 0; i < max; i++) {
		c.push_back(type);
	}
}

#define NUM_COLLECTABLES 64

void CollectableManager::init(GameState* state) {
	this->state = state;
	this->phyManager = &state->phyManager;

	this->buildTable();

	this->collectableMesh.setFilePath("data/meshes/collectable.json");
	this->collectableMesh.init();

	this->shape = this->phyManager->createBoxShape(btVector3(1.0f, 1.0f, 1.0f));

	// Create Texture
	this->textures.push_back("col:norm_stick");
	this->textures.push_back("col:norm_trashbag");
	this->textures.push_back("col:norm_tv");
	this->textures.push_back("col:norm_frigorator");

	this->textures.push_back("col:silver_stick");
	this->textures.push_back("col:silver_trashbag");
	this->textures.push_back("col:silver_tv");
	this->textures.push_back("col:silver_frigorator");

	this->textures.push_back("col:gold_stick");
	this->textures.push_back("col:gold_trashbag");
	this->textures.push_back("col:gold_tv");
	this->textures.push_back("col:gold_frigorator");

	this->textures.push_back("col:suprise_crate");

	// Create Functions
	this->callbacks.push_back([&]() {return 1; });
	this->callbacks.push_back([&]() {return 2; });
	this->callbacks.push_back([&]() {return 4; });
	this->callbacks.push_back([&]() {return 8; });

	this->callbacks.push_back([&]() {return 16; });
	this->callbacks.push_back([&]() {return 32; });
	this->callbacks.push_back([&]() {return 64; });
	this->callbacks.push_back([&]() {return 128; });

	this->callbacks.push_back([&]() {return 256; });
	this->callbacks.push_back([&]() {return 512; });
	this->callbacks.push_back([&]() {return 1024; });
	this->callbacks.push_back([&]() {return 2048; });

	this->callbacks.push_back([&]() {return (rand() % 2 == 0) ? 4096 : -4096; });

	// Create All Objects
	for (int i = 0; i < NUM_COLLECTABLES; i++) {

		float x = (rand() % 512) - 256.0f;
		float z = (rand() % 512) - 256.0f;

		float y = state->levelManager.terrain.data.heightScale +  32.0f + (rand() % 64);

		CollectableType type = this->getTypeFromTable();

		btVector3 v = btVector3(x, y, z);

		btQuaternion q = btQuaternion(0, 0, 0, 1);

		btTransform t = btTransform(q, v);

		CollectableObject colObj;

		colObj.type = type;
		colObj.body = phyManager->createRigidBody(1.0f, t, this->shape);
		colObj.manager = this;

		this->colObjs.push_back(colObj);
	}

	for (int i = 0; i < this->colObjs.size(); i++) {
		colObjs[i].data.data = &colObjs[i];
		colObjs[i].data.id = PT_COLLECTABLE;
		colObjs[i].body->setUserPointer(&colObjs[i].data);
	}

	logger_output("Score: %d\n", this->score);
}

void CollectableManager::render() {
	ShaderManager::sceneShader.bind();

	ShaderManager::sceneShader.setCamera(&state->camera);

	for (int i = 0; i < this->colObjs.size(); i++) {
		CollectableType type = this->colObjs[i].type;
		TextureManager::getTex(this->textures[type])->bind();

		float m[16];

		this->colObjs[i].body->getCenterOfMassTransform().getOpenGLMatrix(m);

		glm::mat4 model = glm::make_mat4(m);

		this->collectableMesh.setModel(model);
		this->collectableMesh.render(&ShaderManager::sceneShader);

		TextureManager::getTex(this->textures[type])->unbind();
	}
	ShaderManager::sceneShader.unbind();
}

void CollectableManager::update(float delta) {
	// Update Timer for colObjects
	for (int i = 0; i < this->colObjs.size(); i++) {
		if (colObjs[i].playerCollide) {
			if (colObjs[i].time <= 0.0f) {
				colObjs[i].playerCollide = false;
				colObjs[i].time = 0.0f;
			}
			else {
				colObjs[i].time -= delta;
			}
		}
	}

	// Check for collisions
	int numManifolds = phyManager->getWorld()->getDispatcher()->getNumManifolds();

	float explosion = 64.0f;

	for (int i = 0; i < numManifolds; i++) {
		btPersistentManifold* contactManifold = phyManager->getWorld()->getDispatcher()->getManifoldByIndexInternal(i);

		btRigidBody* body0 = (btRigidBody*)(contactManifold->getBody0());
		btRigidBody* body1 = (btRigidBody*)(contactManifold->getBody1());

		if (body0 != nullptr && body1 != nullptr) {
			PhysicsData* data0 = (PhysicsData*)body0->getUserPointer();
			PhysicsData* data1 = (PhysicsData*)body1->getUserPointer();

			if (data0 != nullptr && data1 != nullptr) {

				if (data0->id == PT_PLAYER) {
					if (data1->id == PT_COLLECTABLE) {
						
						CollectableObject* obj = (CollectableObject*)(data1->data);

						if (!obj->playerCollide) {
							CollectableType type = obj->type;

							float x = (rand() % 512) - 256.0f;
							float z = (rand() % 512) - 256.0f;

							float y = state->levelManager.terrain.data.heightScale + 32.0f + (rand() % 64);

							int score = this->callbacks[type]();

							this->score += score;

							if (type == CT_SUPRISE_CRATE) {
								if(score < 0) {
									/*
									// Send player flying upwards lol
									btVector3 v = body0->getLinearVelocity();

									v[1] = explosion;

									body0->setLinearVelocity(v);
									*/

									btVector3 pos0 = body0->getCenterOfMassPosition();
									btVector3 pos1 = body1->getCenterOfMassPosition();


									btVector3 p = pos0 - pos1;

									p = p.normalize();

									body0->setLinearVelocity(p * explosion);
								}
							}
							logger_output("Score: %d\n", this->score);

							btVector3 v = btVector3(x, y, z);
							btQuaternion q = btQuaternion(0, 0, 0, 1);

							btTransform tran = btTransform(q, v);

							type = this->getTypeFromTable();

							obj->body->setCenterOfMassTransform(tran);
							obj->body->activate(true);
							obj->type = type;

							obj->playerCollide = true;
							obj->time = obj->maxTime;

						}
					}
				}

				if (data1->id == PT_PLAYER) {
					// Player it safe
					if (data0->id == PT_COLLECTABLE) {
						CollectableObject* obj = (CollectableObject*)(data0->data);

						if (!obj->playerCollide) {
							CollectableType type = obj->type;

							float x = (rand() % 512) - 256.0f;
							float z = (rand() % 512) - 256.0f;

							float y = state->levelManager.terrain.data.heightScale + 32.0f + (rand() % 64);
							
							int score = this->callbacks[type]();
							
							this->score += score;

							if (type == CT_SUPRISE_CRATE) {
								if (score < 0) {
									// Send player flying upwards lol
									/*
									btVector3 v = body1->getLinearVelocity();

									v[1] = explosion;

									body1->setLinearVelocity(v);
									*/

									btVector3 pos0 = body1->getCenterOfMassPosition();
									btVector3 pos1 = body0->getCenterOfMassPosition();


									btVector3 p = pos0 - pos1;

									p = p.normalize();

									body1->setLinearVelocity(p * explosion);

								}
							}

							logger_output("Score: %d\n", this->score);

							btVector3 v = btVector3(x, y, z);
							btQuaternion q = btQuaternion(0, 0, 0, 1);

							btTransform tran = btTransform(q, v);

							type = this->getTypeFromTable();

							obj->body->setCenterOfMassTransform(tran);
							obj->body->activate(true);
							obj->type = type;

							obj->playerCollide = true;
							obj->time = obj->maxTime;
						}
					}
				}
			}
		}

	}
}

void CollectableManager::release() {
	for (int i = 0; i < this->colObjs.size(); i++) {
		colObjs[i].data.data = nullptr;
		colObjs[i].body->setUserPointer(nullptr);
		this->phyManager->removeRigidBody(colObjs[i].body);
	}
	colObjs.clear();
	this->callbacks.clear();
	this->textures.clear();
	delete this->shape;
	this->collectableMesh.release();
	this->phyManager = nullptr;
	this->state = nullptr;
}

void CollectableManager::buildTable() {
	// Common Collectables

	// Stick ~ 1 (500 / 1000)
	add_v(table, CollectableType::CT_NORM_STICK, 500);
	// Trash Bag ~ 5 (250 / 1000)
	add_v(table, CollectableType::CT_NORM_TRASHBAG, 250);
	// TV ~ 10 (125 / 1000)
	add_v(table, CollectableType::CT_NORM_TV, 125);
	// Refrigerator~ 20 (60 / 1000)
	add_v(table, CollectableType::CT_NORM_FRIGORATOR, 60);
	// Uncommon Collectables

	// Silver Stick ~ 40 (30 / 1000)
	add_v(table, CollectableType::CT_SILVER_STICK, 30);
	// Silver Trash Bag  ~ 80 (10 / 1000)
	add_v(table, CollectableType::CT_SILVER_TRASHBAG, 10);
	// Silver TV ~ 160 (8 / 1000)
	add_v(table, CollectableType::CT_SILVER_TV, 8);
	// Silver Refrigerator ~ 320 (5 / 1000)
	add_v(table, CollectableType::CT_SILVER_FRIGORATOR, 5);
	// Rare Collectables

	// Gold Stick ~ 640 (4 / 1000)
	add_v(table, CollectableType::CT_GOLD_STICK, 4);
	// Gold Trash Bag  ~ 1280 (3 / 1000)
	add_v(table, CollectableType::CT_GOLD_TRASHBAG, 3);
	// Gold TV ~ 2560 (2 / 1000)
	add_v(table, CollectableType::CT_GOLD_TV, 2);
	// Gold Refrigerator ~ 5120 (2 / 1000)
	add_v(table, CollectableType::CT_GOLD_FRIGORATOR, 2);
	// Legendary Collectable

	// Surprise Crate(r: -10240 -> 10240) (1 / 1000) Be careful with this one because it will be a 50 / 50 chance of winning the game or loosing...
	add_v(table, CollectableType::CT_SUPRISE_CRATE, 1);

	uint32_t seed = std::chrono::system_clock::now().time_since_epoch().count();

	std::shuffle(table.begin(), table.end(), std::default_random_engine(seed));
}

CollectableType CollectableManager::getTypeFromTable() {
	return this->table[rand() % this->table.size()];
	//return CT_SUPRISE_CRATE;
}

void ScoreBoard::init() {
	this->player1Score = 0;
}

void ScoreBoard::render() {
	// Player 1 Score
	FontRender::setColor(glm::vec3(0.0f, 0.0f, 1.0f));
	FontRender::print(0.0f, 0.0f, "Player 1: %d", this->player1Score);
}

void ScoreBoard::release() {
	this->player1Score = 0;
}

void ScoreBoard::addPlayer1Score(int score) {
	this->player1Score += score;
}



// Game State
void GameState::init() {
	this->phyManager.init();

	this->mainRenderPass.setCallback([&]() {
		glm::vec3 cc = glm::vec3(135.0f, 206.0f, 235.0f) / 255.0f;

		RenderSystem::enable(GL_DEPTH_TEST);
		RenderSystem::viewport(0, 0, conf_getWidth(), conf_getHeight());
		RenderSystem::clearColor(cc.r, cc.g, cc.b, 1.0f);
		RenderSystem::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Terrain Rendering
		levelManager.render();

		playerManager.render();

		collectableManager.render();

		RenderSystem::disable(GL_DEPTH_TEST);
	});

	this->hubRenderPass.setCallback([&]() {
		RenderSystem::disable(GL_DEPTH_TEST);
		//MenuManager::render();
		ShaderManager::hubShader.bind();

		ShaderManager::hubShader.setProjection(glm::ortho(0.0f, (float)conf_getWidth(), (float)conf_getHeight(), 0.0f));
		ShaderManager::hubShader.setView(glm::mat4(1.0f));

		ShaderManager::hubShader.unbind();

		FontRender::setColor(glm::vec3(0.0f, 0.0f, 1.0f));
		FontRender::print(0.0f, 0.0f, "Score: %d", collectableManager.score);
		
		MenuManager::gameContextMenu.render();
	});

	this->renderPassMan.addRenderPass(&this->mainRenderPass);
	this->renderPassMan.addRenderPass(&this->hubRenderPass);

	// Initialize Managers
	levelManager.init(this);
	playerManager.init(this);
	collectableManager.init(this);

	MenuManager::gameContextMenu.setCallback([&](UIButtonComponent* comp) {
		MenuManager::gameContextMenu.setShow(false);
		input_setGrab(true);
	});

	camera.init(
		playerManager.getPos(),
		glm::vec2(0.0f),
		glm::vec3(0.0f, 1.0f, 8.0f)
	);

	input_setGrab(true);
}

void GameState::update(float delta) {
	if (!MenuManager::isShow()) {
		if (input_isIMFromConfDown("escape")) {
			MenuManager::gameContextMenu.setShow(true);
			input_setGrab(false);
		}

		// Update Stuff
		phyManager.update(delta, 10);

		camera.update(delta);

		levelManager.update(delta);

		playerManager.update(delta);

		collectableManager.update(delta);
	}
	else {
		if (input_isIMFromConfDown("escape")) {
			MenuManager::gameContextMenu.setShow(false);
			input_setGrab(true);
		}
	}
	MenuManager::gameContextMenu.update(delta);
}

void GameState::fixedUpdate() {
}

void GameState::render() {
	this->renderPassMan.render();
}

void GameState::release() {
	collectableManager.release();
	playerManager.release();
	levelManager.release();
	renderPassMan.release();
	phyManager.release();
}
