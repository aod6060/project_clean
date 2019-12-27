#include "sys.h"

TTF_Font* FontRender::font = nullptr;
uint32_t FontRender::texID = 0;
QuadFontRenderGeometry FontRender::quad;
glm::vec3 FontRender::color;

void FontRender::init() {
	// Font will be 20pt
	font = TTF_OpenFont("data/font/font.ttf", 20);

	TTF_SetFontStyle(font, TTF_STYLE_BOLD);

	glGenTextures(1, &texID);

	quad.init();

	color = glm::vec3(1.0f);
}

void FontRender::release() {
	quad.release();

	glDeleteTextures(1, &texID);

	TTF_CloseFont(font);
}

void FontRender::setColor(const glm::vec3& color) {
	FontRender::color = color;
}

void FontRender::print(float x, float y, const char* format, ...) {
	char buf[1024];

	va_list args;
	va_start(args, format);

	vsprintf(buf, format, args);

	va_end(args);

	std::string str(buf);

	//logger_output(str.c_str());


	SDL_Surface* surf = nullptr;

	SDL_Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;

	surf = TTF_RenderText_Blended(font, str.c_str(), color);


	if (surf == nullptr) {
		return;
	}



	glBindTexture(GL_TEXTURE_2D, texID);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		surf->w,
		surf->h,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		surf->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	float width = surf->w;
	float height = surf->h;

	SDL_FreeSurface(surf);

	// Render Text

	ShaderManager::fontRenderShader.bind();

	ShaderManager::fontRenderShader.setProj(
		glm::ortho(0.0f, (float)conf_getWidth(), (float)conf_getHeight(), 0.0f)
	);

	ShaderManager::fontRenderShader.setView(
		glm::mat4(1.0f)
	);

	ShaderManager::fontRenderShader.setModel(
		glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.0f)) *
		glm::scale(glm::mat4(1.0f), glm::vec3(width, height, 0.0f))
	);

	ShaderManager::fontRenderShader.setColor(
		FontRender::color
	);

	glBindTexture(GL_TEXTURE_2D, texID);
	quad.render(&ShaderManager::fontRenderShader);
	glBindTexture(GL_TEXTURE_2D, 0);

	ShaderManager::fontRenderShader.unbind();

}

glm::vec2 FontRender::getSize(const char* format, ...) {
	char buf[1024];

	va_list args;
	va_start(args, format);

	vsprintf(buf, format, args);

	va_end(args);

	std::string str(buf);

	int width;
	int height;

	TTF_SizeText(font, str.c_str(), &width, &height);

	return glm::vec2(width, height);
}
