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

    bool Awake();

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

    bool ajustcreated = false;
    bool inpause = false;
    bool inajustes = false;
    bool fullscreen = true;
    bool vsycn = true;

    int buttonFx;



    Timer timer;

    //gui list
    List<GuiControl*> PauseButtons;
    List<GuiControl*> AjustesButtons;

    pugi::xml_node parameters;
};