#include "InventoryScreen.h"
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


InventoryScreen::InventoryScreen(bool startEnabled) : Module(startEnabled)
{
    name.Create("inventory");
}

InventoryScreen::~InventoryScreen()
{
}


bool InventoryScreen::Start() {
	
    buttonFx = app->audio->LoadFx("Assets/Audio/Fx/basic_click.wav");

    inventory1 = app->tex->LoadSP("Assets/Textures/InventarioCatCola.png", true);
    inventory2 = app->tex->LoadSP("Assets/Textures/InventarioCatnip.png", true);
    inventory3 = app->tex->LoadSP("Assets/Textures/InventarioPatatas.png", true);
    inventory4 = app->tex->LoadSP("Assets/Textures/InventarioYogur.png", true);
    inventory5 = app->tex->LoadSP("Assets/Textures/InventarioChuches.png", true);
    inventory6 = app->tex->LoadSP("Assets/Textures/InventarioCaramelos.png", true);
    inventory7 = app->tex->LoadSP("Assets/Textures/InventarioBirra.png", true);
    inventory8 = app->tex->LoadSP("Assets/Textures/InventarioHelado.png", true);
    inventory9 = app->tex->LoadSP("Assets/Textures/InventarioLeche.png", true);
    inventory10 = app->tex->LoadSP("Assets/Textures/InventarioVelocidad.png", true);
    inventory11 = app->tex->LoadSP("Assets/Textures/InventarioPastel.png", true);
    inventory12 = app->tex->LoadSP("Assets/Textures/InventarioMapaEnemigo.png", true);
    inventory13 = app->tex->LoadSP("Assets/Textures/InventarioMapaMazmorra.png", true);
    inventory14 = app->tex->LoadSP("Assets/Textures/InventarioUbicacion.png", true);


    app->render->camera.x = 0;
    app->render->camera.y = 0;

	SDL_GetWindowSize(app->win->window, &screenWidth, &screenHeight);

    CreateInventoryButtons();

	return true;
}
bool InventoryScreen::PreUpdate() {

    if (app->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN)
    {
        app->fadeToBlack->FadeToBlackTransition(this, (Module*)app->scene, 0);
        app->entityManager->Play();
    }

	return true;
}
bool InventoryScreen::Update(float dt) {
    
	valorCoca.Create("x%d", app->scene->GetColaQuantity());
    valorBirra.Create("x%d", app->scene->GetBirraQuantity());
    valorPatatas.Create("x%d", app->scene->GetPatatasQuantity());
    valorCaramelos.Create("x%d", app->scene->GetCaramelosQuantity());
    valorVelocidad.Create("x%d", app->scene->GetVelocidadQuantity());
    valorYogur.Create("x%d", app->scene->GetYogurQuantity());


    if (app->input->GetButton(ControlID::UP) == KEY_REPEAT) //arriba
    {
        if (timer.ReadMSec() >= 200)
        {
            if (menuIndex > 1) menuIndex--;
            else menuIndex = 14;
            timer.Start();
        }
    }
    if (app->input->GetButton(ControlID::DOWN) == KEY_REPEAT) //abajo
    {
        if (timer.ReadMSec() >= 200)
        {
            if (menuIndex < 14) menuIndex++;
            else menuIndex = 1;
            timer.Start();
        }
    }
    if (app->input->GetButton(ControlID::LEFT) == KEY_REPEAT) //izquierda
    {
        if (timer.ReadMSec() >= 200)
        {
            if (menuIndex > 1) menuIndex--;
            else menuIndex = 14;
            timer.Start();
        }
    }
    if (app->input->GetButton(ControlID::RIGHT) == KEY_REPEAT) //derecha
    {
        if (timer.ReadMSec() >= 200)
        {
            if (menuIndex < 14) menuIndex++;
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

    if (menuIndex == 1) app->render->DrawTexture(inventory1.get(), 0, 0, 0, NULL);
    else if (menuIndex == 2) app->render->DrawTexture(inventory2.get(), 0, 0, 0, NULL);
    else if (menuIndex == 3) app->render->DrawTexture(inventory3.get(), 0, 0, 0, NULL);
    else if (menuIndex == 4) app->render->DrawTexture(inventory4.get(), 0, 0, 0, NULL);
    else if (menuIndex == 5) app->render->DrawTexture(inventory5.get(), 0, 0, 0, NULL);
    else if (menuIndex == 6) app->render->DrawTexture(inventory6.get(), 0, 0, 0, NULL);
    else if (menuIndex == 7) app->render->DrawTexture(inventory7.get(), 0, 0, 0, NULL);
    else if (menuIndex == 8) app->render->DrawTexture(inventory8.get(), 0, 0, 0, NULL);
    else if (menuIndex == 9) app->render->DrawTexture(inventory9.get(), 0, 0, 0, NULL);
    else if (menuIndex == 10) app->render->DrawTexture(inventory10.get(), 0, 0, 0, NULL);
    else if (menuIndex == 11) app->render->DrawTexture(inventory11.get(), 0, 0, 0, NULL);
    else if (menuIndex == 12) app->render->DrawTexture(inventory12.get(), 0, 0, 0, NULL);
    else if (menuIndex == 13) app->render->DrawTexture(inventory13.get(), 0, 0, 0, NULL);
    else if (menuIndex == 14) app->render->DrawTexture(inventory14.get(), 0, 0, 0, NULL);
    
    return true;
}

bool InventoryScreen::PostUpdate() {

    //draw item quantity
    app->render->DrawText(valorCoca.GetString(), 100, 365, 64, 64);
	app->render->DrawText(valorBirra.GetString(), 325, 650, 64, 64);
	app->render->DrawText(valorYogur.GetString(), 775, 365, 64, 64);
    app->render->DrawText(valorCaramelos.GetString(), 100, 650, 64, 64);
	app->render->DrawText(valorVelocidad.GetString(), 1000, 650, 64, 64);
	app->render->DrawText(valorPatatas.GetString(), 550, 365, 64, 64);
	
	return true;
}


bool InventoryScreen::CleanUp(){


	if (inventory1 != nullptr)
	{
		app->tex->UnLoadSP(inventory1);
		inventory1 = nullptr;
	}

	if (inventory2 != nullptr)
	{
		app->tex->UnLoadSP(inventory2);
		inventory2 = nullptr;
	}

	if (inventory3 != nullptr)
	{
		app->tex->UnLoadSP(inventory3);
		inventory3 = nullptr;
	}

	if (inventory4 != nullptr)
	{
		app->tex->UnLoadSP(inventory4);
		inventory4 = nullptr;
	}	
    if (inventory5 != nullptr)
    {
        app->tex->UnLoadSP(inventory5);
        inventory5 = nullptr;
    }

    if (inventory6 != nullptr)
    {
        app->tex->UnLoadSP(inventory6);
        inventory6 = nullptr;
    }

    if (inventory7 != nullptr)
    {
        app->tex->UnLoadSP(inventory7);
        inventory7 = nullptr;
    }

    if (inventory8 != nullptr)
    {
        app->tex->UnLoadSP(inventory8);
        inventory8 = nullptr;
    }
    if (inventory9 != nullptr)
    {
        app->tex->UnLoadSP(inventory9);
        inventory9 = nullptr;
    }

    if (inventory10 != nullptr)
    {
        app->tex->UnLoadSP(inventory10);
        inventory10 = nullptr;
    }

    if (inventory11 != nullptr)
    {
        app->tex->UnLoadSP(inventory11);
        inventory11 = nullptr;
    }

    if (inventory12 != nullptr)
    {
        app->tex->UnLoadSP(inventory12);
        inventory12 = nullptr;
    }

	if (inventory13 != nullptr)
	{
		app->tex->UnLoadSP(inventory13);
		inventory13 = nullptr;
	}

	if (inventory14 != nullptr)
	{
		app->tex->UnLoadSP(inventory14);
		inventory14 = nullptr;
	}

    ListItem<GuiControlButton*>* controlListItem = nullptr;
    for (controlListItem = inventoryButtons.start; controlListItem != NULL; controlListItem = controlListItem->next) {
        app->guiManager->DestroyGuiControl(controlListItem->data);
    }
    inventoryButtons.Clear();

	return true;
}
void InventoryScreen::CreateInventoryButtons() {

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
        inventoryButtons.Add((GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "Patatas", { posBtX, posBtY, wBt, hBt }, this));

        wBt = 190;
        hBt = 40;
        posBtX = screenWidth - 500;
        posBtY = screenHeight - 650;
        inventoryButtons.Add((GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, "Yogur", { posBtX, posBtY, wBt, hBt }, this));

        wBt = 190;
        hBt = 40;
        posBtX = screenWidth - 250;
        posBtY = screenHeight - 650;
        inventoryButtons.Add((GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, "Chuches", { posBtX, posBtY, wBt, hBt }, this));

        //fila 2
        wBt = 190;
        hBt = 40;
        posBtX = screenWidth - 1250;
        posBtY = screenHeight - 450;
        inventoryButtons.Add((GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 6, "Caramelos", { posBtX, posBtY, wBt, hBt }, this));

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

		wBt = 190;
		hBt = 40;
		posBtX = screenWidth - 250;
		posBtY = screenHeight - 450;
		inventoryButtons.Add((GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 10, "Velocidad", { posBtX, posBtY, wBt, hBt }, this));

        //fila 3
		wBt = 190;
		hBt = 40;
		posBtX = screenWidth - 1250;
		posBtY = screenHeight - 250;
		inventoryButtons.Add((GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 11, "Pastel", { posBtX, posBtY, wBt, hBt }, this));

        wBt = 190;
        hBt = 40;
        posBtX = screenWidth - 950;
        posBtY = screenHeight - 250;
        inventoryButtons.Add((GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 12, "Enemigos", { posBtX, posBtY, wBt, hBt }, this));

		
		wBt = 190;
		hBt = 40;
		posBtX = screenWidth - 750;
		posBtY = screenHeight - 250;
		inventoryButtons.Add((GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 13, "Dungeon", { posBtX, posBtY, wBt, hBt }, this));

		wBt = 190;
		hBt = 40;
		posBtX = screenWidth - 500;
		posBtY = screenHeight - 250;
		inventoryButtons.Add((GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 14, "Ubicacion", { posBtX, posBtY, wBt, hBt }, this));
        
        
    }
}