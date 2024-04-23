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

    CreatePauseButtons();

	return true;
}
bool PauseMenu::Update(float dt) {
    if (buttoncreated == true) {
        // TODO: pasar esto a OnClick()
        if (PauseButtons.At(0)->data->state == GuiControlState::PRESSED)
        {
           app->fadeToBlack->FadeToBlackTransition((Module*)app->pause, (Module*)app->scene, 0.0f);
            buttoncreated = false;
        }
        if (PauseButtons.At(1)->data->state == GuiControlState::PRESSED)
        {

        }
        if (PauseButtons.At(2)->data->state == GuiControlState::PRESSED){
            app->fadeToBlack->FadeToBlackTransition((Module*)app->pause, (Module*)app->titlescreen, 0.0f);
            app->scene->Disable();//En este caso no da problemas porque no es el mismo modulo desde el que se ejecuta la linea
            buttoncreated = false;
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

	return true;
}
void PauseMenu::CreatePauseButtons() {

    if (PauseButtons.Count() == 0)
    {
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
}