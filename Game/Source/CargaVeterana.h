#pragma once
#include "Module.h"
#include "SDL/include/SDL.h"
#include "Window.h"
#include "GuiControlButton.h"
#include "GuiManager.h"
#include "GuiControl.h"
#include "Timer.h"
#include "Pathfinding.h"
#include "Animation.h"
#include "Path.h"

struct SDL_Texture;


class CargaVeterana : public Module
{
public:

    CargaVeterana(bool startEnabled = false);
    virtual ~CargaVeterana();

    bool Start();

    bool Update(float dt);

    bool PostUpdate();

    bool CleanUp();

   
    
public:

    SDL_Texture* logoScreenTex;
    int logoFX;
    int menuIndex = 1;
    Timer timer;

    int screenWidth;
    int screenHeight;
           
    int introScreenFx;
    //gui list
    List<GuiControl*> titleButtons;
    Animation dumbellAnim;

    Path IAnimationPath;

private:


};