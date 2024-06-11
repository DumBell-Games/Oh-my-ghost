#include "GuiManager.h"
#include "App.h"
#include "Textures.h"

#include "GuiControlButton.h"
#include "GuiControlPhysButton.h"
#include "GuiCombatHP.h"
#include "Audio.h"
#include "Log.h"

GuiManager::GuiManager(bool startEnabled) : Module(startEnabled)
{
	name.Create("guiManager");
}

GuiManager::~GuiManager() {}

bool GuiManager::Start()
{
	return true;
}

// L15: DONE1: Implement CreateGuiControl function that instantiates a new GUI control and add it to the list of controls
GuiControl* GuiManager::CreateGuiControl(GuiControlType type, int id, const char* text, SDL_Rect bounds, Module* observer, bool independentPointer, SDL_Rect sliderBounds)
{
	GuiControl* guiControl = nullptr;

	//Call the constructor according to the GuiControlType
	switch (type)
	{
	case GuiControlType::BUTTON:
	{
		guiControl = new GuiControlButton(id, bounds, text);
		break;
	}
	case GuiControlType::PHYSBUTTON_BOX:
	{
		guiControl = new GuiControlPhysButton(id, type, bounds, text);
		break;
	}
	case GuiControlType::PHYSBUTTON_CIRCLE:
	{
		guiControl = new GuiControlPhysButton(id, type, bounds, text);
		break;
	}
	case GuiControlType::PROGRESS_BAR:
	{
		guiControl = new GuiCombatHP(id, bounds, text);
	}
	}

	if (guiControl) {
		//Set the observer
		guiControl->observer = observer;

		// Created GuiControls are add it to the list of controls
		if (!independentPointer)
			guiControlsList.Add(guiControl);
	}

	return guiControl;
}

GuiControl* GuiManager::CreateGuiControl(GuiControlType type, int id, const char* text, SDL_Rect bounds, GuiCallback_f observer, bool independentPointer, SDL_Rect sliderBounds)
{
	GuiControl* guiControl = CreateGuiControl(type,id,text,bounds,nullptr, independentPointer,sliderBounds);

	if (guiControl) {
		//Set the observer
		guiControl->onClick = observer;
	}

	return guiControl;
}

void GuiManager::DestroyGuiControl(GuiControl* ctrl)
{
	ListItem<GuiControl*>* item = guiControlsList.At(guiControlsList.Find(ctrl));
	guiControlsList.Del(item);
	RELEASE(ctrl);
}

List<GuiControl*> GuiManager::LoadLayout(pugi::xml_node layoutLayer)
{
	List<GuiControl*> list;
	LOG("LoadLayout is unfinished");
	return list;
}

bool GuiManager::Update(float dt)
{	
	ListItem<GuiControl*>* control = guiControlsList.start;

	while (control != nullptr)
	{
		control->data->Update(dt);
		control = control->next;
	}

	return true;
}

bool GuiManager::PostUpdate()
{
	ListItem<GuiControl*>* control = guiControlsList.start;

	while (control != nullptr)
	{
		control->data->Render();
		control = control->next;
	}

	return true;
}

bool GuiManager::CleanUp()
{
	ListItem<GuiControl*>* control = guiControlsList.start;

	while (control != nullptr)
	{
		
		RELEASE(control->data);
		control = control->next;
	}
	guiControlsList.Clear();

	return true;
}
