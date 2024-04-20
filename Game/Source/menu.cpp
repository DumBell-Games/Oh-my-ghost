#include "menu.h"
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
#include "IntroScreen.h"
#include "TeamScreen.h"

TitleScreen::TitleScreen(bool startEnabled) : Module()
{
}

// Destructor
TitleScreen::~TitleScreen()
{}

// Called before render is available
bool TitleScreen::Start()
{
    app->scene->active = false;
    app->scene->Disable();
    
    menuFx = app->audio->LoadFx("Assets/Audio/Fx/menuFX.wav");
    
    menu1 = app->tex->Load("Assets/Screens/mainMenu1.jpg");
    menu2 = app->tex->Load("Assets/Screens/mainMenu2.jpg");
    menu3 = app->tex->Load("Assets/Screens/mainMenu3.jpg");
    menu4 = app->tex->Load("Assets/Screens/mainMenu4.jpg");

    app->render->camera.x = 0;
    app->render->camera.y = 0;

    SDL_GetWindowSize(app->win->window, &screenWidth, &screenHeight);

    CreateTitleButtons();

    return true;
}

// Called each loop iteration
bool TitleScreen::Update(float dt)
{   
    if (menuIndex == 1)
    {
        if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
        {
            app->audio->UnloadFx(menuFx);
            app->fadeToBlack->FadeToBlackTransition((Module*)app->titlescreen, (Module*)app->scene, 0.0f);

            app->titlescreen->Disable();
            app->titlescreen->active = false;

            app->scene->Enable();
            app->scene->active = true;

            app->guiManager->active = false;
            app->guiManager->Disable();

            app->audio->PlayFx(app->scene->cityFx);
        }
    }

    if (menuIndex == 2)
    {
        if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
        {
            app->audio->UnloadFx(menuFx);
            app->fadeToBlack->FadeToBlackTransition((Module*)app->titlescreen, (Module*)app->scene, 0.0f);

            app->titlescreen->Disable();
            app->titlescreen->active = false;

            app->scene->Enable();
            app->scene->active = true;

            app->guiManager->active = false;
            app->guiManager->Disable();

            app->audio->PlayFx(app->scene->cityFx);
        }
    }

    if (menuIndex == 3)
    {
        if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
        {
            app->audio->UnloadFx(menuFx);
            app->fadeToBlack->FadeToBlackTransition((Module*)app->titlescreen, (Module*)app->scene, 0.0f);

            app->titlescreen->Disable();
            app->titlescreen->active = false;

            app->scene->Enable();
            app->scene->active = true;

            app->guiManager->active = false;
            app->guiManager->Disable();

            app->audio->PlayFx(app->scene->cityFx);
        }
    }   
    
    if (menuIndex == 4)
    {
        if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
        {
            SDL_Quit();
        }
    }

    if (app->input->GetAxis(ControlID::MOVE_VERTICAL) <= -0.4f) //arriba
    {
        if (timer.ReadMSec() >= 200)
        {
            if (menuIndex > 1) menuIndex--;
            else menuIndex = 4;
            timer.Start();
        }
    }
    if (app->input->GetAxis(ControlID::MOVE_VERTICAL) >= 0.4f) //abajo
    {
        if (timer.ReadMSec() >= 200)
        {
            if (menuIndex < 4) menuIndex++;
            else menuIndex = 1;
            timer.Start();
        }
    }

    //if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
    //{
    //    if (menuIndex == 1) App->network->optionsIndex = 1;
    //    //else if (menuIndex == 2) App->network->optionsIndex = 2;
    //    //else if (menuIndex == 3) App->network->optionsIndex = 3;
    //    else if (menuIndex == 4) return UPDATE_STOP;
    //}

    ListItem<GuiControl*>* controlListItem = nullptr;
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
  
    if (menuIndex == 1) app->render->DrawTexture(menu1, 0, 0, NULL);
    else if (menuIndex == 2) app->render->DrawTexture(menu2, 0, 0, NULL);
    else if (menuIndex == 3) app->render->DrawTexture(menu3, 0, 0, NULL);
    else if (menuIndex == 4) app->render->DrawTexture(menu4, 0, 0, NULL);

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

    //TODO: cambiar cuando se cambie la funcionalidad de los botones
    ListItem<GuiControl*>* controlListItem = nullptr;
    for (controlListItem = titleButtons.start; controlListItem != NULL; controlListItem = controlListItem->next)
    {
        delete controlListItem->data;
    }
    titleButtons.Clear();

    return true;
}

void TitleScreen::CreateTitleButtons()
{
    if (this->active)
    {
        int wBt = 190;
        int hBt = 40;
        int posBtX = screenWidth / 2 - 220;
        int posBtY = screenHeight / 2 - 10;
        titleButtons.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Start", { posBtX, posBtY, wBt, hBt }, this));
        wBt = 190;
        hBt = 40;
        posBtX = screenWidth / 2 - 220;
        posBtY = screenHeight / 2 + 100;
        titleButtons.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "Continue", { posBtX, posBtY, wBt, hBt }, this));
        //resume buton
        wBt = 190;
        hBt = 40;
        posBtX = screenWidth / 2 - 220;
        posBtY = screenHeight / 2 + 210;
        titleButtons.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "Optiones", { posBtX, posBtY, wBt, hBt }, this));
        //fullscreen buton
        wBt = 30;
        hBt = 30;
        posBtX = screenWidth / 2 + 450;
        posBtY = screenHeight / 2 - 330;
        titleButtons.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, "Exit", { posBtX, posBtY, wBt, hBt }, this));
    }
}