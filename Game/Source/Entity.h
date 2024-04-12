#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Point.h"
#include "SString.h"
#include "Input.h"
#include "Render.h"
#include "SDL/include/SDL.h"
#include <memory>

enum class EntityType
{
	PLAYER,
	ITEM,
	DIALOG_TRIGGER,
	UNKNOWN
};

class PhysBody;

class Entity
{
public:

	Entity(EntityType type) : type(type), active(true) {}

	virtual bool Awake()
	{
		return true;
	}

	virtual bool Start()
	{
		return true;
	}

	virtual bool Update(float dt)
	{
		return true;
	}

	virtual bool Render() 
	{
		return true;
	}

	virtual bool CleanUp()
	{
		return true;
	}

	void OnCollision(PhysBody const * bodyA, PhysBody const * bodyB) {}

	virtual bool LoadState(pugi::xml_node&)
	{
		return true;
	}

	virtual bool SaveState(pugi::xml_node&)
	{
		return true;
	}

	void Entity::Enable()
	{
		if (!active)
		{
			active = true;
			Start();
		}
	}

	void Entity::Disable()
	{
		if (active)
		{
			active = false;
			CleanUp();
		}
	}

	virtual void OnCollision(PhysBody* physA, PhysBody* physB) {

	};

	int GetId() const { return id; }

	// TODO actualizar posicion del pbody en caso de que exista
	virtual void SetPosition(iPoint newPos) {
		position = newPos;
	}

	iPoint GetPosition() {
		return position;
	}


public:

	SString name;
	int id;
	EntityType type;
	std::shared_ptr<SDL_Texture> texture;
	std::unique_ptr<PhysBody> pbody;
	bool active = true;
	pugi::xml_node parameters; 

	// Possible properties, it depends on how generic we
	// want our Entity class, maybe it's not renderable...
	iPoint position;       
	bool renderable = true;
};

#endif // __ENTITY_H__