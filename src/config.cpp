#include "sys.h"

const static std::string g_type = "config";
const static std::string g_version = "1.2";

static std::string g_config_path = "config.json";

static std::string g_caption = "";

static uint32_t g_width = 0;
static uint32_t g_height = 0;
static bool g_isFullscreen = false;

static std::map<std::string, InputMapping> g_inputMappings;

static Keyboard _conf_toKeyboard(std::string key);
static MouseButtons _conf_toMouseButtons(std::string mb);

static LoggerType g_loggerType;

static LoggerType _conf_toLoggerType(std::string key);

static float g_fov;

void conf_init() {
	std::ifstream in(g_config_path);

	srand(time(nullptr));

	Json::Value root;

	in >> root;

	Json::Value type = root["type"];
	Json::Value version = root["version"];

	//std::cout << type.asString() << ", " << version.asString() << std::endl;

	if (type.asString() == g_type && version.asString() == g_version) {

		// Window Section
		Json::Value window = root["window"];

		g_caption = window["caption"].asString();
		g_width = window["width"].asUInt();
		g_height = window["height"].asUInt();

		g_isFullscreen = window["is-fullscreen"].asBool();


		// input-mapping Section
		Json::Value inputMapping = root["input-mapping"];
		if (inputMapping.isArray()) {
			for (int i = 0; i < inputMapping.size(); i++) {
				Json::Value temp = inputMapping[i];

				std::string name = temp["name"].asString();
				bool isMouseButton = temp["is-mouse-button"].asBool();
				std::string input = temp["input"].asString();

				InputMapping mapping;

				if (isMouseButton) {
					mapping.isMouseButton = isMouseButton;
					mapping.mb = _conf_toMouseButtons(input);
				}
				else {
					mapping.isMouseButton = isMouseButton;
					mapping.key = _conf_toKeyboard(input);
				}

				g_inputMappings[name] = mapping;
			}
		}


		// logger section
		// console, file, both
		Json::Value logger = root["logger"];

		g_loggerType = _conf_toLoggerType(logger["type"].asString());

		// Graphics
		Json::Value graphics = root["graphics"];

		g_fov = graphics["fov"].asFloat();
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

InputMapping* conf_getInputMapping(std::string name) {
	return &g_inputMappings[name];
}

static Keyboard _conf_toKeyboard(std::string key) {
	std::map<std::string, Keyboard> keys = {
		{"KB_UNKNOWN", KB_UNKNOWN},
		{"KB_A", KB_A},
		{"KB_B", KB_B},
		{"KB_C", KB_C},
		{"KB_D", KB_D},
		{"KB_E", KB_E},
		{"KB_F", KB_F},
		{"KB_G", KB_G},
		{"KB_H", KB_H},
		{"KB_I", KB_I},
		{"KB_J", KB_J},
		{"KB_K", KB_K},
		{"KB_L", KB_L},
		{"KB_M", KB_M},
		{"KB_N", KB_N},
		{"KB_O", KB_O},
		{"KB_P", KB_P},
		{"KB_Q", KB_Q},
		{"KB_R", KB_R},
		{"KB_S", KB_S},
		{"KB_T", KB_T},
		{"KB_U", KB_U},
		{"KB_V", KB_V},
		{"KB_W", KB_W},
		{"KB_X", KB_X},
		{"KB_Y", KB_Y},
		{"KB_Z", KB_Z},
		{"KB_1", KB_1},
		{"KB_2", KB_2},
		{"KB_3", KB_3},
		{"KB_4", KB_4},
		{"KB_5", KB_5},
		{"KB_6", KB_6},
		{"KB_7", KB_7},
		{"KB_8", KB_8},
		{"KB_9", KB_9},
		{"KB_0", KB_0},
		{"KB_RETURN", KB_RETURN},
		{"KB_ESCAPE", KB_ESCAPE},
		{"KB_BACKSPACE", KB_BACKSPACE},
		{"KB_TAB", KB_TAB},
		{"KB_SPACE", KB_SPACE},
		{"KB_MINUS", KB_MINUS},
		{"KB_EQUALS", KB_EQUALS},
		{"KB_LEFTBRACKET", KB_LEFTBRACKET},
		{"KB_RIGHTBRACKET", KB_RIGHTBRACKET},
		{"KB_BACKSLASH", KB_BACKSLASH},
		{"KB_NONUSHASH", KB_NONUSHASH},
		{"KB_SEMICOLON", KB_SEMICOLON},
		{"KB_APOSTROPHE", KB_APOSTROPHE},
		{"KB_GRAVE", KB_GRAVE},
		{"KB_COMMA", KB_COMMA},
		{"KB_PERIOD", KB_PERIOD},
		{"KB_SLASH", KB_SLASH},
		{"KB_CAPSLOCK", KB_CAPSLOCK},
		{"KB_F1", KB_F1},
		{"KB_F2", KB_F2},
		{"KB_F3", KB_F3},
		{"KB_F4", KB_F4},
		{"KB_F5", KB_F5},
		{"KB_F6", KB_F6},
		{"KB_F7", KB_F7},
		{"KB_F8", KB_F8},
		{"KB_F9", KB_F9},
		{"KB_F10", KB_F10},
		{"KB_F11", KB_F11},
		{"KB_F12", KB_F12},
		{"KB_PRINTSCREEN", KB_PRINTSCREEN},
		{"KB_SCROLLLOCK", KB_SCROLLLOCK},
		{"KB_PAUSE", KB_PAUSE},
		{"KB_INSERT", KB_INSERT},
		{"KB_HOME", KB_HOME},
		{"KB_PAGEUP", KB_PAGEUP},
		{"KB_DELETE", KB_DELETE},
		{"KB_END", KB_END},
		{"KB_PAGEDOWN", KB_PAGEDOWN},
		{"KB_RIGHT", KB_RIGHT},
		{"KB_LEFT", KB_LEFT},
		{"KB_DOWN", KB_DOWN},
		{"KB_UP", KB_UP},
		{"KB_NUMLOCKCLEAR", KB_NUMLOCKCLEAR},
		{"KB_KP_DIVIDE", KB_KP_DIVIDE},
		{"KB_KP_MULTIPLY", KB_KP_MULTIPLY},
		{"KB_KP_MINUS", KB_KP_MINUS},
		{"KB_KP_PLUS", KB_KP_PLUS},
		{"KB_KP_ENTER", KB_KP_ENTER},
		{"KB_KP_1", KB_KP_1},
		{"KB_KP_2", KB_KP_2},
		{"KB_KP_3", KB_KP_3},
		{"KB_KP_4", KB_KP_4},
		{"KB_KP_5", KB_KP_5},
		{"KB_KP_6", KB_KP_6},
		{"KB_KP_7", KB_KP_7},
		{"KB_KP_8", KB_KP_8},
		{"KB_KP_9", KB_KP_9},
		{"KB_KP_0", KB_KP_0},
		{"KB_KP_PERIOD", KB_KP_PERIOD},
		{"KB_NONUSBACKSLASH", KB_NONUSBACKSLASH},
		{"KB_APPLICATION", KB_APPLICATION},
		{"KB_POWER", KB_POWER},
		{"KB_KP_EQUALS", KB_KP_EQUALS},
		{"KB_F13", KB_F13},
		{"KB_F14", KB_F14},
		{"KB_F15", KB_F15},
		{"KB_F16", KB_F16},
		{"KB_F17", KB_F17},
		{"KB_F18", KB_F18},
		{"KB_F19", KB_F19},
		{"KB_F20", KB_F20},
		{"KB_F21", KB_F21},
		{"KB_F22", KB_F22},
		{"KB_F23", KB_F23},
		{"KB_F24", KB_F24},
		{"KB_EXECUTE", KB_EXECUTE},
		{"KB_HELP", KB_HELP},
		{"KB_MENU", KB_MENU},
		{"KB_SELECT", KB_SELECT},
		{"KB_STOP", KB_STOP},
		{"KB_AGAIN", KB_AGAIN},
		{"KB_UNDO", KB_UNDO},
		{"KB_CUT", KB_CUT},
		{"KB_COPY", KB_COPY},
		{"KB_PASTE", KB_PASTE},
		{"KB_FIND", KB_FIND},
		{"KB_MUTE", KB_MUTE},
		{"KB_VOLUMEUP", KB_VOLUMEUP},
		{"KB_VOLUMEDOWN", KB_VOLUMEDOWN},
		{"KB_KP_COMMA", KB_KP_COMMA},
		{"KB_KP_EQUALSAS400", KB_KP_EQUALSAS400},
		{"KB_INTERNATIONAL1", KB_INTERNATIONAL1},
		{"KB_INTERNATIONAL2", KB_INTERNATIONAL2},
		{"KB_INTERNATIONAL3", KB_INTERNATIONAL3},
		{"KB_INTERNATIONAL4", KB_INTERNATIONAL4},
		{"KB_INTERNATIONAL5", KB_INTERNATIONAL5},
		{"KB_INTERNATIONAL6", KB_INTERNATIONAL6},
		{"KB_INTERNATIONAL7", KB_INTERNATIONAL7},
		{"KB_INTERNATIONAL8", KB_INTERNATIONAL8},
		{"KB_INTERNATIONAL9", KB_INTERNATIONAL9},
		{"KB_LANG1", KB_LANG1},
		{"KB_LANG2", KB_LANG2},
		{"KB_LANG3", KB_LANG3},
		{"KB_LANG4", KB_LANG4},
		{"KB_LANG5", KB_LANG5},
		{"KB_LANG6", KB_LANG6},
		{"KB_LANG7", KB_LANG7},
		{"KB_LANG8", KB_LANG8},
		{"KB_LANG9", KB_LANG9},
		{"KB_ALTERASE", KB_ALTERASE},
		{"KB_SYSREQ", KB_SYSREQ},
		{"KB_CANCEL", KB_CANCEL},
		{"KB_CLEAR", KB_CLEAR},
		{"KB_PRIOR", KB_PRIOR},
		{"KB_RETURN2", KB_RETURN2},
		{"KB_SEPARATOR", KB_SEPARATOR},
		{"KB_OUT", KB_OUT},
		{"KB_OPER", KB_OPER},
		{"KB_CLEARAGAIN", KB_CLEARAGAIN},
		{"KB_CRSEL", KB_CRSEL},
		{"KB_EXSEL", KB_EXSEL},
		{"KB_KP_00", KB_KP_00},
		{"KB_KP_000", KB_KP_000},
		{"KB_THOUSANDSSEPARATOR", KB_THOUSANDSSEPARATOR},
		{"KB_DECIMALSEPARATOR", KB_DECIMALSEPARATOR},
		{"KB_CURRENCYUNIT", KB_CURRENCYUNIT},
		{"KB_CURRENCYSUBUNIT", KB_CURRENCYSUBUNIT},
		{"KB_KP_LEFTPAREN", KB_KP_LEFTPAREN},
		{"KB_KP_RIGHTPAREN", KB_KP_RIGHTPAREN},
		{"KB_KP_LEFTBRACE", KB_KP_LEFTBRACE},
		{"KB_KP_RIGHTBRACE", KB_KP_RIGHTBRACE},
		{"KB_KP_TAB", KB_KP_TAB},
		{"KB_KP_BACKSPACE", KB_KP_BACKSPACE},
		{"KB_KP_A", KB_KP_A},
		{"KB_KP_B", KB_KP_B},
		{"KB_KP_C", KB_KP_C},
		{"KB_KP_D", KB_KP_D},
		{"KB_KP_E", KB_KP_E},
		{"KB_KP_F", KB_KP_F},
		{"KB_KP_XOR", KB_KP_XOR},
		{"KB_KP_POWER", KB_KP_POWER},
		{"KB_KP_PERCENT", KB_KP_PERCENT},
		{"KB_KP_LESS", KB_KP_LESS},
		{"KB_KP_GREATER", KB_KP_GREATER},
		{"KB_KP_AMPERSAND", KB_KP_AMPERSAND},
		{"KB_KP_DBLAMPERSAND", KB_KP_DBLAMPERSAND},
		{"KB_KP_VERTICALBAR", KB_KP_VERTICALBAR},
		{"KB_KP_DBLVERTICALBAR", KB_KP_DBLVERTICALBAR},
		{"KB_KP_COLON", KB_KP_COLON},
		{"KB_KP_HASH", KB_KP_HASH},
		{"KB_KP_SPACE", KB_KP_SPACE},
		{"KB_KP_AT", KB_KP_AT},
		{"KB_KP_EXCLAM", KB_KP_EXCLAM},
		{"KB_KP_MEMSTORE", KB_KP_MEMSTORE},
		{"KB_KP_MEMRECALL", KB_KP_MEMRECALL},
		{"KB_KP_MEMCLEAR", KB_KP_MEMCLEAR},
		{"KB_KP_MEMADD", KB_KP_MEMADD},
		{"KB_KP_MEMSUBTRACT", KB_KP_MEMSUBTRACT},
		{"KB_KP_MEMMULTIPLY", KB_KP_MEMMULTIPLY},
		{"KB_KP_MEMDIVIDE", KB_KP_MEMDIVIDE},
		{"KB_KP_PLUSMINUS", KB_KP_PLUSMINUS},
		{"KB_KP_CLEAR", KB_KP_CLEAR},
		{"KB_KP_CLEARENTRY", KB_KP_CLEARENTRY},
		{"KB_KP_BINARY", KB_KP_BINARY},
		{"KB_KP_OCTAL", KB_KP_OCTAL},
		{"KB_KP_DECIMAL", KB_KP_DECIMAL},
		{"KB_KP_HEXADECIMAL", KB_KP_HEXADECIMAL},
		{"KB_LCTRL", KB_LCTRL},
		{"KB_LSHIFT", KB_LSHIFT},
		{"KB_LALT", KB_LALT},
		{"KB_LGUI", KB_LGUI},
		{"KB_RCTRL", KB_RCTRL},
		{"KB_RSHIFT", KB_RSHIFT},
		{"KB_RALT", KB_RALT},
		{"KB_RGUI", KB_RGUI},
		{"KB_MODE", KB_MODE},
		{"KB_AUDIONEXT", KB_AUDIONEXT},
		{"KB_AUDIOPREV", KB_AUDIOPREV},
		{"KB_AUDIOSTOP", KB_AUDIOSTOP},
		{"KB_AUDIOPLAY", KB_AUDIOPLAY},
		{"KB_AUDIOMUTE", KB_AUDIOMUTE},
		{"KB_MEDIASELECT", KB_MEDIASELECT},
		{"KB_WWW", KB_WWW},
		{"KB_MAIL", KB_MAIL},
		{"KB_CALCULATOR", KB_CALCULATOR},
		{"KB_COMPUTER", KB_COMPUTER},
		{"KB_AC_SEARCH", KB_AC_SEARCH},
		{"KB_AC_HOME", KB_AC_HOME},
		{"KB_AC_BACK", KB_AC_BACK},
		{"KB_AC_FORWARD", KB_AC_FORWARD},
		{"KB_AC_STOP", KB_AC_STOP},
		{"KB_AC_REFRESH", KB_AC_REFRESH},
		{"KB_AC_BOOKMARKS", KB_AC_BOOKMARKS},
		{"KB_BRIGHTNESSDOWN", KB_BRIGHTNESSDOWN},
		{"KB_BRIGHTNESSUP", KB_BRIGHTNESSUP},
		{"KB_DISPLAYSWITCH", KB_DISPLAYSWITCH},
		{"KB_KBDILLUMTOGGLE", KB_KBDILLUMTOGGLE},
		{"KB_KBDILLUMDOWN", KB_KBDILLUMDOWN},
		{"KB_KBDILLUMUP", KB_KBDILLUMUP},
		{"KB_EJECT", KB_EJECT},
		{"KB_SLEEP", KB_SLEEP},
		{"KB_APP1", KB_APP1},
		{"KB_APP2", KB_APP2},
		{"KB_AUDIOREWIND", KB_AUDIOREWIND},
		{"KB_AUDIOFASTFORWARD", KB_AUDIOFASTFORWARD}
	};
	return keys[key];
}

static MouseButtons _conf_toMouseButtons(std::string mb) {

	std::map<std::string, MouseButtons> mouseButtons = {
		{"MBS_LEFT", MBS_LEFT},
		{"MBS_MIDDLE", MBS_MIDDLE},
		{"MBS_RIGHT", MBS_RIGHT},
	};

	return mouseButtons[mb];
}	

LoggerType conf_getLoggerType() {
	return g_loggerType;
}

static LoggerType _conf_toLoggerType(std::string key) {
	std::map<std::string, LoggerType> loggerType = {
		{"console", LT_CONSOLE},
		{"file", LT_FILE},
		{"both", LT_BOTH}
	};
	return loggerType[key];
}

float conf_getFOV() {
	return g_fov;
}