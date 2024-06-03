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
	character_Menu = app->tex->LoadSP("Assets/Textures/Character menu.png", true);

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
	app->render->DrawTexture(character_Menu.get(), 0, 0, 0, NULL);

	nivel.Create("%d", nive);
	ataque.Create("%d", ataqu);
	defensa.Create("%d", defens);
	velocidad.Create("%d", velocida);
	vida_actual.Create("%d/", vidaA);
	vida_maxima.Create("%d", vidaM);
	mana_maxima.Create("%d", manaM);
	mana_actual.Create("%d/", manaA);
	experiencia_siguiente.Create("%d", experienciaS);
	experiencia_total.Create("%d", experienciaT);
	
	return true;
}
bool Character_Menu::PostUpdate() {

	

	app->render->DrawText(nivel.GetString(), 1200, 90, 32, 64);
	app->render->DrawText(ataque.GetString(), 350, 120, 64, 64);
	app->render->DrawText(defensa.GetString(), 350, 280, 64, 64);
	app->render->DrawText(velocidad.GetString(), 350, 400, 64, 64);
	app->render->DrawText(vida_actual.GetString(), 975, 240, 128, 64);
	app->render->DrawText(vida_maxima.GetString(), 1100, 240, 96, 64);
	app->render->DrawText(mana_actual.GetString(), 1000, 410, 96, 64);
	app->render->DrawText(mana_maxima.GetString(), 1100, 410, 96, 64);
	app->render->DrawText(experiencia_siguiente.GetString(), 650, 770, 150, 64);
	app->render->DrawText(experiencia_total.GetString(), 650, 660, 150, 64);
	
	return true;
}
bool Character_Menu::CleanUp() {

	app->tex->UnLoadSP(character_Menu);
	character_Menu = nullptr;
	
	return true;
}