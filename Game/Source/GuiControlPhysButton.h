#pragma once
#include "GuiControl.h"

#include "Point.h"
#include "SString.h"
#include "Physics.h"

#include <memory>


static void DestroyPtr(PhysBody* p);

using Unique_PhysBody = std::unique_ptr<PhysBody, void(*)(PhysBody*)>;

// This class should NOT be instantiated when the physics module is disabled
class GuiControlPhysButton : public GuiControl
{
public:
	GuiControlPhysButton(uint32 id, GuiControlType type, SDL_Rect bounds, const char* text);

	virtual ~GuiControlPhysButton();

	// Called each loop iteration
	bool Update(float dt);


private:

	Unique_PhysBody pbody;

	int mouseX, mouseY;
};

