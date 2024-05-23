#include "QuestPatatasScreen.h"
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
#include "Timer.h"


QuestPatatasScreen::QuestPatatasScreen(bool startEnabled) : Module(startEnabled)
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

    name.Create("questPatatasScreen");
}

// Destructor
QuestPatatasScreen::~QuestPatatasScreen()
{}

// Called before render is available
bool QuestPatatasScreen::Start()
{
    logoScreenTex = app->tex->Load("Assets/Textures/patatasQuest.png");

    app->render->camera.x = 0;
    app->render->camera.y = 0;
        

    SDL_GetWindowSize(app->win->window, &screenWidth, &screenHeight);
   

    return true;
}

// Called each loop iteration
bool QuestPatatasScreen::Update(float dt)
{

	// dumbellAnim.Update();
	// IAnimationPath.Update();


       
    return true;
}

bool QuestPatatasScreen::PostUpdate()
{
    
    // app->render->DrawTexture(logoScreenTex, 0, 0,  &IAnimationPath.GetCurrentAnimation()->GetCurrentFrame(), 1.0f);   
    app->render->DrawTexture(logoScreenTex, 0, 0);
    //if press E disable this
    if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
    {
        app->questPatatasScreen->Disable();
        app->questPatatasScreen->active = false;
        app->scene->paused = false;
	}

 


    return true;
}

// Called before quitting
bool QuestPatatasScreen::CleanUp()
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


