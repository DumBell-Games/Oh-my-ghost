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
#include "Creditos.h"
#include "Video.h"


TeamScreen::TeamScreen(bool startEnabled) : Module(startEnabled)
{
	/*for (int fila = 0; fila < 7; fila++)
	{
	    for (int columna = 0; columna < 9; columna++)
		{
			int frameX = columna * SCREEN_WIDTH;
			int frameY = fila * SCREEN_HEIGHT;
			dumbellAnim.PushBack({ frameX, frameY, SCREEN_WIDTH, SCREEN_HEIGHT },  3);
		}
	}

	dumbellAnim.speed = 0.02f;
	dumbellAnim.loop = false;
	IAnimationPath.PushBack({ 0.0f, 0.0f }, 200, &dumbellAnim);*/

    name.Create("teamScreen");

}

// Destructor
TeamScreen::~TeamScreen()
{}

// Called before render is available
bool TeamScreen::Start()
{
    //logoScreenTex = app->tex->Load("Assets/Screens/TeamLogo.png");

    app->creditos->Disable();

    app->video->Initialize("Assets/Videos/Logo_Dumbell.avi");

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
    if (!app->video->isVideoFinished)
    {
        app->video->GrabAVIFrame();

    }

	// dumbellAnim.Update();
	// IAnimationPath.Update();

    if (app->input->GetButton(ControlID::CONFIRM) == KEY_DOWN || app->video->isVideoFinished)
    {
        app->fadeToBlack->FadeToBlackTransition((Module*)app->teamScreen, (Module*)app->introScreen, 0.0f);

	}

       
    return true;
}

bool TeamScreen::PostUpdate()
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

    app->video->CloseAVI();

    return true;
}


