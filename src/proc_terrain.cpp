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
	//uint32_t r = rand() % 2;
	this->isMaskQuad = rand() % 2;

	// Resize Maps
	uint32_t size2 = this->size * this->size;

	// Clear Maps
	this->elevation.clear();
	this->mask.clear();
	this->maskedElevation.clear();
	this->moister.clear();
	this->blend.clear();
	this->biomes.clear();
	this->terrainType.clear();

	// Create Maps
	this->elevation.resize(size2);
	this->mask.resize(size2);
	this->maskedElevation.resize(size2);
	this->moister.resize(size2);
	this->blend.resize(size2);
	this->biomes.resize(size2);
	this->terrainType.resize(size2);

	// Create Pixels
	this->elevation_pixels.resize(size2);
	this->mask_pixels.resize(size2);
	this->mask_elevation_pixels.resize(size2);
	this->moister_pixels.resize(size2);
	this->blend_map_pixels.resize(size2);
	this->biome_pixels.resize(size2);

	this->generate();

	// Textures
	// Elevation
	this->generateTextures();
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

static int proc_thread_callback(void* ptr) {

	//ProcTerrain* terrain = (ProcTerrain*)data;
	ProcTerrainThreadData* data = (ProcTerrainThreadData*)ptr;

	float seedf = (float)data->gen->seed;

	// Generate Map
	for (int y = data->y; y < data->height; y++) {
		for (int x = data->x; x < data->width; x++) {
			int i = y * data->gen->size + x;

			// Elevation
			float n = 0;

			std::for_each(data->gen->elevationWaves.begin(), data->gen->elevationWaves.end(), [&](ProcTerrainHeightmapWave& wave) {
				if (wave.noise == PTWT_PERLIN) {
					float nt = (glm::perlin(glm::vec3(x / wave.wave, y / wave.wave, seedf + wave.seed)) + 1.0f) * 0.5f;
					n += nt * wave.weight;
				}
				else if (wave.noise == PTWT_SIMPLEX) {
					float nt = (glm::simplex(glm::vec3(x / wave.wave, y / wave.wave, seedf + wave.seed)) + 1.0f) * 0.5f;
					n += nt * wave.weight;
				}
			});

			data->gen->elevation[i] = glm::clamp(
				n,
				0.0f,
				1.0f
			);

			// Island Mask
			data->gen->mask[i] = data->gen->toMask(x, y, 10.0f, (ProcTerrainMaskType)data->gen->isMaskQuad);

			// Masked Elevation
			float me = data->gen->elevation[i] * data->gen->mask[i];

			data->gen->maskedElevation[i] = me;

			// Moister Map
			glm::vec2 mo;
			mo.x = (me > 0.25f) ? me : 0.0f;

			float temp = 0;

			std::for_each(data->gen->moisterWaves.begin(), data->gen->moisterWaves.end(), [&](ProcTerrainHeightmapWave& wave) {
				if (wave.noise == PTWT_PERLIN) {
					float tt = ((glm::perlin(glm::vec3(x / wave.wave, y / wave.wave, seedf + wave.seed)) + 1.0f) * 0.5f) * me;
					temp += tt * wave.weight;
				}
				else if (wave.noise == PTWT_SIMPLEX) {
					float tt = ((glm::simplex(glm::vec3(x / wave.wave, y / wave.wave, seedf + wave.seed)) + 1.0f) * 0.5f) * me;
					temp += tt * wave.weight;
				}
			});

			mo.y = (me > 0.3f) ? temp : 0.0f;

			data->gen->moister[i] = mo;

			// Blend Map
			if (me <= data->gen->beachLevel) {
				data->gen->blend[i] = glm::vec3(0.0f, 0.0f, 0.0f);
			}
			else if (me > data->gen->beachLevel) {
				float t = glm::length(data->gen->moister[i]);

				if (t >= data->gen->beachLevel && t < data->gen->grassLevel) {
					data->gen->blend[i] = glm::vec3(1.0f, 0.0f, 0.0f);
				}
				else if (t >= data->gen->grassLevel && t < data->gen->forestLevel) {
					data->gen->blend[i] = glm::vec3(0.0f, 1.0f, 0.0f);
				}
				else if (t >= data->gen->forestLevel) {
					data->gen->blend[i] = glm::vec3(0.0f, 0.0f, 1.0f);
				}
			}

			// Biome Map
			if (me <= data->gen->beachLevel) {
				data->gen->biomes[i] = glm::vec3(0.0f, 0.0f, 1.0f);
				data->gen->terrainType[i] = PTT_OCEAN;
			}
			else if (me > data->gen->beachLevel) {
				float t = glm::length(data->gen->moister[i]);
				if (t >= data->gen->beachLevel && t < data->gen->grassLevel) {
					data->gen->biomes[i] = glm::vec3(194.0f, 256.0f, 128.0f) / 256.0f;
					data->gen->terrainType[i] = PTT_BEACH;
				}
				else if (t >= data->gen->grassLevel && t < data->gen->forestLevel) {
					data->gen->biomes[i] = glm::vec3(0.3f, 1.0f, 0.3f);
					data->gen->terrainType[i] = PTT_GRASS;
				}
				else if (t >= 0.5f) {
					data->gen->biomes[i] = glm::vec3(139.0f, 69.0f, 19.0f) / 256.0f;
					data->gen->terrainType[i] = PTT_FOREST;
				}
			}

			// Pixel Section
			// Elevation
			uint8_t e = data->gen->elevation[i] * 255;
			data->gen->elevation_pixels[i] = RGB_COLOR(e, e, e);

			// Mask
			uint8_t m = data->gen->mask[i] * 255;
			data->gen->mask_pixels[i] = RGB_COLOR(m, m, m);

			// Masked Elevation
			uint8_t men = data->gen->maskedElevation[i] * 255;
			data->gen->mask_elevation_pixels[i] = RGB_COLOR(men, men, men);

			// Moister
			uint8_t mor = data->gen->moister[i].r * 255;
			uint8_t mog = data->gen->moister[i].g * 255;
			data->gen->moister_pixels[i] = RGB_COLOR(mor, 0, mog);

			// Blend Map
			uint8_t br = data->gen->blend[i].r * 255;
			uint8_t bg = data->gen->blend[i].g * 255;
			uint8_t bb = data->gen->blend[i].b * 255;
			data->gen->blend_map_pixels[i] = RGB_COLOR(br, bg, bb);

			// Biome
			uint8_t bmr = data->gen->biomes[i].r * 255;
			uint8_t bmg = data->gen->biomes[i].g * 255;
			uint8_t bmb = data->gen->biomes[i].b * 255;
			data->gen->biome_pixels[i] = RGB_COLOR(bmr, bmg, bmb);
		}
	}

	return 0;
}

