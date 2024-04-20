#include "PauseMenu.h"
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


PauseMenu::PauseMenu(bool startEnabled) : Module()
{
}

PauseMenu::~PauseMenu()
{
}

bool PauseMenu::Start() {
	   
    app->render->camera.x = 0;
	app->render->camera.y = 0;

	SDL_GetWindowSize(app->win->window, &screenWidth, &screenHeight);

	CreatePauseButtons();


	return true;
}
bool PauseMenu::Update(float dt) {
    
   

	return true;
}
bool PauseMenu::PostUpdate() {
   
    if (pauseButtons.At(0)->data->state == GuiControlState::PRESSED)
    {
        app->pause->active = false;
        app->pause->Disable();

        app->guiManager->active = false;
        app->guiManager->Disable();

        app->scene->active = true;
        app->scene->Enable();

        app->fadeToBlack->FadeToBlackTransition((Module*)app->pause, (Module*)app->scene, 0.0f);
    }
    if (pauseButtons.At(1)->data->state == GuiControlState::PRESSED)
    {
        //app->fadeToBlack->FadeToBlackTransition((Module*)app->pause, (Module*)app->options, 0.0f);

        //app->pause->active = false;
        //app->pause->Disable();

        //app->options->active = true;
        //app->options->Enable();

        //app->guiManager->active = false;
        //app->guiManager->Disable();
    }
    if (pauseButtons.At(2)->data->state == GuiControlState::PRESSED)
    {
        SDL_Quit();
    }

   /* ListItem<GuiControl*>* controlListItem = nullptr;
    for (controlListItem = pauseButtons.start; controlListItem != NULL; controlListItem = controlListItem->next)
    {
        if (controlListItem->data != nullptr)
        {
            controlListItem->data->state = GuiControlState::NORMAL;
        }
    }
    if (pauseButtons[menuIndex - 1] != nullptr)
    {
        pauseButtons[menuIndex - 1]->state = GuiControlState::SELECTED;
    }*/
	
	return true;
}
bool PauseMenu::CleanUp(){

    ListItem<GuiControl*>* controlListItem = nullptr;
    for (controlListItem = pauseButtons.start; controlListItem != NULL; controlListItem = controlListItem->next)
    {
        delete controlListItem->data;
    }
    pauseButtons.Clear();

	return true;
}
void PauseMenu::CreatePauseButtons() {

    if ((Module*)app->pause->active)
    {
        int wBt = 190;
        int hBt = 40;
        int posBtX = screenWidth - 250;
        int posBtY = screenHeight - 650;
        pauseButtons.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Continuar", { posBtX, posBtY, wBt, hBt }, this));
        wBt = 190;
        hBt = 40;
        posBtX = screenWidth - 250;
        posBtY = screenHeight - 550;
        pauseButtons.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "Opciones", { posBtX, posBtY, wBt, hBt }, this));
        wBt = 190;
        hBt = 40;
        posBtX = screenWidth - 250;
        posBtY = screenHeight - 450;
        pauseButtons.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "Salir", { posBtX, posBtY, wBt, hBt }, this));
        
    }
}