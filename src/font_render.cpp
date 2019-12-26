#include "sys.h"

TTF_Font* FontRender::font = nullptr;
uint32_t FontRender::texID = 0;

void FontRender::init() {
	// Font will be 20pt
	font = TTF_OpenFont("data/font/font.ttf", 20);


}

void FontRender::release() {


	TTF_CloseFont(font);
}
