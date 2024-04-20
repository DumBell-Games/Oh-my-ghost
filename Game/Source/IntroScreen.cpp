#include "IntroScreen.h"
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
#include "TeamScreen.h"
#include "menu.h"

IntroScreen::IntroScreen(bool startEnabled) : Module()
{
}

// Destructor
IntroScreen::~IntroScreen()
{}

// Called before render is available
bool IntroScreen::Start()
{
    introScreenTex = app->tex->Load("Assets/Screens/IntroScreen.png");

    introScreenFx = app->audio->LoadFx("Assets/Audio/Fx/Musica_Pantalla_de_TituloFinal.wav");

    app->titlescreen->active = false;
    app->titlescreen->Disable();
    app->scene->active = false;
    app->scene->Disable();

    app->render->camera.x = 0;
    app->render->camera.y = 0;

    //TODO: cambiar musica
    //app->audio->PlayMusic("Assets/Music/titleScreen.ogg", 1.0f);
    SDL_GetWindowSize(app->win->window, &screenWidth, &screenHeight);


    return true;
}

// Called each loop iteration
bool IntroScreen::Update(float dt)
{
    if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN )
    {
        app->audio->UnloadFx(introScreenFx);
        app->fadeToBlack->FadeToBlackTransition((Module*)app->introScreen, (Module*)app->titlescreen, 0.0f);

        app->introScreen->Disable();
        app->introScreen->active = false;

        app->titlescreen->Enable();
        app->titlescreen->active = true;

        app->audio->PlayFx(app->titlescreen->menuFx);
    }

    
    return true;
}

bool IntroScreen::PostUpdate()
{
    app->render->DrawTexture(introScreenTex, 0, 0, NULL);

    return true;
}

// Called before quitting
bool IntroScreen::CleanUp()
{
    if (introScreenTex != nullptr)
    {
        app->tex->UnLoad(introScreenTex);
        introScreenTex = nullptr;
    }


    return true;
}

