#pragma once
#include "Module.h"
#include "SDL/include/SDL.h"
#include "Window.h"
#include "GuiControlButton.h"
#include "GuiManager.h"
#include "GuiControl.h"
#include "Timer.h"

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

    SDL_Texture* menu1;
    SDL_Texture* menu2;
    SDL_Texture* menu3;
    SDL_Texture* menu4;
    int titleSound;
    int menuIndex = 1;
    Timer timer;

    int screenWidth;
    int screenHeight;

    int menuFx;
           
    //gui list
    List<GuiControl*> titleButtons;

private:


};