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
    name.Create("vsync");
}

PauseMenu::~PauseMenu()
{
}

bool PauseMenu::Awake() {
    vsycn = parameters.attribute("value").as_bool();

    return true;
}

bool PauseMenu::Start() {

  buttonFx = app->audio->LoadFx("Assets/Audio/Fx/basic_click.wav");

  pause1 = app->tex->Load("Assets/Screens/ContinuarSelect_Pausa.png");
  pause2 = app->tex->Load("Assets/Screens/OpcionesSelect_Pausa.png");
  pause3 = app->tex->Load("Assets/Screens/SalirSelect_Pausa.png");

    app->render->camera.x = 0;
	app->render->camera.y = 0;

	SDL_GetWindowSize(app->win->window, &screenWidth, &screenHeight);
    inpause = true;
    CreatePauseButtons();

	return true;
}
bool PauseMenu::Update(float dt) {


    

    if (PauseButtons.Count() != 0) {
        if (app->input->GetButton(ControlID::UP) == KEY_REPEAT) //arriba
        {
            if (timer.ReadMSec() >= 200)
            {
                if (PauseIndex > 1) PauseIndex--;
                else PauseIndex = 3;
                timer.Start();
            }
        }
        if (app->input->GetButton(ControlID::DOWN) == KEY_REPEAT) //abajo
        {
            if (timer.ReadMSec() >= 200)
            {
                if (PauseIndex < 3) PauseIndex++;
                else PauseIndex = 1;
                timer.Start();
            }
        }

        ListItem<GuiControlButton*>* controlListItem = nullptr;
        for (controlListItem = PauseButtons.start; controlListItem != NULL; controlListItem = controlListItem->next)
        {
            if (controlListItem->data != nullptr)
            {
                controlListItem->data->state = GuiControlState::NORMAL;
            }
        }
        if (PauseButtons[PauseIndex - 1] != nullptr)
        {
            PauseButtons[PauseIndex - 1]->state = GuiControlState::SELECTED;
        }

        if (app->input->GetButton(ControlID::CONFIRM) == KEY_DOWN && PauseButtons.Count() >= PauseIndex && timer.ReadMSec() >= 200)
        {
            app->audio->PlayFx(buttonFx);
            PauseButtons[PauseIndex - 1]->state = GuiControlState::PRESSED;
            PauseButtons[PauseIndex - 1]->NotifyMouseClick();
            timer.Start();
        }
    }
    if (AjustesButtons.Count() != 0) {
        if (app->input->GetButton(ControlID::UP) == KEY_REPEAT) //arriba
        {
            if (timer.ReadMSec() >= 200)
            {
                if (AjustesIndex > 1) AjustesIndex--;
                else AjustesIndex = 7;
                timer.Start();
            }
        }
        if (app->input->GetButton(ControlID::DOWN) == KEY_REPEAT) //abajo
        {
           if (timer.ReadMSec() >= 200)
            {
                if (AjustesIndex < 7) AjustesIndex++;
                else AjustesIndex = 1;
                timer.Start();
            }
        }

        ListItem<GuiControlButton*>* controlListItem = nullptr;
        for (controlListItem = AjustesButtons.start; controlListItem != NULL; controlListItem = controlListItem->next)
        {
            if (controlListItem->data != nullptr)
            {
                controlListItem->data->state = GuiControlState::NORMAL;
            }
        }
        if (AjustesButtons[AjustesIndex - 1] != nullptr)
        {
            AjustesButtons[AjustesIndex - 1]->state = GuiControlState::SELECTED;
        }

        if (app->input->GetButton(ControlID::CONFIRM) == KEY_DOWN && AjustesButtons.Count() >= AjustesIndex && timer.ReadMSec() >= 200)
        {
            app->audio->PlayFx(buttonFx);
            AjustesButtons[AjustesIndex - 1]->state = GuiControlState::PRESSED;
            AjustesButtons[AjustesIndex - 1]->NotifyMouseClick();
            timer.Start();
        }
    }


    

	return true;
}
bool PauseMenu::PostUpdate() {
    if (inpause == true) {
        weigth = screenWidth - 700;
      
        if (PauseIndex == 1) app->render->DrawTexture(pause1, weigth, 0, 0, NULL);
        else if (PauseIndex == 2) app->render->DrawTexture(pause2, weigth, 0, 0, NULL);
        else if (PauseIndex == 3) app->render->DrawTexture(pause3, weigth, 0, 0, NULL);
    }

	return true;
}

