#include "GuiManager.h"
#include "App.h"
#include "Textures.h"

#include "GuiControlButton.h"
#include "Audio.h"

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
GuiControl* GuiManager::CreateGuiControl(GuiControlType type, int id, const char* text, SDL_Rect bounds, Module* observer, SDL_Rect sliderBounds)
{
	GuiControl* guiControl = nullptr;

	//Call the constructor according to the GuiControlType
	switch (type)
	{
	case GuiControlType::BUTTON:
		guiControl = new GuiControlButton(id, bounds, text);
		break;
	}

	if (guiControl) {
		//Set the observer
		guiControl->observer = observer;

		// Created GuiControls are add it to the list of controls
		guiControlsList.Add(guiControl);
	}

	return guiControl;
}

GuiControl* GuiManager::CreateGuiControl(GuiControlType type, int id, const char* text, SDL_Rect bounds, GuiCallback_f observer, SDL_Rect sliderBounds)
{
	GuiControl* guiControl = CreateGuiControl(type,id,text,bounds,nullptr,sliderBounds);

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
