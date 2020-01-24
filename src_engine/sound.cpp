#include "sys.h"

// Sound File
void SoundFile::init(Json::Value& value) {
	std::string cat = value["category"].asString();
	bool looping = value["looping"].asBool();
	uint32_t channelPool = value["channel-pool"].asUInt();
	std::string path = value["path"].asString();
	this->category = SoundManager::getCategory(cat);
	SoundManager::system->createStream(
		path.c_str(), 
		FMOD_2D, 
		nullptr, 
		&this->sound);
	sound->setMode((looping) ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);
	this->channels.resize(channelPool);
}

void SoundFile::release() {
	channels.clear();

	sound->release();
}

void SoundFile::play() {
	this->current += 1;

	if (this->current < this->channels.size()) {}
	else {
		this->current = 0;
	}
	SoundManager::system->playSound(this->sound, nullptr, false, &this->channels[this->current]);
}

void SoundFile::setPause(bool pause) {
	if (this->channels[this->current] != nullptr) {
		this->channels[this->current]->setPaused(pause);
	}
}

void SoundFile::stop() {
	this->channels[this->current]->stop();
	this->channels[this->current] = nullptr;
}

float SoundFile::getTimeInSeconds() {
	uint32_t time = 0;
	this->sound->getLength(&time, FMOD_TIMEUNIT_MS);
	return (float)time / 1000.0f;
}

void SoundFile::setVolumn(float volumn) {
	if (this->channels[this->current] != nullptr) {
		this->channels[this->current]->setVolume(volumn);
	}
}


// Sound Manager
std::string SoundManager::f_type = "sound";

std::string SoundManager::f_version = "1.1";

uint32_t SoundManager::version;

FMOD::System* SoundManager::system = nullptr;

float SoundManager::masterVolumn = 1.0f;

float SoundManager::musicVolumn = 1.0f;

float SoundManager::ambientVolumn = 1.0f;

float SoundManager::soundfxVolumn = 1.0f;

std::map<std::string, SoundFile> SoundManager::soundFiles;

void SoundManager::init() {
	std::ifstream in("data/config/sound.json");
	
	Json::Value root;

	in >> root;

	std::string type = root["type"].asString();
	std::string version = root["version"].asString();

	if (type == SoundManager::f_type && version == SoundManager::f_version) {

		FMOD::System_Create(&system);

		system->getVersion(&SoundManager::version);

		if (SoundManager::version < FMOD_VERSION) {
			logger_output("FMOD lib version %08x doesn't match header version %08x", version, FMOD_VERSION);
		}

		uint32_t channelPoolMax = root["channel-pool-max"].asUInt();

		system->init(channelPoolMax, FMOD_INIT_NORMAL, nullptr);

		// Handle Default Volumn
		Json::Value defVolumn = root["default-volumn"];

		SoundManager::masterVolumn = defVolumn["master"].asFloat();
		SoundManager::musicVolumn = defVolumn["music"].asFloat();
		SoundManager::ambientVolumn = defVolumn["ambient"].asFloat();
		SoundManager::soundfxVolumn = defVolumn["sound-fx"].asFloat();

		Json::Value files = root["files"];

		if (files.isArray()) {

			for (int i = 0; i < files.size(); i++) {
				Json::Value temp = files[i];

				std::string name = temp["name"].asString();

				SoundManager::soundFiles[name].init(temp);
			}
		}
	}
	in.close();
}

void SoundManager::update(float delta) {

	std::map<std::string, SoundFile>::iterator it;
	
	for (it = SoundManager::soundFiles.begin(); it != SoundManager::soundFiles.end(); it++) {
		switch (it->second.category) {

		case SoundCategory::SC_SOUND_FX:
			it->second.setVolumn(SoundManager::soundfxVolumn * SoundManager::masterVolumn);
			break;
		case SoundCategory::SC_AMBIENT:
			it->second.setVolumn(SoundManager::ambientVolumn * SoundManager::masterVolumn);
			break;
		case SoundCategory::SC_MUSIC:
			it->second.setVolumn(SoundManager::musicVolumn * SoundManager::masterVolumn);
			break;
		}


	}

	system->update();
}

void SoundManager::release() {

	std::map<std::string, SoundFile>::iterator it;

	for (it = SoundManager::soundFiles.begin(); it != SoundManager::soundFiles.end(); it++) {
		it->second.release();
	}

	SoundManager::soundFiles.clear();

	system->close();
	system->release();

}

SoundFile* SoundManager::getSound(std::string name) {
	return &soundFiles[name];
}

SoundCategory SoundManager::getCategory(std::string cat) {
	static std::map<std::string, SoundCategory> c = {
		{"sound-fx", SoundCategory::SC_SOUND_FX},
		{"ambient", SoundCategory::SC_AMBIENT},
		{"music", SoundCategory::SC_MUSIC} 
	};
	return c[cat];
}
