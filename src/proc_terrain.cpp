#include "sys.h"


#define RGB_COLOR(r, g, b) (r | g << 8 | b << 16 | 255 << 24)

void ProcTerrain::init() {
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

	std::cout << this->mainWave.wave << std::endl;
	std::cout << this->mainWave.weight << std::endl;

	float seedf = seed;

	// Generate Map
	for (int y = 0; y < this->size; y++) {
		for (int x = 0; x < this->size; x++) {
			int i = y * this->size + x;

			// Elevation
			float n1 = (glm::perlin(glm::vec3(x / this->mainWave.wave, y / this->mainWave.wave, seedf)) + 1.0f) * 0.5f;
			float n2 = (glm::perlin(glm::vec3(x / this->secondaryWave.wave, y / this->secondaryWave.wave, seedf + 1)) + 1.0f) * 0.5f;
			float n3 = (glm::perlin(glm::vec3(x / this->trinaryWave.wave, y / this->trinaryWave.wave, seedf + 2)) + 1.0f) * 0.5f;

			float n =
				n1 * mainWave.weight +
				n2 * secondaryWave.weight +
				n3 * trinaryWave.weight;


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

			float t1 = ((glm::perlin(glm::vec3(x / mainMoisterWave.wave, y / mainMoisterWave.wave, seed + 3)) + 1.0f) * 0.5f) * me;
			float t2 = ((glm::perlin(glm::vec3(x / secondaryMoisterWave.wave, y / secondaryMoisterWave.wave, seed + 4)) + 1.0f) * 0.5f) * me;
			float t3 = ((glm::perlin(glm::vec3(x / trinaryMoisterWave.wave, y / trinaryMoisterWave.wave, seed + 5)) + 1.0f) * 0.5f) * me;
			float temp = 
				t1 * mainMoisterWave.weight + 
				t2 * secondaryMoisterWave.weight + 
				t3 * trinaryMoisterWave.weight;
			
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