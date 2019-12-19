#include "sys.h"


#define RGB_COLOR(r, g, b) (r | g << 8 | b << 16 | 255 << 24)

#define PROC_TERRAIN_TYPE "terrain"
#define PROC_TERRAIN_VERSION "1.0"

void ProcTerrain::loadConfig(std::string path) {
	std::ifstream in(path);

	Json::Value root;

	in >> root;

	Json::Value type = root["type"];
	Json::Value version = root["version"];

	if (type.asString() == PROC_TERRAIN_TYPE && version.asString() == PROC_TERRAIN_VERSION) {
		// Elevation Waves
		Json::Value elevationWaves = root["elevation-waves"];
		for (int i = 0; i < elevationWaves.size(); i++) {
			Json::Value temp = elevationWaves[i];

			ProcTerrainHeightmapWave wave;

			wave.wave = temp["wave"].asFloat();
			wave.weight = temp["weight"].asFloat();
			wave.seed = temp["seed"].asFloat();
			wave.noise = (temp["noise"].asString() == "perlin") ? PTWT_PERLIN : PTWT_SIMPLEX;

			this->elevationWaves.push_back(wave);
		}

		// Moister Waves
		Json::Value moisterWaves = root["moister-waves"];
		for (int i = 0; i < moisterWaves.size(); i++) {
			Json::Value temp = moisterWaves[i];

			ProcTerrainHeightmapWave wave;

			wave.wave = temp["wave"].asFloat();
			wave.weight = temp["weight"].asFloat();
			wave.seed = temp["seed"].asFloat();
			wave.noise = (temp["noise"].asString() == "perlin") ? PTWT_PERLIN : PTWT_SIMPLEX;

			this->moisterWaves.push_back(wave);
		}

		// Levels
		Json::Value levels = root["levels"];

		this->beachLevel = levels["beach"].asFloat();
		this->grassLevel = levels["grass"].asFloat();
		this->forestLevel = levels["forest"].asFloat();
	}

	in.close();
}

