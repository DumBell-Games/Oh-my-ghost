#include "App.h"
#include "Window.h"
#include "Render.h"

#include "Defs.h"
#include "Log.h"
#include <thread>
#include "Textures.h"

#define VSYNC true

Render::Render(bool startEnabled) : Module(startEnabled)
{
	name.Create("renderer");
	background.r = 0;
	background.g = 0;
	background.b = 0;
	background.a = 0;
}

// Destructor
Render::~Render()
{}

// Called before render is available
bool Render::Awake(pugi::xml_node config)
{
	LOG("Create SDL rendering context");
	bool ret = true;

	Uint32 flags = SDL_RENDERER_ACCELERATED;

	// L04: DONE 6: Load the VSYNC status from config.xml and adapt the code to set it on / off
	if (config.child("vsync").attribute("value").as_bool()) {
		flags |= SDL_RENDERER_PRESENTVSYNC;
		LOG("Using vsync");
	}
	else {
		LOG("vsync OFF");
	}
	 
	renderer = SDL_CreateRenderer(app->win->window, -1, flags);

	if(renderer == NULL)
	{
		LOG("Could not create the renderer! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		camera.w = app->win->screenSurface->w;
		camera.h = app->win->screenSurface->h;
		camera.x = 0;
		camera.y = 0;
	}

	//initialise the SDL_ttf library
	TTF_Init();

	//load a font into memory
	primary_font = TTF_OpenFont(config.child("fonts").child("primary_font").attribute("fontPath").as_string(), config.child("fonts").child("primary_font").attribute("fontSize").as_int());
	secondary_font = TTF_OpenFont(config.child("fonts").child("secondary_font").attribute("fontPath").as_string(), config.child("fonts").child("secondary_font").attribute("fontSize").as_int());

	return ret;
}

// Called before the first frame
bool Render::Start()
{
	LOG("render start");
	// back background
	SDL_RenderGetViewport(renderer, &viewport);
	return true;
}

// Called each loop iteration
bool Render::PreUpdate()
{
	SDL_RenderClear(renderer);
	return true;
}

bool Render::Update(float dt)
{
	return true;
}

bool Render::PostUpdate()
{
	SDL_SetRenderDrawColor(renderer, background.r, background.g, background.b, background.a);
	SDL_RenderPresent(renderer);
	return true;
}

// Called before quitting
bool Render::CleanUp()
{
	LOG("Destroying SDL render");
	SDL_DestroyRenderer(renderer);
	return true;
}

void Render::SetBackgroundColor(SDL_Color color)
{
	background = color;
}

void Render::SetViewPort(const SDL_Rect& rect)
{
	SDL_RenderSetViewport(renderer, &rect);
}

void Render::ResetViewPort()
{
	SDL_RenderSetViewport(renderer, &viewport);
}

// Blit to screen
bool Render::DrawTexture(SDL_Texture* texture, int x, int y, const SDL_Rect* section, float speed, double angle, int pivotX, int pivotY, bool useCamera) const
{
	bool ret = true;
	uint scale = app->win->GetScale();

	SDL_Rect rect{0,0,0,0};
	if (useCamera)
	{
		rect.x = (int)(camera.x * speed);
		rect.y = (int)(camera.y * speed);
	}
	rect.x += x * scale;
	rect.y += y * scale;

	if(section != NULL)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	rect.w *= scale;
	rect.h *= scale;

	SDL_Point* p = NULL;
	SDL_Point pivot;

	if(pivotX != INT_MAX && pivotY != INT_MAX)
	{
		pivot.x = pivotX;
		pivot.y = pivotY;
		p = &pivot;
	}

	if(SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, SDL_FLIP_NONE) != 0)
	{
 		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

// This version of DrawTexture takes the x and y coordinates of the defined center of the texture, or top-left corner if local origin coordinates not set
bool Render::DrawTextureScaled(SDL_Texture* texture, int x, int y, const SDL_Rect* section, int targetScale, float speed, double angle, int originX, int originY, bool useCamera) const
{
	bool ret = true;
	uint scale = app->win->GetScale();

	originX *= targetScale;
	originY *= targetScale;

	SDL_Rect rect{ 0,0,0,0 };
	if (useCamera)
	{
		rect.x = (int)(camera.x * speed);
		rect.y = (int)(camera.y * speed);
	}
	rect.x += (x-originX) * scale;
	rect.y += (y-originY) * scale;

	if (section != NULL)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	rect.w *= scale;
	rect.h *= scale;

	rect.w *= targetScale;
	rect.h *= targetScale;

	SDL_Point* p = NULL;
	SDL_Point pivot;

	if (originX != INT_MAX && originY != INT_MAX)
	{
		pivot.x = originX;
		pivot.y = originY;
		p = &pivot;


	}

	if (SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, SDL_FLIP_NONE) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawRectangle(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, bool use_camera) const
{
	bool ret = true;
	uint scale = app->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect rec(rect);
	if(use_camera)
	{
		rec.x = (int)(camera.x + rect.x * scale);
		rec.y = (int)(camera.y + rect.y * scale);
		rec.w *= scale;
		rec.h *= scale;
	}

	int result = (filled) ? SDL_RenderFillRect(renderer, &rec) : SDL_RenderDrawRect(renderer, &rec);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = app->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;

	if(use_camera)
		result = SDL_RenderDrawLine(renderer, camera.x + x1 * scale, camera.y + y1 * scale, camera.x + x2 * scale, camera.y + y2 * scale);
	else
		result = SDL_RenderDrawLine(renderer, x1 * scale, y1 * scale, x2 * scale, y2 * scale);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawCircle(int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, bool use_camera)
{
	return filled ?
		DrawCircleFilled(x, y, radius, r, g, b, a, use_camera)
		: DrawCircleEmpty(x, y, radius, r, g, b, a, use_camera);
}

// Uses the Midpoint Circle Algorithm to draw full circles. position is the circle's center
bool Render::DrawCircleFilled(int posX, int posY, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool useCamera) const
{
	bool ret = true;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	//Original code taken from https://stackoverflow.com/a/74745126
	
	// 35/49 is a biased approximation of 1/sqrt(2)
	int arrSize = radius * 8 * 35 / 49;
	arrSize = (arrSize + (8 - 1)) & -8;
	
	std::vector<SDL_Point> points;
	points.reserve(arrSize);

	const int32_t diameter = (radius * 2);

	int32_t x = (radius - 1);
	int32_t y = 0;
	int32_t tx = 1;
	int32_t ty = 1;
	int32_t error = (tx - diameter);

	while (x >= y)
	{
		// Each of the following renders an octant of the circle
		points.push_back({ posX + x, posY - y });
		points.push_back({ posX + x, posY + y });
		points.push_back({ posX - x, posY - y });
		points.push_back({ posX - x, posY + y });
		points.push_back({ posX + y, posY - x });
		points.push_back({ posX + y, posY + x });
		points.push_back({ posX - y, posY - x });
		points.push_back({ posX - y, posY + x });

		if (error <= 0)
		{
			++y;
			error += ty;
			ty += 2;
		}

		if (error > 0)
		{
			--x;
			tx += 2;
			error += (tx - diameter);
		}
	}

	if (!SDL_RenderDrawLines(renderer, points.data(), points.size()))
	{
		LOG("Cannot draw circle to screen. SDL_RenderDrawLines error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawCircleEmpty(int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = app->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;
	SDL_Point points[360];

	float factor = (float)M_PI / 180.0f;

	for(uint i = 0; i < 360; ++i)
	{
		points[i].x = (int)(x * scale + camera.x) + (radius * cos(i * factor));
		points[i].y = (int)(y * scale + camera.y) + (radius * sin(i * factor));
	}

	result = SDL_RenderDrawPoints(renderer, points, 360);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawText(const char* text, int posx, int posy, int w, int h, SDL_Color color, TTF_Font* font) {

	SDL_Surface* surface = TTF_RenderText_Solid(font == nullptr ? primary_font : font, text, color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

	int texW = 0;
	int texH = 0;
	SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
	SDL_Rect dstrect = { posx, posy, w == 0 ? texW : w, h == 0 ? texH : h };

	SDL_RenderCopy(renderer, texture, NULL, &dstrect);

	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);

	return true;
}

// L14: TODO 6: Implement a method to load the state
// for now load camera's x and y
bool Render::LoadState(pugi::xml_node node) {

	camera.x = node.child("camera").attribute("x").as_int();
	camera.y = node.child("camera").attribute("y").as_int();

	return true;
}
// L14: TODO 8: Create a method to save the state of the renderer
// using append_child and append_attribute
bool Render::SaveState(pugi::xml_node node) {
	
	pugi::xml_node camNode = node.append_child("camera");
	camNode.append_attribute("x").set_value(camera.x);
	camNode.append_attribute("y").set_value(camera.y);

	return true;
}

void Render::aplicarFiltreVermell(SDL_Texture* texturaOriginal, SDL_Rect* pos)
{
	// Per quan rep dany

	// Aplicar el filtre de color vermell
	SDL_SetTextureColorMod(texturaOriginal, 255, 0, 0);

	// Renderitzar la imatge amb el filtre de color vermell
	SDL_RenderCopy(renderer, texturaOriginal, pos, nullptr);
	SDL_RenderPresent(renderer);

	// Esperar 1 segon
	std::this_thread::sleep_for(std::chrono::seconds(1));

	// Renderitzar la imatge original de nou per eliminar el filtre de color vermell
	SDL_SetTextureColorMod(texturaOriginal, 255, 255, 255);
	SDL_RenderCopy(renderer, texturaOriginal, pos, nullptr);
	SDL_RenderPresent(renderer);
}

