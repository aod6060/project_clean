#pragma once


/*
	This will pull textures from the 
*/
struct TextureManager {
	static std::string TERRAIN_ELEVATION;
	static std::string TERRAIN_MASK;
	static std::string TERRAIN_MASKED_ELEVATION;
	static std::string TERRAIN_MOISTER;
	static std::string TERRAIN_BLEND_MAP;
	static std::string TERRAIN_BIOMES;
	static std::string TERRAIN_BLUR_BLEND_MAP;

	static std::map<std::string, Texture2D> textures;

	static void init();
	static void release();

	static Texture2D* getTex(std::string name);

};