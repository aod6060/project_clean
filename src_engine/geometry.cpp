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
	RenderSystem::drawElements(GL_TRIANGLES, indencies.size(), GL_UNSIGNED_INT);
	indencies.unbind();

	shader->unbindAttr();
}

void SceneGeometry::MeshGeometry::release() {
	indencies.release();
	normals.release();
	texCoords.release();
	vertices.release();

	f.clear();
	v.clear();

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

	this->geometry.clear();
}



// TerrainGeometry
void StaticTerrainGeometry::setHeightMapFilePath(std::string path) {
	this->heightMapFilePath = path;
}

void StaticTerrainGeometry::setHeightScale(float scale) {
	this->heightScale = scale;
}

void StaticTerrainGeometry::setBlendMap(std::string path) {
	this->blendMapPath = path;
}

void StaticTerrainGeometry::setBlackChannel(Texture2D* channel) {
	this->blackChannel = channel;
}

void StaticTerrainGeometry::setRedChannel(Texture2D* channel) {
	this->redChannel = channel;
}

void StaticTerrainGeometry::setGreenChannel(Texture2D* channel) {
	this->greenChannel = channel;
}

void StaticTerrainGeometry::setBlueChannel(Texture2D* channel) {
	this->blueChannel = channel;
}

void StaticTerrainGeometry::init() {
	SDL_Surface* surf = IMG_Load(this->heightMapFilePath.c_str());


	if (surf == nullptr) {
		logger_output("%: doesn't exist...", this->heightMapFilePath.c_str());
		return;
	}


	this->width = surf->w;
	this->height = surf->h;

	float halfWidth = this->width * 0.5f;
	float halfHeight = this->height * 0.5f;

	this->heights.resize(this->width * this->height);

	v.resize(this->width * this->height);

	SDL_LockSurface(surf);

	SDL_Color* colors = (SDL_Color*)surf->pixels;

	// Create Heights
	for (int y = 0; y < this->height; y++) {
		for (int x = 0; x < this->width; x++) {
			SDL_Color color = colors[y * this->width + x];

			float height = ((float)color.r / 256.0f) * this->heightScale;

			this->heights[y * this->width + x] = height;

			v[y * this->width + x] = glm::vec3(x - halfWidth, height, y - halfHeight);

		}
	}

	struct Triangle {
		uint32_t p1;
		uint32_t p2;
		uint32_t p3;
	};

	std::vector<Triangle> triangles;

	for (int y = 0; y < this->height - 1; y++) {
		for (int x = 0; x < this->width - 1; x++) {
			Triangle t1;
			Triangle t2;

			t1.p1 = y * this->width + x;
			t1.p2 = y * this->width + (x + 1);
			t1.p3 = (y + 1) * this->width + x;

			t2.p1 = (y + 1) * this->width + x;
			t2.p2 = y * this->width + (x + 1);
			t2.p3 = (y + 1) * this->width + (x + 1);

			triangles.push_back(t1);
			triangles.push_back(t2);
		}
	}

	std::vector<glm::vec3> normals(this->width * this->height);

	// Create Indexes and Face Normals
	this->indinces.init();
	for (int i = 0; i < triangles.size(); i++) {
		glm::vec3 U;
		glm::vec3 V;

		glm::vec3 N;

		U = v[triangles[i].p2] - v[triangles[i].p1];
		V = v[triangles[i].p3] - v[triangles[i].p1];

		N = glm::cross(U, V);

		N = glm::normalize(N);

		normals[triangles[i].p1] += N;
		normals[triangles[i].p2] += N;
		normals[triangles[i].p3] += N;


		// Faces
		this->indinces.set3f(
			triangles[i].p1,
			triangles[i].p2,
			triangles[i].p3
		);

	}
	this->indinces.update();

	// Create Vertices

	vertices.init();
	texCoords.init();
	this->normals.init();
	for (int y = 0; y < this->height; y++) {
		for (int x = 0; x < this->width; x++) {

			float height = this->heights[y * this->width + x];

			// Vertices
			//v[y * this->width + x] = glm::vec3(x - halfWidth, height, y - halfHeight);

			vertices.set3f(v[y * this->width + x]);

			float tx = ((float)x / (float)this->width);
			float ty = ((float)y / (float)this->height);

			texCoords.set2f(tx, ty);

			glm::vec3 n = glm::normalize(normals[y * this->width + x]);

			this->normals.set3f(n);
		}
	}
	vertices.update();
	texCoords.update();
	this->normals.update();

	// Create Indencies

	SDL_UnlockSurface(surf);

	SDL_FreeSurface(surf);


	//this->blendMap.loadTexture(this->blendMapPath);

}

