#include "game_states.h"
#include "menu_manager.h"

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

	this->waterGeom.init();

	float y = terrain.data.beachLevel * terrain.data.heightScale - 2.0f;

	this->waterShape = phyManager->createStaticPlaneShape(btVector3(0, 1, 0), 0.0f);
	this->waterBody = phyManager->createRigidBody(0.0f, btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, y, 0)), waterShape);
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
	phyManager->removeRigidBody(this->waterBody);
	delete waterShape;
	waterGeom.release();
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

	/*
	this->position = glm::vec3(
		x,
		state->levelManager.terrain.data.getY(x, z),
		z
	);
	*/

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

		/*
		if (input_isIMFromConfDown("move-jump")) {
			this->moveJump = true;
		}

		this->moveRunning = input_isIMFromConfPress("move-run");
		this->moveForward = input_isIMFromConfPress("move-forward");
		this->moveBackward = input_isIMFromConfPress("move-backward");
		this->strafeLeft = input_isIMFromConfPress("strafe-left");
		this->strafeRight = input_isIMFromConfPress("strafe-right");

		if (this->moveForward || this->moveBackward || this->strafeLeft || this->strafeRight) {
			this->yrot = rot.y;
		}

		*/

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
			vel[0] += btSin(yrad) * speed * d * modif;
			vel[2] -= btCos(yrad) * speed * d * modif;
		}

		if (input_isIMFromConfPress("move-backward")) {
			this->yrot = rot.y;
			vel[0] -= btSin(yrad) * speed * d * modif;
			vel[2] += btCos(yrad) * speed * d * modif;
		}

		if (input_isIMFromConfPress("strafe-left")) {
			this->yrot = rot.y;
			vel[0] -= btCos(yrad) * speed * d * modif;
			vel[2] -= btSin(yrad) * speed * d * modif;
		}

		if (input_isIMFromConfPress("strafe-right")) {
			this->yrot = rot.y;
			vel[0] += btCos(yrad) * speed * d * modif;
			vel[2] += btSin(yrad) * speed * d * modif;
		}

		body->setLinearVelocity(vel);
		
	}
}

void PlayerManager::fixedUpdate() {
	// Do Nothing for now...

	/*
	if (input_getGrab()) {
		float yrad = glm::radians(this->yrot);

		float speed = this->speed;


		btVector3 vel = body->getLinearVelocity();

		vel[0] = 0;
		vel[2] = 0;

		if (this->moveJump) {
			vel[1] = 10.0f;
			this->moveJump = false;
		}

		if (this->moveRunning) {
			speed *= 4.0f;
		}

		if (this->moveForward) {
			vel[0] += btSin(yrad) * speed;
			vel[2] -= btCos(yrad) * speed;
		}

		if (this->moveBackward) {
			vel[0] -= btSin(yrad) * speed;
			vel[2] += btCos(yrad) * speed;
		}

		if (this->strafeLeft) {
			vel[0] -= btCos(yrad) * speed;
			vel[2] -= btSin(yrad) * speed;
		}

		if (this->strafeRight) {
			vel[0] += btCos(yrad) * speed;
			vel[2] += btSin(yrad) * speed;
		}

		body->setLinearVelocity(vel);

		this->position = this->getPos();
	}
	*/
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

		RenderSystem::disable(GL_DEPTH_TEST);
	});

	this->hubRenderPass.setCallback([&]() {
		RenderSystem::disable(GL_DEPTH_TEST);
		//MenuManager::render();
		ShaderManager::hubShader.bind();

		ShaderManager::hubShader.setProjection(glm::ortho(0.0f, (float)conf_getWidth(), (float)conf_getHeight(), 0.0f));
		ShaderManager::hubShader.setView(glm::mat4(1.0f));

		ShaderManager::hubShader.unbind();

		MenuManager::gameContextMenu.render();
	});

	this->renderPassMan.addRenderPass(&this->mainRenderPass);
	this->renderPassMan.addRenderPass(&this->hubRenderPass);

	// Initialize Managers
	levelManager.init(this);
	playerManager.init(this);

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

	}
	else {
		if (input_isIMFromConfDown("escape")) {
			MenuManager::gameContextMenu.setShow(false);
			input_setGrab(true);
		}
	}

	//MenuManager::update(delta);
	MenuManager::gameContextMenu.update(delta);
}

void GameState::fixedUpdate() {
	if (!MenuManager::isShow()) {
		//phyManager.stepSimulation();

		playerManager.fixedUpdate();
	}
}

void GameState::render() {
	this->renderPassMan.render();
}

void GameState::release() {
	playerManager.release();
	levelManager.release();
	renderPassMan.release();
	phyManager.release();
}

