#include "game_states.h"

void StartMenuState::init() {
	this->time = 0.0f;
	this->maxTime = 20.0f;

	this->uiTime = 0.0f;
	this->uiMaxTime = 1.0f;

	// Init Textures
	//this->startMenu.loadTexture("data/textures/menu/startmenu.png");

	// Render Pass
	this->hubGeom.init();

	this->mainRenderPass.setCallback([&]() {
		RenderSystem::viewport(0, 0, conf_getWidth(), conf_getHeight());
		RenderSystem::clearColor(0, 0, 0, 1);
		RenderSystem::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		RenderSystem::disable(GL_DEPTH_TEST);

		ShaderManager::hubShader.bind();

		ShaderManager::hubShader.setProjection(
			glm::ortho(0.0f, (float)conf_getWidth(), (float)conf_getHeight(), 0.0f)
		);

		ShaderManager::hubShader.setView(glm::mat4(1.0f));

		ShaderManager::hubShader.setModel(
			glm::translate(glm::mat4(1.0f), glm::vec3(0.0f)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(conf_getWidth(), conf_getHeight(), 0.0f))
		);

		//startMenu.bind();
		TextureManager::getTex("menu:startmenu")->bind();
		this->hubGeom.render(&ShaderManager::hubShader);
		TextureManager::getTex("menu:startmenu")->unbind();
		//startMenu.unbind();

		ShaderManager::hubShader.unbind();

		manager.render(&ShaderManager::uiShader);
	});

	this->renderPassManager.addRenderPass(&this->mainRenderPass);

	this->_uiInit();
}

void StartMenuState::update(float delta) {
	if (input_checkUpdate() || input_checkMotionUpdate()) {
		this->time = 0.0f;
	}
	else {
		if (this->time >= this->maxTime) {
			this->callback->changeState("logo1");
		}
		else {
			this->time += delta;
		}
	}

	if (input_isIMFromConfDown("ui-action")) {
		logger_output("%d\n", input_checkUpdate());
	}

	if (this->uiTime >= this->uiMaxTime && !this->manager.isShow()) {
		this->manager.setShow(true);
	}
	else {
		this->uiTime += delta;
	}

	this->manager.update(delta);
}

void StartMenuState::fixedUpdate() {

}

void StartMenuState::render() {
	this->renderPassManager.render();
}

void StartMenuState::release() {
	UISystem::removeManager(&this->manager);
	this->manager.release();

	this->renderPassManager.release();
	manager.release();
	hubGeom.release();
	//this->startMenu.release();
}

void StartMenuState::_uiInit() {
	float x = conf_getWidth() * 0.4f;
	float y = conf_getHeight() * 0.6f;

	float width = conf_getWidth() * 0.2f;
	float height = conf_getHeight() * 0.3f;

	// Start Button
	startButton.setTitle("Start");
	startButton.init();
	startButton.setPosition(glm::vec2(
		x + (width * 0.5f - startButton.size.x * 0.5f),
		y + 32.0f
	));

	startButton.setButtonCallback([&](UIButtonComponent* comp) {
		this->callback->changeState("game");
	});



	// Exit Button
	exitButton.setTitle("Exit");
	exitButton.init();
	exitButton.setPosition(glm::vec2(
		x + (width * 0.5f - exitButton.size.x * 0.5f),
		y + 64.0f
	));

	exitButton.setButtonCallback([&](UIButtonComponent* comp) {
		win_exit();
	});


	// UI Manager
	this->manager.setHasBackground(true);
	manager.setPosition(glm::vec2(x, y));
	manager.setSize(glm::vec2(width, height));
	manager.setBackgroundColor(glm::vec3(0.1f));
	manager.setShow(false);

	manager.addComponent(&startButton);
	manager.addComponent(&exitButton);

	manager.init();

	UISystem::addManager(&this->manager);
}