void StaticTerrainGeometry::render(TerrainShader* shader) {

	//blendMap.bind();
	blendMap->bind();
	blackChannel->bind(GL_TEXTURE1);
	redChannel->bind(GL_TEXTURE2);
	greenChannel->bind(GL_TEXTURE3);
	blueChannel->bind(GL_TEXTURE4);

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

	indinces.bind();
	RenderSystem::drawElements(GL_TRIANGLES, indinces.size(), GL_UNSIGNED_INT);
	indinces.unbind();

	shader->unbindAttr();

	//blendMap.unbind();
	blendMap->unbind();
	blackChannel->unbind(GL_TEXTURE1);
	redChannel->unbind(GL_TEXTURE2);
	greenChannel->unbind(GL_TEXTURE3);
	blueChannel->unbind(GL_TEXTURE4);

}

void StaticTerrainGeometry::release() {
	blackChannel = nullptr;
	redChannel = nullptr;
	greenChannel = nullptr;
	blueChannel = nullptr;

	//this->blendMap.release();
	this->indinces.release();
	this->normals.release();
	this->texCoords.release();
	this->vertices.release();
}

void StaticTerrainGeometry::setBlendMap(Texture2D* blendMap) {
	this->blendMap = blendMap;
}

// HUB Geometry
void QuadHUBGeometry::init() {
	// Vertices
	vertices.init();
	vertices.set3f(0.0f, 0.0f, 0.0f);
	vertices.set3f(1.0f, 0.0f, 0.0f);
	vertices.set3f(0.0f, 1.0f, 0.0f);
	vertices.set3f(1.0f, 1.0f, 0.0f);
	vertices.update();
	// TexCoords
	texCoords.init();
	texCoords.set2f(0.0f, 0.0f);
	texCoords.set2f(1.0f, 0.0f);
	texCoords.set2f(0.0f, 1.0f);
	texCoords.set2f(1.0f, 1.0f);
	texCoords.update();
	// Indencies
	indencies.init();
	indencies.set3f(0, 1, 2);
	indencies.set3f(2, 1, 3);
	indencies.update();
}

void QuadHUBGeometry::render(HUBShader* shader) {
	shader->bindAttr();
	vertices.bind();
	shader->verticesPointer();
	vertices.unbind();
	texCoords.bind();
	shader->texCoordPointer();
	texCoords.unbind();

	
	indencies.bind();
	RenderSystem::drawElements(GL_TRIANGLES, indencies.size(), GL_UNSIGNED_INT);
	indencies.unbind();
	shader->unbindAttr();
}

void QuadHUBGeometry::release() {
	this->indencies.release();
	this->texCoords.release();
	this->vertices.release();
}


// WaterGeometry
void WaterGeometry::init() {
	// Vertices
	vertices.init();
	vertices.set3f(-1.0f, 0.0f, 1.0f);
	vertices.set3f(1.0f, 0.0f, 1.0f);
	vertices.set3f(-1.0f, 0.0f, -1.0f);
	vertices.set3f(1.0f, 0.0f, -1.0f);
	vertices.update();

	// TexCoords
	texCoords.init();
	texCoords.set2f(0.0f, 0.0f);
	texCoords.set2f(1.0f, 0.0f);
	texCoords.set2f(0.0f, 1.0f);
	texCoords.set2f(1.0f, 1.0f);
	texCoords.update();
	
	// Normals
	normals.init();
	normals.set3f(0.0f, 1.0f, 0.0f);
	normals.set3f(0.0f, 1.0f, 0.0f);
	normals.set3f(0.0f, 1.0f, 0.0f);
	normals.set3f(0.0f, 1.0f, 0.0f);
	normals.update();

	// Indencies
	indencies.init();
	indencies.set3f(0, 1, 2);
	indencies.set3f(2, 1, 3);
	indencies.update();
}

