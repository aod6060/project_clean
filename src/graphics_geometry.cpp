#include "sys.h"

// AbstractTestShaderGeometry
void AbstractTestShaderGeometry::render(TestShader* shader) {
	shader->bindAttr();

	this->vertices.bind();
	shader->verticesPointer();
	this->vertices.unbind();

	this->texCoords.bind();
	shader->texCoordPointer();
	this->texCoords.unbind();

	this->indencies.bind();
	glDrawElements(GL_TRIANGLES, indencies.size(), GL_UNSIGNED_INT, 0);
	this->indencies.unbind();

	shader->unbindAttr();
}

void AbstractTestShaderGeometry::release() {
	indencies.release();
	this->texCoords.release();
	this->vertices.release();
}

void AbstractTestShaderGeometry::getVertices(std::vector<glm::vec3>& vertices) {

	for (int i = 0; i < this->vertices.list.size(); i += 3) {
		float x = this->vertices.list[i];
		float y = this->vertices.list[i + 1];
		float z = this->vertices.list[i + 2];

		vertices.push_back(glm::vec3(x, y, z));
	}
}

// QuadTestShaderGeomentry
void QuadTestShaderGeomentry::init() {
	this->vertices.init();
	vertices.set3f(-1.0f, 1.0f, 0.0f);
	vertices.set3f(1.0f, 1.0f, 0.0f);
	vertices.set3f(-1.0f, -1.0f, 0.0f);
	vertices.set3f(1.0f, -1.0f, 0.0f);
	this->vertices.update();

	this->texCoords.init();
	this->texCoords.set2f(0.0f, 0.0f);
	this->texCoords.set2f(1.0f, 0.0f);
	this->texCoords.set2f(0.0f, 1.0f);
	this->texCoords.set2f(1.0f, 1.0f);
	this->texCoords.update();

	this->indencies.init();
	this->indencies.set3f(0, 1, 2);
	this->indencies.set3f(2, 1, 3);
	this->indencies.update();
}