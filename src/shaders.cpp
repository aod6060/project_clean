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

// HUBShader
void HUBShader::init() {
	vertex.init(GL_VERTEX_SHADER, "data/shaders/hub.vert");
	fragment.init(GL_FRAGMENT_SHADER, "data/shaders/hub.frag");

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

	program.bindAttr();
	program.enableAttr("vertices");
	program.enableAttr("texCoords");
	program.unbindAttr();
	program.disableAttr("vertices");
	program.disableAttr("texCoords");

	program.unbind();
}

// Attributes
void HUBShader::verticesPointer() {
	this->pointerAttr("vertices", 3, GL_FLOAT);
}

void HUBShader::texCoordPointer() {
	program.pointerAttr("texCoords", 2, GL_FLOAT);
}

// Uniforms
void HUBShader::setProjection(const glm::mat4& proj) {
	program.setMat4("proj", proj);
}

void HUBShader::setView(const glm::mat4& view) {
	program.setMat4("view", view);
}

void HUBShader::setModel(const glm::mat4& model) {
	program.setMat4("model", model);
}


// WaterShader
void WaterShader::init() {
	vertex.init(GL_VERTEX_SHADER, "data/shaders/water.vert");
	fragment.init(GL_FRAGMENT_SHADER, "data/shaders/water.frag");

	program.addShader(&vertex);
	program.addShader(&fragment);

	program.init();

	program.bind();

	program.createUniform("proj");
	program.createUniform("view");
	program.createUniform("model");
	program.createUniform("tex0");
	program.set1i("tex0", 0);
	program.createUniform("texScale");
	this->setTexScale(1.0f);
	program.createUniform("timeDelta");
	program.set1f("timeDelta", 0.0f);

	program.setAttr("vertices", 0);
	program.setAttr("texCoords", 1);
	program.setAttr("normals", 2);

	program.bindAttr();
	program.enableAttr("vertices");
	program.enableAttr("texCoords");
	program.enableAttr("normals");
	program.unbindAttr();
	program.disableAttr("vertices");
	program.disableAttr("texCoords");
	program.disableAttr("normals");

	program.unbind();
}

// Attributes
void WaterShader::verticePointer() {
	program.pointerAttr("vertices", 3, GL_FLOAT);
}

void WaterShader::texCoordPointer() {
	program.pointerAttr("texCoords", 2, GL_FLOAT);
}

void WaterShader::normalPointer() {
	program.pointerAttr("normals", 3, GL_FLOAT);
}

// Uniforms
void WaterShader::setProjection(const glm::mat4& proj) {
	program.setMat4("proj", proj);
}

void WaterShader::setView(const glm::mat4& view) {
	program.setMat4("view", view);
}

void WaterShader::setModel(const glm::mat4& model) {
	program.setMat4("model", model);
}

void WaterShader::setCamera(Camera* camera) {
	this->setProjection(camera->getProjection());
	this->setView(camera->getView());
}

void WaterShader::setTexScale(float scale) {
	program.set1f("texScale", scale);
}

void WaterShader::setTimeDelta(float timeDelta) {
	program.set1f("timeDelta", timeDelta);
}

// Blur Shader
void BlurPreProcessShader::init() {

}

// Attributes
void BlurPreProcessShader::verticesPointer() {
	this->pointerAttr("vertices", 3, GL_FLOAT);
}

void BlurPreProcessShader::texCoordPointer() {

}

// Uniforms
void BlurPreProcessShader::setProj(const glm::mat4& proj) {

}

void BlurPreProcessShader::setView(const glm::mat4& view) {

}

void BlurPreProcessShader::setModel(const glm::mat4& model) {

}

void BlurPreProcessShader::setSize(float size) {

}

// ShaderManager
// Regular Shaders
SceneShader ShaderManager::sceneShader;
TerrainShader ShaderManager::terrainShader;
HUBShader ShaderManager::hubShader;
WaterShader ShaderManager::waterShader;

// Pre Process Shaders
BlurPreProcessShader ShaderManager::blurPreProcessShader;

void ShaderManager::init() {
	sceneShader.init();
	terrainShader.init();
	hubShader.init();
	waterShader.init();
	blurPreProcessShader.init();
}

void ShaderManager::release() {
	blurPreProcessShader.release();
	waterShader.release();
	hubShader.release();
	terrainShader.release();
	sceneShader.release();
}