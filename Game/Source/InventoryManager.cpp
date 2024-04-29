#include "InventoryManager.h"
#include "Player.h"
#include "DialogTriggerEntity.h"
#include "App.h"
#include "Textures.h"
#include "Scene.h"
#include "Npc.h"
#include "Enemies.h"
#include "TransitionTrigger.h"
#include "ItemCola.h"

#include "Defs.h"
#include "Log.h"

InventoryManager::InventoryManager(bool startEnabled) : Module(startEnabled)
{
	name.Create("inventorymanager");
	needsAwaking = true;
}

// Destructor
InventoryManager::~InventoryManager()
{}

// Called before render is available
bool InventoryManager::Awake(pugi::xml_node config)
{
	LOG("Loading Inventory Manager");
	bool ret = true;

	//Iterates over the entities and calls the Awake
	ListItem<Inventory*>* item;
	Inventory* pItem = NULL;

	for (item = items.start; item != NULL && ret == true; item = item->next)
	{
		pItem = item->data;

		if (pItem->active == false) continue;
		ret = item->data->Awake();
	}

	awoken = true;

	return ret;

}

bool InventoryManager::Start() {

	bool ret = true; 

	//Iterates over the entities and calls Start
	ListItem<Inventory*>* item;
	Inventory* pItem = NULL;

	for (item = items.start; item != NULL && ret == true; item = item->next)
	{
		pItem = item->data;

		if (pItem->active == false) continue;
		ret = item->data->Start();
	}

	started = true;

	return ret;
}

// Called before quitting
bool InventoryManager::CleanUp()
{
	bool ret = true;
	ListItem<Inventory*>* item;
	item = items.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		RELEASE(item->data);
		item = item->prev;
	}

	items.Clear();

	awoken = started = false;

	return ret;
}

Inventory* InventoryManager::CreateItem(ItemType type, pugi::xml_node& data)
{
	Inventory* item = nullptr;

	//L03: DONE 3a: Instantiate entity according to the type and add the new entity to the list of Entities
	switch (type)
	{
		case ItemType::COLA:
		item = new Cola();
		break;	
	default:
		break;
	}

	if (item) {
		item->parameters = data;
		if (item->active) {
			// Si EntityManager ya está activo, activa la entidad directamente a menos de que esté explicitamente desactivada en su constructor
			if (awoken) item->Awake();
			if (started) item->Start();
		}
	}

	items.Add(item);

	return item;
}

void InventoryManager::DestroyItem(Inventory* item)
{
	ListItem<Inventory*>* iTem;

	for (iTem = items.start; iTem != NULL; iTem = iTem->next)
	{
		if (iTem->data == item) items.Del(iTem);
	}
}

void InventoryManager::AddItem(Inventory* item)
{
	if ( item != nullptr) items.Add(item);
}

bool InventoryManager::Update(float dt)
{
	bool ret = true;
	ListItem<Inventory*>* item;
	Inventory* pItem = NULL;

	for (item = items.start; item != NULL && ret == true; item = item->next)
	{
		pItem = item->data;

		if (pItem->active == false) continue;
		ret = item->data->Update(dt);
	}

	return ret;
}

bool InventoryManager::LoadState(pugi::xml_node node) {

	return true;
}

bool InventoryManager::SaveState(pugi::xml_node node) {
	
	return true;
}