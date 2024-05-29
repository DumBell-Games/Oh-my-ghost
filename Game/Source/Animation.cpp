#include "Animation.h"

#include "App.h"
#include "Render.h"

inline void Animation::Render(SDL_Texture* texture, const iPoint& position)
{
	iPoint p = pivots[currentFrameInt];
	app->render->DrawTexture(texture, position.x, position.y, &GetCurrentFrame(), 1.0F, 0.0, p.x, p.y);
}
