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

DialogTrigger::DialogTrigger() : Entity(EntityType::DIALOG_TRIGGER)
{
	name.Create("dialogTrigger");
}

DialogTrigger::~DialogTrigger() {}

bool DialogTrigger::Awake() {

	return true;
}

bool DialogTrigger::Start() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	faceTexturePath = parameters.attribute("facetexturepath").as_string("");
	repeatDialog = parameters.attribute("repeat").as_bool(false);
	

	std::string fontTarget = parameters.attribute("font").as_string("primary");
	

	played = false;

	//Cargar dialogos
	for (pugi::xml_node itemNode = parameters.child("sentences").child("sentence"); itemNode; itemNode = itemNode.next_sibling("sentence"))
	{
		dialogues.Add(app->dialogManager->CreateDialog(itemNode, parameters.attribute("name").as_string(), faceTexturePath.GetString(), fontTarget.c_str(), 0, parameters.attribute("mapid").as_int()));
	}

	//Si el dialogo se reite, cargar las lineas que se repite
	if (repeatDialog) {
		for (pugi::xml_node itemNode = parameters.child("repeat_sentences").child("sentence"); itemNode; itemNode = itemNode.next_sibling("sentence"))
		{
			dialoguesRepeat.Add(app->dialogManager->CreateDialog(itemNode, parameters.attribute("name").as_string(), faceTexturePath.GetString(), fontTarget.c_str()));
		}
	}


	//initilize textures
	texture = app->tex->Load(texturePath);

	if (faceTexturePath != "") {
		faceTexture = app->tex->Load(faceTexturePath.GetString());
	}

	pbody = app->physics->CreateRectangleSensor(position.x, position.y, 150, 256, bodyType::KINEMATIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::DIALOG_TRIGGER;




	return true;
}

bool DialogTrigger::Update(float dt)
{
	app->render->DrawTexture(texture, position.x, position.y);

	return true;
}

bool DialogTrigger::CleanUp()
{

	SDL_DestroyTexture(texture);
	SDL_DestroyTexture(faceTexture);

	ListItem<Dialog*>* item;
	Dialog* pDialog = nullptr;

	for (item = dialogues.start; item != NULL; item = item->next)
	{
		pDialog = item->data;
		//pDialog->face_tex = faceTexture;
		pDialog->CleanUp();
		SDL_DestroyTexture(pDialog->face_tex);
	}

	dialogues.Clear();

	
	pDialog = nullptr;

	for (item = dialoguesRepeat.start; item != NULL; item = item->next)
	{
		pDialog = item->data;
		//pDialog->face_tex = faceTexture;
		SDL_DestroyTexture(pDialog->face_tex);
	}

	dialoguesRepeat.Clear();

	return true;
}

void DialogTrigger::PlayDialog()
{


	//Play el dialogo normal
	if ((played && !repeatDialog) || !played) {
	
	
		ListItem<Dialog*>* item;
		Dialog* pDialog = nullptr;

		for (item = dialogues.start; item != NULL; item = item->next)
		{
			pDialog = item->data;
			app->dialogManager->AddDialog(pDialog);
		}
		played = true;


	//Play el dialogo repetido
	}else if (played && repeatDialog) {

		
		ListItem<Dialog*>* item;
		Dialog* pDialog = nullptr;

		for (item = dialoguesRepeat.start; item != NULL; item = item->next)
		{
			pDialog = item->data;
			app->dialogManager->AddDialog(pDialog);
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



