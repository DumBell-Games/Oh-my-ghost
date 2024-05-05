#pragma once
#include "Module.h"
#include "SDL/include/SDL.h"
#include "Window.h"
#include "GuiControlButton.h"
#include "GuiManager.h"
#include "GuiControl.h"
#include "Timer.h"

class InventoryScreen : public Module
{
public:

    InventoryScreen(bool startEnabled = true);
    virtual ~InventoryScreen();

    bool Start();

    bool Update(float dt);

    bool PostUpdate();

    bool CleanUp();

    void CreateInventoryButtons();

public:

    SDL_Texture* continuar;
    SDL_Texture* ajustes;
    SDL_Texture* salir;
    int titleSound;
    int menuIndex = 1;
    
    int screenWidth;
    int screenHeight;
    bool buttoncreated = false;
    int buttonFx;


    Timer timer;

    //gui list
    List<GuiControl*> inventoryButtons;
};