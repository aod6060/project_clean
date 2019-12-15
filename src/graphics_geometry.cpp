#include "sys.h"

// MeshGeometry
void SceneGeometry::MeshGeometry::setParent(SceneGeometry* parent) {
	this->parent = parent;
}

void SceneGeometry::MeshGeometry::init() {

	// Veretx Buffers
	this->vertices.init();
	this->texCoords.init();
	this->normals.init();
	std::for_each(this->v.begin(), this->v.end(), [&](Vertex& vertex) {
		vertices.set3f(vertex.position);
		texCoords.set2f(vertex.texCoord);
		normals.set3f(vertex.normal);
	});
	this->vertices.update();
	this->texCoords.update();
	this->normals.update();

	// Index Buffer
	this->indencies.init();
	std::for_each(f.begin(), f.end(), [&](Face& face) {
		indencies.set3f(face.p1, face.p2, face.p3);
	});
	this->indencies.update();
}

void SceneGeometry::MeshGeometry::render(SceneShader* shader) {

	shader->setModel(parent->model * this->matrix);

	shader->bindAttr();

	vertices.bind();
	shader->verticesPointer();
	vertices.unbind();

	texCoords.bind();
	shader->texCoordPointer();
	texCoords.unbind();

	normals.bind();
	shader->normalPointer();
	normals.unbind();

	indencies.bind();
	glDrawElements(GL_TRIANGLES, indencies.size(), GL_UNSIGNED_INT, 0);
	indencies.unbind();

	shader->unbindAttr();
}

void SceneGeometry::MeshGeometry::release() {
	indencies.release();
	normals.release();
	texCoords.release();
	vertices.release();

	parent = nullptr;
}

// SceneGeometry
void SceneGeometry::setFilePath(std::string path) {
	this->path = path;
}

void SceneGeometry::setModel(const glm::mat4& model) {
	this->model = model;
}

void SceneGeometry::loadScene(std::string path, Scene& scene) {
	std::ifstream in(path);

	Json::Value root;

	in >> root;

	Json::Value type = root["type"];
	Json::Value version = root["version"];

	if (type.asString() == this->TYPE && version.asString() == this->VERSION) {
		scene.type = type.asString();
		scene.version = version.asString();

		// Loading Meshes
		Json::Value meshes = root["meshes"];

		for (int i = 0; i < meshes.size(); i++) {
			Json::Value temp = meshes[i];
			Mesh mesh;

			// Matrix
			Json::Value matrix = temp["matrix"];
			float m[16];

			for (int i = 0; i < matrix.size(); i++) {
				m[i] = matrix[i].asFloat();
			}

			mesh.matrix = glm::make_mat4(m);

			// Vertices
			Json::Value vertices = temp["vertices"];

			for (int j = 0; j < vertices.size(); j++) {
				Json::Value temp2 = vertices[j];
				Vertex vertex;

				vertex.position = glm::vec3(
					temp2["x"].asFloat(),
					temp2["y"].asFloat(),
					temp2["z"].asFloat()
				);

				//logger_output("Vertex:[%f,%f,%f]\n", vertex.position.x, vertex.position.y, vertex.position.z);

				vertex.texCoord = glm::vec2(
					temp2["tx"].asFloat(),
					temp2["ty"].asFloat()
				);

				//logger_output("TexCoords:[%f,%f]\n", vertex.texCoord.x, vertex.texCoord.y);

				vertex.normal = glm::vec3(
					temp2["nx"].asFloat(),
					temp2["ny"].asFloat(),
					temp2["nz"].asFloat()
				);

				//logger_output("Normal:[%f,%f,%f]\n", vertex.normal.x, vertex.normal.y, vertex.normal.z);

				mesh.vertices.push_back(vertex);
			}

			// Faces
			Json::Value faces = temp["faces"];

			for (int j = 0; j < faces.size(); j++) {
				Json::Value temp2 = faces[j];

				Face face;

				face.p1 = temp2["p1"].asInt();
				face.p2 = temp2["p2"].asInt();
				face.p3 = temp2["p3"].asInt();

				//logger_output("face: [%d, %d, %d]\n", face.p1, face.p2, face.p3);

				mesh.faces.push_back(face);
			}

			scene.meshes.push_back(mesh);
		}
	}

	in.close();
}

void SceneGeometry::init() {
	// Load File and build scene object
	this->loadScene(this->path, this->scene);

	std::for_each(this->scene.meshes.begin(), this->scene.meshes.end(), [&](Mesh& mesh) {
		MeshGeometry meshGeom;

		meshGeom.setParent(this);

		// Matrix
		meshGeom.matrix = mesh.matrix;

		// Vertex
		std::for_each(mesh.vertices.begin(), mesh.vertices.end(), [&](Vertex& vertex) {
			meshGeom.v.push_back(vertex);
		});

		// Face
		std::for_each(mesh.faces.begin(), mesh.faces.end(), [&](Face& face) {
			meshGeom.f.push_back(face);
		});

		this->geometry.push_back(meshGeom);
	});

	std::for_each(this->geometry.begin(), this->geometry.end(), [&](MeshGeometry& geom) {
		geom.init();
	});

}

void SceneGeometry::render(SceneShader* shader) {
	std::for_each(this->geometry.begin(), this->geometry.end(), [&](MeshGeometry& geom) {
		geom.render(shader);
	});
}

void SceneGeometry::release() {
	std::for_each(this->geometry.begin(), this->geometry.end(), [&](MeshGeometry& geom) {
		geom.release();
	});

}
