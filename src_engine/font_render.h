#pragma once


struct FontRender {
	
	static TTF_Font* font;
	static uint32_t texID;
	static QuadFontRenderGeometry quad;
	static glm::vec3 color;

	static void init();

	static void release();

	static void setColor(const glm::vec3& color);

	static void print(float x, float y, const char* format, ...);

	static glm::vec2 getSize(const char* format, ...);

};