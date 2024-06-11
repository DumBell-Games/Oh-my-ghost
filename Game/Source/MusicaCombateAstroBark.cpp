#include "App.h"
#include "Audio.h"
#include "MusicaCombateAstroBark.h"
#include "Scene.h"
#include "MusicaCiudad.h"

#include "SDL/include/SDL.h"
#include "SDL_mixer/include/SDL_mixer.h"

MusicaCombateAB::MusicaCombateAB(bool startEnabled) : Module(startEnabled)
{
	name.Create("MusicaCombateAB");
	needsAwaking = true;
}

// Destructor
MusicaCombateAB::~MusicaCombateAB()
{}

// Called MusicaCielo render is available
bool MusicaCombateAB::Start()
{
	LOG("Loading Scene");
	bool ret = true;
	
	casinoMusic = app->audio->LoadFx("Assets/Audio/Fx/combateBoss.wav");	

	app->audio->PlayFx(casinoMusic, 100);

	return ret;
}

//clean up
bool MusicaCombateAB::CleanUp()
{	
	if (casinoMusic != NULL)
	{
		app->audio->UnloadFx(casinoMusic);
		casinoMusic = NULL;
	}

	return true;
}
