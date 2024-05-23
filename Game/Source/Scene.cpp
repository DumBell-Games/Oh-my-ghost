#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Optick/include/optick.h"
#include "DialogTriggerEntity.h"
#include "SilverWings.h"
#include "Enemies.h"
#include "Physics.h"
#include "FadeToBlack.h"
#include "PauseMenu.h"
#include "menu.h"
#include "ItemCola.h"
#include "ItemYogur.h"
#include "ItemCaramelos.h"
#include "ItemPatatas.h"
#include "ItemBirra.h"
#include "ItemVelocidad.h"
#include "InventoryScreen.h"
#include "Character_Menu.h"
#include "Aprendiz.h"
#include "Veterana.h"
#include "QuestPatatasScreen.h"


#include "Defs.h"
#include "Log.h"
#include "GuiControl.h"
#include "GuiManager.h"
#include "GuiControlButton.h"

Scene::Scene(bool startEnabled) : Module(startEnabled)
{
	name.Create("scene");
	needsAwaking = true;
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake(pugi::xml_node config)
{
	LOG("Loading Scene");
	bool ret = true;

	//L03: DONE 3b: Instantiate the player using the entity manager
	//L04 DONE 7: Get player paremeters
	player = (Player*) app->entityManager->CreateEntity(EntityType::PLAYER, config.child("player"));
	//Assigns the XML node to a member in player
	//player->config = config.child("player");

	// iterate all items in the scene
	// Check https://pugixml.org/docs/quickstart.html#access
	
	for (pugi::xml_node itemNode = config.child("cola"); itemNode; itemNode = itemNode.next_sibling("cola"))
	{
		Cola* cola = (Cola*)app->entityManager->CreateEntity(EntityType::COLA, itemNode);
		//item->parameters = itemNode;
	}
	for (pugi::xml_node itemNode = config.child("yogur"); itemNode; itemNode = itemNode.next_sibling("yogur"))
	{
		Yogur* yogur = (Yogur*)app->entityManager->CreateEntity(EntityType::YOGUR, itemNode);
		//item->parameters = itemNode;
	}
	for (pugi::xml_node itemNode = config.child("caramelos"); itemNode; itemNode = itemNode.next_sibling("caramelos"))
	{
		Caramelos* caramelos = (Caramelos*)app->entityManager->CreateEntity(EntityType::CARAMELOS, itemNode);
		//item->parameters = itemNode;
	}
	for (pugi::xml_node itemNode = config.child("patatas"); itemNode; itemNode = itemNode.next_sibling("patatas"))
	{
		Patatas* patatas = (Patatas*)app->entityManager->CreateEntity(EntityType::PATATAS, itemNode);
		//item->parameters = itemNode;
	}
	for (pugi::xml_node itemNode = config.child("birra"); itemNode; itemNode = itemNode.next_sibling("birra"))
	{
		Birra* birra = (Birra*)app->entityManager->CreateEntity(EntityType::BIRRA, itemNode);
		//item->parameters = itemNode;
	}
	for (pugi::xml_node itemNode = config.child("velocidad"); itemNode; itemNode = itemNode.next_sibling("velocidad"))
	{
		Velocidad* velocidad = (Velocidad*)app->entityManager->CreateEntity(EntityType::VELOCIDAD, itemNode);
		//item->parameters = itemNode;
	}
	for (pugi::xml_node itemNode = config.child("dialogTrigger"); itemNode; itemNode = itemNode.next_sibling("dialogTrigger"))
	{
		DialogTrigger* dialogTrigger = (DialogTrigger*)app->entityManager->CreateEntity(EntityType::DIALOG_TRIGGER, itemNode);
		//dialogTrigger->parameters = itemNode;
	}
	for (pugi::xml_node itemNode = config.child("silverWings"); itemNode; itemNode = itemNode.next_sibling("silverWings"))
	{
		SilverWings* silverWings = (SilverWings*)app->entityManager->CreateEntity(EntityType::SILVERWINGS, itemNode);
		//npc->parameters = itemNode;
	}
	for (pugi::xml_node itemNode = config.child("aprendiz"); itemNode; itemNode = itemNode.next_sibling("aprendiz"))
	{
		Aprendiz* aprendiz = (Aprendiz*)app->entityManager->CreateEntity(EntityType::APRENDIZ, itemNode);
		//npc->parameters = itemNode;
	}
	for (pugi::xml_node itemNode = config.child("veterana"); itemNode; itemNode = itemNode.next_sibling("veterana"))
	{
		Veterana* veterana = (Veterana*)app->entityManager->CreateEntity(EntityType::VETERANA, itemNode);
		//npc->parameters = itemNode;
	}
	for (pugi::xml_node itemNode = config.child("enemy"); itemNode; itemNode = itemNode.next_sibling("enemy"))
	{
		Enemy* enemy = (Enemy*)app->entityManager->CreateEntity(EntityType::ENEMY, itemNode);
		//enemy->parameters = itemNode;
	}

	awoken = true;

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	// NOTE: We have to avoid the use of paths in the code, we will move it later to a config file
	img = app->tex->Load("Assets/Textures/test.png"); 
	
	//Music is commented so that you can add your own music
	//app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	//Get the size of the texture
	app->tex->GetSize(img, texW, texH);

	textPosX = (float)windowW / 2 - (float)texW / 2;
	textPosY = (float)windowH / 2 - (float)texH / 2;

	// Texture to highligh mouse position 
	mouseTileTex = app->tex->Load("Assets/Maps/tileSelection.png");
		
	app->audio->PlayMusic("Assets/Audio/Fx/centralFauna.wav");

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	OPTICK_EVENT();
	//L02 DONE 3: Make the camera movement independent of framerate
	float camSpeed = 1; 

	if(app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y -= (int)ceil(camSpeed * dt);

	if(app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y += (int)ceil(camSpeed * dt);

	if(app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x -= (int)ceil(camSpeed * dt);

	if(app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x += (int)ceil(camSpeed * dt);

	// L09 DONE 6: Implement a method that repositions the player in the map with a mouse click
	iPoint origin = iPoint(2, 21);
	//app->map->pathfinding->CreatePath(origin, app->map->WorldToMap(player->position.x,player->position.y));

	// L13: Get the latest calculated path and draw
	const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();
	for (uint i = 0; i < path->Count(); ++i)
	{		
		iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		if (app->DebugEnabled())
		{
			app->render->DrawTexture(mouseTileTex, pos.x, pos.y);
		}
	}
	
	// L14: DONE 3: Request App to Load / Save when pressing the keys F5 (save) / F6 (load)
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->SaveRequest();
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) app->LoadRequest();
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) fullscreen = true;
	if (app->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN) fullscreen = false;
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) 
		app->pause->Enable();
	if (app->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN) 
		app->inventoryScreen->Enable();
	
	if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) 
		app->characterMenu->Enable();
	
	if (fullscreen == true) {
		app->win->FullscreenMode();
	}
	else {
		app->win->UnFullscreenMode();
	}

	if (app->scene->GetPatatasQuantity() == 3) {
		app->scene->ColaPicked();
		app->scene->ColaPicked();
		app->scene->ColaPicked();
		app->scene->ColaPicked();
		app->scene->ColaPicked();
		if (app->input->GetButton(ControlID::CONFIRM) == KEY_DOWN) {
			app->scene->PatatasPicked();
		}
	}
	if (app->scene->GetCaramelosQuantity() == 3) {
		app->scene->VelocidadPicked();
		app->scene->VelocidadPicked();
		app->scene->VelocidadPicked();
		app->scene->VelocidadPicked();
		app->scene->VelocidadPicked();
		if (app->input->GetButton(ControlID::CONFIRM) == KEY_DOWN) {
			app->scene->CaramelosPicked();
		}
	}
	if (app->scene->GetYogurQuantity() == 3) {
		app->scene->BirraPicked();
		app->scene->BirraPicked();
		app->scene->BirraPicked();
		app->scene->BirraPicked();
		app->scene->BirraPicked();
		if (app->input->GetButton(ControlID::CONFIRM) == KEY_DOWN) {
			app->scene->YogurPicked();
		}
	}
	

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;



	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		//Destroy all the buttons in the title screen
		/*ListItem<GuiControl*>* controlListMenu = nullptr;
		for (controlListMenu = app->titlescreen->titleButtons.start; controlListMenu != NULL; controlListMenu = controlListMenu->next)
		{
			app->guiManager->DestroyGuiControl(controlListMenu->data);
		}
		app->titlescreen->titleButtons.Clear();*/

		/*app->guiManager->active = true;
		app->guiManager->Enable();
		app->pause->Enable();
		app->pause->active = true;
		app->pause->CreatePauseButtons();*/

		app->fadeToBlack->FadeToBlackTransition((Module*)app->scene, (Module*)app->pause, 0.0f);
	}


	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	if (player) {
		app->entityManager->DestroyEntity(player);
		player = nullptr;
	}

	if (cityFx > 0) {
		app->audio->UnloadFx(cityFx);
		cityFx = 0;
	}

	awoken = false; // TODO temporal, parche para que spawneen las entidades

	return true;
}

iPoint Scene::GetPLayerPosition() {
	return player->position;
}

bool Scene::OnGuiMouseClickEvent(GuiControl* control)
{
	// L15: DONE 5: Implement the OnGuiMouseClickEvent method
	LOG("Press Gui Control: %d", control->id);

	return true;
}

