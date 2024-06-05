#include "PersonajesScreen.h"
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
#include "ItemBirra.h"
#include "ItemCaramelos.h"
#include "ItemCola.h"
#include "ItemPatatas.h"
#include "ItemVelocidad.h"
#include "ItemYogur.h"


PersonajesScreen::PersonajesScreen(bool startEnabled) : Module(startEnabled)
{
    name.Create("personajesScreen");
}

PersonajesScreen::~PersonajesScreen()
{
}


bool PersonajesScreen::Start() {
	
    buttonFx = app->audio->LoadFx("Assets/Audio/Fx/basic_click.wav");

    personaje1 = app->tex->LoadSP("Assets/Textures/personaje1.png", true);
    personaje2 = app->tex->LoadSP("Assets/Textures/personaje2.png", true);
    personaje3 = app->tex->LoadSP("Assets/Textures/personaje3.png", true);
    personaje4 = app->tex->LoadSP("Assets/Textures/personaje4.png", true);
    personaje5 = app->tex->LoadSP("Assets/Textures/personaje5.png", true);
    personaje6 = app->tex->LoadSP("Assets/Textures/personaje6.png", true);
    personaje7 = app->tex->LoadSP("Assets/Textures/personaje7.png", true);
    personaje8 = app->tex->LoadSP("Assets/Textures/personaje8.png", true);
    personaje9 = app->tex->LoadSP("Assets/Textures/personaje9.png", true);


    app->render->camera.x = 0;
    app->render->camera.y = 0;

	SDL_GetWindowSize(app->win->window, &screenWidth, &screenHeight);

    CreateInventoryButtons();


	return true;
}
bool PersonajesScreen::PreUpdate() {

    if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
    {
        app->fadeToBlack->FadeToBlackTransition(this, (Module*)app->scene, 0);
        app->entityManager->Play();
    }


	return true;
}
bool PersonajesScreen::Update(float dt) {

    if (app->input->GetButton(ControlID::LEFT) == KEY_REPEAT) //izquierda
    {
        if (timer.ReadMSec() >= 200)
        {
            if (menuIndex > 1) menuIndex--;
            else menuIndex = 9;
            timer.Start();
        }
    }
    if (app->input->GetButton(ControlID::RIGHT) == KEY_REPEAT) //derecha
    {
        if (timer.ReadMSec() >= 200)
        {
            if (menuIndex < 9) menuIndex++;
            else menuIndex = 1;
            timer.Start();
        }
    }

    ListItem<GuiControlButton*>* controlListItem = nullptr;
    for (controlListItem = inventoryButtons.start; controlListItem != NULL; controlListItem = controlListItem->next)
    {
        if (controlListItem->data != nullptr)
        {
            controlListItem->data->state = GuiControlState::NORMAL;
        }
    }
    if (inventoryButtons[menuIndex - 1] != nullptr)
    {
        inventoryButtons[menuIndex - 1]->state = GuiControlState::SELECTED;
    }

    if (app->input->GetButton(ControlID::CONFIRM) == KEY_DOWN && inventoryButtons.Count() >= menuIndex)
    {
        inventoryButtons[menuIndex - 1]->state = GuiControlState::PRESSED;
        inventoryButtons[menuIndex - 1]->NotifyMouseClick();
    }

    if (menuIndex == 1) app->render->DrawTexture(personaje1.get(), 0, 0, 0, NULL);
    else if (menuIndex == 2) app->render->DrawTexture(personaje2.get(), 0, 0, 0, NULL);
    else if (menuIndex == 3) app->render->DrawTexture(personaje3.get(), 0, 0, 0, NULL);
    else if (menuIndex == 4) app->render->DrawTexture(personaje4.get(), 0, 0, 0, NULL);
    else if (menuIndex == 5) app->render->DrawTexture(personaje5.get(), 0, 0, 0, NULL);
    else if (menuIndex == 6) app->render->DrawTexture(personaje6.get(), 0, 0, 0, NULL);
    else if (menuIndex == 7) app->render->DrawTexture(personaje7.get(), 0, 0, 0, NULL);
    else if (menuIndex == 8) app->render->DrawTexture(personaje8.get(), 0, 0, 0, NULL);
    else if (menuIndex == 9) app->render->DrawTexture(personaje9.get(), 0, 0, 0, NULL);
    
    return true;
}

