#pragma once

/*
struct SoundManager {
	
	static float DISTANCE_FACTOR;
	static FMOD::System* system;
	static uint32_t version;

	static std::string f_type;
	static std::string f_version;

	static std::map<std::string, FMOD::Sound*> sounds;
	static std::map<std::string, FMOD::Sound*> musics;

	static FMOD::Channel* music;
	static std::vector<FMOD::Channel*> sfx_channels;
	static uint32_t current_channel;
	
	
	static float masterVolumn;
	static float musicVolumn;
	static float soundVolumn;

	static bool changed;

	static Camera* camera;
	static FMOD_VECTOR lastPos;
	static FMOD_VECTOR forward;
	static FMOD_VECTOR up;
	static FMOD_VECTOR vel;

	static void init();

	static void update(float delta);

	static void release();

	static void loadSounds(std::string path);

	static void setListener(Camera* camera);
	static void releaseListener();

	static void playMusic(std::string music);

	static void playerMusicChannel();

	static void pauseMusicChannel();

	static void stopMusicChannel();

	static float getSoundFXLength(std::string name);

	static void playSoundFX(std::string name, const glm::vec3& pos);

	static void setMasterVolumn(float volumn);

	static void setMusicVolumn(float volumn);

	static void setSoundVolumn(float volumn);
};
*/

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