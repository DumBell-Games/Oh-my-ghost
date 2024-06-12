#include "GuiCombatHP.h"

#include "App.h"
#include "Render.h"

#include "Defs.h"

#include "SDL/include/SDL.h"

GuiCombatHP::GuiCombatHP(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::PROGRESS_BAR,bounds,text)
{
    hpBarBounds = bounds;
    TextBounds = {54,24,128,24};
}

bool GuiCombatHP::Init(const char* frontPath, const char* backPath, const char* maskPath)
{
    frontTexture = app->tex->LoadSP(frontPath, true);
    backTexture = app->tex->LoadSP(backPath, true);
    maskTexture = app->tex->LoadSP(maskPath, true);

    // Asigna el blendmode a maskTexture
    SDL_SetTextureBlendMode(maskTexture.get(), SDL_BlendMode::SDL_BLENDMODE_MOD);

    // Crea una textura independiente. No hace falta añadirla a la lista de duplicados ya que va a ser generada por codigo
    Uint32 format;
    SDL_QueryTexture(backTexture.get(), &format, nullptr, &hpBarBounds.w, &hpBarBounds.h);
    SDL_Texture* mask = SDL_CreateTexture(app->render->renderer, format, SDL_TextureAccess::SDL_TEXTUREACCESS_TARGET, hpBarBounds.w, hpBarBounds.h);
    //SDL_SetTextureBlendMode(mask, SDL_BLENDMODE_MOD);
    maskedTexture = shared_texture_t(app->tex->Add(mask), [](SDL_Texture* t) {app->tex->UnLoad(t); });

    return true;
}

GuiCombatHP::~GuiCombatHP()
{
    app->tex->UnLoadSP(frontTexture);
    app->tex->UnLoadSP(backTexture);
    app->tex->UnLoadSP(maskTexture);
}

bool GuiCombatHP::Render()
{
    SDL_Rect currentHpBounds = hpBarBounds;
    if (trackedValue != nullptr)
        currentHpBounds.w *= CLAMP((float)(*trackedValue-minValue)/maxValue, 0, 1);
    if (frontTexture.get() == nullptr)
    {
        // Fondo -> cantidad actual -> borde
        app->render->DrawRectangle(hpBarBounds, 0U, 0U, 0U, 255U, true, false);
        app->render->DrawRectangle(currentHpBounds, 0U, 255U, 0U, 255U, true, false);
        app->render->DrawRectangle(hpBarBounds, 0U, 0U, 0U, 255U, false, false);
    }
    else
    {
        SDL_Rect localCurrentBounds{ 0,0,currentHpBounds.w,currentHpBounds.h };
        SDL_Rect localBounds{ 0,0,hpBarBounds.w,hpBarBounds.h };

        SDL_SetRenderTarget(app->render->renderer,maskedTexture.get()); // Draw to masked texture
        SDL_SetRenderDrawColor(app->render->renderer, 0,0,0,0);
        SDL_RenderClear(app->render->renderer);

        app->render->DrawRectangle(localCurrentBounds, 0U, 255U, 0U, 255U, true, false, SDL_BLENDMODE_BLEND);
        app->render->DrawTexture(maskTexture.get(), 0, 0, nullptr, 1.0f, 0, INT_MAX, INT_MAX, false, flip);

        SDL_SetRenderTarget(app->render->renderer, nullptr); // Return to drawing to window

        app->render->DrawTexture(backTexture.get(), hpBarBounds.x, hpBarBounds.y, nullptr, 1.0, 0, INT_MAX, INT_MAX, false);
        app->render->DrawTexture(maskedTexture.get(), hpBarBounds.x, hpBarBounds.y, nullptr, 1.0, 0, INT_MAX, INT_MAX, false);
        app->render->DrawTexture(frontTexture.get(), hpBarBounds.x, hpBarBounds.y, nullptr, 1.0, 0, INT_MAX, INT_MAX, false, flip);

        SDL_Rect localTextBounds = hpBarBounds;
        if (flip != SDL_FLIP_NONE)
        {
            localTextBounds.x += hpBarBounds.w - TextBounds.x - TextBounds.w;
            localTextBounds.y += TextBounds.y;
            localTextBounds.w = TextBounds.w;
            localTextBounds.h = TextBounds.h;
        }
        else
        {
            localTextBounds.x +=TextBounds.x;
            localTextBounds.y +=TextBounds.y;
            localTextBounds.w =TextBounds.w;
            localTextBounds.h =TextBounds.h;
        }
        app->render->DrawText(text.GetString(), localTextBounds.x, localTextBounds.y, localTextBounds.w, localTextBounds.h, {0,0,0,255});
        //app->render->DrawTexture(maskTexture.get(), 0, 0, nullptr, 1.0f, 0, INT_MAX, INT_MAX, false);

    }

    return true;
}
