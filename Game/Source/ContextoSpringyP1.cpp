#include "ContextoSpringyP1.h"
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


ContextoSpringyP1::ContextoSpringyP1(bool startEnabled) : Module(startEnabled)
{
	name.Create("teamScreen");
}

// Destructor
ContextoSpringyP1::~ContextoSpringyP1()
{}

// Called before render is available
bool ContextoSpringyP1::Start()
{
    //logoScreenTex = app->tex->Load("Assets/Screens/TeamLogo.png");

    app->video->Initialize("Assets/Videos/OhMyGhost_Cinematica_Context_PrimeraParte.avi");

    app->render->camera.x = 0;
    app->render->camera.y = 0;

    logoFX = app->audio->LoadFx("Assets/Audio/Fx/contextoSpringy.wav");

    

    SDL_GetWindowSize(app->win->window, &screenWidth, &screenHeight);

    app->audio->PlayFx(logoFX);
   

    return true;
}

// Called each loop iteration
bool ContextoSpringyP1::Update(float dt)
{
    if (!app->video->isVideoFinished)
    {
        app->video->GrabAVIFrame();

    }

	// dumbellAnim.Update();
	// IAnimationPath.Update();

    if (app->input->GetButton(ControlID::CONFIRM) == KEY_DOWN || app->video->isVideoFinished)
    {
        app->fadeToBlack->FadeToBlackTransition((Module*)app->contextoSpringyP1, (Module*)app->contextoSpringyP2, 0.0f);

	}

       
    return true;
}

bool ContextoSpringyP1::PostUpdate()
{
    //app->video->Initialize("Assets/Videos/animacionlogo.avi"); 
    if (!app->video->isVideoFinished)
    {
        app->video->GrabAVIFrame();

    }
    // app->render->DrawTexture(logoScreenTex, 0, 0,  &IAnimationPath.GetCurrentAnimation()->GetCurrentFrame(), 1.0f);   
    //app->render->DrawTexture(logoScreenTex, 0, 0);
	


    return true;
}

// Called before quitting
bool ContextoSpringyP1::CleanUp()
{
    if (logoScreenTex != nullptr)
    {
        app->tex->UnLoad(logoScreenTex);
        logoScreenTex = nullptr;
    }

    app->video->CloseAVI();

    return true;
}