/*
void GameState::_initUI() {
	float x = conf_getWidth() * 0.4f;
	float y = conf_getHeight() * 0.2f;

	float width = conf_getWidth() * 0.2f;
	float height = conf_getHeight() * 0.5f;

	// continueButton
	continueButton.setTitle("Continue");
	continueButton.init();
	continueButton.setPosition(glm::vec2(x + (width * 0.5f - continueButton.size.x * 0.5f), y + 32.0f));

	continueButton.setButtonCallback([&](UIButtonComponent* comp) {
		uiManager.setShow(false);
		input_setGrab(true);
		//SoundManager::playerMusicChannel();
	});

	// Master Label
	masterSliderLabel.init();
	masterSliderLabel.setTitle("Master");
	masterSliderLabel.setPosition(glm::vec2(x + (width * 0.5f - FontRender::getSize("Master").x * 0.5), y + 64.0f));
	
	// Master Slider Control
	masterSlider.init();
	masterSlider.setMin(0.0f);
	masterSlider.setMax(1.0f);
	masterSlider.setValue(SoundManager::masterVolumn);
	masterSlider.setPosition(glm::vec2(x + (width * 0.5f - masterSlider.size.x * 0.5f), y + 96.0f));

	// Music Label
	musicSliderLabel.init();
	musicSliderLabel.setTitle("Music");
	musicSliderLabel.setPosition(glm::vec2(x + (width * 0.5f - FontRender::getSize("music").x * 0.5), y + 128.0f));

	// Music Slider Control
	musicSlider.init();
	musicSlider.setMin(0.0f);
	musicSlider.setMax(1.0f);
	musicSlider.setValue(SoundManager::musicVolumn);
	musicSlider.setPosition(glm::vec2(x + (width * 0.5f - musicSlider.size.x * 0.5f), y + 160.0f));

	// Ambient Label
	ambientSliderLabel.init();
	ambientSliderLabel.setTitle("Ambient");
	ambientSliderLabel.setPosition(glm::vec2(x + (width * 0.5f - FontRender::getSize("Ambient").x * 0.5), y + 192.0f));

	// Ambient Slider Control
	ambientSlider.init();
	ambientSlider.setMin(0.0f);
	ambientSlider.setMax(1.0f);
	ambientSlider.setValue(SoundManager::ambientVolumn);
	ambientSlider.setPosition(glm::vec2(x + (width * 0.5f - ambientSlider.size.x * 0.5f), y + 224.0f));

	// SoundFX Label
	soundfxSliderLabel.init();
	soundfxSliderLabel.setTitle("SoundFX");
	soundfxSliderLabel.setPosition(glm::vec2(x + (width * 0.5f - FontRender::getSize("SoundFX").x * 0.5), y + 256.0f));

	// SoundFX Slider Control
	soundfxSlider.init();
	soundfxSlider.setMin(0.0f);
	soundfxSlider.setMax(1.0f);
	soundfxSlider.setValue(SoundManager::ambientVolumn);
	soundfxSlider.setPosition(glm::vec2(x + (width * 0.5f - soundfxSlider.size.x * 0.5f), y + 288.0f));


	// exitButton
	exitButton.setTitle("Exit");
	exitButton.init();
	exitButton.setPosition(glm::vec2(x + (width * 0.5f - exitButton.size.x * 0.5f), y + 320.0f));

	exitButton.setButtonCallback([&](UIButtonComponent* comp) {
		this->callback->changeState("start-menu");
		logger_output("pressed exit button");
	});

	// manager

	this->uiManager.setHasBackground(true);
	this->uiManager.setPosition(glm::vec2(x, y));
	this->uiManager.setSize(glm::vec2(width, height));
	this->uiManager.setBackgroundColor(glm::vec3(0.1f));
	this->uiManager.setShow(false);

	this->uiManager.addComponent(&continueButton);
	this->uiManager.addComponent(&masterSliderLabel);
	this->uiManager.addComponent(&masterSlider);
	this->uiManager.addComponent(&musicSliderLabel);
	this->uiManager.addComponent(&musicSlider);
	this->uiManager.addComponent(&ambientSliderLabel);
	this->uiManager.addComponent(&ambientSlider);
	this->uiManager.addComponent(&soundfxSliderLabel);
	this->uiManager.addComponent(&soundfxSlider);
	this->uiManager.addComponent(&exitButton);

	this->uiManager.init();

	UISystem::addManager(&this->uiManager);

	input_setGrab(true);
}

void GameState::init() {
	this->phyManager.init();

	hubGeom.init();

	this->mainRenderPass.setCallback([&]() {
		glm::mat4 model =
			glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

		RenderSystem::enable(GL_DEPTH_TEST);
		RenderSystem::viewport(0, 0, conf_getWidth(), conf_getHeight());
		RenderSystem::clearColor(135.0f / 255.0f, 206.0f / 255.0f, 235.0f / 255.0f, 1.0f);
		RenderSystem::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Terrain Rendering
		levelManager.render();

		cratesManager.render();

		// Render Water
		ShaderManager::waterShader.bind();
		ShaderManager::waterShader.setCamera(&camera);


		float y = levelManager.terrain.data.beachLevel * levelManager.terrain.data.heightScale;
		ShaderManager::waterShader.setModel(
			glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, y, 0.0f)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(1024.0f, 0.0f, 1024.0f))
		);

		ShaderManager::waterShader.setTexScale(64.0f);

		ShaderManager::waterShader.setTimeDelta(this->waterAnim);

		//water.bind();
		TextureManager::getTex("water:water1")->bind();
		waterGeom.render(&ShaderManager::waterShader);
		//water.unbind();
		TextureManager::getTex("water:water1")->unbind();

		ShaderManager::waterShader.unbind();
		//logger_output("Hello, World 4\n");

		RenderSystem::disable(GL_DEPTH_TEST);
	});

	this->hubRenderPass.setCallback([&]() {
		// Do nothing for the moment

		RenderSystem::disable(GL_DEPTH_TEST);

		ShaderManager::hubShader.bind();

		ShaderManager::hubShader.setProjection(glm::ortho(0.0f, (float)conf_getWidth(), (float)conf_getHeight(), 0.0f));
		ShaderManager::hubShader.setView(glm::mat4(1.0f));

		ShaderManager::hubShader.unbind();

		this->uiManager.render(&ShaderManager::uiShader);

		RenderSystem::enable(GL_DEPTH_TEST);

	});

	renderPassManager.addRenderPass(&this->mainRenderPass);
	renderPassManager.addRenderPass(&this->hubRenderPass);

	levelManager.init(this);
	cratesManager.init(this);

	waterGeom.init();

	
	camera.setPhysicsManager(&this->phyManager);
	camera.setJumpSpeed(10.0f);

	float x = (rand() % 512) - 256.0f;
	float z = (rand() % 512) - 256.0f;

	camera.init(
		glm::vec3(x, this->levelManager.terrain.data.getY(x, z) + 2.0f, z),
		glm::vec2(0.0f),
		conf_getFOV(),
		(float)conf_getWidth() / (float)conf_getHeight(),
		1.0f,
		1024.0f,
		64.0f,
		512.0f);

	this->cameraData.id = PhysicObjectType::POT_PLAYER;

	camera.body->setUserPointer(&this->cameraData);

	float y = levelManager.terrain.data.beachLevel * levelManager.terrain.data.heightScale - 2.0;

	this->shape = phyManager.createStaticPlaneShape(btVector3(0, 1, 0), 0.0f);
	this->body = phyManager.createRigidBody(0.0f, btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, y, 0)), shape);

	this->_initUI();

	//SoundManager::playMusic("song1");
	//SoundManager::setMusicVolumn(0.0f);
	//SoundManager::setListener(&this->camera);
	
	SoundManager::getSound("song1")->play();
	SoundManager::getSound("ocean-ambients")->play();
}

void GameState::update(float delta) {

	if (!uiManager.isShow()) {
		if (input_isIMFromConfDown("escape")) {
			uiManager.setShow(true);
			input_setGrab(false);
		}

		camera.update(delta);

		// Water animation
		this->waterAnim += delta * 0.001f;

		if (this->waterAnim >= 1.0f) {
			this->waterAnim -= 1.0f;
		}
	}
	else {
		if (input_isIMFromConfDown("escape")) {
			uiManager.setShow(false);
			input_setGrab(true);
		}

		SoundManager::masterVolumn = masterSlider.getValue();
		SoundManager::musicVolumn = musicSlider.getValue();
		SoundManager::ambientVolumn = ambientSlider.getValue();
		SoundManager::soundfxVolumn = soundfxSlider.getValue();
	}

	uiManager.update(delta);
}

void GameState::fixedUpdate() {
	if (!uiManager.isShow()) {
		phyManager.stepSimulation();
		camera.fixedUpdate();

		cratesManager.fixedUpdate();
	}
}

void GameState::render() {
	renderPassManager.render();
}

void GameState::release() {
	//SoundManager::stopMusicChannel();

	SoundManager::getSound("ocean-ambients")->stop();
	SoundManager::getSound("song1")->stop();

	UISystem::removeManager(&this->uiManager);
	uiManager.release();

	phyManager.removeRigidBody(this->body);
	delete this->shape;

	this->renderPassManager.release();

	waterGeom.release();
	//terrain.release();
	cratesManager.release();
	levelManager.release();
	//multiMeshTest.release();

	hubGeom.release();

	//SoundManager::releaseListener();
	camera.body->setUserPointer(nullptr);
	camera.release();
	phyManager.release();
}




// Level Manager
void LevelManager::init(GameState* state) {
	this->state = state;

	this->phyManager = &this->state->phyManager;
	

	terrain.init();

	//terrain.setBlackChannel(&this->dirt1);
	terrain.setBlackChannel(TextureManager::getTex("terrain:dirt1"));
	//terrain.setRedChannel(&this->sand1);
	terrain.setRedChannel(TextureManager::getTex("terrain:sand1"));
	//terrain.setGreenChannel(&this->grass1);
	terrain.setGreenChannel(TextureManager::getTex("terrain:grass1"));
	//terrain.setBlueChannel(&this->dirt2);
	terrain.setBlueChannel(TextureManager::getTex("terrain:dirt2"));

	this->shape = this->phyManager->createHeightfiledCollisionShape(
		terrain.data
	);

	float offset = terrain.data.heightScale * 0.5f;
	btVector3 v(0.0f, offset, 0.0f);

	this->body = this->phyManager->createRigidBody(0, btTransform(btQuaternion(0, 0, 0, 1), v), this->shape);

	this->data.id = PhysicObjectType::POT_TERRAIN;

	this->body->setUserPointer(&data);

	//this->body->setUserPointer(&PhysicObjectType::POT_TERRAIN);
}

void LevelManager::render() {

	glm::mat4 model;

	float m[16];

	this->body->getWorldTransform().getOpenGLMatrix(m);

	model = glm::make_mat4(m);

	ShaderManager::terrainShader.bind();
	ShaderManager::terrainShader.setCamera(&this->state->camera);
	ShaderManager::terrainShader.setTexScale(128.0f);

	ShaderManager::terrainShader.setModel(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	terrain.render(&ShaderManager::terrainShader);

	ShaderManager::terrainShader.unbind();

}

void LevelManager::release() {
	this->body->setUserPointer(nullptr);
	phyManager->removeRigidBody(this->body);
	delete shape;
	terrain.release();
	phyManager = nullptr;
	state = nullptr;
}


#define NUM_CRATES 128

bool callbackFunc(
	btManifoldPoint& cp, 
	const btCollisionObject* obj1, 
	const btCollisionObject* obj2,
	int index0,
	int index1) {


	return false;
}
// CreateManager 
void CratesManager::init(GameState* state) {
	this->state = state;
	this->phyManager = &this->state->phyManager;

	this->crappyCrate.setFilePath("data/meshes/objects/crate/crappy_crate.json");
	this->crappyCrate.init();


	this->shape = this->phyManager->createBoxShape(btVector3(1, 1, 1));

	this->data.id = PhysicObjectType::POT_CRATE;

	for (int i = 0; i < NUM_CRATES; i++) {
		float x = (rand() % 512) - 256.0f;
		float z = (rand() % 512) - 256.0f;
		float y = (rand() % 64) + state->levelManager.terrain.data.heightScale + 20;

		btVector3 v(x, y, z);

		btQuaternion q(0, 0, 0, 1);

		btTransform start(q, v);

		btRigidBody* body = this->phyManager->createRigidBody(1.0f, start, this->shape);
		
		body->setUserPointer(&this->data);
		//body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_HAS_COLLISION_SOUND_TRIGGER);
		
		this->bodies.push_back(body);
	}
}

void CratesManager::fixedUpdate() {

	for (int i = 0; i < bodies.size(); i++) {
		
	}
}

void CratesManager::render() {
	ShaderManager::sceneShader.bind();


	ShaderManager::sceneShader.setCamera(&this->state->camera);

	TextureManager::getTex("obj:crappy-crate")->bind();
	for (int i = 0; i < NUM_CRATES; i++) {
		float m[16];

		bodies[i]->getCenterOfMassTransform().getOpenGLMatrix(m);

		glm::mat4 model = glm::make_mat4(m);

		
		//ShaderManager::sceneShader.setModel(glm::translate(glm::mat4(1.0f), glm::vec3( 0.0f, 0.0f, 0.0f)));


		this->crappyCrate.setModel(model);
		this->crappyCrate.render(&ShaderManager::sceneShader);

	}
	TextureManager::getTex("obj:crappy-crate")->unbind();

	ShaderManager::sceneShader.unbind();
}

void CratesManager::release() {

	for (int i = 0; i < NUM_CRATES; i++) {
		this->bodies[i]->setUserPointer(nullptr);
		this->phyManager->removeRigidBody(this->bodies[i]);
	}

	this->bodies.clear();

	delete this->shape;

	this->crappyCrate.release();
	this->phyManager = nullptr;
	this->state = nullptr;
}

*/
