#include "menu.h"
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "Map.h"
#include "Reload.h"
#include "GuiManager.h"
#include "GuiControl.h"
#include "GuiControlButton.h"
#include "SDL/include/SDL.h"
#include "FadeToBlack.h"
#include "Input.h"
#include "Scene.h"
#include "IntroScreen.h"
#include "TeamScreen.h"
#include <iostream>
#include "Combat.h"
#include <vector>

TitleScreen::TitleScreen(bool startEnabled) : Module(startEnabled)
{
    name.Create("titlescreen");
}

// Destructor
TitleScreen::~TitleScreen()
{}

// Called before render is available
bool TitleScreen::Start()
{
    buttonFx = app->audio->LoadFx("Assets/Audio/Fx/buttonFX.wav");
    menuFx = app->audio->LoadFx("Assets/Audio/Fx/menuFX.wav");
    
    menu1 = app->tex->Load("Assets/Screens/mainMenu1.jpg");
    menu2 = app->tex->Load("Assets/Screens/mainMenu2.jpg");
    menu3 = app->tex->Load("Assets/Screens/mainMenu3.jpg");
    menu4 = app->tex->Load("Assets/Screens/mainMenu4.jpg");

    app->render->camera.x = 0;
    app->render->camera.y = 0;

    SDL_GetWindowSize(app->win->window, &screenWidth, &screenHeight);

    CreateTitleButtons();

    app->audio->PlayFx(menuFx);

    return true;
}

