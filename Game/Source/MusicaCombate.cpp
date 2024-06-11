#include "App.h"
#include "Audio.h"
#include "MusicaCombate.h"
#include "Scene.h"
#include "MusicaCiudad.h"

#include "SDL/include/SDL.h"
#include "SDL_mixer/include/SDL_mixer.h"

MusicaCombate::MusicaCombate(bool startEnabled) : Module(startEnabled)
{
	name.Create("scene");
	needsAwaking = true;
}

// Destructor
MusicaCombate::~MusicaCombate()
{}

// Called MusicaCielo render is available
bool MusicaCombate::Start()
{
	LOG("Loading Scene");
	bool ret = true;
	
	casinoMusic = app->audio->LoadFx("Assets/Audio/Fx/combatMusic.wav");	

	app->audio->PlayFx(casinoMusic, 100);

	return ret;
}

//clean up
bool MusicaCombate::CleanUp()
{	
	app->audio->UnloadFx(casinoMusic);

	return true;
}