bool PauseMenu::CleanUp(){
    if (inpause == true) {


        if (pause1 != nullptr)
        {
            app->tex->UnLoad(pause1);
            pause1 = nullptr;
        }

        if (pause2 != nullptr)
        {
            app->tex->UnLoad(pause2);
            pause2 = nullptr;
        }

        if (pause3 != nullptr)
        {
            app->tex->UnLoad(pause3);
            pause3 = nullptr;
        }
    }


    ListItem<GuiControlButton*>* controlListItem = nullptr;
    for (controlListItem = PauseButtons.start; controlListItem != NULL; controlListItem = controlListItem->next) {
        app->guiManager->DestroyGuiControl(controlListItem->data);
    }
    PauseButtons.Clear();
    ListItem<GuiControlButton*>* controlListItem2 = nullptr;
    for (controlListItem2 = AjustesButtons.start; controlListItem2 != NULL; controlListItem2 = controlListItem2->next) {
        app->guiManager->DestroyGuiControl(controlListItem2->data);
    }
    AjustesButtons.Clear();

	return true;
}
void PauseMenu::CreatePauseButtons() {

    if (PauseButtons.Count() == 0 && AjustesButtons.Count() == 0)
    {
        
            if (inpause == true) {
                buttoncreated = true;
                int wBt = 390;
                int hBt = 140;
                int posBtX = screenWidth - 550;
                int posBtY = screenHeight - 850;
                PauseButtons.Add((GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Continuar", { posBtX, posBtY, wBt, hBt }, [this](GuiControl* g) {continuar(g); }));
                wBt = 370;
                hBt = 100;
                posBtX = screenWidth - 535;
                posBtY = screenHeight - 700;
                PauseButtons.Add((GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "Opciones", { posBtX, posBtY, wBt, hBt }, [this](GuiControl* g) {opciones(g); }));
                wBt = 190;
                hBt = 80;
                posBtX = screenWidth - 400;
                posBtY = screenHeight - 300;
                PauseButtons.Add((GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "Salir", { posBtX, posBtY, wBt, hBt }, [this](GuiControl* g) {salir(g); }));
                
              
            }
            else if (inajustes == true) {
                ajustcreated = true;
                int wBt ;
                int hBt;
                int posBtX ;
                int posBtY ;
                
                wBt = 190;
                hBt = 40;
                posBtX = screenWidth - 750;
                posBtY = screenHeight - 550;
                AjustesButtons.Add((GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Fullscreen", { posBtX, posBtY, wBt, hBt }, [this](GuiControl* g) {fullscreen1(g); }));
                wBt = 190;
                hBt = 40;
                posBtX = screenWidth - 750;
                posBtY = screenHeight - 450;
                AjustesButtons.Add((GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "Vsycn", { posBtX, posBtY, wBt, hBt }, [this](GuiControl* g) {vyscn(g); }));
                wBt = 70;
                hBt = 40;
                posBtX = screenWidth - 750;
                posBtY = screenHeight - 350;
                AjustesButtons.Add((GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "Volum -", { posBtX, posBtY, wBt, hBt }, [this](GuiControl* g) {volumen1(g); }));
                wBt = 70;
                hBt = 40;
                posBtX = screenWidth - 630;
                posBtY = screenHeight - 350;
                AjustesButtons.Add((GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, "Volum +", { posBtX, posBtY, wBt, hBt }, [this](GuiControl* g) {volumen2(g); }));
                wBt = 70;
                hBt = 40;
                posBtX = screenWidth - 750;
                posBtY = screenHeight - 250;
                AjustesButtons.Add((GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, "Fbx -", { posBtX, posBtY, wBt, hBt }, [this](GuiControl* g) {fx1(g); }));
                wBt = 70;
                hBt = 40;
                posBtX = screenWidth - 630;
                posBtY = screenHeight - 250;
                AjustesButtons.Add((GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 6, "Fbx +", { posBtX, posBtY, wBt, hBt }, [this](GuiControl* g) {fx2(g); }));
                wBt = 190;
                hBt = 40;
                posBtX = screenWidth - 750;
                posBtY = screenHeight - 150;
                AjustesButtons.Add((GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 7, "Volver", { posBtX, posBtY, wBt, hBt }, [this](GuiControl* g) {volver(g); }));

            }
        
        
    }
    
}

    void PauseMenu::continuar(GuiControl * ctrl)
    {
        app->fadeToBlack->FadeToBlackTransition((Module*)app->pause, (Module*)app->scene, 0.0f);
        buttoncreated = false;
        app->audio->PlayFx(buttonFx);
    }
    void PauseMenu::opciones(GuiControl * ctrl)
    {
        inpause = false;
        inajustes = true;
        CleanUp();
        
        CreatePauseButtons();
    }
    void PauseMenu::salir(GuiControl * ctrl)
    {
        app->fadeToBlack->FadeToBlackTransition((Module*)app->pause, (Module*)app->titlescreen, 0.0f);
        app->scene->Disable();
        buttoncreated = false;
        app->audio->PlayFx(buttonFx);
    }
    void PauseMenu::volver(GuiControl* ctrl) {
        CleanUp();
        inpause = true;
        inajustes = false;
        CreatePauseButtons();
    }
    void PauseMenu::fullscreen1(GuiControl* ctrl) {
        if (fullscreen == true) {
            app->win->FullscreenMode();
            fullscreen = false;
        }
        else {
            app->win->UnFullscreenMode();
            fullscreen = true;
        }
    }
    void PauseMenu::vyscn(GuiControl* ctrl) {
        if (vsycn == true) {
            vsycn = false;
        }
        else if (vsycn == false) {
            vsycn = true;
        }
    }
    void PauseMenu::volumen1(GuiControl* ctrl) {
        app->audio->MusicDown();
    }
    void PauseMenu::volumen2(GuiControl* ctrl) {
        app->audio->MusicUp();
    }
    void PauseMenu::fx1(GuiControl* ctrl) {
        app->audio->FxDown();
    }
    void PauseMenu::fx2(GuiControl* ctrl) {
        app->audio->FxUp();
    }