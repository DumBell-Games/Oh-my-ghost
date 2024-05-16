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
#include "Entity.h"
#include "EntityManager.h"

#include "Defs.h"
#include "Log.h"
#include "EnumUtils.h"

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

	inventoryPath = config.attribute("inventoryFile").as_string("inventory.xml");

	//CreateItem(ItemType::PATATA)

	/*
	* //There's no Awake() method for items
	//Iterates over the entities and calls the Init
	ListItem<ItemData*>* item;
	ItemData* pItem = NULL;

	for (item = items.start; item != NULL && ret == true; item = item->next)
	{
		pItem = item->data;
	}
	*/

	awoken = true;

	return ret;

}

bool InventoryManager::Start() {

	bool ret = true; 

	//Iterates over the entities and calls Init
	ListItem<ItemData*>* item;
	ItemData* pItem = NULL;

	for (item = items.start; item != NULL && ret == true; item = item->next)
	{
		pItem = item->data;

		if (pItem->active == false) continue;
		ret = item->data->Init();
	}

	started = true;

	return ret;
}

// Called before quitting
bool InventoryManager::CleanUp()
{
	bool ret = true;
	ListItem<ItemData*>* item;
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

ItemData* InventoryManager::CreateItem(ItemType type, pugi::xml_node& data)
{
	ItemData* item = nullptr;

	//L03: DONE 3a: Instantiate entity according to the type and add the new entity to the list of Entities

	if (item) {
		item->parameters = data;
		if (item->active) {
			// Si EntityManager ya está activo, activa la entidad directamente a menos de que esté explicitamente desactivada en su constructor
			if (started) item->Init();
		}
	}

	items.Add(item);

	return item;
}

// Returns the item with the name provided, nullptr if not found
ItemData* InventoryManager::GetItem(const char* name)
{
	//SIN PROBAR: SI CRASHEA ITERA LA LISTA COMO SIEMPRE EN VEZ DE USAR ESTO
	return items.Find([&name](ItemData* i) { return i->name == name; })->data;
}

void InventoryManager::DestroyItem(ItemData* item)
{
	ListItem<ItemData*>* iTem;

	for (iTem = items.start; iTem != NULL; iTem = iTem->next)
	{
		if (iTem->data == item) items.Del(iTem);
	}
}

void InventoryManager::AddItem(ItemData* item)
{
	if ( item != nullptr) 
	{
		items.Add(item);
	}
}

bool InventoryManager::Update(float dt)
{
	return true;
}

bool InventoryManager::LoadState(pugi::xml_node node) {
	LOG("INVENTORY LOADING NOT FINISHED");
	for (pugi::xml_node item = node.child("item"); item != NULL; item = item.next_sibling("item"))
	{
		CreateItem((ItemType)item.attribute("type").as_int(enum2val(ItemType::UNKNOWN)),item);
	}
	return true;
}

bool InventoryManager::SaveState(pugi::xml_node node) {
	LOG("INVENTORY SAVING NOT FINISHED");
	for (ListItem<ItemData*>* item = 0; item != NULL; item = item->next)
	{
		item->data->SaveState(node.append_child("item"));
	}
	return true;
}