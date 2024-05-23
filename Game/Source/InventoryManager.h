#ifndef __INVENTORYMANAGER_H__
#define __INVENTORYMANAGER_H__

#include "Module.h"
#include "ItemData.h"
#include "List.h"

class InventoryManager : public Module
{
public:

	InventoryManager(bool startEnabled = true);

	// Destructor
	virtual ~InventoryManager();

	// Called before render is available
	bool Awake(pugi::xml_node config);

	// Called after Awake
	bool Start();

	// Called every frame
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	// Additional methods
	ItemData* CreateItem(ItemType type, pugi::xml_node& data);
	
	// Busca un objeto que cumpla con la condicion
	template <typename Comp>
	ItemData* GetItem(Comp condicion);

	// Busca un objeto segun su tipo (devuelve el primero que se encuentre)
	ItemData* GetItemByType(ItemType type);

	void DestroyItem(ItemData* item);

	void AddItem(ItemData* item);
	
	bool LoadState(pugi::xml_node node);

	bool SaveState(pugi::xml_node node);

public:

	List<ItemData*> items;

	bool started = false;
	
};

#endif // __INVENTORYMANAGER_H__
