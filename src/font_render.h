#pragma once


struct FontRender {
	
	static TTF_Font* font;
	static uint32_t texID;


	static void init();

	static void release();

};