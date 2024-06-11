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
#include "Creditos.h"

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
    buttonFx = app->audio->LoadFx("Assets/Audio/Fx/basic_click.wav");
    menuFx = app->audio->LoadFx("Assets/Audio/Fx/menuFX.wav");
    
    menu1 = app->tex->LoadSP("Assets/Screens/mainMenu1.png", true);
    menu2 = app->tex->LoadSP("Assets/Screens/mainMenu2.png", true);
    menu3 = app->tex->LoadSP("Assets/Screens/mainMenu3.png", true);
    menu4 = app->tex->LoadSP("Assets/Screens/mainMenu4.png", true);

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
        titleButtons[menuIndex - 1]->NotifyMouseClick();
    }

  /*
    if (menuIndex == 1) app->render->DrawTexture(menu1, 0, 0, NULL);
    else if (menuIndex == 2) app->render->DrawTexture(menu2, 0, 0, NULL);
    else if (menuIndex == 3) app->render->DrawTexture(menu3, 0, 0, NULL);
    else if (menuIndex == 4) app->render->DrawTexture(menu4, 0, 0, NULL);*/

    return true;
}

bool TitleScreen::PostUpdate()
{
    if (menuIndex == 1) app->render->DrawTexture(menu1.get(), 0, 0, NULL);
    else if (menuIndex == 2) app->render->DrawTexture(menu2.get(), 0, 0, NULL);
    else if (menuIndex == 3) app->render->DrawTexture(menu3.get(), 0, 0, NULL);
    else if (menuIndex == 4) app->render->DrawTexture(menu4.get(), 0, 0, NULL);


    return true;
}

// Called before quitting
bool TitleScreen::CleanUp()
{
    if (menu1 != nullptr)
    {
        app->tex->UnLoadSP(menu1);
        menu1 = nullptr;
    }

    if (menu2 != nullptr)
    {
        app->tex->UnLoadSP(menu2);
        menu2 = nullptr;
    }

    if (menu3 != nullptr)
    {
        app->tex->UnLoadSP(menu3);
        menu3 = nullptr;
    }

    if (menu4 != nullptr)
    {
        app->tex->UnLoadSP(menu4);
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
void Credits(GuiControl* ctrl);
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
        titleButtons.Add((GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "Options", { posBtX, posBtY, wBt, hBt }, Options));
        wBt = 190;
        hBt = 40;
        posBtX = screenWidth / 2 - 220;
        posBtY = screenHeight / 2 + 210;
        titleButtons.Add((GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "Credits", { posBtX, posBtY, wBt, hBt }, Credits));
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

void Credits(GuiControl* ctrl)
{
    app->fadeToBlack->FadeToBlackTransition((Module*)app->titlescreen, (Module*)app->creditos, 0.0f);
}

void Exit(GuiControl* ctrl)
{
    app->Quit(); 
}