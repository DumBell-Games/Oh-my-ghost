#pragma once
#include "Module.h"
#include "SDL/include/SDL.h"
#include "Window.h"
#include "GuiControlButton.h"
#include "GuiManager.h"
#include "GuiControl.h"
#include "Timer.h"
#include "SString.h"

class InventoryScreen : public Module
{
public:

    InventoryScreen(bool startEnabled = true);
    virtual ~InventoryScreen();

    bool Start();

    bool PreUpdate();

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

    SDL_Texture* inventory1;
    SDL_Texture* inventory2;
    SDL_Texture* inventory3;
    SDL_Texture* inventory4;
    SDL_Texture* inventory5;
    SDL_Texture* inventory6;
    SDL_Texture* inventory7;
    SDL_Texture* inventory8;
    SDL_Texture* inventory9;
    SDL_Texture* inventory10;
    SDL_Texture* inventory11;
    SDL_Texture* inventory12;
    SDL_Texture* inventory13;
    SDL_Texture* inventory14;

    SString valorCoca;
	SString valorBirra;
	SString valorCaramelos;
	SString valorPatatas;
    SString valorVelocidad;
    SString valorYogur;

    Timer timer;

    //gui list
    List<GuiControlButton*> inventoryButtons;
};