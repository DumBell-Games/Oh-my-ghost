#include "GuiCombatHP.h"

#include "App.h"
#include "Render.h"

#include "Defs.h"

GuiCombatHP::GuiCombatHP(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::PROGRESS_BAR,bounds,text)
{

    hpBarBounds = bounds;
}

GuiCombatHP::~GuiCombatHP()
{
}

bool GuiCombatHP::Update(float dt)
{
    SDL_Rect currentHpBounds = hpBarBounds;
    if (trackedValue != nullptr)
        currentHpBounds.w *= CLAMP((float)(*trackedValue-minValue)/maxValue, 0, 1);

    // Fondo -> cantidad actual -> borde
    app->render->DrawRectangle(hpBarBounds, 0U, 0U, 0U, 255U, true, false);
    app->render->DrawRectangle(currentHpBounds, 0U, 255U, 0U, 255U, true, false);
    app->render->DrawRectangle(hpBarBounds, 0U, 0U, 0U, 255U, false, false);

    return true;
}
