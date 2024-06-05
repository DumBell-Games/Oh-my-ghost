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

class Tienda : public Module
{
public:

    Tienda(bool startEnabled = true);
    virtual ~Tienda();

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

    shared_texture_t tienda1;
    shared_texture_t tienda2;
    shared_texture_t tienda3;
    shared_texture_t tienda4;
    shared_texture_t tienda5;
    shared_texture_t tienda6;
    shared_texture_t tienda7;
    shared_texture_t tienda8;
    shared_texture_t tienda9;
    shared_texture_t tienda10;
    shared_texture_t tienda11;
    shared_texture_t tienda12;
    shared_texture_t tienda13;

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