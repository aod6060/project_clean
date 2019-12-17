#pragma once

enum ProcTerrainType {
	PTT_OCEAN = 0,
	PTT_BEACH,
	PTT_GRASS,
	PTT_TREE
};

enum ProcTerrainMaskType {
	PTMT_SPHERE_GRADIENT = 0,
	PTMT_QUAD_GRADIENT
};

struct ProcTerrainHeightmapWave {
	float wave;
	float weight;
};

struct ProcTerrain {
	// Proc Terrain
	uint32_t size = 1024.0f;

	// Height Map Generation
	uint32_t seed = 0;

	// Proc Terrain Heightmap Waves

	// Main Wave
	ProcTerrainHeightmapWave mainWave = {
		128.0f,
		0.5f
	};

	// Secondary Wave
	ProcTerrainHeightmapWave secondaryWave = {
		64.0f,
		0.35f
	};

	// Trinary Wave
	ProcTerrainHeightmapWave trinaryWave = {
		16.0f,
		0.15f
	};

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
	Texture2D blendMap;
	Texture2D biomesMap;

	void init();
	void release();

	float toMask(float x, float y, float radius, ProcTerrainMaskType type);
};