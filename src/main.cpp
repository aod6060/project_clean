#include "sys.h"


int main(int argc, char** argv) {
	conf_init();

	std::cout << "Width: " << conf_getWidth() << std::endl;
	std::cout << "Height: " << conf_getHeight() << std::endl;
	std::cout << "IsFullscreen: " << conf_isFullscreen() << std::endl;

	std::getchar();
	return 0;
}