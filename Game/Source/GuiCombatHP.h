#pragma once
#include "GuiControl.h"
#include "Textures.h"
class GuiCombatHP :
    public GuiControl
{
public:

	GuiCombatHP(uint32 id, SDL_Rect bounds, const char* text);

	bool Init(const char* frontPath, const char* backPath, const char* maskPath);

	virtual ~GuiCombatHP();

	// Called each loop iteration
	bool Render() override;

public:

	int* trackedValue;
	int maxValue = 100;
	int minValue = 0;

	SDL_Rect hpBarBounds;
	SDL_Rect TextBounds;

	shared_texture_t frontTexture = nullptr;
	shared_texture_t backTexture = nullptr;
	shared_texture_t maskTexture = nullptr;
	shared_texture_t maskedTexture = nullptr;

	SDL_RendererFlip flip = SDL_FLIP_NONE;
};

