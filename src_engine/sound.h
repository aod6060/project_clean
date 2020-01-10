#pragma once


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

	static void init();

	static void update();

	static void release();

	static void loadSounds(std::string path);

	static void setListener(Camera* camera);

	static void playMusic(std::string music);

	static void playerMusicChannel();

	static void pauseMusicChannel();

	static void stopMusicChannel();

	static void setMasterVolumn(float volumn);

	static void setMusicVolumn(float volumn);

	static void setSoundVolumn(float volumn);
};