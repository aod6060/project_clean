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


// Mesh Shader
void SceneShader::init() {
	vertex.init(GL_VERTEX_SHADER, "data/shaders/scene.vert");
	fragment.init(GL_FRAGMENT_SHADER, "data/shaders/scene.frag");

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
	program.setAttr("normals", 2);

	this->bindAttr();
	program.enableAttr("vertices");
	program.enableAttr("texCoords");
	program.enableAttr("normals");
	this->unbindAttr();

	program.disableAttr("vertices");
	program.disableAttr("texCoords");
	program.disableAttr("normals");

	program.unbind();

}

// Attributes
void SceneShader::verticesPointer() {
	this->pointerAttr("vertices", 3, GL_FLOAT);
}

void SceneShader::texCoordPointer() {
	this->pointerAttr("texCoords", 2, GL_FLOAT);
}

void SceneShader::normalPointer() {
	this->pointerAttr("normals", 3, GL_FLOAT);
}

// Uniforms
void SceneShader::setProjective(const glm::mat4& proj) {
	this->program.setMat4("proj", proj);
}

void SceneShader::setView(const glm::mat4& view) {
	this->program.setMat4("view", view);
}

void SceneShader::setModel(const glm::mat4& model) {
	this->program.setMat4("model", model);
}

void SceneShader::setCamera(Camera* camera) {
	this->setProjective(camera->getProjection());
	this->setView(camera->getView());
}



// Terrain Shader
void TerrainShader::init() {
	vertex.init(GL_VERTEX_SHADER, "data/shaders/terrain.vert");
	fragment.init(GL_FRAGMENT_SHADER, "data/shaders/terrain.frag");

	program.addShader(&vertex);
	program.addShader(&fragment);

	program.init();

	program.bind();

	program.createUniform("proj");
	program.createUniform("view");
	program.createUniform("model");
	program.createUniform("texScale");
	program.set1f("texScale", 1.0f);

	program.createUniform("blendMap");
	program.set1i("blendMap", 0);
	program.createUniform("channelBlack");
	program.set1i("channelBlack", 1);
	program.createUniform("channelRed");
	program.set1i("channelRed", 2);
	program.createUniform("channelGreen");
	program.set1i("channelGreen", 3);
	program.createUniform("channelBlue");
	program.set1i("channelBlue", 4);

	program.setAttr("vertices", 0);
	program.setAttr("texCoords", 1);
	program.setAttr("normals", 2);

	this->bindAttr();
	program.enableAttr("vertices");
	program.enableAttr("texCoords");
	program.enableAttr("normals");
	this->unbindAttr();

	program.disableAttr("vertices");
	program.disableAttr("texCoords");
	program.disableAttr("normals");

	program.unbind();
}

// Attributes
void TerrainShader::verticesPointer() {
	program.pointerAttr("vertices", 3, GL_FLOAT);
}

void TerrainShader::texCoordPointer() {
	program.pointerAttr("texCoords", 2, GL_FLOAT);
}

void TerrainShader::normalPointer() {
	program.pointerAttr("normals", 3, GL_FLOAT);
}

// Uniforms
void TerrainShader::setProjective(const glm::mat4& proj) {
	program.setMat4("proj", proj);
}

void TerrainShader::setView(const glm::mat4& view) {
	program.setMat4("view", view);
}

void TerrainShader::setModel(const glm::mat4& model) {
	program.setMat4("model", model);
}

void TerrainShader::setCamera(Camera* camera) {
	this->setProjective(camera->getProjection());
	this->setView(camera->getView());
}

void TerrainShader::setTexScale(float scale) {
	program.set1f("texScale", scale);
}
