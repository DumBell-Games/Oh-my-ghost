#include "TeamScreen.h"
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
#include "IntroScreen.h"
#include "menu.h"
#include "Scene.h"


TeamScreen::TeamScreen(bool startEnabled) : Module(startEnabled)
{
    name.Create("teamScreen");
}

// Destructor
TeamScreen::~TeamScreen()
{}

// Called before render is available
bool TeamScreen::Start()
{
    logoScreenTex = app->tex->Load("Assets/Screens/TeamLogo.png");

    app->render->camera.x = 0;
    app->render->camera.y = 0;

    logoFX = app->audio->LoadFx("Assets/Audio/Fx/Sonido_Dumbell_Games_Logo.wav");

    

    SDL_GetWindowSize(app->win->window, &screenWidth, &screenHeight);

    app->audio->PlayFx(logoFX);
   

    return true;
}

// Called each loop iteration
bool TeamScreen::Update(float dt)
{
    if (app->input->GetButton(ControlID::CONFIRM) == KEY_DOWN)
    {
        app->fadeToBlack->FadeToBlackTransition((Module*)app->teamScreen, (Module*)app->introScreen, 0.0f);

	}

       
    return true;
}

bool TeamScreen::PostUpdate()
{
    
    app->render->DrawTexture(logoScreenTex, 0, 0, NULL);    

    return true;
}

// Called before quitting
bool TeamScreen::CleanUp()
{
    if (logoFX > 0) {
        app->audio->UnloadFx(logoFX);
        logoFX = 0;
    }

    if (logoScreenTex != nullptr)
    {
        app->tex->UnLoad(logoScreenTex);
        logoScreenTex = nullptr;
    }

    return true;
}


