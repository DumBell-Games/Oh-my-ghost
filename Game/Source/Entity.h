#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Point.h"
#include "SString.h"
#include "Input.h"
#include "Render.h"
#include "PropertiesStruct.h"
#include "Textures.h"
#include "EnumUtils.h"

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
	CHUCHES,
	HELADO,
	PASTEL,
	SILVERWINGS,
	APRENDIZ,
	VETERANA,
	CONSERJE,
	DEIDAD,
	ASTROBARK,
	SEGISMUNDA,
	SISEBUTO,
	VAGABUNDO,
	DOLLY,
	OSO,
	MRBESTIA,
	DJBUNNY,
	KIWI,
	COBAYA,
	TIENDAOPEN,
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
	virtual bool LoadState(pugi::xml_node& node)
	{
		node.append_attribute("type").set_value(enum2val(type));
		return true;
	}

	virtual bool SaveState(pugi::xml_node& node)
	{
		node.append_attribute("type").set_value(enum2val(type));
		node.append_attribute("X").set_value(position.x);
		node.append_attribute("Y").set_value(position.y);
		return true;
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