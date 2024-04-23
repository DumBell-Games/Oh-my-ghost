#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Module.h"
#include "Entity.h"
#include "List.h"

class EntityManager : public Module
{
public:

	EntityManager(bool startEnabled = true);

	// Destructor
	virtual ~EntityManager();

	// Called before render is available
	bool Awake(pugi::xml_node config);

	// Called after Awake
	bool Start();

	// Called every frame
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	// Additional methods
	Entity* CreateEntity(EntityType type, pugi::xml_node& data);

	Entity* CreateFromMap(char name, pugi::xml_node& data);

	void DestroyEntity(Entity* entity);

	void AddEntity(Entity* entity);
	
	bool LoadState(pugi::xml_node node);

	bool SaveState(pugi::xml_node node);

public:

	List<Entity*> entities;

	bool started = false;

};

#endif // __ENTITYMANAGER_H__
