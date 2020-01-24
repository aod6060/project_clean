#include "game_states.h"
#include "menu_manager.h"

void StartMenuState::init() {
	this->time = 0.0f;
	this->maxTime = 20.0f;

	this->uiTime = 0.0f;
	this->uiMaxTime = 1.0f;

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

		MenuManager::startMenu.render();

		ShaderManager::hubShader.unbind();
	});

	this->renderPassManager.addRenderPass(&this->mainRenderPass);
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

	if (this->uiTime >= this->uiMaxTime && !MenuManager::startMenu.isShow()) {
		//this->manager.setShow(true);
		MenuManager::startMenu.setShow(true);
	}
	else {
		this->uiTime += delta;
	}

	MenuManager::startMenu.update(delta);
}

void StartMenuState::fixedUpdate() {

}

void StartMenuState::render() {
	this->renderPassManager.render();
}

void StartMenuState::release() {
	this->renderPassManager.release();
	hubGeom.release();
	//this->startMenu.release();
}