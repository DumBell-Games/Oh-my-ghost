#ifndef __RENDER_H__
#define __RENDER_H__

#include "Module.h"

#include "Point.h"

#include "SDL/include/SDL.h"
#include "SDL_ttf/include/SDL_ttf.h"

class Render : public Module
{
public:

	Render(bool startEnabled = true);

	// Destructor
	virtual ~Render();

	// Called before render is available
	bool Awake(pugi::xml_node config);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void SetViewPort(const SDL_Rect& rect);
	void ResetViewPort();

	// Drawing
	bool DrawTexture(SDL_Texture* texture, int x, int y, const SDL_Rect* section = NULL, float speed = 1.0f, double angle = 0, int pivotX = INT_MAX, int pivotY = INT_MAX, bool useCamera = true) const;
	bool DrawTextureScaled(SDL_Texture* texture, int x, int y, const SDL_Rect* section = NULL, int targetScale = 1, float speed = 1.0f, double angle = 0, int pivotX = INT_MAX, int pivotY = INT_MAX, bool useCamera = true) const;
	bool DrawRectangle(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool filled = true, bool useCamera = true) const;
	bool DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool useCamera = true) const;
	bool DrawCircle(int x1, int y1, int redius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool useCamera = true) const;
	bool DrawText(const char* text, int posX, int posY, int w, int h, SDL_Color color = { 255, 255, 255, 255 }, TTF_Font* font = nullptr);

	// Set background color
	void SetBackgroundColor(SDL_Color color);

	//
	bool LoadState(pugi::xml_node node);

	//
	bool SaveState(pugi::xml_node node);


	// Funci� per aplicar un filtre de color vermell durant 1 segon a una imatge
	void aplicarFiltreVermell(SDL_Texture* texturaOriginal, SDL_Rect* pos);

public:

	SDL_Renderer* renderer;
	SDL_Rect camera;
	SDL_Rect viewport;
	SDL_Color background;
	TTF_Font* font;
	TTF_Font* primary_font;
	TTF_Font* secondary_font;
};

#endif // __RENDER_H__