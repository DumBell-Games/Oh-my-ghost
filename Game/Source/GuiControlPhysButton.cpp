#include "GuiControlPhysButton.h"
#include "App.h"

void DestroyPtr(PhysBody* p)
{
	app->physics->DestroyBody(p);
}

GuiControlPhysButton::GuiControlPhysButton(uint32 id, GuiControlType type, SDL_Rect bounds, const char* text) : GuiControl(type,bounds,text), pbody(nullptr,DestroyPtr)
{
	PhysBody* pb = nullptr;

	switch (type)
	{
	case GuiControlType::PHYSBUTTON_CIRCLE:
		pb = app->physics->CreateCircle(bounds.x, bounds.y, bounds.w / 2, STATIC);
		break;
	case GuiControlType::PHYSBUTTON_BOX:
	default:
		pbody = Unique_PhysBody(app->physics->CreateRectangle(bounds.x, bounds.y, bounds.w, bounds.y, STATIC),DestroyPtr);
		break;
	}
	pb->body->GetFixtureList()[0].SetSensor(true);
	pb->ctype = ColliderType::GUI;

	pbody = Unique_PhysBody(pb, DestroyPtr);

}

GuiControlPhysButton::~GuiControlPhysButton()
{
}

bool GuiControlPhysButton::Update(float dt)
{

	if (state != GuiControlState::DISABLED)
	{
		// Check for click
		app->input->GetMousePosition(mouseX, mouseY);

		if (state != GuiControlState::NON_CLICKABLE)
		{
			for (b2Fixture* f = pbody->body->GetFixtureList(); f; f = f->GetNext())
			{
				// mouseX and mouseY are updated in GuiManager
				if (f->TestPoint(b2Vec2(PIXEL_TO_METERS(mouseX), PIXEL_TO_METERS(mouseY))))
				{
					KeyState k = app->input->GetMouseButtonDown(SDL_BUTTON_LEFT);
					if (k == KEY_REPEAT)
					{
						state = GuiControlState::PRESSED;
					}
					else 
					{
						state = GuiControlState::FOCUSED;
						if (k == KEY_UP)
							NotifyMouseClick();
					}
				}
				else
				{
					state = GuiControlState::NORMAL;
				}
			}
		}

		// Render

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
		}

		app->render->DrawText(text.GetString(), bounds.x, bounds.y, bounds.w, bounds.h);
	}
	return true;
}
