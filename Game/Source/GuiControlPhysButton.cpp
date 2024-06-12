#include "GuiControlPhysButton.h"
#include "App.h"

#include "Log.h"

void DestroyPtr(PhysBody* p)
{
	app->physics->DestroyBody(p);
}

GuiControlPhysButton::GuiControlPhysButton(uint32 id, GuiControlType type, SDL_Rect bounds, const char* text) : GuiControl(type,bounds,text), pbody(nullptr,DestroyPtr)
{
	PhysBody* pb = nullptr;

	int x = bounds.x; //-app->render->camera.x;
	int y = bounds.y; //-app->render->camera.y;

	switch (type)
	{
	case GuiControlType::PHYSBUTTON_CIRCLE:
	{
		int rad = bounds.w / 2;
		pb = app->physics->CreateCircle(x+rad, y+rad, rad, KINEMATIC);
		break;
	}
	case GuiControlType::PHYSBUTTON_BOX:
	default:
	{
		pb = app->physics->CreateRectangle(x, y, bounds.w, bounds.y, KINEMATIC);
		break;
	}
	}
	pb->body->GetFixtureList()[0].SetSensor(true);
	pb->ctype = ColliderType::GUI;

	pbody = Unique_PhysBody(pb, DestroyPtr);

}

GuiControlPhysButton::~GuiControlPhysButton()
{
	app->tex->UnLoadSP(bgTexture);
	app->tex->UnLoadSP(bgTextureClicked);
	app->tex->UnLoadSP(fgTexture);
}

bool GuiControlPhysButton::Init(const char* bgPath, const char* bgClickedPath, const char* fgPath, const char* text)
{
	bgTexture = app->tex->LoadSP(bgPath, true);
	bgTextureClicked = app->tex->LoadSP(bgClickedPath, true);
	fgTexture = app->tex->LoadSP(fgPath, true);
	this->text = text;

	return true;
}

bool GuiControlPhysButton::Update(float dt)
{
	// TODO move physbody to camera-relative position
	// bounds variable is relative to camera
	//float x = PIXEL_TO_METERS(bounds.x);// - app->render->camera.x;
	//float y = PIXEL_TO_METERS(bounds.y);// - app->render->camera.y;
	//pbody->body->SetTransform({ x, y }, pbody->GetRotation());

	if (state != GuiControlState::DISABLED)
	{

		// Check for click if clickable
		if (state != GuiControlState::NON_CLICKABLE)
		{
			app->input->GetMousePosition(mouseX, mouseY);
			float localMouseX, localMouseY;
			localMouseX = PIXEL_TO_METERS(mouseX);
			localMouseY = PIXEL_TO_METERS(mouseY);
			bool notified = false;
			for (b2Fixture* f = pbody->body->GetFixtureList(); f && !notified; f = f->GetNext())
			{
				if (f->TestPoint(b2Vec2(localMouseX, localMouseY)))
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

		
	}
	return true;
}

bool GuiControlPhysButton::Render()
{
	if (state == GuiControlState::DISABLED) return true;

	if (bgTexture.get() && bgTextureClicked.get())
	{
		SDL_Texture* toDraw = (state == GuiControlState::FOCUSED) ? bgTextureClicked.get() : bgTexture.get();
		app->render->DrawTexture(toDraw, bounds.x, bounds.y, nullptr, 1.0F, pbody->GetRotation(), bounds.w / 2, bounds.h / 2, false);
	}
	else if (type == GuiControlType::PHYSBUTTON_BOX)switch (state)
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
	else if (type == GuiControlType::PHYSBUTTON_CIRCLE)
	{
		int rad = bounds.w / 2;
		int x = bounds.x + rad;
		int y = bounds.y + rad;
		switch (state)
		{
		case GuiControlState::NON_CLICKABLE:
			app->render->DrawCircle(x, y, rad, 200, 200, 200, 255, true, false);
			break;
		case GuiControlState::NORMAL:
			app->render->DrawCircle(x, y, rad, 0, 0, 255, 255, true, false);
			break;
		case GuiControlState::FOCUSED:
			app->render->DrawCircle(x, y, rad, 0, 0, 20, 255, true, false);
			break;
		case GuiControlState::PRESSED:
			app->render->DrawCircle(x, y, rad, 0, 255, 0, 255, true, false);
			break;
		}
	}

	if (fgTexture.get())
	{
		app->render->DrawTexture(fgTexture.get(), bounds.x, bounds.y, nullptr, 1.0F, pbody->GetRotation(), bounds.w / 2, bounds.h / 2, false);
	}
	else
		app->render->DrawText(text.GetString(), bounds.x, bounds.y, bounds.w, bounds.h);

	return true;
}

inline void GuiControlPhysButton::SetRotation(float degrees)
{
	pbody->body->SetTransform(pbody->body->GetTransform().p, degrees);
}