// Called each loop iteration
bool TitleScreen::Update(float dt)
{   

    if (app->input->GetButton(ControlID::UP) == KEY_REPEAT) //arriba
    {
        if (timer.ReadMSec() >= 200)
        {
            if (menuIndex > 1) menuIndex--;
            else menuIndex = 4;
            timer.Start();
        }
    }
    if (app->input->GetButton(ControlID::DOWN) == KEY_REPEAT) //abajo
    {
        if (timer.ReadMSec() >= 200)
        {
            if (menuIndex < 4) menuIndex++;
            else menuIndex = 1;
            timer.Start();
        }
    }

    ListItem<GuiControlButton*>* controlListItem = nullptr;
    for (controlListItem = titleButtons.start; controlListItem != NULL; controlListItem = controlListItem->next)
    {
        if (controlListItem->data != nullptr)
        {
            controlListItem->data->state = GuiControlState::NORMAL;
        }
    }
    if (titleButtons[menuIndex - 1] != nullptr)
    {
        titleButtons[menuIndex - 1]->state = GuiControlState::SELECTED;
    }

    if (app->input->GetButton(ControlID::CONFIRM) == KEY_DOWN && titleButtons.Count() >= menuIndex)
    {
        app->audio->PlayFx(buttonFx);
        titleButtons[menuIndex - 1]->state = GuiControlState::PRESSED;
        titleButtons[menuIndex - 1]->NotifyObserver();
    }

  /*
    if (menuIndex == 1) app->render->DrawTexture(menu1, 0, 0, NULL);
    else if (menuIndex == 2) app->render->DrawTexture(menu2, 0, 0, NULL);
    else if (menuIndex == 3) app->render->DrawTexture(menu3, 0, 0, NULL);
    else if (menuIndex == 4) app->render->DrawTexture(menu4, 0, 0, NULL);*/


    // Combat de mostra -> Tecla C
    if (app->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
    {
        // Crear equips de un enemic o mes
        std::cout << "Init combat" << std::endl;
        std::vector<Enemic*> equipJugador;
        std::vector<Enemic*> equipOponent;

        // Cada enemic
        Enemic enemic1("Enemic1", 10, 10, 10, 10);
        // Els seus atacs
        Atac atac1("Atac1", 50);
        Atac atac2("Atac2", 50);
        Atac atac3("Atac3", 50);
        Atac atac4("Atac4", 50);
        enemic1.atacs.push_back(atac1);
        enemic1.atacs.push_back(atac2);
        enemic1.atacs.push_back(atac3);
        enemic1.atacs.push_back(atac4);

        Enemic enemic2("Enemic2", 100, 100, 100, 100);
        enemic2.atacs.push_back(atac1);
        enemic2.atacs.push_back(atac2);
        enemic2.atacs.push_back(atac3);
        enemic2.atacs.push_back(atac4);

        equipJugador.push_back(&enemic2);
        equipOponent.push_back(&enemic1);

        // Comen�ar el combat
        Combat nouCombat(equipJugador, equipOponent);
        nouCombat.Iniciar();
        std::cout << "End Combat" << std::endl;
    }

    return true;
}

bool TitleScreen::PostUpdate()
{
    if (menuIndex == 1) app->render->DrawTexture(menu1, 0, 0, NULL);
    else if (menuIndex == 2) app->render->DrawTexture(menu2, 0, 0, NULL);
    else if (menuIndex == 3) app->render->DrawTexture(menu3, 0, 0, NULL);
    else if (menuIndex == 4) app->render->DrawTexture(menu4, 0, 0, NULL);

    return true;
}

// Called before quitting
bool TitleScreen::CleanUp()
{
    if (menu1 != nullptr)
    {
        app->tex->UnLoad(menu1);
        menu1 = nullptr;
    }

    if (menu2 != nullptr)
    {
        app->tex->UnLoad(menu2);
        menu2 = nullptr;
    }

    if (menu3 != nullptr)
    {
        app->tex->UnLoad(menu3);
        menu3 = nullptr;
    }

    if (menu4 != nullptr)
    {
        app->tex->UnLoad(menu4);
        menu4 = nullptr;
    }

    if (menuFx > 0) {
        app->audio->UnloadFx(menuFx);
        menuFx = 0;
    }


    ListItem<GuiControlButton*>* controlListItem = nullptr;
    for (controlListItem = titleButtons.start; controlListItem != NULL; controlListItem = controlListItem->next)
    {
        app->guiManager->DestroyGuiControl(controlListItem->data);
    }
    titleButtons.Clear();

    return true;
}

void NewGame(GuiControl* ctrl);
void Continue(GuiControl* ctrl);
void Options(GuiControl* ctrl);
void Exit(GuiControl* ctrl);

void TitleScreen::CreateTitleButtons()
{
    if (this->active)
    {
        int wBt = 190;
        int hBt = 40;
        int posBtX = screenWidth / 2 - 220;
        int posBtY = screenHeight / 2 - 10;
        titleButtons.Add((GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Start", { posBtX, posBtY, wBt, hBt }, NewGame));
        wBt = 190;
        hBt = 40;
        posBtX = screenWidth / 2 - 220;
        posBtY = screenHeight / 2 + 100;
        titleButtons.Add((GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "Continue", { posBtX, posBtY, wBt, hBt }, Continue));
        wBt = 190;
        hBt = 40;
        posBtX = screenWidth / 2 - 220;
        posBtY = screenHeight / 2 + 210;
        titleButtons.Add((GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "Optiones", { posBtX, posBtY, wBt, hBt }, Options));
        wBt = 30;
        hBt = 30;
        posBtX = screenWidth / 2 + 450;
        posBtY = screenHeight / 2 - 330;
        titleButtons.Add((GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, "Exit", { posBtX, posBtY, wBt, hBt }, Exit));
    }
}

void NewGame(GuiControl* ctrl)
{
    app->map->ChangeMap(0);
}

void Continue(GuiControl* ctrl)
{
    app->map->ChangeMap(1); // TODO el id de mapa debe depender de lo que haya en la partida guardada en el caso de continuar partida. Para ello esta llamada debe cambiarse por una al gestor de partidas (cuando exista)
}

void Options(GuiControl* ctrl)
{
}

void Exit(GuiControl* ctrl)
{
    app->Quit();
}