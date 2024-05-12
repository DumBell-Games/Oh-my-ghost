#pragma once
#include "Module.h"
#include "SDL/include/SDL.h"
#include "Window.h"
#include "GuiControlButton.h"
#include "GuiManager.h"
#include "GuiControl.h"
#include "Timer.h"

class Character_Menu : public Module
{
public:
	Character_Menu(bool start = true);
	virtual ~Character_Menu();

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

public:
	int screenWidth;
	int screenHeight;

	SDL_Texture* character_Menu;
};