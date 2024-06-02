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
	{
		int rad = bounds.w / 2;
		pb = app->physics->CreateCircle(bounds.x + rad, bounds.y + rad, bounds.w / 2, STATIC);
		break;
	}
	case GuiControlType::PHYSBUTTON_BOX:
	default:
	{
		pb = app->physics->CreateRectangle(bounds.x, bounds.y, bounds.w, bounds.y, STATIC);
		break;
	}
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
	// TODO move physbody to camera-relative position
	// bounds variable is relative to camera
	pbody->body->SetTransform({ (float)(-app->render->camera.x + bounds.x), (float)(-app->render->camera.y + bounds.y) }, pbody->GetRotation());

	if (state != GuiControlState::DISABLED)
	{

		// Check for click if clickable
		if (state != GuiControlState::NON_CLICKABLE)
		{
			app->input->GetMousePosition(mouseX, mouseY);
			bool notified = false;
			for (b2Fixture* f = pbody->body->GetFixtureList(); f && !notified; f = f->GetNext())
			{
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
						if (k == KEY_UP) {
							NotifyMouseClick();
							notified = true;
						}
					}
				}
				else
				{
					state = GuiControlState::NORMAL;
				}
			}
		}

		// Render

		if (bgTexture.get())
		{
			SDL_Texture* toDraw = (state != GuiControlState::PRESSED) ? bgTextureClicked.get() : bgTexture.get();
			app->render->DrawTexture(toDraw, bounds.x, bounds.y, nullptr, 1.0F, pbody->GetRotation(), bounds.w / 2, bounds.h / 2, false);
		}
		else switch (state)
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

		if (fgTexture.get())
		{
			app->render->DrawTexture(fgTexture.get(), bounds.x, bounds.y, nullptr, 1.0F, pbody->GetRotation(), bounds.w/2, bounds.h/2, false);
		}
		else
			app->render->DrawText(text.GetString(), bounds.x, bounds.y, bounds.w, bounds.h);
	}
	return true;
}

inline void GuiControlPhysButton::SetRotation(float degrees)
{
	pbody->body->SetTransform(pbody->body->GetTransform().p, degrees);
}
