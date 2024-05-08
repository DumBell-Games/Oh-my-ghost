#ifndef __WINDOW_H__
#define __WINDOW_H__

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define SCREEN_SIZE 1

#include "Module.h"

struct SDL_Window;
struct SDL_Surface;

class Window : public Module
{
public:

	Window(bool startEnabled = true);

	// Destructor
	virtual ~Window();

	// Called before render is available
	bool Awake(pugi::xml_node config);

	// Called before quitting
	bool CleanUp();

	// Changae title
	void SetTitle(const char* title);

	// Retrive window size
	void GetWindowSize(uint& width, uint& height) const;

	// Retrieve window scale
	uint GetScale() const;

	void FullscreenMode();

	void UnFullscreenMode();

public:
	// The window we'll be rendering to
	SDL_Window* window;

	// The surface contained by the window
	SDL_Surface* screenSurface;

private:
	SString title;
	uint width;
	uint height;
	uint scale;
};

#endif // __WINDOW_H__