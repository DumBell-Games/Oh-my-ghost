#include "Creditos.h"
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
#include "Video.h"


Creditos::Creditos(bool startEnabled) : Module(startEnabled)
{
	name.Create("creditos");
}

// Destructor
Creditos::~Creditos()
{}

// Called before render is available
bool Creditos::Start()
{
    //logoScreenTex = app->tex->Load("Assets/Screens/TeamLogo.png");

    if (app->creditos->isEnabled)
    {
        app->video->Initialize("Assets/Videos/creditos.avi");
    }

    app->render->camera.x = 0;
    app->render->camera.y = 0;

    logoFX = app->audio->LoadFx("Assets/Audio/Fx/creditos.wav");

    

    SDL_GetWindowSize(app->win->window, &screenWidth, &screenHeight);

    app->audio->PlayFx(logoFX);
   

    return true;
}

// Called each loop iteration
bool Creditos::Update(float dt)
{
    if (!app->video->isVideoFinished)
    {
        app->video->GrabAVIFrame();

    }
   

	// dumbellAnim.Update();
	// IAnimationPath.Update();


       
    return true;
}

bool Creditos::PostUpdate()
{
    //app->video->Initialize("Assets/Videos/animacionlogo.avi"); 
    if (!app->video->isVideoFinished)
    {
        app->video->GrabAVIFrame();

    }
    if (app->video->isVideoFinished)
    {
        app->fadeToBlack->FadeToBlackTransition((Module*)app->creditos, (Module*)app->titlescreen, 0.0f);

    }
    // app->render->DrawTexture(logoScreenTex, 0, 0,  &IAnimationPath.GetCurrentAnimation()->GetCurrentFrame(), 1.0f);   
    //app->render->DrawTexture(logoScreenTex, 0, 0);
	


    return true;
}

// Called before quitting
bool Creditos::CleanUp()
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

    app->video->CloseAVI();

    return true;
}


