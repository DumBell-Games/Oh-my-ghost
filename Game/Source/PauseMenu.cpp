#include "PauseMenu.h"
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "GuiManager.h"
#include "GuiControl.h"
#include "GuiControlButton.h"
#include "SDL/include/SDL.h"

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


	return true;
}
bool PauseMenu::Update(float dt) {

	return true;
}
bool PauseMenu::PostUpdate() {

   /* ListItem<GuiControl*>* controlListItem = nullptr;
    for (controlListItem = PauseButtons.start; controlListItem != NULL; controlListItem = controlListItem->next)
    {
        if (controlListItem->data != nullptr)
        {
            controlListItem->data->state = GuiControlState::NORMAL;
        }
    }
    if (PauseButtons[menuIndex - 1] != nullptr)
    {
        PauseButtons[menuIndex - 1]->state = GuiControlState::SELECTED;
    }*/
	
	return true;
}
bool PauseMenu::CleanUp(){

    ListItem<GuiControl*>* controlListItem = nullptr;
    for (controlListItem = PauseButtons.start; controlListItem != NULL; controlListItem = controlListItem->next)
    {
        delete controlListItem->data;
    }
    PauseButtons.Clear();

	return true;
}
void PauseMenu::CreatePauseButtons() {

    if (this->active)
    {
        int wBt = 190;
        int hBt = 40;
        int posBtX = screenWidth - 250;
        int posBtY = screenHeight - 650;
        PauseButtons.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Continuar", { posBtX, posBtY, wBt, hBt }, this));
        wBt = 190;
        hBt = 40;
        posBtX = screenWidth - 250;
        posBtY = screenHeight - 550;
        PauseButtons.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "Opciones", { posBtX, posBtY, wBt, hBt }, this));
        wBt = 190;
        hBt = 40;
        posBtX = screenWidth - 250;
        posBtY = screenHeight - 450;
        PauseButtons.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "Salir", { posBtX, posBtY, wBt, hBt }, this));
        
    }
}