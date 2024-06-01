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

	//post update
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Additional methods
	Entity* CreateEntity(EntityType type, pugi::xml_node& data);

	void DestroyEntity(Entity* entity);

	void AddEntity(Entity* entity);

	EntityType EntityManager::val2enum(int val) {
		switch (val) {
		case 0: return EntityType::PLAYER;
		case 1: return EntityType::SILVERWINGS;
		case 2: return EntityType::APRENDIZ;
		case 3: return EntityType::VETERANA;
		case 4: return EntityType::CONSERJE;
		case 5: return EntityType::DEIDAD;
		case 6: return EntityType::DJBUNNY;
		case 7: return EntityType::ASTROBARK;
		case 8: return EntityType::SEGISMUNDA;
		case 9: return EntityType::VAGABUNDO;
		case 10: return EntityType::MRBESTIA;
		case 11: return EntityType::SISEBUTO;
		case 12: return EntityType::DOLLY;
		case 13: return EntityType::OSO;
		case 14: return EntityType::KIWI;
		case 15: return EntityType::ENEMY;
		/*case 16: return EntityType::TRANSITION;*/
		case 17: return EntityType::COLA;
		case 18: return EntityType::YOGUR;
		case 19: return EntityType::PATATAS;
		case 20: return EntityType::CARAMELOS;
		case 21: return EntityType::BIRRA;
		case 22: return EntityType::VELOCIDAD;
		case 23: return EntityType::DIALOG_TRIGGER;
		default: return EntityType::UNKNOWN;
		}
	}

	bool LoadState(pugi::xml_node node);

	bool SaveState(pugi::xml_node node);

public:

	List<Entity*> entities;

	bool started = false;

};

#endif // __ENTITYMANAGER_H__
