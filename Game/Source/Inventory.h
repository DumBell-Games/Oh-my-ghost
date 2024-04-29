#ifndef __INVENTORY_H__
#define __INVENTORY_H__

#include "Point.h"
#include "SString.h"
#include "Input.h"
#include "Render.h"

enum class ItemType : char
{
	COLA,
	UNKNOWN
};

class PhysBody;

class Inventory
{
public:

	Inventory(ItemType type) : type(type), active(true) {}

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

	void Inventory::Enable()
	{
		if (!active)
		{
			active = true;
			Start();
		}
	}

	void Inventory::Disable()
	{
		if (active)
		{
			active = false;
			CleanUp();
		}
	}

	virtual void OnCollision(PhysBody* physA, PhysBody* physB) {

	};

	virtual void SetPosition(iPoint newPos)
	{
		position = newPos;
	}



public:

	SString name;
	ItemType type;
	bool active = true;
	pugi::xml_node parameters; 
	std::string descripcion;
	std::string tipo;
	int curacion;
	int itemFx;

	// Possible properties, it depends on how generic we
	// want our Entity class, maybe it's not renderable...
	iPoint position;       
	bool renderable = true;
};

#endif // __INVENTORY_H__