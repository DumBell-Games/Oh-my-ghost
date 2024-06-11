#pragma once
#include "Module.h"
#include "SDL/include/SDL.h"
#include "Window.h"
#include "GuiControlButton.h"
#include "GuiManager.h"
#include "GuiControl.h"
#include "Timer.h"
#include "Textures.h"

struct SDL_Texture;


class TitleScreen : public Module
{
public:

    TitleScreen(bool startEnabled = true);
    virtual ~TitleScreen();

    bool Start();

    bool Update(float dt);

    bool PostUpdate();

    bool CleanUp();

    void CreateTitleButtons();
    
public:

    shared_texture_t menu1;
    shared_texture_t menu2;
    shared_texture_t menu3;
    shared_texture_t menu4;

    int titleSound;
    int menuIndex = 1;
    Timer timer;

    int screenWidth;
    int screenHeight;

    int menuFx;
    int buttonFx;
           
    //gui list
    List<GuiControlButton*> titleButtons;

private:


};