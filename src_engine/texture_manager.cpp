#include "sys.h"

std::string TextureManager::TERRAIN_ELEVATION = "_terrain::_elevation";
std::string TextureManager::TERRAIN_MASK = "_terrain::_mask";
std::string TextureManager::TERRAIN_MASKED_ELEVATION = "_terrain::_masked_elevation";
std::string TextureManager::TERRAIN_MOISTER = "_terrain::_moister";
std::string TextureManager::TERRAIN_BLEND_MAP = "_terrain::_blend_map";
std::string TextureManager::TERRAIN_BIOMES = "_terrain::_biomes";
std::string TextureManager::TERRAIN_BLUR_BLEND_MAP = "_terrain::_blur_blend_map";

std::map<std::string, Texture2D> TextureManager::textures;

/*
	any texture that has _ in there name is internal to the engine...
	textures used outside of the engine will have the directory name
	first [dir]:[name]...
*/
void TextureManager::init() {
	// Internal Textures

	// Terrain
	textures[TextureManager::TERRAIN_ELEVATION].initEmpty(1024, 1024);
	textures[TextureManager::TERRAIN_MASK].initEmpty(1024, 1024);
	textures[TextureManager::TERRAIN_MASKED_ELEVATION].initEmpty(1024, 1024);
	textures[TextureManager::TERRAIN_MOISTER].initEmpty(1024, 1024);
	textures[TextureManager::TERRAIN_BLEND_MAP].initEmpty(1024, 1024);
	textures[TextureManager::TERRAIN_BIOMES].initEmpty(1024, 1024);

	textures[TextureManager::TERRAIN_BLUR_BLEND_MAP].initEmpty(1024, 1024);

	// External Loader
	std::ifstream in("data/config/tex.json");

	Json::Value root;

	in >> root;

	Json::Value type = root["type"];
	Json::Value version = root["version"];

	if (type.asString() == "texture" && version.asString() == "1.0") {
		Json::Value texs = root["textures"];

		if (texs.isArray()) {
			for (int i = 0; i < texs.size(); i++) {
				Json::Value temp = texs[i];

				Json::Value name = temp["name"];
				Json::Value path = temp["path"];

				textures[name.asString()].loadTexture(path.asString());
			}
		}
	}

	in.close();
}

void TextureManager::release() {
	std::map<std::string, Texture2D>::iterator it;

	for (it = textures.begin(); it != textures.end(); it++) {
		it->second.release();
	}

	textures.clear();
}

Texture2D* TextureManager::getTex(std::string name) {
	return &textures[name];
}
