#include "Character_Menu.h"
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "GuiManager.h"
#include "GuiControl.h"
#include "GuiControlButton.h"
#include "SDL/include/SDL.h"
#include "FadeToBlack.h"
#include "Input.h"
#include "Scene.h"
#include "menu.h"

Character_Menu::Character_Menu(bool startEnabled) : Module(startEnabled)
{
}

Character_Menu::~Character_Menu()
{
}

bool Character_Menu::Start() {
	character_Menu = app->tex->Load("Assets/Textures/Character menu.png");

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	SDL_GetWindowSize(app->win->window, &screenWidth, &screenHeight);
	
	return true;
}
bool Character_Menu::PreUpdate() {
	if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
	{
		app->fadeToBlack->FadeToBlackTransition(this, (Module*)app->scene, 0);
	}
	return true;
}
bool Character_Menu::Update(float dt) {
	
	return true;
}
bool Character_Menu::PostUpdate() {

	app->render->DrawTexture(character_Menu, 0, 0, 0, NULL);
	
	return true;
}
bool Character_Menu::CleanUp() {

	app->tex->UnLoad(character_Menu);
	character_Menu = nullptr;
	
	return true;
}