#include "game_states.h"

void Logo2State::init() {
	this->time = 0.0f;
	this->maxTime = 3.0f;

	// Init Textures
	this->logo.loadTexture("data/textures/logos/teh_island_engine_v_1_0_0.png");

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

		float logoWidth = 512.0f;
		float logoHeight = 128.0f;

		float logoHW = logoWidth * 0.5f;
		float logoHH = logoHeight * 0.5f;

		float screenHW = (float)conf_getWidth() * 0.5f;
		float screenHH = (float)conf_getHeight() * 0.5f;

		float logoX = screenHW - logoHW;
		float logoY = screenHH - logoHH;

		ShaderManager::hubShader.setModel(
			glm::translate(glm::mat4(1.0f), glm::vec3(logoX, logoY, 0.0f)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(logoWidth, logoHeight, 0.0f))
		);

		logo.bind();
		this->hubGeom.render(&ShaderManager::hubShader);
		logo.unbind();

		ShaderManager::hubShader.unbind();

	});

	this->renderPassManager.addRenderPass(&this->mainRenderPass);

}

void Logo2State::update(float delta) {
	if (input_checkUpdate()) {
		this->callback->changeState("start-menu");
	}

	if (this->time >= this->maxTime) {
		this->callback->changeState("start-menu");
	}
	else {
		this->time += delta;
	}

	
}

void Logo2State::fixedUpdate() {

}

void Logo2State::render() {
	this->renderPassManager.render();
}

void Logo2State::release() {
	this->renderPassManager.release();
	hubGeom.release();
	logo.release();
}