void ProcTerrain::init(std::string path) {
	this->loadConfig(path);

	this->seed = rand() % UINT_MAX;
	uint32_t r = rand() % 2;


	// Resize Maps
	uint32_t size2 = this->size * this->size;

	this->elevation.resize(size2);
	this->mask.resize(size2);
	this->maskedElevation.resize(size2);
	this->moister.resize(size2);
	this->blend.resize(size2);
	this->biomes.resize(size2);
	this->terrainType.resize(size2);

	float seedf = seed;

	// Generate Map
	for (int y = 0; y < this->size; y++) {
		for (int x = 0; x < this->size; x++) {
			int i = y * this->size + x;

			/*
			// Elevation
			float n1 = (glm::perlin(glm::vec3(x / this->mainWave.wave, y / this->mainWave.wave, seedf)) + 1.0f) * 0.5f;
			float n2 = (glm::perlin(glm::vec3(x / this->secondaryWave.wave, y / this->secondaryWave.wave, seedf + 1)) + 1.0f) * 0.5f;
			float n3 = (glm::perlin(glm::vec3(x / this->trinaryWave.wave, y / this->trinaryWave.wave, seedf + 2)) + 1.0f) * 0.5f;

			float n =
				n1 * mainWave.weight +
				n2 * secondaryWave.weight +
				n3 * trinaryWave.weight;

			*/

			// Elevation
			float n = 0;

			std::for_each(this->elevationWaves.begin(), this->elevationWaves.end(), [&](ProcTerrainHeightmapWave& wave) {
				if (wave.noise == PTWT_PERLIN) {
					float nt = (glm::perlin(glm::vec3(x / wave.wave, y / wave.wave, seedf + wave.seed)) + 1.0f) * 0.5f;
					n += nt * wave.weight;
				}
				else if (wave.noise == PTWT_SIMPLEX) {
					float nt = (glm::simplex(glm::vec3(x / wave.wave, y / wave.wave, seedf + wave.seed)) + 1.0f) * 0.5f;
					n += nt * wave.weight;
				}
			});

			elevation[i] = glm::clamp(
				n,
				0.0f,
				1.0f
			);

			// Island Mask
			mask[i] = this->toMask(x, y, 10.0f, (ProcTerrainMaskType)r);

			// Masked Elevation
			float me = elevation[i] * mask[i];

			maskedElevation[i] = me;

			// Moister Map
			glm::vec2 mo;
			mo.x = (me > 0.25f) ? me : 0.0f;

			/*
			float t1 = ((glm::perlin(glm::vec3(x / mainMoisterWave.wave, y / mainMoisterWave.wave, seed + 3)) + 1.0f) * 0.5f) * me;
			float t2 = ((glm::perlin(glm::vec3(x / secondaryMoisterWave.wave, y / secondaryMoisterWave.wave, seed + 4)) + 1.0f) * 0.5f) * me;
			float t3 = ((glm::perlin(glm::vec3(x / trinaryMoisterWave.wave, y / trinaryMoisterWave.wave, seed + 5)) + 1.0f) * 0.5f) * me;
			float temp = 
				t1 * mainMoisterWave.weight + 
				t2 * secondaryMoisterWave.weight + 
				t3 * trinaryMoisterWave.weight;
			*/

			float temp = 0;

			std::for_each(this->moisterWaves.begin(), this->moisterWaves.end(), [&](ProcTerrainHeightmapWave& wave) {
				if (wave.noise == PTWT_PERLIN) {
					float tt = ((glm::perlin(glm::vec3(x / wave.wave, y / wave.wave, seedf + wave.seed)) + 1.0f) * 0.5f) * me;
					temp += tt * wave.weight;
				}
				else if (wave.noise == PTWT_SIMPLEX) {
					float tt = ((glm::simplex(glm::vec3(x / wave.wave, y / wave.wave, seedf + wave.seed)) + 1.0f) * 0.5f) * me;
					temp += tt * wave.weight;
				}
			});

			/*
			temp = glm::clamp(
				temp,
				0.0f,
				1.0f
			);
			*/

			mo.y = (me > 0.3f) ? temp : 0.0f;

			moister[i] = mo;

			// Blend Map
			if (me <= this->beachLevel) {
				this->blend[i] = glm::vec3(0.0f, 0.0f, 0.0f);
			}
			else if (me > this->beachLevel) {
				float t = glm::length(moister[i]);

				if (t >= this->beachLevel && t < this->grassLevel) {
					this->blend[i] = glm::vec3(1.0f, 0.0f, 0.0f);
				}
				else if (t >= this->grassLevel && t < this->forestLevel) {
					this->blend[i] = glm::vec3(0.0f, 1.0f, 0.0f);
				}
				else if (t >= this->forestLevel) {
					this->blend[i] = glm::vec3(0.0f, 0.0f, 1.0f);
				}
			}

			// Biome Map
			if (me <= this->beachLevel) {
				this->biomes[i] = glm::vec3(0.0f, 0.0f, 1.0f);
				this->terrainType[i] = PTT_OCEAN;
			}
			else if (me > this->beachLevel) {
				float t = glm::length(moister[i]);
				if (t >= this->beachLevel && t < this->grassLevel) {
					this->biomes[i] = glm::vec3(194.0f, 256.0f, 128.0f) / 256.0f;
					this->terrainType[i] = PTT_BEACH;
				}
				else if (t >= this->grassLevel && t < this->forestLevel) {
					this->biomes[i] = glm::vec3(0.3f, 1.0f, 0.3f);
					this->terrainType[i] = PTT_GRASS;
				}
				else if (t >= 0.5f) {
					this->biomes[i] = glm::vec3(139.0f, 69.0f, 19.0f) / 256.0f;
					this->terrainType[i] = PTT_FOREST;
				}
			}
		}
	}


	std::vector<uint32_t> elevation_pixels(size2);
	std::vector<uint32_t> mask_pixels(size2);
	std::vector<uint32_t> mask_elevation_pixels(size2);
	std::vector<uint32_t> moister_pixels(size2);
	std::vector<uint32_t> blend_map_pixels(size2);
	std::vector<uint32_t> biome_pixels(size2);


	for (int y = 0; y < this->size; y++) {
		for (int x = 0; x < this->size; x++) {
			int i = y * this->size + x;

			// Elevation
			uint8_t e = this->elevation[i] * 255;
			elevation_pixels[i] = RGB_COLOR(e, e, e);

			// Mask
			uint8_t m = this->mask[i] * 255;
			mask_pixels[i] = RGB_COLOR(m, m, m);

			// Masked Elevation
			uint8_t me = this->maskedElevation[i] * 255;
			mask_elevation_pixels[i] = RGB_COLOR(me, me, me);

			// Moister
			uint8_t mor = this->moister[i].r * 255;
			uint8_t mog = this->moister[i].g * 255;
			moister_pixels[i] = RGB_COLOR(mor, 0, mog);

			// Blend Map
			uint8_t br = this->blend[i].r * 255;
			uint8_t bg = this->blend[i].g * 255;
			uint8_t bb = this->blend[i].b * 255;
			blend_map_pixels[i] = RGB_COLOR(br, bg, bb);

			// Biome
			uint8_t bmr = this->biomes[i].r * 255;
			uint8_t bmg = this->biomes[i].g * 255;
			uint8_t bmb = this->biomes[i].b * 255;
			biome_pixels[i] = RGB_COLOR(bmr, bmg, bmb);

		}
	}

	// Textures
	// Elevation
	this->elevationTex.init(
		size,
		size,
		4,
		elevation_pixels.data()
	);

	// Mask
	this->maskTex.init(
		size,
		size,
		4,
		mask_pixels.data()
	);

	// Masked Elevation
	this->maskedElevationTex.init(
		size,
		size,
		4,
		mask_elevation_pixels.data()
	);

	// Moister
	this->moisterTex.init(
		size,
		size,
		4,
		moister_pixels.data()
	);

	// Blend Map
	this->blendMapTex.init(
		size,
		size,
		4,
		blend_map_pixels.data()
	);

	// Biome
	this->biomesMapTex.init(
		this->size,
		this->size,
		4,
		biome_pixels.data()
	);

}

