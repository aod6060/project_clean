#include "sys.h"

const static std::string g_type = "config";
const static std::string g_version = "1.0";

static std::string g_config_path = "config.json";

static std::string g_caption = "";

static uint32_t g_width = 0;
static uint32_t g_height = 0;
static bool g_isFullscreen = false;

void conf_init() {
	std::ifstream in(g_config_path);

	Json::Value root;

	in >> root;

	Json::Value type = root["type"];
	Json::Value version = root["version"];

	//std::cout << type.asString() << ", " << version.asString() << std::endl;

	if (type.asString() == g_type && version.asString() == g_version) {

		Json::Value window = root["window"];

		g_caption = window["caption"].asString();
		g_width = window["width"].asUInt();
		g_height = window["height"].asUInt();

		g_isFullscreen = window["is-fullscreen"].asBool();

	}
	else {
		throw std::exception("Error: Configuration file doesn't exist or isn't the right type of file...");
	}

	in.close();
}

uint32_t conf_getWidth() {
	return g_width;
}

uint32_t conf_getHeight() {
	return g_height;
}

bool conf_isFullscreen() {
	return g_isFullscreen;
}

std::string conf_getCaption() {
	return g_caption;
}