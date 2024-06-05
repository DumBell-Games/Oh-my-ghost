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

class PersonajesScreen : public Module
{
public:

    PersonajesScreen(bool startEnabled = true);
    virtual ~PersonajesScreen();

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

    shared_texture_t personaje1;
    shared_texture_t personaje2;
    shared_texture_t personaje3;
    shared_texture_t personaje4;
    shared_texture_t personaje5;
    shared_texture_t personaje6;
    shared_texture_t personaje7;
    shared_texture_t personaje8;
    shared_texture_t personaje9;
   

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