void ProcTerrain::release() {
	elevationTex.release();
	maskTex.release();
	maskedElevationTex.release();
	moisterTex.release();
	blendMapTex.release();
	biomesMapTex.release();
}

float ProcTerrain::toMask(float x, float y, float radius, ProcTerrainMaskType type) {

	if (type == ProcTerrainMaskType::PTMT_SPHERE_GRADIENT) {
		float distX = fabs(x - size * 0.5f);
		float distY = fabs(y - size * 0.5f);
		float dist = sqrt(distX * distX + distY * distY);

		float max_width = size * 0.5f - radius;
		float delta = dist / max_width;
		float gradient = delta * delta;

		return fmax(0.0f, 1.0f - gradient);
	}
	else if(type == ProcTerrainMaskType::PTMT_QUAD_GRADIENT) {
		float distX = fabs(x - size * 0.5f);
		float distY = fabs(y - size * 0.5f);
		float dist = fmax(distX, distY);

		float max_width = size * 0.5f - radius;
		float delta = dist / max_width;
		float gradient = delta * delta;

		return fmax(0.0f, 1.0f - gradient);
	}
	return 1.0f;
}


void ProcTerrainGeometry::init() {
	//this->data.loadConfig("data/terrain/regular.json");
	this->data.init("data/terrain/regular.json");

	this->width = this->data.size;
	this->height = this->data.size;

	this->heights.resize(this->width * this->height);

	// Create Heights
	float halfWidth = this->width * 0.5f;
	float halfHeight = this->height * 0.5f;

	v.resize(this->width * this->height);

	for (int y = 0; y < this->height; y++) {
		for (int x = 0; x < this->width; x++) {
			float height = this->data.maskedElevation[y * this->width + x] * this->heightScale;
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

	// Create Triangles
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

	// Create Indencies and Face Normals
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
			vertices.set3f(v[y*this->width + x]);

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
}

void ProcTerrainGeometry::render(TerrainShader* shader) {
	data.blendMapTex.bind();
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
	glDrawElements(GL_TRIANGLES, indinces.size(), GL_UNSIGNED_INT, 0);
	indinces.unbind();

	shader->unbindAttr();

	data.blendMapTex.unbind();
	blackChannel->unbind(GL_TEXTURE1);
	redChannel->unbind(GL_TEXTURE2);
	greenChannel->unbind(GL_TEXTURE3);
	blueChannel->unbind(GL_TEXTURE4);
}

void ProcTerrainGeometry::release() {
	blackChannel = nullptr;
	redChannel = nullptr;
	greenChannel = nullptr;
	blueChannel = nullptr;

	this->indinces.release();
	this->normals.release();
	this->texCoords.release();
	this->vertices.release();

	this->data.release();
}

void ProcTerrainGeometry::setBlackChannel(Texture2D* channel) {
	this->blackChannel = channel;
}

void ProcTerrainGeometry::setRedChannel(Texture2D* channel) {
	this->redChannel = channel;
}

void ProcTerrainGeometry::setGreenChannel(Texture2D* channel) {
	this->greenChannel = channel;
}

void ProcTerrainGeometry::setBlueChannel(Texture2D* channel) {
	this->blueChannel = channel;
}

void ProcTerrainGeometry::setHeightScale(float scale) {
	this->heightScale = scale;
}