void WaterGeometry::render(WaterShader* shader) {
	shader->bindAttr();

	vertices.bind();
	shader->verticePointer();
	vertices.unbind();

	texCoords.bind();
	shader->texCoordPointer();
	texCoords.unbind();

	normals.bind();
	shader->normalPointer();
	normals.unbind();

	indencies.bind();
	RenderSystem::drawElements(GL_TRIANGLES, indencies.size(), GL_UNSIGNED_INT);
	indencies.unbind();

	shader->unbindAttr();
}

void WaterGeometry::release() {
	indencies.release();
	normals.release();
	texCoords.release();
	vertices.release();
}

void QuadBlurPreProcessGeometry::init() {
	vertices.init();
	vertices.set3f(0.0f, 0.0f, 0.0f);
	vertices.set3f(1.0f, 0.0f, 0.0f);
	vertices.set3f(0.0f, 1.0f, 0.0f);
	vertices.set3f(1.0f, 1.0f, 0.0f);
	vertices.update();

	texCoords.init();
	texCoords.set2f(0.0f, 0.0f);
	texCoords.set2f(1.0f, 0.0f);
	texCoords.set2f(0.0f, 1.0f);
	texCoords.set2f(1.0f, 1.0f);
	texCoords.update();

	indencies.init();
	indencies.set3f(0, 1, 2);
	indencies.set3f(2, 1, 3);
	indencies.update();
}

void QuadBlurPreProcessGeometry::render(BlurPreProcessShader* shader) {
	shader->bindAttr();

	vertices.bind();
	shader->verticesPointer();
	vertices.unbind();

	texCoords.bind();
	shader->texCoordPointer();
	texCoords.unbind();

	indencies.bind();
	RenderSystem::drawElements(GL_TRIANGLES, indencies.size(), GL_UNSIGNED_INT);
	indencies.unbind();

	shader->unbindAttr();
}

void QuadBlurPreProcessGeometry::release() {
	indencies.release();
	texCoords.release();
	vertices.release();
}


void QuadFontRenderGeometry::init() {
	vertices.init();
	vertices.set3f(0.0f, 0.0f, 0.0f);
	vertices.set3f(1.0f, 0.0f, 0.0f);
	vertices.set3f(0.0f, 1.0f, 0.0f);
	vertices.set3f(1.0f, 1.0f, 0.0f);
	vertices.update();

	texCoords.init();
	texCoords.set2f(0.0f, 0.0f);
	texCoords.set2f(1.0f, 0.0f);
	texCoords.set2f(0.0f, 1.0f);
	texCoords.set2f(1.0f, 1.0f);
	texCoords.update();

	indencies.init();
	indencies.set3f(0, 1, 2);
	indencies.set3f(2, 1, 3);
	indencies.update();
}

void QuadFontRenderGeometry::render(FontRenderShader* shader) {
	shader->bindAttr();

	vertices.bind();
	shader->verticesPointer();
	vertices.unbind();

	texCoords.bind();
	shader->texCoordsPointer();
	texCoords.unbind();

	indencies.bind();
	RenderSystem::drawElements(GL_TRIANGLES, indencies.size(), GL_UNSIGNED_INT);
	indencies.unbind();

	shader->unbindAttr();
}

void QuadFontRenderGeometry::release() {
	indencies.release();
	texCoords.release();
	vertices.release();
}



// QuadUIGeometry
void QuadUIGeometry::init() {
	vertices.init();
	vertices.set3f(0.0f, 0.0f, 0.0f);
	vertices.set3f(1.0f, 0.0f, 0.0f);
	vertices.set3f(0.0f, 1.0f, 0.0f);
	vertices.set3f(1.0f, 1.0f, 0.0f);
	vertices.update();

	indencies.init();
	indencies.set3f(0, 1, 2);
	indencies.set3f(2, 1, 3);
	indencies.update();
}

void QuadUIGeometry::render(UIShader* shader) {
	shader->bindAttr();

	vertices.bind();
	shader->verticesPointer();
	vertices.unbind();

	indencies.bind();
	RenderSystem::drawElements(GL_TRIANGLES, vertices.size(), GL_UNSIGNED_INT);
	indencies.unbind();

	shader->unbindAttr();
}

void QuadUIGeometry::release() {
	indencies.release();
	vertices.release();
}

