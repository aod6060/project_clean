#include "game.h"
#include "proc_terrain_test.h"
#include "ui_test.h"

int main(int argc, char** argv) {

	if (argc > 1) {

		if (std::string(argv[1]) == "-terrain") {
			ProcTerrainTestWindowCallback cb;

			conf_init("config/proc_terrain_test.json");

			win_init(&cb);

			win_update();

			win_release();
		}
		else if (std::string(argv[1]) == "-ui_test") {
			UITestWindowCallback cb;

			conf_init("config/ui_test.json");

			win_init(&cb);

			win_update();

			win_release();
		}
	}
	else {
		GameStateWindowCallback windowCB;

		conf_init("config/config.json");

		win_init(&windowCB);

		win_update();

		win_release();
	}

	return 0;
}