#include "App.h"
#include "Audio.h"
#include "MusicaDiscoteca.h"
#include "Scene.h"
#include "MusicaCiudad.h"

#include "SDL/include/SDL.h"
#include "SDL_mixer/include/SDL_mixer.h"

MusicaDiscoteca::MusicaDiscoteca(bool startEnabled) : Module(startEnabled)
{
	name.Create("scene");
	needsAwaking = true;
}

// Destructor
MusicaDiscoteca::~MusicaDiscoteca()
{}

// Called MusicaCielo render is available
bool MusicaDiscoteca::Start()
{
	LOG("Loading Scene");
	bool ret = true;
	
	discoMusic = app->audio->LoadFx("Assets/Audio/Fx/discotecaFx.wav");
	app->musicaCiudad->Disable();

	app->audio->PlayFx(discoMusic);

	return ret;
}

//clean up
bool MusicaDiscoteca::CleanUp()
{	
	app->audio->UnloadFx(discoMusic);

	return true;
}
