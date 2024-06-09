#ifndef __GUIMANAGER_H__
#define __GUIMANAGER_H__

#include "Module.h"
#include "GuiControl.h"
#include "GuiGroup.h"

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

	 bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Additional methods
	GuiControl* CreateGuiControl(GuiControlType type, int id, const char* text, SDL_Rect bounds, Module* observer, bool independentPointer = false, SDL_Rect sliderBounds = { 0,0,0,0 });

	// Creates GuiControl with a function as callback instead of a full module
	GuiControl* CreateGuiControl(GuiControlType type, int id, const char* text, SDL_Rect bounds, GuiCallback_f observer, bool independentPointer = false, SDL_Rect sliderBounds = { 0,0,0,0 });

	void DestroyGuiControl(GuiControl* ctrl);

	// Loads a full GuiGroup from an xml node generated with Tiled
	List<GuiControl*> LoadLayout(pugi::xml_node layoutLayer);
  
public:

	List<GuiControl*> guiControlsList;
	SDL_Texture* texture;

};

#endif // __GUIMANAGER_H__
