#include "Animation.h"

#include "App.h"
#include "Render.h"

void Animation::Render(SDL_Texture* texture, const iPoint& position, bool useCamera, int scale)
{
	iPoint p = pivots[currentFrameInt];
	app->render->DrawTextureScaled(texture, position.x, position.y, &GetCurrentFrame(), scale, 1.0F, 0.0, p.x, p.y, useCamera);
}
