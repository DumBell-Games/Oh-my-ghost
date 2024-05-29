#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Point.h"
#include "SString.h"
#include "Input.h"
#include "Render.h"
#include "PropertiesStruct.h"
#include "Textures.h"

enum class EntityType : char
{
	PLAYER,
	ITEM,
	DIALOG_TRIGGER,
	ENEMY,
	TRANSITION,
	COLA,
	YOGUR,
	BIRRA,
	PATATAS,
	CARAMELOS,
	VELOCIDAD,
	SILVERWINGS,
	APRENDIZ,
	VETERANA,
	CONSERJE,
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

	virtual bool PostUpdate()
	{
		return true;
	}

	virtual bool CleanUp()
	{
		return true;
	}

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

	virtual void OnEndCollision(PhysBody* bodyA, PhysBody* bodyB) {

	};

	virtual void SetPosition(iPoint newPos)
	{
		position = newPos;
	}



public:

	SString name;
	EntityType type;
	bool active = true;
	pugi::xml_node parameters; 
	std::string descripcion;
	std::string tipo;
	int curacion;
	int itemFx;
	PhysBody* ibody;
	shared_texture_t texture;


	// Possible properties, it depends on how generic we
	// want our Entity class, maybe it's not renderable...
	iPoint position;       
	bool renderable = true;
};

#endif // __ENTITY_H__