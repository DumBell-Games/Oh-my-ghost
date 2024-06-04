#pragma once
#include "Module.h"
#include "SDL/include/SDL.h"
#include "Window.h"
#include "GuiControlButton.h"
#include "GuiManager.h"
#include "GuiControl.h"
#include "Timer.h"
#include "SString.h"
#include "Textures.h"

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

    shared_texture_t inventory1;
    shared_texture_t inventory2;
    shared_texture_t inventory3;
    shared_texture_t inventory4;
    shared_texture_t inventory5;
    shared_texture_t inventory6;
    shared_texture_t inventory7;
    shared_texture_t inventory8;
    shared_texture_t inventory9;
    shared_texture_t inventory10;
    shared_texture_t inventory11;
    shared_texture_t inventory12;
    shared_texture_t inventory13;
    shared_texture_t inventory14;

    SString valorCoca;
	SString valorBirra;
	SString valorCaramelos;
	SString valorPatatas;
    SString valorVelocidad;
    SString valorYogur;
    SString dineroActual;

    Timer timer;

    //gui list
    List<GuiControlButton*> inventoryButtons;
};