bool PersonajesScreen::PostUpdate() {

    //draw item quantity
    
	
	return true;
}


bool PersonajesScreen::CleanUp(){


	if (personaje1 != nullptr)
	{
		app->tex->UnLoadSP(personaje1);
		personaje1 = nullptr;
	}

	if (personaje2 != nullptr)
	{
		app->tex->UnLoadSP(personaje2);
		personaje2 = nullptr;
	}

	if (personaje3 != nullptr)
	{
		app->tex->UnLoadSP(personaje3);
		personaje3 = nullptr;
	}

	if (personaje4 != nullptr)
	{
		app->tex->UnLoadSP(personaje4);
		personaje4 = nullptr;
	}	
    if (personaje5 != nullptr)
    {
        app->tex->UnLoadSP(personaje5);
        personaje5 = nullptr;
    }

    if (personaje6 != nullptr)
    {
        app->tex->UnLoadSP(personaje6);
        personaje6 = nullptr;
    }

    if (personaje7 != nullptr)
    {
        app->tex->UnLoadSP(personaje7);
		personaje7 = nullptr;
    }

    if (personaje8 != nullptr)
    {
        app->tex->UnLoadSP(personaje8);
        personaje8 = nullptr;
    }
    if (personaje9 != nullptr)
    {
        app->tex->UnLoadSP(personaje9);
        personaje9 = nullptr;
    }

    ListItem<GuiControlButton*>* controlListItem = nullptr;
    for (controlListItem = inventoryButtons.start; controlListItem != NULL; controlListItem = controlListItem->next) {
        app->guiManager->DestroyGuiControl(controlListItem->data);
    }
    inventoryButtons.Clear();

	return true;
}
void PersonajesScreen::CreateInventoryButtons() {

    if (this->active)
    {
        //fila 1
        int wBt = 190;
        int hBt = 40;
        int posBtX = screenWidth - 1250;
        int posBtY = screenHeight - 650;
        inventoryButtons.Add((GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Cola", { posBtX, posBtY, wBt, hBt }, this));

        wBt = 190;
        hBt = 40;
        posBtX = screenWidth - 950;
        posBtY = screenHeight - 650;
        inventoryButtons.Add((GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "Catnip", { posBtX, posBtY, wBt, hBt }, this));

        wBt = 190;
        hBt = 40;
        posBtX = screenWidth - 750;
        posBtY = screenHeight - 650;
        inventoryButtons.Add((GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "Pat", { posBtX, posBtY, wBt, hBt }, this));

        wBt = 190;
        hBt = 40;
        posBtX = screenWidth - 500;
        posBtY = screenHeight - 650;
        inventoryButtons.Add((GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, "Yogur", { posBtX, posBtY, wBt, hBt }, this));

        wBt = 190;
        hBt = 40;
        posBtX = screenWidth - 400;
        posBtY = screenHeight - 650;
        inventoryButtons.Add((GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, "Chu", { posBtX, posBtY, wBt, hBt }, this));

        //fila 2
        wBt = 190;
        hBt = 40;
        posBtX = screenWidth - 1250;
        posBtY = screenHeight - 450;
        inventoryButtons.Add((GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 6, "Car", { posBtX, posBtY, wBt, hBt }, this));

        wBt = 190;
        hBt = 40;
        posBtX = screenWidth - 950;
        posBtY = screenHeight - 450;
        inventoryButtons.Add((GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 7, "Birra", { posBtX, posBtY, wBt, hBt }, this));


        wBt = 190;
        hBt = 40;
        posBtX = screenWidth - 750;
        posBtY = screenHeight - 450;
        inventoryButtons.Add((GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 8, "Helado", { posBtX, posBtY, wBt, hBt }, this));

        wBt = 190;
        hBt = 40;
        posBtX = screenWidth - 500;
        posBtY = screenHeight - 450;
        inventoryButtons.Add((GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 9, "Leche", { posBtX, posBtY, wBt, hBt }, this));
    }
    
}