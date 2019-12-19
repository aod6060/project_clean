#pragma once

enum ProcTerrainType {
	PTT_OCEAN = 0,
	PTT_BEACH,
	PTT_GRASS,
	PTT_FOREST
};

enum ProcTerrainMaskType {
	PTMT_SPHERE_GRADIENT = 0,
	PTMT_QUAD_GRADIENT
};

enum ProcTerrainWaveType {
	PTWT_PERLIN = 0,
	PTWT_SIMPLEX
};

struct ProcTerrainHeightmapWave {
	float wave;
	float weight;
	float seed;
	ProcTerrainWaveType noise;
};

struct ProcTerrain {
	// Proc Terrain
	uint32_t size = 1024.0f;

	// Height Map Generation
	uint32_t seed = 0;

	// Proc Terrain Heightmap Waves

	std::vector<ProcTerrainHeightmapWave> elevationWaves;
	
	std::vector<ProcTerrainHeightmapWave> moisterWaves;

	float beachLevel = 0.25f;
	float grassLevel = 0.35f;
	float forestLevel = 0.5f;

	std::vector<float> elevation;
	std::vector<float> mask;
	std::vector<float> maskedElevation;
	std::vector<glm::vec2> moister;
	std::vector<glm::vec3> blend;
	std::vector<glm::vec3> biomes;
	std::vector<ProcTerrainType> terrainType;

	Texture2D elevationTex;
	Texture2D maskTex;
	Texture2D maskedElevationTex;
	Texture2D moisterTex;
	Texture2D blendMapTex;
	Texture2D biomesMapTex;

	void loadConfig(std::string path);

	void init(std::string path);

	void release();

	float toMask(float x, float y, float radius, ProcTerrainMaskType type);
};

struct ProcTerrainGeometry : public IGeometry<TerrainShader> {
	ProcTerrain data;
	float heightScale = 64.0f;
	int width = 0;
	int height = 0;

	std::vector<float> heights;
	std::vector<glm::vec3> v;

	VertexBuffer vertices;
	VertexBuffer texCoords;
	VertexBuffer normals;
	IndexBuffer indinces;

	Texture2D* blackChannel;
	Texture2D* redChannel;
	Texture2D* greenChannel;
	Texture2D* blueChannel;

	virtual void init();

	virtual void render(TerrainShader* shader);

	virtual void release();

	void setBlackChannel(Texture2D* channel);
	void setRedChannel(Texture2D* channel);
	void setGreenChannel(Texture2D* channel);
	void setBlueChannel(Texture2D* channel);

	void setHeightScale(float scale);

};