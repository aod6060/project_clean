#include "ui_test.h"


void UITestWindowCallback::init() {
	float x = conf_getWidth() * 0.2f;
	float y = conf_getHeight() * 0.2f;

	float width = conf_getWidth() * 0.6f;
	float height = conf_getHeight() * 0.6f;

	// HelloWorld
	helloWorld.init();
	//helloWorld.setColor(glm::vec3(1.0f, 1.0f, 1.0f));
	helloWorld.setPosition(glm::vec2(x + 2.0f, y));
	helloWorld.setTitle("Hello, World");

	// TestButton1
	testButton1.setTitle("Test Button 1");
	testButton1.init();
	testButton1.setPosition(glm::vec2(x + 2.0f, y + 28.0f));
	
	testButton1.setButtonCallback([&](UIButtonComponent* button) {
		std::stringstream ss;
		ss << "count: " << this->counter;
		//logger_output("%s\n", ss.str().c_str());
		testLabel1.setTitle(ss.str());
		this->counter++;
	});

	// TestLabel1
	testLabel1.init();
	testLabel1.setPosition(glm::vec2(testButton1.position.x + testButton1.getSize().x + 10.0f, y + 28.0f));
	//testLabel1.setColor(glm::vec3(1.0f));
	testLabel1.setTitle("");


	// TestCheckBox2
	testCheckBox1.setTitle("Test Check Box 2");
	testCheckBox1.init();
	testCheckBox1.setPosition(glm::vec2(x + 2.0f, y + 58.0f));

	testCheckBox1.setCallback([&](UICheckBoxComponent* checkBox) {
		//logger_output("%d\n", checkBox->isChecked());

		std::stringstream ss;

		ss << "checkBox: " << checkBox->checked;
		testLabel2.setTitle(ss.str());
	});


	// TestLabel2
	testLabel2.init();
	testLabel2.setTitle("");
	glm::vec2 location = FontRender::getSize("Test Check Box 2");

	testLabel2.setPosition(glm::vec2(testCheckBox1.getPosition().x + testCheckBox1.getSize().x + location.x + 10.0f, y + 54.0f));


	// testTexBox
	testTexBox.init();
	testTexBox.setPosition(glm::vec2(x + 2.0f, y + 84.0f));


	testTexBox.setCallback([&](UITextBoxComponent* box) {
		// Do something here in a bit...

		if (box->getText().length() > 0) {
			logger_output("%s\n", box->getText().c_str());
		}
	});


	// testSelectButton
	testSelectButton.addValue(UISelectButtonType("Hello, World", 0));
	testSelectButton.addValue(UISelectButtonType("What you want???", 1));
	testSelectButton.addValue(UISelectButtonType("I\'m just some programmer...", 2));
	testSelectButton.addValue(UISelectButtonType("This is an example", 3));
	testSelectButton.addValue(UISelectButtonType("I\'m a maverick.", 4));
	testSelectButton.init();

	testSelectButton.setPosition(glm::vec2(x + 2.0f, y + 118.0f));
	testSelectButton.setCallback([&](UISelectButtonComponent* sb) {
		logger_output("%d: %s\n", sb->getCurrentValue(), sb->getCurrentID().c_str());
	});


	// testSlider
	testSlider.init();
	testSlider.setMin(-1.0f);
	testSlider.setMax(1.0f);
	testSlider.setValue(-1.0f);
	testSlider.setPosition(glm::vec2(x + 2.0f, y + 148.0f));
	
	manager.addComponent(&helloWorld);
	manager.addComponent(&testButton1);
	manager.addComponent(&testLabel1);
	manager.addComponent(&testCheckBox1);
	manager.addComponent(&testLabel2);
	manager.addComponent(&testTexBox);
	manager.addComponent(&testSelectButton);
	manager.addComponent(&testSlider);

	manager.setBackgroundColor(glm::vec3(0.1f));
	manager.setHasBackground(true);

	manager.setPosition(glm::vec2(x, y));
	manager.setSize(glm::vec2(width, height));

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