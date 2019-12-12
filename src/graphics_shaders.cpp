#include "sys.h"

// AbstractShader
void AbstractShader::bind() {
	program.bind();
}

void AbstractShader::unbind() {
	program.unbind();
}

void AbstractShader::release() {
	program.release();
	fragment.release();
	vertex.release();
}

void AbstractShader::bindAttr() {
	program.bindAttr();
}

void AbstractShader::unbindAttr() {
	program.unbindAttr();
}

void AbstractShader::pointerAttr(
	std::string name,
	uint32_t size,
	GLenum type) {
	program.pointerAttr(name, size, type);
}




// TestShader
void TestShader::init() {
	// Shader
	vertex.init(GL_VERTEX_SHADER, "data/shaders/test.vert");
	fragment.init(GL_FRAGMENT_SHADER, "data/shaders/test.frag");

	// Program
	program.addShader(&vertex);
	program.addShader(&fragment);

	program.init();

	program.bind();
	program.createUniform("proj");
	program.createUniform("view");
	program.createUniform("model");
	program.createUniform("tex0");
	program.set1i("tex0", 0);

	program.setAttr("vertices", 0);
	program.setAttr("texCoords", 1);

	this->bindAttr();
	program.enableAttr("vertices");
	program.enableAttr("texCoords");
	this->unbindAttr();
	program.disableAttr("vertices");
	program.disableAttr("texCoords");

	program.unbind();
}

// Set Attributes
void TestShader::verticesPointer() {
	this->pointerAttr("vertices", 3, GL_FLOAT);
}

void TestShader::texCoordPointer() {
	this->pointerAttr("texCoords", 2, GL_FLOAT);
}

// Set Uniforms
void TestShader::setProjection(const glm::mat4& proj) {
	program.setMat4("proj", proj);
}

void TestShader::setView(const glm::mat4& view) {
	program.setMat4("view", view);
}

void TestShader::setModel(const glm::mat4& model) {
	program.setMat4("model", model);
}

