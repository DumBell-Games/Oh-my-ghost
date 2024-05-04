#ifndef __INVENTORYMANAGER_H__
#define __INVENTORYMANAGER_H__

#include "Module.h"
#include "Inventory.h"
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
	Inventory* CreateItem(EntityType type, pugi::xml_node& data);

	void DestroyItem(Inventory* item);

	void AddItem(Inventory* item);
	
	bool LoadState(pugi::xml_node node);

	bool SaveState(pugi::xml_node node);

public:

	List<Inventory*> items;

	SString inventoryPath;

	bool started = false;
	
};

#endif // __INVENTORYMANAGER_H__
