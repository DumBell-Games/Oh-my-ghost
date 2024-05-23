#pragma once
#include "Module.h"
#include "SDL/include/SDL.h"
#include "Window.h"
#include "GuiControlButton.h"
#include "GuiManager.h"
#include "GuiControl.h"
#include "Timer.h"
#include "Animation.h"
#include "Path.h"

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

    SDL_Texture* introScreenTex = nullptr;
    int introScreenFx = 0;

    Timer timer;

    int screenWidth = 0;
    int screenHeight = 0;
           
    //gui list
    List<GuiControl*> titleButtons;

    Path IAnimationPath;
    Animation omgAnim;

private:


};