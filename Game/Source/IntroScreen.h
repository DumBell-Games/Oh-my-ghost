#pragma once
#include "Module.h"
#include "SDL/include/SDL.h"
#include "Window.h"
#include "GuiControlButton.h"
#include "GuiManager.h"
#include "GuiControl.h"
#include "Timer.h"

struct SDL_Texture;


class IntroScreen : public Module
{
public:

    IntroScreen(bool startEnabled = true);
    virtual ~IntroScreen();

    bool Start();

    bool Update(float dt);

    bool PostUpdate();

    bool CleanUp();

    
public:

    SDL_Texture* introScreenTex;
    int introScreenFx;

    Timer timer;

    int screenWidth;
    int screenHeight;
           
    //gui list
    List<GuiControl*> titleButtons;

private:


};