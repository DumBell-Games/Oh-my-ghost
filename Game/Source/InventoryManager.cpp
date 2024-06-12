#include "InventoryManager.h"
#include "Player.h"
#include "DialogTriggerEntity.h"
#include "App.h"
#include "Textures.h"
#include "Scene.h"
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

	// Carga "items vacios" de cada tipo para gestionar el inventario
	for (pugi::xml_node itemNode = config.child("item"); itemNode != NULL; itemNode = itemNode.next_sibling("item"))
	{
		CreateItem((ItemType)itemNode.attribute("type").as_int(enum2val(ItemType::UNKNOWN)), itemNode);
	}

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
		ret = item->data->Init ();
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
	switch (type)
	{
	case ItemType::COLA:
	{
		item = new ItemData(type);
		break;
	}
	case ItemType::YOGUR:
	{
		item = new ItemData(type);
		break;
	}
	case ItemType::BIRRA:
	{
		item = new ItemData(type);
		break;
	}
	case ItemType::PATATAS:
	{
		item = new ItemData(type);
		break;
	}
	case ItemType::CARAMELOS:
	{
		item = new ItemData(type);
		break;
	}
	case ItemType::VELOCIDAD:
	{
		item = new ItemData(type);
		// Crear derivado de ItemData para el efecto de velocidad
		// item = new ItemDVelocidad();
		break;
	}
	case ItemType::UNKNOWN:
	default:
	{
		item = new ItemData(type);
		break;
	}
	}

	if (item) {
		item->parameters = data;
		if (item->active) {
			// Si EntityManager ya está activo, activa la entidad directamente a menos de que esté explicitamente desactivada en su constructor
			if (started) item->Init ();
		}
	}

	items.Add(item);

	return item;
}

// Devuelve el primer objeto que cumpla la condición, nullptr en caso de no encontrar ninguno. Este método acepta un "functor", que es cualquier cosa que pueda ser llamada como una función.
// En este caso acepta cualquier functor que tenga el siguiente formato de función: bool FuncName(ItemData*)
template <typename Comp>
ItemData* InventoryManager::GetItem(Comp condicion)
{
	//SIN PROBAR: SI CRASHEA ITERA LA LISTA COMO SIEMPRE EN VEZ DE USAR ESTO
	return items.Find(condicion)->data;
}

ItemData* InventoryManager::GetItemByType(ItemType type) {
	ListItem<ItemData*>* listItem = items.Find([type](const ItemData* i) { return i->type == type; });
	if (listItem)
		return listItem->data;
	else return nullptr;
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
	bool ret = true;
	for (pugi::xml_node itemNode = node.child("item"); itemNode != NULL; itemNode = itemNode.next_sibling("item"))
	{
		ItemType type = (ItemType)itemNode.attribute("type").as_int(enum2val(ItemType::UNKNOWN));
		ItemData* i = GetItem([&type](ItemData* i) { return i->type == type; });
		if (i)
			ret = i->LoadState(itemNode);
		else
		{
			LOG("ERROR: OBJETO GUARDADO NO TIENE TIPO CORRESPONDIENTE EN LA LISTA DE OBJETOS POSIBLES");
			CreateItem(ItemType::UNKNOWN, pugi::xml_node())
				->LoadState(itemNode);

		}
	}
	return true;
}

bool InventoryManager::SaveState(pugi::xml_node node) {
	LOG("INVENTORY SAVING NOT FINISHED");
	bool ret = true;
	for (ListItem<ItemData*>* item = items.start; item; item = item->next)
	{
		ret = item->data->SaveState(node.append_child("item"));
	}
	return true;
}