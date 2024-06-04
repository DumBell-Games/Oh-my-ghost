#include "App.h"
#include "Audio.h"
#include "MusicaTaberna.h"
#include "Scene.h"
#include "MusicaCiudad.h"

#include "SDL/include/SDL.h"
#include "SDL_mixer/include/SDL_mixer.h"

MusicaTaberna::MusicaTaberna(bool startEnabled) : Module(startEnabled)
{
	name.Create("scene");
	needsAwaking = true;
}

// Destructor
MusicaTaberna::~MusicaTaberna()
{}

// Called MusicaCielo render is available
bool MusicaTaberna::Start()
{
	LOG("Loading Scene");
	bool ret = true;
	
	tabernaMusic = app->audio->LoadFx("Assets/Audio/Fx/barFx.wav");

	app->audio->PlayFx(tabernaMusic);
	app->musicaCiudad->Disable();

	return ret;
}

//clean up
bool MusicaTaberna::CleanUp()
{	
	app->audio->UnloadFx(tabernaMusic);

	return true;
}
