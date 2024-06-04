#include "App.h"
#include "Audio.h"
#include "MusicaCiudad.h"
#include "Scene.h"
#include "SDL/include/SDL.h"
#include "SDL_mixer/include/SDL_mixer.h"



MusicaCiudad::MusicaCiudad(bool startEnabled) : Module(startEnabled)
{
	name.Create("scene");
	needsAwaking = true;
}

// Destructor
MusicaCiudad::~MusicaCiudad()
{}

// Called before render is available
bool MusicaCiudad::Start()
{
	LOG("Loading Scene");
	bool ret = true;
	
	//ciudadMusic = app->audio->LoadFx("Assets/Audio/Fx/centralFauna.wav");

	app->audio->PlayMusic("Assets/Audio/Music/centralFauna.ogg", 0);

	return ret;
}


//clean up
bool MusicaCiudad::CleanUp()
{
	app->audio->StopMusic(0);

	return true;
}

