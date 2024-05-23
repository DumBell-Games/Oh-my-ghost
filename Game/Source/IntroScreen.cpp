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

IntroScreen::IntroScreen(bool startEnabled) : Module(startEnabled)
{
	for (int columna = 0; columna < 8; columna++)
	{
		for (int fila = 0; fila < 5; fila++)
		{
			int frameX = columna * SCREEN_WIDTH;
			int frameY = fila * SCREEN_HEIGHT;
			omgAnim.PushBack({ frameX, frameY, SCREEN_WIDTH, SCREEN_HEIGHT }, 3);
		}
	}

	omgAnim.speed = 0.02f;
	omgAnim.loop = false;
	IAnimationPath.PushBack({ 0.0f, 0.0f }, 200, &omgAnim);
    name.Create("introScreen");
}

// Destructor
IntroScreen::~IntroScreen()
{}

// Called before render is available
bool IntroScreen::Start()
{
    introScreenTex = app->tex->Load("Assets/Textures/Spritesheet OMG.png");

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

    app->audio->PlayFx(introScreenFx);

    return true;
}

// Called each loop iteration
bool IntroScreen::Update(float dt)
{
    omgAnim.Update();
    IAnimationPath.Update();

    if (app->input->GetButton(ControlID::CONFIRM) == KEY_DOWN || omgAnim.HasFinished())
    {
        app->fadeToBlack->FadeToBlackTransition((Module*)app->introScreen, (Module*)app->titlescreen, 0.0f);
    }
    
    return true;
}

bool IntroScreen::PostUpdate()
{
    app->render->DrawTexture(introScreenTex, 0, 0,  &IAnimationPath.GetCurrentAnimation()->GetCurrentFrame(), 1.0f);    
    return true;
}

// Called before quitting
bool IntroScreen::CleanUp()
{
    if (introScreenFx != -1) {
        app->audio->UnloadFx(introScreenFx);
        introScreenFx = 0;
    }
    if (introScreenTex != nullptr)
    {
        app->tex->UnLoad(introScreenTex);
        introScreenTex = nullptr;
    }


    return true;
}

