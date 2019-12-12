#include "game.h"


void GameWindowCallback::init() {
	glEnable(GL_DEPTH_TEST);

	this->testShader.init();

	quad.init();
	// 0
	quad.set3f(-1.0f, 1.0f, 0.0f);
	// 1
	quad.set3f(1.0f, 1.0f, 0.0f);
	// 2
	quad.set3f(-1.0f, -1.0f, 0.0f);
	// 3
	quad.set3f(1.0f, -1.0f, 0.0f);
	quad.update();

	quadTexCoords.init();
	quadTexCoords.set2f(0.0f, 0.0f);
	quadTexCoords.set2f(1.0f, 0.0f);
	quadTexCoords.set2f(0.0f, 1.0f);
	quadTexCoords.set2f(1.0f, 1.0f);
	quadTexCoords.update();

	quadIndencies.init();
	quadIndencies.set3f(0, 1, 2);
	quadIndencies.set3f(2, 1, 3);
	quadIndencies.update();

	angry.loadTexture("data/textures/angry.png");
}

void GameWindowCallback::update(float delta) {
	//std::cout << delta << std::endl;

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
	glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)conf_getWidth() / (float)conf_getHeight(), 1.0f, 1024.0f);

	glm::mat4 view = glm::mat4(1.0f);

	glm::mat4 model =
		glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f)) *
		glm::rotate(glm::mat4(1.0f), glm::radians(yrot), glm::vec3(0.0f, 1.0f, 0.0f));

	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	testShader.bind();

	/*
	testProgram.setMat4("proj", proj);
	testProgram.setMat4("view", view);
	testProgram.setMat4("model", model);
	*/

	testShader.setProjection(proj);
	testShader.setView(view);
	testShader.setModel(model);

	angry.bind();
	//testProgram.bindAttr();
	testShader.bindAttr();

	quad.bind();
	//testProgram.pointerAttr("vertices", 3, GL_FLOAT);
	testShader.verticesPointer();
	quad.unbind();

	quadTexCoords.bind();
	//testProgram.pointerAttr("texCoords", 2, GL_FLOAT);
	testShader.texCoordPointer();
	quadTexCoords.unbind();

	quadIndencies.bind();
	glDrawElements(GL_TRIANGLES, quadIndencies.size(), GL_UNSIGNED_INT, 0);
	quadIndencies.unbind();

	testShader.unbindAttr();

	angry.unbind();

	testShader.unbind();

}

void GameWindowCallback::release() {
	angry.release();

	quadIndencies.release();
	quadTexCoords.release();
	quad.release();

	testShader.release();
}

