#include "EntityManager.h"
#include "Player.h"
#include "Item.h"
#include "DialogTriggerEntity.h"
#include "App.h"
#include "Textures.h"
#include "Scene.h"
#include "Npc.h"
#include "Enemies.h"

#include "Defs.h"
#include "Log.h"

EntityManager::EntityManager(bool startEnabled) : Module(startEnabled)
{
	name.Create("entitymanager");
	needsAwaking = true;
}

// Destructor
EntityManager::~EntityManager()
{}

// Called before render is available
bool EntityManager::Awake(pugi::xml_node config)
{
	LOG("Loading Entity Manager");
	bool ret = true;

	//Iterates over the entities and calls the Awake
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Awake();
	}

	awoken = true;

	return ret;

}

bool EntityManager::Start() {

	bool ret = true; 

	//Iterates over the entities and calls Start
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Start();
	}

	started = true;

	return ret;
}

// Called before quitting
bool EntityManager::CleanUp()
{
	bool ret = true;
	ListItem<Entity*>* item;
	item = entities.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		RELEASE(item->data);
		item = item->prev;
	}

	entities.Clear();

	awoken = started = false;

	return ret;
}

Entity* EntityManager::CreateEntity(EntityType type, pugi::xml_node& data)
{
	Entity* entity = nullptr; 

	//L03: DONE 3a: Instantiate entity according to the type and add the new entity to the list of Entities
	switch (type)
	{
	case EntityType::PLAYER:
		entity = new Player();
		break;
	case EntityType::ITEM:
		entity = new Item();
		break;
	case EntityType::DIALOG_TRIGGER:
		entity = new DialogTrigger();
		break;
	case EntityType::NPC:
		entity = new Npc();
		break;
	case EntityType::ENEMY:
		entity = new Enemy();
		break;
		
	default:
		break;
	}

	if (entity) {
		entity->parameters = data;
		if (entity->active) {
			// Si EntityManager ya est� activo, activa la entidad directamente a menos de que est� explicitamente desactivada en su constructor
			if (awoken) entity->Awake();
			if (started) entity->Start();
		}
	}

	entities.Add(entity);

	return entity;
}

Entity* EntityManager::CreateFromMap(char typeId, pugi::xml_node& data)
{
	Entity* entity = nullptr;

	EntityType type = static_cast<EntityType>(1);


	return entity;
}

void EntityManager::DestroyEntity(Entity* entity)
{
	ListItem<Entity*>* item;

	for (item = entities.start; item != NULL; item = item->next)
	{
		if (item->data == entity) entities.Del(item);
	}
}

void EntityManager::AddEntity(Entity* entity)
{
	if ( entity != nullptr) entities.Add(entity);
}

bool EntityManager::Update(float dt)
{
	bool ret = true;
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Update(dt);
	}

	return ret;
}

bool EntityManager::LoadState(pugi::xml_node node) {

	return true;
}

bool EntityManager::SaveState(pugi::xml_node node) {
	
	return true;
}