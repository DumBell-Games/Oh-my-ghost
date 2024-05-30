#include "EntityManager.h"
#include "Player.h"
#include "DialogTriggerEntity.h"
#include "App.h"
#include "Textures.h"
#include "Scene.h"
#include "SilverWings.h"
#include "Enemies.h"
#include "TransitionTrigger.h"
#include "ItemCola.h"
#include "ItemYogur.h"
#include "ItemCaramelos.h"
#include "ItemPatatas.h"
#include "ItemBirra.h"
#include "ItemVelocidad.h"
#include "Aprendiz.h"
#include "Veterana.h"
#include "Conserje.h"
#include "Deidad.h"
#include "DjBunny.h"
#include "AstroBark.h"
#include "Segismunda.h"
#include "Vagabundo.h"
#include "SrBestia.h"
#include "Dolly.h"
#include "OsoCarpintero.h"
#include "Kiwi.h"
#include "Sisebuto.h"

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
	case EntityType::SILVERWINGS:
		entity = new SilverWings();
		break;
	case EntityType::APRENDIZ:
		entity = new Aprendiz();
		break;
	case EntityType::VETERANA:
		entity = new Veterana();
		break;
	case EntityType::CONSERJE:
		entity = new Conserje();
		break;
	case EntityType::DEIDAD:
		entity = new Deidad();
		break;	
	case EntityType::DJBUNNY:
		entity = new DjBunny();
		break;
	case EntityType::ASTROBARK:
		entity = new Astrobark();
		break;
	case EntityType::SEGISMUNDA:
		entity = new Segismunda();
		break;
	case EntityType::VAGABUNDO:
		entity = new Vagabundo();
		break;
	case EntityType::MRBESTIA:
		entity = new SrBestia();
		break;
	case EntityType::SISEBUTO:
		entity = new Sisebuto();
		break;
	case EntityType::DOLLY:
		entity = new Dolly();
		break;
	case EntityType::OSO:
		entity = new Oso();
		break;
	case EntityType::KIWI:
		entity = new Kiwi();
		break;
	case EntityType::ENEMY:
		entity = new Enemy();
		break;
	case EntityType::TRANSITION:
		entity = new TransitionTrigger();
		break;
	case EntityType::COLA:
		entity = new Cola();
		break;
	case EntityType::YOGUR:
		entity = new Yogur();
		break;
	case EntityType::PATATAS:
		entity = new Patatas();
		break;
	case EntityType::CARAMELOS:
		entity = new Caramelos();
		break;
	case EntityType::BIRRA:
		entity = new Birra();
		break;
	case EntityType::VELOCIDAD:
		entity = new Velocidad();
		break;
	case EntityType::DIALOG_TRIGGER:
		entity = new DialogTrigger();
		break;
					

		
	default:
		break;
	}

	if (entity) {
		entity->parameters = data;
		if (entity->active) {
			// Si EntityManager ya está activo, activa la entidad directamente a menos de que esté explicitamente desactivada en su constructor
			if (awoken) entity->Awake();
			if (started) entity->Start();
		}
	}

	entities.Add(entity);

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

//post update
bool EntityManager::PostUpdate()
{
	bool ret = true;
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->PostUpdate();
	}

	return ret;
}

bool EntityManager::LoadState(pugi::xml_node node) {

	return true;
}

bool EntityManager::SaveState(pugi::xml_node node) {
	
	return true;
}