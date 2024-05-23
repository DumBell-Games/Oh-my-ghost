#ifndef __GUICONTROL_H__
#define __GUICONTROL_H__

#include "Input.h"
#include "Render.h"
#include "Module.h"

#include "Point.h"
#include "SString.h"

#include <functional>

#include "SDL/include/SDL.h"

class GuiControl;
// Atajo para una función que toma un parametro GuiControl* y no devuelve nada
using GuiCallback_f = std::function<void(GuiControl*)>;

enum class GuiControlType
{
	BUTTON,
	TOGGLE,
	CHECKBOX,
	SLIDER,
	SLIDERBAR,
	COMBOBOX,
	DROPDOWNBOX,
	INPUTBOX,
	VALUEBOX,
	SPINNER
};

enum class GuiControlState
{
	DISABLED,
	NON_CLICKABLE,
	NORMAL,
	FOCUSED,
	PRESSED,
	SELECTED
};

class GuiControl
{
public:

	// Constructor
	GuiControl(GuiControlType type, uint32 id) : type(type), id(id), state(GuiControlState::NORMAL) {}

	// Constructor
	GuiControl(GuiControlType type, SDL_Rect bounds, const char* text) :
		type(type),
		state(GuiControlState::NORMAL),
		bounds(bounds),
		text(text)
	{
		color.r = 255; color.g = 255; color.b = 255;
		texture = NULL;
	}

	// Called each loop iteration
	virtual bool Update(float dt)
	{
		return true;
	}

	// 
	void SetTexture(SDL_Texture* tex)
	{
		texture = tex;
		section = { 0, 0, 0, 0 };
	}

	// 
	void SetObserver(Module* module)
	{
		observer = module;
	}

	void SetOnClick(GuiCallback_f func)
	{
		onClick = func;
	}

	void SetOnHover(GuiCallback_f func)
	{
		onHover = func;
	}

	// Executes the OnGuiMouseClick() function(s)
	void NotifyMouseClick()
	{
		if (observer)
			observer->OnGuiMouseClickEvent(this);
		if (onClick)
			onClick(this);
	}

	// Executes the OnHover() function
	void NotifyOnHover()
	{
		if (onHover)
			onHover(this);
	}

public:

	uint32 id;
	GuiControlType type;
	GuiControlState state;

	SString text;           // Control text (if required)
	SDL_Rect bounds;        // Position and size
	SDL_Color color;        // Tint color

	SDL_Texture* texture;   // Texture atlas reference
	SDL_Rect section;       // Texture atlas base section

	Module* observer = nullptr;        // Observer 
	GuiCallback_f onClick; // Callback function for when element is clicked
	GuiCallback_f onHover; // Callback function for when mouse is hovering over element

};

#endif // __GUICONTROL_H__