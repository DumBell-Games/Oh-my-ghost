#include "CargaAstroBark.h"
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


CargaAB::CargaAB(bool startEnabled) : Module(startEnabled)
{
	name.Create("CargaAB");
}

// Destructor
CargaAB::~CargaAB()
{}

// Called before render is available
bool CargaAB::Start()
{
    //logoScreenTex = app->tex->Load("Assets/Screens/TeamLogo.png");

    if (app->cargaAB->isEnabled)
    {
        app->video->Initialize("Assets/Videos/cargaAB.avi");
    }

    app->render->camera.x = 0;
    app->render->camera.y = 0;

    //logoFX = app->audio->LoadFx("Assets/Audio/Fx/creditos.wav");

    

    SDL_GetWindowSize(app->win->window, &screenWidth, &screenHeight);

    app->audio->PlayFx(logoFX);
   

    return true;
}

// Called each loop iteration
bool CargaAB::Update(float dt)
{
         
    return true;
}

bool CargaAB::PostUpdate()
{
    //app->video->Initialize("Assets/Videos/animacionlogo.avi"); 
    if (!app->video->isVideoFinished)
    {
        app->video->GrabAVIFrame();

    }
    if (app->video->isVideoFinished)
    {
        app->cargaAB->Disable();

    }
    // app->render->DrawTexture(logoScreenTex, 0, 0,  &IAnimationPath.GetCurrentAnimation()->GetCurrentFrame(), 1.0f);   
    //app->render->DrawTexture(logoScreenTex, 0, 0);
	


    return true;
}

// Called before quitting
bool CargaAB::CleanUp()
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


