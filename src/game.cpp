#include "game.h"



void GameWindowCallback::init() {
	angry.loadTexture("data/textures/angry.png");

	grass1.loadTexture("data/textures/terrain/grass1.png");
	grass2.loadTexture("data/textures/terrain/grass2.png");
	dirt1.loadTexture("data/textures/terrain/dirt1.png");
	dirt2.loadTexture("data/textures/terrain/dirt2.png");
	sand1.loadTexture("data/textures/terrain/sand1.png");

	water.loadTexture("data/textures/water/temp_water1.png");

	this->mainRenderPass.setCallback([&](RenderContext* context) {
		glm::mat4 model =
			glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

		context->enable(GL_DEPTH_TEST);
		context->clearColor(0.0f, 0.0f, 1.0f, 1.0f);
		context->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		MainRenderPass* pass = context->getPass<MainRenderPass>();

		// Terrain Rendering
		pass->terrainShader.bind();
		pass->terrainShader.setCamera(&camera);
		pass->terrainShader.setTexScale(32.0f);

		pass->terrainShader.setModel(model);
		terrain.render(&pass->terrainShader);

		pass->terrainShader.unbind();

		// Render Meshes
		pass->sceneShader.bind();
		pass->sceneShader.setCamera(&camera);
		sand1.bind();
		//angry.bind();
		multiMeshTest.setModel(model);
		multiMeshTest.render(&pass->sceneShader);
		sand1.unbind();
		//angry.unbind();
		pass->sceneShader.unbind();


		// Render Water
		pass->waterShader.bind();
		pass->waterShader.setCamera(&camera);
		
		float y = this->terrain.data.beachLevel * this->terrain.heightScale;
		pass->waterShader.setModel(
			glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, y, 0.0f)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(1024.0f, 0.0f, 1024.0f))
		);

		pass->waterShader.setTexScale(32.0f);

		pass->waterShader.setTimeDelta(this->waterAnim);

		water.bind();
		waterGeom.render(&pass->waterShader);
		water.unbind();

		pass->waterShader.unbind();

		context->disable(GL_DEPTH_TEST);
	});

	renderPassManager.addRenderPass(&this->mainRenderPass);
	renderPassManager.init();

	multiMeshTest.setFilePath("data/meshes/multi_mesh_test.json");
	multiMeshTest.init();

	camera.init(
		glm::vec3(0.0f),
		glm::vec2(0.0f),
		conf_getFOV(),
		(float)conf_getWidth() / (float)conf_getHeight(),
		1.0f,
		1024.0f);

	//terrain.setHeightMapFilePath("data/terrain/static_terrain_height_map.png");
	//terrain.setBlendMap("data/terrain/static_terrain_rgb_map.png");
	terrain.init();

	terrain.setBlackChannel(&this->dirt1);
	terrain.setRedChannel(&this->sand1);
	terrain.setGreenChannel(&this->grass1);
	terrain.setBlueChannel(&this->dirt2);

	waterGeom.init();
}

void GameWindowCallback::update(float delta) {
	if (input_isIMFromConfDown("escape")) {
		win_exit();
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

	if (input_isIMFromConfDown("toggle-input-mode")) {
		input_toggleGrab();
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

void GameWindowCallback::fixedUpdate() {
}

void GameWindowCallback::render() {
	renderPassManager.render();
}

void GameWindowCallback::release() {
	waterGeom.release();
	terrain.release();
	multiMeshTest.release();
	//testShader.release();
	renderPassManager.release();

	water.release();

	sand1.release();
	dirt2.release();
	dirt1.release();
	grass2.release();
	grass1.release();

	angry.release();
}

