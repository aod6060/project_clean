#pragma once


// Geometry Section
template<typename T>
struct IGeometry {
	virtual void init() = 0;
	virtual void render(T* shader) = 0;
	virtual void release() = 0;
};

struct SceneGeometry : public IGeometry<SceneShader> {

	const std::string TYPE = "scene";
	const std::string VERSION = "1.0";

	struct Vertex {
		glm::vec3 position;
		glm::vec2 texCoord;
		glm::vec3 normal;
	};

	struct Face {
		uint32_t p1;
		uint32_t p2;
		uint32_t p3;
	};

	struct Mesh {
		glm::mat4 matrix;
		std::vector<Vertex> vertices;
		std::vector<Face> faces;
	};

	struct Scene {
		std::string type;
		std::string version;
		std::vector<Mesh> meshes;
	};

	struct MeshGeometry : public IGeometry<SceneShader> {
		SceneGeometry* parent = nullptr;

		std::vector<Vertex> v;
		std::vector<Face> f;

		glm::mat4 matrix;

		VertexBuffer vertices;
		VertexBuffer texCoords;
		VertexBuffer normals;
		IndexBuffer indencies;

		void setParent(SceneGeometry* parent);

		virtual void init();
		virtual void render(SceneShader* shader);
		virtual void release();
	};


	std::string path;
	glm::mat4 model;
	Scene scene;
	std::vector<MeshGeometry> geometry;

	void setFilePath(std::string path);
	void setModel(const glm::mat4& model);

	void loadScene(std::string path, Scene& scene);

	virtual void init();
	virtual void render(SceneShader* shader);
	virtual void release();
};

struct StaticTerrainGeometry : public IGeometry<TerrainShader> {
	std::string heightMapFilePath;
	float heightScale = 64.0f;
	int width = 0;
	int height = 0;

	std::vector<float> heights;

	std::vector<glm::vec3> v;

	VertexBuffer vertices;
	VertexBuffer texCoords;
	VertexBuffer normals;
	IndexBuffer indinces;

	std::string blendMapPath;

	Texture2D blendMap;

	Texture2D* blackChannel;
	Texture2D* redChannel;
	Texture2D* greenChannel;
	Texture2D* blueChannel;

	void setHeightMapFilePath(std::string path);

	void setBlendMap(std::string path);

	void setBlackChannel(Texture2D* channel);
	void setRedChannel(Texture2D* channel);
	void setGreenChannel(Texture2D* channel);
	void setBlueChannel(Texture2D* channel);

	void setHeightScale(float scale);

	virtual void init();

	virtual void render(TerrainShader* shader);

	virtual void release();

};

struct QuadHUBGeometry : public IGeometry<HUBShader> {

	VertexBuffer vertices;
	VertexBuffer texCoords;
	IndexBuffer indencies;

	virtual void init();
	virtual void render(HUBShader* shader);
	virtual void release();
};

struct WaterGeometry : public IGeometry<WaterShader> {
	VertexBuffer vertices;
	VertexBuffer texCoords;
	VertexBuffer normals;
	IndexBuffer indencies;

	virtual void init();
	virtual void render(WaterShader* shader);
	virtual void release();
};

