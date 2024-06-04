#include "App.h"
#include "Audio.h"
#include "MusicaMansion.h"
#include "Scene.h"
#include "MusicaCiudad.h"

#include "SDL/include/SDL.h"
#include "SDL_mixer/include/SDL_mixer.h"

MusicaMansion::MusicaMansion(bool startEnabled) : Module(startEnabled)
{
	name.Create("scene");
	needsAwaking = true;
}

// Destructor
MusicaMansion::~MusicaMansion()
{}

// Called MusicaCielo render is available
bool MusicaMansion::Start()
{
	LOG("Loading Scene");
	bool ret = true;
	
	mansionMusic = app->audio->LoadFx("Assets/Audio/Music/007-James-Bond-Theme.wav");

	app->audio->PlayFx(mansionMusic);
	app->musicaCiudad->Disable();

	return ret;
}

//clean up
bool MusicaMansion::CleanUp()
{	
	app->audio->UnloadFx(mansionMusic);

	return true;
}
