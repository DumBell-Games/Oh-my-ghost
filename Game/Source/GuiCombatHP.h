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
	int maxValue = 100;
	int minValue = 0;

	SDL_Rect hpBarBounds;

};

