#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "Module.h"
#include "map"

using namespace std;

#define DEFAULT_MUSIC_FADE_TIME 2.0f

struct _Mix_Music;
struct Mix_Chunk;


class Audio : public Module
{
public:

	Audio(bool startEnabled = true);

	// Destructor
	virtual ~Audio();

	// Called before render is available
	bool Awake(pugi::xml_node config);

	// Called before quitting
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* path, float fadeTime);

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(unsigned int fx, int repeat = 0, int channel = -1);

	// Unload WAV  
	bool UnloadFx(unsigned int fx);

	bool StopFx(int channel); 
	
	bool StopMusic(float fadeTime = DEFAULT_MUSIC_FADE_TIME);

	bool LoadAudioFx(const char* name);

	bool LoadAudioMusic(const char* name, float fadeTime);

	//change music
	bool ChangeMusic(const char* path, float fadeTime);

	bool ResumeFx(unsigned int fx);

	bool UnloadMusic();

	bool MusicUp();

	bool MusicDown();

	bool FxUp();

	bool FxDown();
		

public:

	bool playingMusic = false;
	int volumen = 64;
	int fx1 = 64;

	map<unsigned int, int> activeChannels;

private:

	_Mix_Music* music;
	List<Mix_Chunk *>	fx;

	
};

#endif // __AUDIO_H__