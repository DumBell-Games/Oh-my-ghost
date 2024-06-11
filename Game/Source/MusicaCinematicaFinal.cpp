#include "App.h"
#include "Audio.h"
#include "MusicaCinematicaFinal.h"
#include "Scene.h"
#include "MusicaCombateAstroBark.h"

#include "SDL/include/SDL.h"
#include "SDL_mixer/include/SDL_mixer.h"

MusicaCFinal::MusicaCFinal(bool startEnabled) : Module(startEnabled)
{
	name.Create("scene");
	needsAwaking = true;
}

// Destructor
MusicaCFinal::~MusicaCFinal()
{}

// Called MusicaCielo render is available
bool MusicaCFinal::Start()
{
	LOG("Loading Scene");
	bool ret = true;
	
	cieloMusic = app->audio->LoadFx("Assets/Audio/Fx/OhMyGhost_Cinematica_Flashback_Corregida_1.wav");

	app->musicaCombateAB->Disable();
	app->audio->PlayFx(cieloMusic);

	return ret;
}

//clean up
bool MusicaCFinal::CleanUp()
{	
	app->audio->UnloadFx(cieloMusic);

	return true;
}
