#pragma once

enum SoundCategory {
	SC_SOUND_FX = 0,
	SC_AMBIENT,
	SC_MUSIC,
	SC_SIZE
};

struct SoundFile {
	SoundCategory category;
	FMOD::Sound* sound;
	std::vector<FMOD::Channel*> channels;
	uint32_t current = 0;

	void init(Json::Value& value);

	void release();

	void play();

	void setPause(bool pause);

	void stop();

	float getTimeInSeconds();

	void setVolumn(float volumn);

};

struct SoundManager {
	static std::string f_type;
	static std::string f_version;

	static FMOD::System* system;

	static uint32_t version;

	static float masterVolumn;
	static float musicVolumn;
	static float ambientVolumn;
	static float soundfxVolumn;

	static std::map<std::string, SoundFile> soundFiles;

	static void init();

	static void update(float delta);

	static void release();

	static SoundFile* getSound(std::string name);

	static SoundCategory getCategory(std::string cat);

};