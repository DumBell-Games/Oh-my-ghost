#ifndef __ITEMDATA_H__
#define __ITEMDATA_H__

#include "Point.h"
#include "SString.h"
#include "Input.h"
#include "Render.h"
#include "Enemic.h"

class PhysBody;

enum class ItemType : char
{
	COLA,
	YOGUR,
	BIRRA,
	PATATAS,
	CARAMELOS,
	VELOCIDAD,
	UNKNOWN
};

class ItemData
{
public:

	ItemData(ItemType type) : type(type), active(true) {}

	virtual bool Init()
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

	virtual void UseOn(Personatge* p)
	{
		p->salutActual += curacion;
		if (p->salutActual > p->salutTotal) p->salutActual = p->salutTotal;
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
};

#endif // __ITEMDATA_H__