void ProcTerrain::generate() {
	// Handle Map Size
	int tsize = 16;
	int osize = size / tsize;

	//std::cout << "tsize=" << tsize << " osize=" << osize << std::endl;
	logger_output("tsize=%d, osize=%d\n", tsize, osize);

	std::vector<ProcTerrainThreadHolder> threads(tsize * tsize);

	int i = 0;

	for (int y = 0; y < tsize; y++) {
		for (int x = 0; x < tsize; x++) {
			std::stringstream ss;
			ss << "Thread: " << i;
			int o = y * tsize + x;

			threads[o].data.id = i;
			threads[o].data.x = x * osize;
			threads[o].data.y = y * osize;
			threads[o].data.width = x * osize + osize;
			threads[o].data.height = y * osize + osize;
			threads[o].data.gen = this;
			threads[o].thread = SDL_CreateThread(proc_thread_callback, ss.str().c_str(), &threads[o].data);
		}
	}


	std::vector<ProcTerrainThreadHolder>::iterator it;

	for (it = threads.begin(); it != threads.end(); it++) {
		int ret;
		SDL_WaitThread(it->thread, &ret);
	}
}

void ProcTerrain::generateTextures() {
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

// Geometry
void ProcTerrainGeometry::init() {
	//this->data.loadConfig("data/terrain/regular.json");
	this->data.init("data/terrain/regular.json");

	//this->blendMap.initEmpty(this->data.size, this->data.size);

	/*
	BlurStandardPreProcess blur;

	blur.setSize(this->data.size, this->data.size);

	blur.setInputTexture(&this->data.blendMapTex);
	blur.setOutputTexture(&this->blendMap);


	blur.init();
	blur.build();
	blur.process();
	blur.release();

	this->width = this->data.size;
	this->height = this->data.size;
	*/

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




	RenderBuffer depth;
	FrameBuffer frameBuffer;

	QuadBlurPreProcessGeometry quad;


	quad.init();


	uint32_t width = data.blendMapTex.width;
	uint32_t height = data.blendMapTex.height;

	logger_output("%d, %d\n", width, height);

	depth.init();
	depth.load(GL_DEPTH_COMPONENT, width, height);

	blendMap.initEmpty(width, height);

	
	frameBuffer.init();

	logger_output("TextureID: %d, RenderBufferID: %d, FrameBufferID: %d\n", blendMap.id, depth.id, frameBuffer.id);

	frameBuffer.bind();
	frameBuffer.setRenderBuffer(&depth, GL_DEPTH_ATTACHMENT);
	frameBuffer.setTexture2D(&this->blendMap, GL_COLOR_ATTACHMENT0);
	frameBuffer.unbind();
	frameBuffer.checkForErrors();




	frameBuffer.bind();
	RenderSystem::viewport(0, 0, width, height);
	RenderSystem::clearColor(1.0f, 0.0f, 0.0f, 1.0f);
	RenderSystem::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	frameBuffer.unbind();

	frameBuffer.release();
	depth.release();
	quad.release();
}

void ProcTerrainGeometry::render(TerrainShader* shader) {
	//data.blendMapTex.bind();
	blendMap.bind();
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

	//data.blendMapTex.unbind();
	blendMap.release();
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

	this->blendMap.release();

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
