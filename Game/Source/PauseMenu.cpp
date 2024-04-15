#include "PauseMenu.h"
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "GuiManager.h"
#include "GuiControl.h"
#include "GuiControlButton.h"
#include "SDL/include/SDL.h"

PauseMenu::PauseMenu(bool startEnabled) : Module()
{
}

PauseMenu::~PauseMenu()
{}

bool PauseMenu::Start() {

	return true;
}
bool PauseMenu::Update(float dt) {

	return true;
}
bool PauseMenu::PostUpdate() {

	return true;
}
bool PauseMenu::CleanUp(){
	return true;
}
void PauseMenu::CreatePauseButtons() {

}