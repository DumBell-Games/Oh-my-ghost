#include "App.h"
#include "Audio.h"
#include "MusicaCielo.h"
#include "Scene.h"

#include "SDL/include/SDL.h"
#include "SDL_mixer/include/SDL_mixer.h"

MusicaCielo::MusicaCielo(bool startEnabled) : Module(startEnabled)
{
	name.Create("scene");
	needsAwaking = true;
}

// Destructor
MusicaCielo::~MusicaCielo()
{}

// Called MusicaCielo render is available
bool MusicaCielo::Start()
{
	LOG("Loading Scene");
	bool ret = true;
	
	cieloMusic = app->audio->LoadFx("Assets/Audio/Fx/cielo.wav");

	app->audio->PlayFx(cieloMusic);

	return ret;
}

//clean up
bool MusicaCielo::CleanUp()
{	
	app->audio->UnloadFx(cieloMusic);

	return true;
}
