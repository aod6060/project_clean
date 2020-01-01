#include "game.h"



void GameWindowCallback::init() {
	angry.loadTexture("data/textures/angry.png");

	grass1.loadTexture("data/textures/terrain/grass1.png");
	grass2.loadTexture("data/textures/terrain/grass2.png");
	dirt1.loadTexture("data/textures/terrain/dirt1.png");
	dirt2.loadTexture("data/textures/terrain/dirt2.png");
	sand1.loadTexture("data/textures/terrain/sand1.png");

	water.loadTexture("data/textures/water/temp_water1.png");

	hubGeom.init();

	this->mainRenderPass.setCallback([&]() {
		glm::mat4 model =
			glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

		RenderSystem::enable(GL_DEPTH_TEST);
		RenderSystem::viewport(0, 0, conf_getWidth(), conf_getHeight());
		RenderSystem::clearColor(135.0f / 255.0f, 206.0f / 255.0f, 235.0f / 255.0f, 1.0f);
		RenderSystem::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//MainRenderPass* pass = context->getPass<MainRenderPass>();

		// Terrain Rendering
		ShaderManager::terrainShader.bind();
		ShaderManager::terrainShader.setCamera(&camera);
		ShaderManager::terrainShader.setTexScale(32.0f);

		ShaderManager::terrainShader.setModel(model);
		terrain.render(&ShaderManager::terrainShader);

		ShaderManager::terrainShader.unbind();

		// Render Meshes
		ShaderManager::sceneShader.bind();
		ShaderManager::sceneShader.setCamera(&camera);
		angry.bind();
		//angry.bind();
		multiMeshTest.setModel(model);
		multiMeshTest.render(&ShaderManager::sceneShader);
		angry.unbind();
		//angry.unbind();
		ShaderManager::sceneShader.unbind();


		// Render Water
		ShaderManager::waterShader.bind();
		ShaderManager::waterShader.setCamera(&camera);
		
		
		float y = this->terrain.data.beachLevel * this->terrain.heightScale;
		ShaderManager::waterShader.setModel(
			glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, y, 0.0f)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(1024.0f, 0.0f, 1024.0f))
		);

		ShaderManager::waterShader.setTexScale(32.0f);

		ShaderManager::waterShader.setTimeDelta(this->waterAnim);

		water.bind();
		waterGeom.render(&ShaderManager::waterShader);
		water.unbind();

		ShaderManager::waterShader.unbind();

		RenderSystem::disable(GL_DEPTH_TEST);
	});
	
	this->hubRenderPass.setCallback([&]() {
		// Do nothing for the moment

		RenderSystem::disable(GL_DEPTH_TEST);

		ShaderManager::hubShader.bind();

		ShaderManager::hubShader.setProjection(glm::ortho(0.0f, (float)conf_getWidth(), (float)conf_getHeight(), 0.0f));
		ShaderManager::hubShader.setView(glm::mat4(1.0f));

		ShaderManager::hubShader.unbind();

		FontRender::setColor(glm::vec3(1.0f, 0.0f, 0.0f));
		FontRender::print(1.0f, 1.0f, "player position: [%f, %f, %f]", camera.pos.x, camera.pos.y, camera.pos.z);
		FontRender::setColor(glm::vec3(0.0f, 0.0f, 1.0f));
		FontRender::print(1.0f, 22.0f, "player rotation: [%f, %f]", camera.rot.x, camera.rot.y);


		this->uiManager.render(&ShaderManager::uiShader);

		RenderSystem::enable(GL_DEPTH_TEST);

	});

	renderPassManager.addRenderPass(&this->mainRenderPass);
	renderPassManager.addRenderPass(&this->hubRenderPass);

	multiMeshTest.setFilePath("data/meshes/multi_mesh_test.json");
	multiMeshTest.init();

	camera.init(
		glm::vec3(0.0f),
		glm::vec2(0.0f),
		conf_getFOV(),
		(float)conf_getWidth() / (float)conf_getHeight(),
		1.0f,
		1024.0f);

	terrain.init();

	terrain.setBlackChannel(&this->dirt1);
	terrain.setRedChannel(&this->sand1);
	terrain.setGreenChannel(&this->grass1);
	terrain.setBlueChannel(&this->dirt2);

	waterGeom.init();

	this->_initUI();
}

void GameWindowCallback::update(float delta) {

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

		uiManager.update(delta);
	}
}

void GameWindowCallback::fixedUpdate() {
}

void GameWindowCallback::render() {
	renderPassManager.render();
}

void GameWindowCallback::release() {
	UISystem::removeManager(&this->uiManager);

	uiManager.release();

	this->uiManager.release();

	waterGeom.release();
	terrain.release();
	multiMeshTest.release();
	//testShader.release();

	water.release();

	sand1.release();
	dirt2.release();
	dirt1.release();
	grass2.release();
	grass1.release();

	angry.release();

	hubGeom.release();
}

void GameWindowCallback::_initUI() {
	float x = conf_getWidth() * 0.4f;
	float y = conf_getHeight() * 0.2f;

	float width = conf_getWidth() * 0.2f;
	float height = conf_getHeight() * 0.4f;

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
		win_exit();
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