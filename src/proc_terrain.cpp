#include "sys.h"


void ProcTerrain::init() {
	this->seed = rand() % UINT_MAX;
	uint32_t r = rand() % 2;


	// Resize Maps
	float size2 = this->size * this->size;

	this->elevation.resize(size2);
	this->mask.resize(size2);
	this->maskedElevation.resize(size2);
	this->moister.resize(size2);
	this->blend.resize(size2);
	this->biomes.resize(size2);
	this->terrainType.resize(size2);


	for (int y = 0; y < this->size; y++) {
		for (int x = 0; x < this->size; x++) {
			int i = y * this->size + x;

			// Elevation
			float n1 = (glm::perlin(glm::vec3(x / this->mainWave.wave, y / this->mainWave.wave, seed)) + 1.0f) * 0.5f;
			float n2 = (glm::perlin(glm::vec3(x / this->secondaryWave.wave, y / this->secondaryWave.wave, seed + 1)) + 1.0f) * 0.5f;
			float n3 = (glm::perlin(glm::vec3(x / this->trinaryWave.wave, y / this->trinaryWave.wave, seed + 2)) + 1.0f) * 0.5f;

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

			float t1 = ((glm::perlin(glm::vec3(x / 32.0f, y / 32.0f, seed + 3)) + 1.0f) * 0.5f) * me;
			float t2 = ((glm::perlin(glm::vec3(x / 16.0f, y / 16.0f, seed + 4)) + 1.0f) * 0.5f) * me;
			float t3 = ((glm::perlin(glm::vec3(x / 8.0f, y / 8.0f, seed + 5)) + 1.0f) * 0.5f) * me;
			float temp = t1 * 0.5f + t2 * 0.35f + t3 * 0.15f;
			
			mo.y = (me > 0.3f) ? temp : 0.0f;

			moister[i] = mo;

			// Blend Map
			if (me <= 0.25f) {
				this->blend[i] = glm::vec3(0.0f, 0.0f, 0.0f);
			}

			else if (me > 0.25f) {
				float t = glm::length(moister[i]);

				if (t >= 0.25f && t < 0.35f) {

				}
			}

			// Biome Map
		}
	}
}

void ProcTerrain::release() {

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