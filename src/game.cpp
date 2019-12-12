#include "game.h"


void GameWindowCallback::init() {
	this->quad.init();
	angry.loadTexture("data/textures/angry.png");

	this->mainRenderPass.setCallback([&](RenderContext* context) {
		glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)conf_getWidth() / (float)conf_getHeight(), 1.0f, 1024.0f);

		glm::mat4 view = glm::mat4(1.0f);

		glm::mat4 model =
			glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(yrot), glm::vec3(0.0f, 1.0f, 0.0f));
		
		context->enable(GL_DEPTH_TEST);
		context->clearColor(0.0f, 0.0f, 1.0f, 1.0f);
		context->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		MainRenderPass* pass = context->getPass<MainRenderPass>();
		
		pass->testShader.bind();
		pass->testShader.setProjection(proj);
		pass->testShader.setView(view);
		pass->testShader.setModel(model);

		angry.bind();

		quad.render(&pass->testShader);

		angry.unbind();

		pass->testShader.unbind();

		context->disable(GL_DEPTH_TEST);

	});

	renderPassManager.addRenderPass(&this->mainRenderPass);
	renderPassManager.init();
}

void GameWindowCallback::update(float delta) {
	if (input_isKeyDown(Keyboard::KB_ESCAPE)) {
		win_exit();
	}

	yrot += 32.0f * delta;

	if (yrot >= 360.0f) {
		yrot -= 360.0f;
	}
}

void GameWindowCallback::fixedUpdate() {
}

void GameWindowCallback::render() {
	renderPassManager.render();
}

void GameWindowCallback::release() {
	angry.release();
	quad.release();
	//testShader.release();
	renderPassManager.release();
}

