#ifndef __GUIMANAGER_H__
#define __GUIMANAGER_H__

#include "Module.h"
#include "GuiControl.h"

#include "List.h"

#include <functional>

class GuiManager : public Module
{
public:

	// Constructor
	GuiManager(bool startEnabled = true);

	// Destructor
	virtual ~GuiManager();

	// Called before the first frame
	 bool Start();

	 // Called each loop iteration
	 bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	// Additional methods
	GuiControl* CreateGuiControl(GuiControlType type, int id, const char* text, SDL_Rect bounds, Module* observer, SDL_Rect sliderBounds = { 0,0,0,0 });

	// Creates GuiControl with a function as callback instead of a full module
	GuiControl* CreateGuiControl(GuiControlType type, int id, const char* text, SDL_Rect bounds, GuiOnClick_f observer, SDL_Rect sliderBounds = { 0,0,0,0 });

	void DestroyGuiControl(GuiControl* ctrl);
  
public:

	List<GuiControl*> guiControlsList;
	SDL_Texture* texture;

};

#endif // __GUIMANAGER_H__
