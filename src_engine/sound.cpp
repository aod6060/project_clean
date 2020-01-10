#include "sys.h"

float SoundManager::DISTANCE_FACTOR = 1.0f;
FMOD::System* SoundManager::system;
uint32_t SoundManager::version;

std::string SoundManager::f_type = "sound";
std::string SoundManager::f_version = "1.0";

std::map<std::string, FMOD::Sound*> SoundManager::sounds;
std::map<std::string, FMOD::Sound*> SoundManager::musics;

FMOD::Channel* SoundManager::music = nullptr;
std::vector<FMOD::Channel*> SoundManager::sfx_channels;
uint32_t SoundManager::current_channel = 0;

float SoundManager::masterVolumn = 1.0f;
float SoundManager::musicVolumn = 0.5f;
float SoundManager::soundVolumn = 0.5f;

void SoundManager::init() {
	FMOD::System_Create(&system);

	system->getVersion(&version);

	if (version < FMOD_VERSION) {
		logger_output("FMOD lib version %08x doesn't match header version %08x", version, FMOD_VERSION);
	}

	system->init(128, FMOD_INIT_NORMAL, nullptr);

	system->set3DSettings(1.0f, DISTANCE_FACTOR, 1.0f);

	loadSounds("data/config/sound.json");

	sfx_channels.resize(64);
}

void SoundManager::update() {


	if (music != nullptr) {
		float v = musicVolumn * masterVolumn;

		music->setVolume(v);
	}

	for (int i = 0; i < sfx_channels.size(); i++) {

		if (sfx_channels[i] != nullptr) {
			float v = soundVolumn * masterVolumn;

			sfx_channels[i]->setVolume(v);
		}

	}

	system->update();
}

void SoundManager::release() {
	std::map<std::string, FMOD::Sound*>::iterator it;

	// Release Musics
	for (it = musics.begin(); it != musics.end(); it++) {
		it->second->release();
	}

	musics.clear();

	// Release Sounds
	for (it = sounds.begin(); it != sounds.end(); it++) {
		it->second->release();
	}

	sounds.clear();

	system->close();
	system->release();
}


void SoundManager::loadSounds(std::string path) {
	std::ifstream in(path);

	Json::Value root;

	in >> root;

	std::string type = root["type"].asString();
	std::string version = root["version"].asString();

	if (type == f_type && version == f_version) {
		// SFX
		Json::Value sfx = root["sfx"];

		if (sfx.isArray()) {
			for (int i = 0; i < sfx.size(); i++) {
				Json::Value temp = sfx[i];

				std::string name = temp["name"].asString();
				std::string path = temp["path"].asString();

				system->createStream(
					path.c_str(),
					FMOD_3D,
					nullptr,
					&sounds[name]
				);
				sounds[name]->set3DMinMaxDistance(0.5f * DISTANCE_FACTOR, 1024.0f * DISTANCE_FACTOR);
				sounds[name]->setMode(FMOD_LOOP_OFF);
			}
		}

		// Music
		Json::Value music = root["music"];

		if (music.isArray()) {

			for (int i = 0; i < music.size(); i++) {
				Json::Value temp = music[i];

				std::string name = temp["name"].asString();
				std::string path = temp["path"].asString();

				system->createStream(
					path.c_str(),
					FMOD_2D,
					0,
					&musics[name]
				);
				musics[name]->setMode(FMOD_LOOP_NORMAL);
			}
		}
	}

	in.close();
}

void SoundManager::setListener(Camera* camera) {

}

void SoundManager::playMusic(std::string music) {
	system->playSound(musics[music], 0, false, &SoundManager::music);
}

void SoundManager::playerMusicChannel() {
	if (SoundManager::music != nullptr) {
		SoundManager::music->setPaused(false);
	}
}

void SoundManager::pauseMusicChannel() {
	if (SoundManager::music != nullptr) {
		SoundManager::music->setPaused(true);
	}
}


void SoundManager::stopMusicChannel() {
	if (SoundManager::music != nullptr) {
		SoundManager::music->stop();
		SoundManager::music = nullptr;
	}
}

void SoundManager::setMasterVolumn(float volumn) {
	masterVolumn = volumn;
}

void SoundManager::setMusicVolumn(float volumn) {
	musicVolumn = volumn;
}

void SoundManager::setSoundVolumn(float volumn) {
	soundVolumn = volumn;
}