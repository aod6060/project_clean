#include "sys.h"

/*
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
float SoundManager::soundVolumn = 1.0f;

bool SoundManager::changed = true;

Camera* SoundManager::camera = nullptr;
FMOD_VECTOR SoundManager::lastPos;
FMOD_VECTOR SoundManager::forward;
FMOD_VECTOR SoundManager::up;
FMOD_VECTOR SoundManager::vel;

void SoundManager::init() {
	FMOD::System_Create(&system);

	system->getVersion(&version);

	if (version < FMOD_VERSION) {
		logger_output("FMOD lib version %08x doesn't match header version %08x", version, FMOD_VERSION);
	}

	system->init(128, FMOD_INIT_3D_RIGHTHANDED, nullptr);

	system->set3DSettings(1.0f, DISTANCE_FACTOR, 1.0f);

	loadSounds("data/config/sound.json");

	sfx_channels.resize(64);
}

void SoundManager::update(float delta) {

	if (changed) {
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

		changed = false;
	}



	if (camera != nullptr) {
		FMOD_VECTOR pos;

		pos.x = camera->pos.x;
		pos.y = camera->pos.y;
		pos.z = camera->pos.z;

		glm::mat4 view = camera->getView();

		glm::vec3 f = glm::vec3(
			view[2][0],
			view[2][1],
			view[2][2]
		);

		f = glm::normalize(f);

		forward.x = f.x;
		forward.y = f.y;
		forward.z = f.z;

		vel.x = (pos.x - lastPos.x) * 1000 / delta;
		vel.y = (pos.y - lastPos.y) * 1000 / delta;
		vel.z = (pos.z - lastPos.z) * 1000 / delta;

		lastPos = pos;

		system->set3DListenerAttributes(0, &pos, &vel, &forward, &up);

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
				sounds[name]->set3DMinMaxDistance(0.5f * DISTANCE_FACTOR, 10000.0f* DISTANCE_FACTOR);
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
	SoundManager::camera = camera;

	lastPos.x = camera->pos.x;
	lastPos.y = camera->pos.y;
	lastPos.z = camera->pos.z;

	forward = { 0.0f, 0.0f, 1.0f };
	up = { 0.0f, 1.0f, 0.0f };

}

void SoundManager::releaseListener() {
	camera = nullptr;
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

float SoundManager::getSoundFXLength(std::string name) {
	uint32_t temp;
	sounds[name]->getLength(&temp, FMOD_TIMEUNIT_MS);
	return (float)temp / 1000.0f;
}

#define MAX_BOX 64
void SoundManager::playSoundFX(std::string name, const glm::vec3& pos) {
	if (camera != nullptr) {

		glm::vec3 cpos = camera->pos;


		glm::vec3 minBox = cpos - glm::vec3(MAX_BOX);
		glm::vec3 maxBox = cpos + glm::vec3(MAX_BOX);

		if (
			minBox.x <= pos.x &&
			minBox.y <= pos.y &&
			minBox.z <= pos.z &&
			maxBox.x >= pos.x &&
			maxBox.y >= pos.y &&
			maxBox.z >= pos.z
			) {
			FMOD_VECTOR p = { pos.x, pos.y, pos.z };
			FMOD_VECTOR v = { 0.0f, 0.0f, 0.0f };

			if (SoundManager::current_channel < sfx_channels.size()) {}
			else {
				SoundManager::current_channel = 0;
			}

			system->playSound(sounds[name], nullptr, true, &sfx_channels[SoundManager::current_channel]);

			sfx_channels[SoundManager::current_channel]->set3DAttributes(&p, &v);

			sfx_channels[SoundManager::current_channel]->setPaused(false);

			SoundManager::current_channel += 1;
		}
	}
}

void SoundManager::setMasterVolumn(float volumn) {
	masterVolumn = volumn;
	changed = true;
}

void SoundManager::setMusicVolumn(float volumn) {
	musicVolumn = volumn;
	changed = true;
}

void SoundManager::setSoundVolumn(float volumn) {
	soundVolumn = volumn;
	changed = true;
}
*/

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
