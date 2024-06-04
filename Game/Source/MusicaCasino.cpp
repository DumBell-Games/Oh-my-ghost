#include "App.h"
#include "Audio.h"
#include "MusicaCasino.h"
#include "Scene.h"
#include "MusicaCiudad.h"

#include "SDL/include/SDL.h"
#include "SDL_mixer/include/SDL_mixer.h"

MusicaCasino::MusicaCasino(bool startEnabled) : Module(startEnabled)
{
	name.Create("scene");
	needsAwaking = true;
}

// Destructor
MusicaCasino::~MusicaCasino()
{}

// Called MusicaCielo render is available
bool MusicaCasino::Start()
{
	LOG("Loading Scene");
	bool ret = true;
	
	casinoMusic = app->audio->LoadFx("Assets/Audio/Fx/casino.wav");

	app->audio->PlayFx(casinoMusic);
	app->musicaCiudad->Disable();

	return ret;
}

//clean up
bool MusicaCasino::CleanUp()
{	
	app->audio->UnloadFx(casinoMusic);

	return true;
}
