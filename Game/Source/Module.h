#ifndef __MODULE_H__
#define __MODULE_H__

#include "SString.h"

#include "PugiXml/src/pugixml.hpp"

class App;
class GuiControl;

class Module
{
public:

	Module(bool startEnabled = false) : active(startEnabled), awoken(startEnabled), needsAwaking(false)
	{}

	void Init()
	{
		active = true;
	}

	bool Enable()
	{
		bool ret = false;
		if (!active) {
			active = true;
			ret = Start();
		}
		return ret;
	}

	bool Disable()
	{
		bool ret = false;
		if (active) {
			active = false;
			ret = CleanUp();
		}
		return ret;
	}

	// Called before render is available
	virtual bool Awake(pugi::xml_node config)
	{
		return true;
	}

	// Called before the first frame
	virtual bool Start()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PreUpdate()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool Update(float dt)
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PostUpdate()
	{
		return true;
	}

	// Called before quitting
	virtual bool CleanUp()
	{
		return true;
	}

	// L14: TODO 2: Create new virtual methods to LoadState / SaveState

	// Called when we want to load data from XML
	virtual bool LoadState(pugi::xml_node node)
	{
		return true;
	}

	// Called when we want to save data from XML
	virtual bool SaveState(pugi::xml_node node)
	{
		return true;
	}

	virtual bool OnGuiMouseClickEvent(GuiControl* control)
	{
		return true;
	}

public:

	SString name;
	bool active;
	bool awoken;
	bool needsAwaking;


};

#endif // __MODULE_H__