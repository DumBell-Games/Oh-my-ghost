#include "Animation.h"

#include "App.h"
#include "Render.h"

void Animation::Render(SDL_Texture* texture, const iPoint& position, bool useCamera)
{
	iPoint p = pivots[currentFrameInt];
	app->render->DrawTexture(texture, position.x-p.x, position.y-p.y, &GetCurrentFrame(), 1.0F, 0.0, p.x, p.y, useCamera);
}
