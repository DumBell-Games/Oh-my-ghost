#pragma once
#include "Module.h"
#include "SDL/include/SDL.h"
#include "Window.h"
#include "GuiControlButton.h"
#include "GuiManager.h"
#include "GuiControl.h"
#include "Timer.h"
#include "Textures.h"

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

    void continuar(GuiControl* ctrl);
    void opciones(GuiControl* ctrl);
    void salir(GuiControl* ctrl);
    void volver(GuiControl* ctrl);
    void fullscreen1(GuiControl* ctrl);
    void vyscn(GuiControl* ctrl);
    void volumen1(GuiControl* ctrl);
    void volumen2(GuiControl* ctrl);
    void fx1(GuiControl* ctrl);
    void fx2(GuiControl* ctrl);

public:

    /*SDL_Texture* continuar;
    SDL_Texture* ajustes;
    SDL_Texture* salir;*/
    int titleSound;
    
    int screenWidth;
    int screenHeight;
    bool buttoncreated = false;

    bool ajustcreated = false;
    bool inpause = false;
    bool inajustes = false;
    bool fullscreen = true;
    bool vsycn = true;

    int buttonFx;
    int PauseIndex = 1;
    int AjustesIndex = 1;

    shared_texture_t pause1;
    shared_texture_t pause2;
    shared_texture_t pause3;

    int weigth;
    int heigth;

    Timer timer;

    //gui list
    List<GuiControlButton*> PauseButtons;
    List<GuiControlButton*> AjustesButtons;

    pugi::xml_node parameters;
};

