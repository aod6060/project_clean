#include "game_states.h"


void GameState::_initUI() {
	float x = conf_getWidth() * 0.4f;
	float y = conf_getHeight() * 0.4f;

	float width = conf_getWidth() * 0.2f;
	float height = conf_getHeight() * 0.3f;

	// continueButton
	continueButton.setTitle("Continue");
	continueButton.init();
	continueButton.setPosition(glm::vec2(x + (width * 0.5f - continueButton.size.x * 0.5f), y + 32.0f));

	continueButton.setButtonCallback([&](UIButtonComponent* comp) {
		uiManager.setShow(false);
		input_setGrab(true);
	});

	// exitButton
	exitButton.setTitle("Exit");
	exitButton.init();
	exitButton.setPosition(glm::vec2(x + (width * 0.5f - exitButton.size.x * 0.5f), y + 64.0f));

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

		ShaderManager::waterShader.setTexScale(32.0f);

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

	camera.init(
		glm::vec3(0.0f, this->levelManager.terrain.data.getY(0, 0) + 2.0f, 0.0f),
		glm::vec2(0.0f),
		conf_getFOV(),
		(float)conf_getWidth() / (float)conf_getHeight(),
		1.0f,
		1024.0f);

	this->_initUI();
}

void GameState::update(float delta) {

	if (!uiManager.isShow()) {
		if (input_isIMFromConfDown("escape")) {
			uiManager.setShow(true);
			input_setGrab(false);
		}

		if (input_isIMFromConfDown("wire-frame-toggle")) {
			this->isWire = !this->isWire;

			if (this->isWire) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
		}

		yrot += 32.0f * delta;

		if (yrot >= 360.0f) {
			yrot -= 360.0f;
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
	}

	uiManager.update(delta);
}

void GameState::fixedUpdate() {
	phyManager.stepSimulation();
}

void GameState::render() {
	renderPassManager.render();
}

void GameState::release() {
	UISystem::removeManager(&this->uiManager);

	uiManager.release();

	this->renderPassManager.release();

	waterGeom.release();
	//terrain.release();
	cratesManager.release();
	levelManager.release();
	//multiMeshTest.release();

	hubGeom.release();

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
	phyManager->removeRigidBody(this->body);
	delete shape;
	terrain.release();
	phyManager = nullptr;
	state = nullptr;
}


#define NUM_CRATES 128

// CreateManager 
void CratesManager::init(GameState* state) {
	this->state = state;
	this->phyManager = &this->state->phyManager;

	this->crappyCrate.setFilePath("data/meshes/objects/crate/crappy_crate.json");
	this->crappyCrate.init();


	this->shape = this->phyManager->createBoxShape(btVector3(1, 1, 1));

	for (int i = 0; i < NUM_CRATES; i++) {
		float x = (rand() % 512) - 256.0f;
		float z = (rand() % 512) - 256.0f;
		float y = (rand() % 64) + state->levelManager.terrain.data.heightScale + 20;

		btVector3 v(x, y, z);

		btQuaternion q(0, 0, 0, 1);

		btTransform start(q, v);

		btRigidBody* body = this->phyManager->createRigidBody(1.0f, start, this->shape);

		this->bodies.push_back(body);
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
		this->phyManager->removeRigidBody(this->bodies[i]);
	}

	this->bodies.clear();

	delete this->shape;

	this->crappyCrate.release();
	this->phyManager = nullptr;
	this->state = nullptr;
}
