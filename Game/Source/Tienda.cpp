#include "Tienda.h"
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
#include "InventoryManager.h"
#include "MusicaCiudad.h"

Tienda::Tienda(bool startEnabled) : Module(startEnabled)
{
    name.Create("inventory");
}

Tienda::~Tienda()
{
}


bool Tienda::Start() {
	
    buttonFx = app->audio->LoadFx("Assets/Audio/Fx/basic_click.wav");

    tienda1 = app->tex->LoadSP("Assets/Textures/tienda1.png", true);
    tienda2 = app->tex->LoadSP("Assets/Textures/tienda2.png", true);
    tienda3 = app->tex->LoadSP("Assets/Textures/tienda3.png", true);
    tienda4 = app->tex->LoadSP("Assets/Textures/tienda4.png", true);
    tienda5 = app->tex->LoadSP("Assets/Textures/tienda5.png", true);
    tienda6 = app->tex->LoadSP("Assets/Textures/tienda6.png", true);
    tienda7 = app->tex->LoadSP("Assets/Textures/tienda7.png", true);
    tienda8 = app->tex->LoadSP("Assets/Textures/tienda8.png", true);
    tienda9 = app->tex->LoadSP("Assets/Textures/tienda9.png", true);
    tienda10 = app->tex->LoadSP("Assets/Textures/tienda10.png", true);
    tienda11 = app->tex->LoadSP("Assets/Textures/tienda11.png", true);
    tienda12 = app->tex->LoadSP("Assets/Textures/tienda12.png", true);
    tienda13 = app->tex->LoadSP("Assets/Textures/tienda13.png", true);


    app->render->camera.x = 0;
    app->render->camera.y = 0;

	SDL_GetWindowSize(app->win->window, &screenWidth, &screenHeight);

    buyFx = app->audio->LoadFx("Assets/Audio/Fx/buy.wav");
    tiendaMusic = app->audio->LoadFx("Assets/Audio/Fx/tiendaFx.wav");

    CreateInventoryButtons();

    app->audio->PlayFx(tiendaMusic, 100);

	return true;
}
bool Tienda::PreUpdate() {
        
   
	return true;
}
bool Tienda::Update(float dt) {

    dineroActual.Create("%d", app->scene->GetMonedasQuantity());

    if (app->input->GetButton(ControlID::LEFT) == KEY_REPEAT) //izquierda
    {
        if (timer.ReadMSec() >= 200)
        {
            if (menuIndex > 1) menuIndex--;
            else menuIndex = 13;
            timer.Start();
        }
    }
    if (app->input->GetButton(ControlID::RIGHT) == KEY_REPEAT) //derecha
    {
        if (timer.ReadMSec() >= 200)
        {
            if (menuIndex < 13) menuIndex++;
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

    if (menuIndex == 1) {
        app->render->DrawTexture(tienda1.get(), 0, 0, 0, NULL);
        if (app->scene->GetMonedasQuantity() > 4)
        {
            if (app->input->GetButton(ControlID::CONFIRM) == KEY_DOWN)
            {
                app->scene->ColaPicked();
                app->audio->PlayFx(buyFx);
                app->scene->RestarCincoMonedas();
            }
        }
    }
    else if (menuIndex == 2)
    {
        app->render->DrawTexture(tienda2.get(), 0, 0, 0, NULL);
        if (app->scene->GetMonedasQuantity() > 9)
        {
            if (app->input->GetButton(ControlID::CONFIRM) == KEY_DOWN)
            {
                app->scene->ColaPicked();
                app->scene->ColaPicked();
                app->audio->PlayFx(buyFx);
                app->scene->RestarDiezMonedas();
            }
        }
    }
    else if (menuIndex == 3) {
        app->render->DrawTexture(tienda3.get(), 0, 0, 0, NULL);
        if (app->scene->GetMonedasQuantity() > 4)
        {
            if (app->input->GetButton(ControlID::CONFIRM) == KEY_DOWN)
            {
                app->scene->YogurPicked();
                app->audio->PlayFx(buyFx);
                app->scene->RestarCincoMonedas();
            }
        }
    }
    else if (menuIndex == 4) {
        app->render->DrawTexture(tienda4.get(), 0, 0, 0, NULL);
        if (app->scene->GetMonedasQuantity() > 4)
        {
            if (app->input->GetButton(ControlID::CONFIRM) == KEY_DOWN)
            {
                //chuches
                app->scene->ChuchesPicked();
                app->audio->PlayFx(buyFx);
                app->scene->RestarCincoMonedas();
            }
        }
    }
    else if (menuIndex == 5) {
        app->render->DrawTexture(tienda5.get(), 0, 0, 0, NULL);
        if (app->scene->GetMonedasQuantity() > 4)
        {
            if (app->input->GetButton(ControlID::CONFIRM) == KEY_DOWN)
            {
                app->scene->PatatasPicked();
                app->audio->PlayFx(buyFx);
                app->scene->RestarCincoMonedas();
            }
        }
    }
    else if (menuIndex == 6) {
        app->render->DrawTexture(tienda6.get(), 0, 0, 0, NULL);
        if (app->scene->GetMonedasQuantity() > 4)
        {
            if (app->input->GetButton(ControlID::CONFIRM) == KEY_DOWN)
            {
                //helado
                app->scene->HeladoPicked();
                app->audio->PlayFx(buyFx);
                app->scene->RestarCincoMonedas();
            }
        }
    }
    else if (menuIndex == 7) {
        app->render->DrawTexture(tienda7.get(), 0, 0, 0, NULL);
        if (app->scene->GetMonedasQuantity() > 4)
        {
            if (app->input->GetButton(ControlID::CONFIRM) == KEY_DOWN)
            {
                app->scene->VelocidadPicked();
                app->audio->PlayFx(buyFx);
                app->scene->RestarCincoMonedas();
            }
        }
    }
    else if (menuIndex == 8) {
        app->render->DrawTexture(tienda8.get(), 0, 0, 0, NULL);
        if (app->scene->GetMonedasQuantity() > 9)
        {
            if (app->input->GetButton(ControlID::CONFIRM) == KEY_DOWN)
            {
                app->scene->VelocidadPicked();
                app->scene->VelocidadPicked();
                app->audio->PlayFx(buyFx);
                app->scene->RestarDiezMonedas();
            }
        }
    }
    else if (menuIndex == 9) {
        app->render->DrawTexture(tienda9.get(), 0, 0, 0, NULL);
        if (app->scene->GetMonedasQuantity() > 4)
        {
            if (app->input->GetButton(ControlID::CONFIRM) == KEY_DOWN)
            {
                app->scene->BirraPicked();
                app->audio->PlayFx(buyFx);
                app->scene->RestarCincoMonedas();
            }
        }
    }
    else if (menuIndex == 10) {
        app->render->DrawTexture(tienda10.get(), 0, 0, 0, NULL);
        if (app->scene->GetMonedasQuantity() > 19)
        {
            if (app->input->GetButton(ControlID::CONFIRM) == KEY_DOWN)
            {
                app->scene->CaramelosPicked();
                app->audio->PlayFx(buyFx);
                app->scene->RestarVeinteMonedas();
            }
        }
    }
    else if (menuIndex == 11){
        app->render->DrawTexture(tienda11.get(), 0, 0, 0, NULL);
        if (app->scene->GetMonedasQuantity() > 4)
        {
            if (app->input->GetButton(ControlID::CONFIRM) == KEY_DOWN)
            {
				//pastel
                app->scene->PastelPicked();
                app->audio->PlayFx(buyFx);
				app->scene->RestarCincoMonedas();

			}
		}
    }
    else if (menuIndex == 12) app->render->DrawTexture(tienda12.get(), 0, 0, 0, NULL);
    else if (menuIndex == 13) app->render->DrawTexture(tienda13.get(), 0, 0, 0, NULL);
    
  
    return true;
}

bool Tienda::PostUpdate() {

	app->render->DrawText(dineroActual.GetString(), 110, 50, 64, 64);
	return true;
}


bool Tienda::CleanUp(){

    app->audio->UnloadFx(buyFx);
	app->audio->UnloadFx(tiendaMusic);

	if (tienda1 != nullptr)
	{
		app->tex->UnLoadSP(tienda1);
		tienda1 = nullptr;
	}

	if (tienda2 != nullptr)
	{
		app->tex->UnLoadSP(tienda2);
		tienda2 = nullptr;
	}

	if (tienda3 != nullptr)
	{
		app->tex->UnLoadSP(tienda3);
		tienda3 = nullptr;
	}

	if (tienda4 != nullptr)
	{
		app->tex->UnLoadSP(tienda4);
		tienda4 = nullptr;
	}	
    if (tienda5 != nullptr)
    {
        app->tex->UnLoadSP(tienda5);
		tienda5 = nullptr;
    }

    if (tienda6 != nullptr)
    {
        app->tex->UnLoadSP(tienda6);
		tienda6 = nullptr;
    }

    if (tienda7 != nullptr)
    {
        app->tex->UnLoadSP(tienda7);
		tienda7 = nullptr;
    }

    if (tienda8 != nullptr)
    {
        app->tex->UnLoadSP(tienda8);
		tienda8 = nullptr;
    }
    if (tienda9 != nullptr)
    {
        app->tex->UnLoadSP(tienda9);
        tienda9 = nullptr;
    }

    if (tienda10 != nullptr)
    {
        app->tex->UnLoadSP(tienda10);
		tienda10 = nullptr;
    }

    if (tienda11 != nullptr)
    {
        app->tex->UnLoadSP(tienda11);
        tienda11 = nullptr;
    }

    if (tienda12 != nullptr)
    {
        app->tex->UnLoadSP(tienda12);
		tienda12 = nullptr;
    }

	if (tienda13 != nullptr)
	{
		app->tex->UnLoadSP(tienda13);
		tienda13 = nullptr;
	}

    ListItem<GuiControlButton*>* controlListItem = nullptr;
    for (controlListItem = inventoryButtons.start; controlListItem != NULL; controlListItem = controlListItem->next) {
        app->guiManager->DestroyGuiControl(controlListItem->data);
    }
    inventoryButtons.Clear();

	return true;
}
void Tienda::CreateInventoryButtons() {

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
        posBtX = screenWidth - 450;
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
		posBtX = screenWidth - 450;
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
        
    }
}