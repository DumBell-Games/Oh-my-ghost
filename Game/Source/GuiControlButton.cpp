#include "GuiControlButton.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"

GuiControlButton::GuiControlButton(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;

	canClick = true;
	drawBasic = false;
}

GuiControlButton::~GuiControlButton()
{

}

bool GuiControlButton::Update(float dt)
{
	if (state != GuiControlState::DISABLED)
	{

		//If the position of the mouse if inside the bounds of the button 
		if (state != GuiControlState::NON_CLICKABLE) {
			if (mouseX > bounds.x && mouseX < bounds.x + bounds.w && mouseY > bounds.y && mouseY < bounds.y + bounds.h) {
			
				state = GuiControlState::FOCUSED;

				if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
					state = GuiControlState::PRESSED;
				}
				
				if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
					NotifyMouseClick();
				}
			}
			else {
				state = GuiControlState::NORMAL;
			}
		}


	}

	return true;
}

bool GuiControlButton::Render()
{
	switch (state)
	{
	case GuiControlState::NON_CLICKABLE:
		app->render->DrawRectangle(bounds, 200, 200, 200, 255, true, false);
		break;
	case GuiControlState::NORMAL:
		app->render->DrawRectangle(bounds, 0, 0, 255, 255, true, false);
		break;
	case GuiControlState::FOCUSED:
		app->render->DrawRectangle(bounds, 0, 0, 20, 255, true, false);
		break;
	case GuiControlState::PRESSED:
		app->render->DrawRectangle(bounds, 0, 255, 0, 255, true, false);
		break;
	default:
		break;
	}

	app->render->DrawText(text.GetString(), bounds.x, bounds.y, bounds.w, bounds.h);

	return true;
}

