#include "DialogTriggerEntity.h"
#include "DialogManager.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Map.h"
#include "PropertiesStruct.h"

#include <vector>
#include <string>


DialogTrigger::DialogTrigger() : Entity(EntityType::DIALOG_TRIGGER)
{
	name.Create("dialogTrigger");
}

DialogTrigger::~DialogTrigger() {}

bool DialogTrigger::Awake() {

	Properties p;
	LoadProperties(parameters, p);
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();

	if (p.list.start) {

		/*texturePath = p.GetProperty("texturepath")->strVal;*/

		SString idString = p.GetProperty("ids")->strVal;

		std::vector<SString> idvec = idString.GetWords(',');

		for (SString& id : idvec) {
			orderedDialogs.push_back(std::stoi(id.GetString()));
		}
	}


	iPoint size;
	size.x = parameters.attribute("w").as_int(128);
	size.y = parameters.attribute("h").as_int(128);

	return true;
}

bool DialogTrigger::Start() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	faceTexturePath = parameters.attribute("facetexturepath").as_string("");
	repeatDialog = parameters.attribute("repeat").as_bool(false);
	

	// texturePath = parameters.attribute("texturepath").as_string();
	// faceTexturePath = parameters.attribute("facetexturepath").as_string("");
	// repeatDialog = parameters.attribute("repeat").as_bool(false);
	std::string fontTarget = parameters.attribute("font").as_string("primary");
	

	played = false;

	//Cargar dialogos
	for (pugi::xml_node itemNode = parameters.child("sentences").child("sentence"); itemNode; itemNode = itemNode.next_sibling("sentence"))
	{
		dialogues.Add(app->dialogManager->CreateDialogs(itemNode, parameters.attribute("name").as_string(), faceTexturePath, fontTarget.c_str(), 0, parameters.attribute("mapid").as_int()));
	}

	//Si el dialogo se reite, cargar las lineas que se repite
	if (repeatDialog) {
		for (pugi::xml_node itemNode = parameters.child("repeat_sentences").child("sentence"); itemNode; itemNode = itemNode.next_sibling("sentence"))
		{
			dialoguesRepeat.Add(app->dialogManager->CreateDialogs(itemNode, parameters.attribute("name").as_string(), faceTexturePath, fontTarget.c_str()));
		}
	}


	//initilize textures
	texture = app->tex->LoadSP(texturePath, true);

	if (faceTexturePath != "") {
		faceTexture = app->tex->LoadSP(faceTexturePath, true);
	}

	pbody = app->physics->CreateRectangleSensor(position.x, position.y, 150, 256, bodyType::KINEMATIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::DIALOG_TRIGGER;




	return true;
}

bool DialogTrigger::Update(float dt)
{
	app->render->DrawTexture(texture.get(), position.x, position.y);

	return true;
}

bool DialogTrigger::CleanUp()
{

	app->tex->UnLoadSP(texture);
	app->tex->UnLoadSP(faceTexture);

	ListItem<Dialog*>* item;
	Dialog* pDialog = nullptr;

	for (item = dialogues.start; item != NULL; item = item->next)
	{
		pDialog = item->data;
		//pDialog->face_tex = faceTexture;
		pDialog->CleanUp();
		app->tex->UnLoadSP(pDialog->face_tex);
	}

	dialogues.Clear();

	
	pDialog = nullptr;

	for (item = dialoguesRepeat.start; item != NULL; item = item->next)
	{
		pDialog = item->data;
		//pDialog->face_tex = faceTexture;
		app->tex->UnLoadSP(pDialog->face_tex);
	}

	dialoguesRepeat.Clear();

	return true;
}

void DialogTrigger::PlayDialog()
{
	//Play el dialogo normal


	ListItem<Dialog*>* item;
	Dialog* pDialog = nullptr;

	if (orderedDialogs.size() > 0) {
		//DialogGroup* dg = app->dialogManager->GetDialogs(orderedDialogs[currentDialog]);
		//
		////Guard clause
		//if (dg == nullptr) return;

		//app->dialogManager->PlayDialog(dg);

		// for (item = dg->dialogs.start; item != NULL; item = item->next)
		// {
		// 	pDialog = item->data;
		// 	app->dialogManager->AddDialog(pDialog);
		// }
		played = true;
		currentDialog++;
		if (currentDialog >= orderedDialogs.size())
		{
			currentDialog = orderedDialogs.size() - 1;
		}
	}

}
	
	
	



void DialogTrigger::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
		case ColliderType::PLAYER:
			//If currentMapId == diaogog->mapID 
			if (!app->dialogManager->isPlaying) {
				PlayDialog();
			}
			break;
	}
}



