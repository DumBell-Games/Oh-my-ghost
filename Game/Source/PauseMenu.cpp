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


PauseMenu::PauseMenu(bool startEnabled) : Module(startEnabled)
{
    name.Create("pause");
}

PauseMenu::~PauseMenu()
{
}

bool PauseMenu::Start() {
	   
    app->render->camera.x = 0;
	app->render->camera.y = 0;

	SDL_GetWindowSize(app->win->window, &screenWidth, &screenHeight);
    inpause = true;
    CreatePauseButtons();

	return true;
}
bool PauseMenu::Update(float dt) {
    if (buttoncreated == true) {
        // TODO: pasar esto a OnClick()
        if (inpause == true && PauseButtons.At(0)->data->state == GuiControlState::PRESSED)
        {
           app->fadeToBlack->FadeToBlackTransition((Module*)app->pause, (Module*)app->scene, 0.0f);
            buttoncreated = false;
        }
        if (inpause == true && PauseButtons.At(1)->data->state == GuiControlState::PRESSED)
        {
            CleanUp();
            inpause = false;
            inajustes = true;
            CreatePauseButtons();
        }
        if (inpause == true && PauseButtons.At(2)->data->state == GuiControlState::PRESSED){
            app->fadeToBlack->FadeToBlackTransition((Module*)app->pause, (Module*)app->titlescreen, 0.0f);
            app->scene->Disable();
            buttoncreated = false;
        }

    }
    if (ajustcreated == true) {
        if (inajustes == true && AjustesButtons.At(0)->data->state == GuiControlState::PRESSED){
            CleanUp();
            inpause = true;
            inajustes = false;
            CreatePauseButtons();
        }
    }

	return true;
}
bool PauseMenu::PostUpdate() {

	return true;
}
bool PauseMenu::CleanUp(){

    ListItem<GuiControl*>* controlListItem = nullptr;
    for (controlListItem = PauseButtons.start; controlListItem != NULL; controlListItem = controlListItem->next) {
        app->guiManager->DestroyGuiControl(controlListItem->data);
    }
    PauseButtons.Clear();
    ListItem<GuiControl*>* controlListItem2 = nullptr;
    for (controlListItem2 = AjustesButtons.start; controlListItem2 != NULL; controlListItem2 = controlListItem2->next) {
        app->guiManager->DestroyGuiControl(controlListItem2->data);
    }
    AjustesButtons.Clear();

	return true;
}
void PauseMenu::CreatePauseButtons() {

    if (PauseButtons.Count() == 0)
    {
        if (AjustesButtons.Count() == 0) {
            if (inpause == true) {
                buttoncreated = true;
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
            else if (inajustes == true) {
                ajustcreated = true;
                int wBt = 190;
                int hBt = 40;
                int posBtX = screenWidth - 750;
                int posBtY = screenHeight - 150;
                AjustesButtons.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Volver", { posBtX, posBtY, wBt, hBt }, this));

            }
        }
        
    }

}