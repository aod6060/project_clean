#include "ui_test.h"


void UITestWindowCallback::init() {
	// HelloWorld
	helloWorld.init();
	//helloWorld.setColor(glm::vec3(1.0f, 1.0f, 1.0f));
	helloWorld.setPosition(glm::vec2(1.0f, 1.0f));
	helloWorld.setTitle("Hello");

	// TestButton1
	testButton1.setTitle("Test Button 1");
	testButton1.init();
	testButton1.setPosition(glm::vec2(1.0f, 24.0f));
	
	testButton1.setButtonCallback([&](UIButtonComponent* button) {
		std::stringstream ss;
		ss << "count: " << this->counter;
		logger_output("%s\n", ss.str().c_str());
		testLabel1.setTitle(ss.str());
		this->counter++;
	});

	// TestLabel1
	testLabel1.init();
	testLabel1.setPosition(glm::vec2(testButton1.getSize().x + 5.0f, 24.0f));
	//testLabel1.setColor(glm::vec3(1.0f));
	testLabel1.setTitle("");

	manager.addComponent(&helloWorld);
	manager.addComponent(&testButton1);
	manager.addComponent(&testLabel1);

	manager.setShow(true);
	manager.init();

	UISystem::addManager(&this->manager);

	this->mainRenderPass.setCallback([&]() {
		RenderSystem::viewport(0, 0, conf_getWidth(), conf_getHeight());
		RenderSystem::clearColor(0.0f, 0.0f, 0.0f, 1.0f);
		RenderSystem::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		RenderSystem::disable(GL_DEPTH_TEST);

		manager.render(&ShaderManager::uiShader);
	});

	this->renderPassManager.addRenderPass(&this->mainRenderPass);
	
}

void UITestWindowCallback::update(float delta) {
	manager.update(delta);

	if (input_isIMFromConfDown("toggle-ui")) {
		manager.toggleShow();
	}
}

void UITestWindowCallback::fixedUpdate() {
	// do nothing for the moment...
	
}

void UITestWindowCallback::render() {
	//manager.render(&ShaderManager::uiShader);
	this->renderPassManager.render();
}

void UITestWindowCallback::release() {
	UISystem::removeManager(&this->manager);

	manager.release();
	testButton1.release();
	helloWorld.release();
}