#include "game.h"


void GameWindowCallback::init() {
	angry.loadTexture("data/textures/angry.png");

	this->mainRenderPass.setCallback([&](RenderContext* context) {

		glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)conf_getWidth() / (float)conf_getHeight(), 1.0f, 1024.0f);

		glm::mat4 view = glm::mat4(1.0f);

		glm::mat4 model =
			glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(this->yrot), glm::vec3(1.0f, 1.0f, 0.0f));

		context->enable(GL_DEPTH_TEST);
		context->clearColor(0.0f, 0.0f, 1.0f, 1.0f);
		context->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		MainRenderPass* pass = context->getPass<MainRenderPass>();

		pass->sceneShader.bind();

		pass->sceneShader.setProjective(proj);
		pass->sceneShader.setView(view);

		angry.bind();

		switch (meshType) {
		case MeshType::MT_CUBE:
			cube.setModel(model);
			cube.render(&pass->sceneShader);
			break;
		case MeshType::MT_SPHERE:
			sphere.setModel(model);
			sphere.render(&pass->sceneShader);
			break;
		case MeshType::MT_CYLINDER:
			cylinder.setModel(model);
			cylinder.render(&pass->sceneShader);
			break;
		case MeshType::MT_TORUS:
			torus.setModel(model);
			torus.render(&pass->sceneShader);
			break;
		case MeshType::MT_MONKEY:
			monkey.setModel(model);
			monkey.render(&pass->sceneShader);
			break;
		case MeshType::MT_MULTI_MESH_TEST:
			model =
				glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -32.0f)) *
				glm::rotate(glm::mat4(1.0f), glm::radians(this->yrot), glm::vec3(1.0f, 1.0f, 0.0f));
			multiMeshTest.setModel(model);
			multiMeshTest.render(&pass->sceneShader);
			break;
		}

		angry.unbind();

		pass->sceneShader.unbind();

		context->disable(GL_DEPTH_TEST);
	});

	renderPassManager.addRenderPass(&this->mainRenderPass);
	renderPassManager.init();

	cube.setFilePath("data/meshes/cube.json");
	cube.init();

	sphere.setFilePath("data/meshes/sphere.json");
	sphere.init();

	cylinder.setFilePath("data/meshes/cylinder.json");
	cylinder.init();

	torus.setFilePath("data/meshes/torus.json");
	torus.init();

	monkey.setFilePath("data/meshes/monkey.json");
	monkey.init();

	multiMeshTest.setFilePath("data/meshes/multi_mesh_test.json");
	multiMeshTest.init();

}

void GameWindowCallback::update(float delta) {
	if (input_isKeyDown(Keyboard::KB_ESCAPE)) {
		win_exit();
	}

	if (input_isKeyDown(Keyboard::KB_F1)) {
		this->isWire = !this->isWire;

		if (this->isWire) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

	if (input_isKeyDown(Keyboard::KB_1)) {
		this->meshType = MeshType::MT_CUBE;
	}

	if (input_isKeyDown(Keyboard::KB_2)) {
		this->meshType = MeshType::MT_SPHERE;
	}

	if (input_isKeyDown(Keyboard::KB_3)) {
		this->meshType = MeshType::MT_CYLINDER;
	}

	if (input_isKeyDown(Keyboard::KB_4)) {
		this->meshType = MeshType::MT_TORUS;
	}

	if (input_isKeyDown(Keyboard::KB_5)) {
		this->meshType = MeshType::MT_MONKEY;
	}

	if (input_isKeyDown(Keyboard::KB_6)) {
		this->meshType = MeshType::MT_MULTI_MESH_TEST;
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
	multiMeshTest.release();
	monkey.release();
	torus.release();
	cylinder.release();
	sphere.release();
	cube.release();
	angry.release();
	//testShader.release();
	renderPassManager.release();
}

