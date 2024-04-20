#include "Reload.h"
#include "App.h"

#include "Box2D/Box2D/Box2D.h"
#include "Log.h"

Reload::Reload() : Module() {
	name.Create("reload");
}

Reload::~Reload() {

}

bool Reload::Awake(pugi::xml_node config)
{
	// TODO Añadir los módulos a recargar desde el xml
	for (pugi::xml_node& presetNode = config.child("presets").child("preset"); presetNode != NULL; presetNode = presetNode.next_sibling("preset"))
	{
		SString presetName = presetNode.attribute("name").as_string();
		float fadeOut = presetNode.attribute("fadeOut").as_float();
		float fadeIn = presetNode.attribute("fadeIn").as_float();
		// Add a preset to the list, keep the pointer to add modules to said preset
		ReloadPreset* preset = presetList.Add(new ReloadPreset(presetName, fadeOut, fadeIn))->data;
		for (pugi::xml_node& moduleNode = presetNode.child("module"); moduleNode != NULL; moduleNode = moduleNode.next_sibling("module"))
		{
			SString type = moduleNode.attribute("type").as_string();
			SString moduleName = moduleNode.attribute("name").as_string();
			Module* mod = app->GetModule(moduleName.GetString());
			if (type == "r")
				preset->AddReload(mod);
			else if (type == "l")
				preset->AddLoad(mod);
			else if (type == "u")
				preset->AddUnload(mod);
		}
	}

	return true;
}

bool Reload::Start()
{
	// Enable blending mode for transparency
	SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_BLEND);
	screenRect = app->render->camera;
	screenRect.x = screenRect.y = 0;

	return true;
}

bool Reload::PostUpdate()
{
	if (activePreset == nullptr && !queue.empty()) {
		activePreset = queue.front(); queue.pop();
		currentStep = ReloadStep::FADE_OUT;
	}

	if (activePreset != nullptr) {

		// Steps in order: fade out -> reload modules -> fade in -> finished
		switch (currentStep)
		{
		case ReloadStep::FADE_OUT: {
			FadeOut();
			break;
		}
		case ReloadStep::RELOAD: {
			ReloadModules();
			break;
		}
		case ReloadStep::FADE_IN: {
			FadeIn();
			break;
		}
		default:
			break;
		}
		//LOG("fadeRatio: %f", fadeRatio);
		//LOG("Alpha: %i", (uint8)(fadeRatio * 255.0f));
		app->render->DrawRectangle(screenRect, 0, 0, 0, (uint8)(fadeRatio * 255.0f), true, false);
	}

	return true;
}

bool Reload::CleanUp()
{
	for (ListItem<ReloadPreset*>* item = presetList.start; item != nullptr; item = item->next)
	{
		if (item->data != nullptr) {
			RELEASE(item->data);
		}
	}
	presetList.Clear();

	return true;
}

bool Reload::QueueReload(SString presetName)
{
	bool ret = false;
	for (size_t i = 0; i < presetList.Count(); i++)
	{
		ListItem<ReloadPreset*>* item = presetList.At(i);
		if (item->data->name == presetName) {
			ret = true;
			queue.push(item->data);
			break;
		}
	}

	return true;
}

void Reload::FadeOut()
{
	if (!timerActive) {
		timer.Start();
		timerActive = true;
	}
	else {
		if (activePreset->fadeOut != 0)
			fadeRatio = (timer.ReadMSec() / 1000.0f) / activePreset->fadeOut;
		else fadeRatio = 1;
		fadeRatio = b2Clamp(fadeRatio, 0.0f, 255.0f);
		if (fadeRatio >= 1.0f) {
			activePreset->fadeOut;
			timerActive = false;
			currentStep = ReloadStep::RELOAD;
		}
	}
}

void Reload::ReloadModules()
{


	for (ListItem<Module*>* item = activePreset->unload.end; item != nullptr; item = item->prev)
	{
		if (item->data != nullptr) item->data->Disable();
	}

	// Algunos modulos requieren recargar la configuración al reiniciarse
	for (ListItem<Module*>* item = activePreset->load.start; item != nullptr; item = item->next)
	{
		if (item->data != nullptr && item->data->needsAwaking && !item->data->awoken) {
			pugi::xml_node config = app->GetConfig(*(item->data));
			item->data->Awake(config);
		}
	}

	for (ListItem<Module*>* item = activePreset->load.start; item != nullptr; item = item->next)
	{
		if (item->data != nullptr) item->data->Enable();
	}

	currentStep = ReloadStep::FADE_IN;
}

void Reload::FadeIn()
{
	if (!timerActive) {
		timer.Start();
		timerActive = true;
	}
	else {
		if (activePreset->fadeIn != 0)
			fadeRatio = 1 - ((timer.ReadMSec() / 1000.0f) / activePreset->fadeIn);
		else fadeRatio = 0;
		fadeRatio = b2Clamp(fadeRatio, 0.0f, 255.0f);
		if (fadeRatio <= b2_epsilon) {
			activePreset = nullptr;
			timerActive = false;
			currentStep = ReloadStep::NONE;
			fadeRatio = 0.0f;
		}
	}
}
