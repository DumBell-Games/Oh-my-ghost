#ifndef __MODULE_H__
#define __MODULE_H__

#include "SString.h"

#include "PugiXml/src/pugixml.hpp"

class App;
class GuiControl;

class Module
{
public:

	Module(bool startEnabled = true) : active(startEnabled), awoken(false), needsAwaking(false)
	{}

	virtual void Init()
	{
		isEnabled = active;
	}

  // Switches isEnabled and calls Start() method
	bool Enable()
	{
		bool ret = false;
		if (!isEnabled) {
			active = isEnabled = true;
			paused = false;
			ret = Start();
		}
		return ret;
	}

  // Switches isEnabled and calls CleanUp() method
	bool Disable()
	{
		bool ret = false;
		if (isEnabled) {
			active = isEnabled = false;
			paused = false;
			if (needsAwaking)
				awoken = false;
			ret = CleanUp();
		}
		return ret;
	}

	//Called before Awake(). All modules execute this regardless of initial state
	virtual bool PostInit()
	{
		return true;
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

	inline bool IsEnabled() const { return isEnabled; }

	// Pauses or unpauses the module, and returns the new pause state
	bool Pause()
	{
		active = paused;
		paused = !active;
		return paused;
	}

public:

	SString name;
	bool active;	
  
	// variable para FadeToBlack. También usado para denotar si el módulo está inicializado o no
	bool isEnabled = true;

	// variables para modulo Reload
	bool awoken;
	bool needsAwaking; // Algunos módulos necesitan volver a ejecutar Awake() cuando se activan (Ejemplo: EntityManager si se ha creado alguna entidad antes de iniciarlo)

	// Indica si el modulo esta pausado o no
	bool paused = false;

};

#endif // __MODULE_H__