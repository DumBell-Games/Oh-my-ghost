#include "Options.h"
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "GuiManager.h"
#include "GuiControl.h"
#include "GuiControlButton.h"
#include "SDL/include/SDL.h"
#include "FadeToBlack.h"
#include "Input.h"
#include "Scene.h"
#include "menu.h"
#include "EntityManager.h"


Options::Options(bool startEnabled) : Module(startEnabled)
{
    name.Create("Options");
}

Options::~Options()
{
}

bool Options::Awake() {
    vsycn = parameters.attribute("value").as_bool();

    return true;
}

bool Options::Start() {


    buttonFx = app->audio->LoadFx("Assets/Audio/Fx/basic_click.wav");

    //smart smart 
    optionsBackground =     app->tex->LoadSP("Assets/Screens/Opciones vacio.png", true);
    fullscreenOFF =         app->tex->LoadSP("Assets/Screens/full screen vacio.png", true);
    fullscreenON =          app->tex->LoadSP("Assets/Screens/full screen marcado.png", true);
    VsyncOFF =              app->tex->LoadSP("Assets/Screens/vysnc vacio.png", true);
    VsyncON =               app->tex->LoadSP("Assets/Screens/vsync marcado.png", true);
    MusicaOFF =             app->tex->LoadSP("Assets/Screens/music vacio.png", true);
    MusicaON =              app->tex->LoadSP("Assets/Screens/music marcado.png", true);
    FxOFF =                 app->tex->LoadSP("Assets/Screens/sound vacio.png", true);
    FxON =                  app->tex->LoadSP("Assets/Screens/sound marcado.png", true);

    fullScreen = fullscreenOFF;
    Vsync = VsyncON;
    Musica = MusicaON;
    Fx = FxON;



    app->render->camera.x = 0;
	app->render->camera.y = 0;

	SDL_GetWindowSize(app->win->window, &screenWidth, &screenHeight);
	return true;
}
bool Options::Update(float dt)
{   
    app->scene->PlayerQuieto();
    if(app->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN)
    {
        app->options->Disable();
        app->scene->PlayerDinamico();
    }
  

	return true;
}
bool Options::PostUpdate() {
    app->render->DrawTexture(optionsBackground.get(), 0, 0, 0, NULL);
    app->render->DrawTexture(fullScreen.get(), 139, 337, 0, NULL);
    app->render->DrawTexture(Vsync.get(), 139, 420, 0, NULL);
    app->render->DrawTexture(Musica.get(), 139, 645, 0, NULL);
    app->render->DrawTexture(Fx.get(), 139, 727, 0, NULL);

	return true;
}

bool Options::CleanUp(){
    if (inpause == true) {

        if (optionsBackground != nullptr)
        {
            app->tex->UnLoadSP(optionsBackground);
            optionsBackground = nullptr;
        }

        if (fullscreenOFF != nullptr)
        {
            app->tex->UnLoadSP(fullscreenOFF);
			fullscreenOFF = nullptr;
        }

        if (fullscreenON != nullptr)
        {
            app->tex->UnLoadSP(fullscreenON);
			fullscreenON = nullptr;
        }
        
        if (VsyncOFF != nullptr)
        {
            app->tex->UnLoadSP(VsyncOFF);
			VsyncOFF = nullptr;
        }

		if (VsyncON != nullptr)
		{
			app->tex->UnLoadSP(VsyncON);
			VsyncON = nullptr;
		}

		if (MusicaOFF != nullptr)
		{
			app->tex->UnLoadSP(MusicaOFF);
			MusicaOFF = nullptr;
		}

		if (MusicaON != nullptr)
		{
			app->tex->UnLoadSP(MusicaON);
			MusicaON = nullptr;
		}
		if (FxOFF != nullptr)
		{
			app->tex->UnLoadSP(FxOFF);
			FxOFF = nullptr;
		}
        if (FxON != nullptr)
        {
            app->tex->UnLoadSP(FxON);
            FxON = nullptr;
        }

    }

	return true;
}
