#include "game.h"


int main(int argc, char** argv) {
	GameWindowCallback windowCB;

	conf_init();

	win_init(&windowCB);

	win_update();
	
	win_release();

	return 0;
}