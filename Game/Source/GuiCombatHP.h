#pragma once
#include "GuiControl.h"
class GuiCombatHP :
    public GuiControl
{
public:

	GuiCombatHP(uint32 id, SDL_Rect bounds, const char* text);

	virtual ~GuiCombatHP();

	// Called each loop iteration
	bool Update(float dt) override;

public:

	int* trackedValue;

	SDL_Rect hpBarBounds;

};

