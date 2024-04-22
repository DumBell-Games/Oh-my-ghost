#pragma once
#include "Module.h"
#include "SDL/include/SDL.h"
#include "Window.h"
#include "GuiControlButton.h"
#include "GuiManager.h"
#include "GuiControl.h"
#include "Timer.h"

class PauseMenu : public Module
{
public:

    PauseMenu(bool startEnabled = true);
    virtual ~PauseMenu();

    bool Start();

    bool Update(float dt);

    bool PostUpdate();

    bool CleanUp();

    void CreatePauseButtons();

public:

    SDL_Texture* continuar;
    SDL_Texture* ajustes;
    SDL_Texture* salir;
    int titleSound;
    int menuIndex = 1;
    
    int screenWidth;
    int screenHeight;
    bool buttoncreated = false;

    //gui list
    List<GuiControl*> PauseButtons;
};