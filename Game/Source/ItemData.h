#ifndef __ITEMDATA_H__
#define __ITEMDATA_H__

#include "Point.h"
#include "SString.h"
#include "Input.h"
#include "Render.h"
#include "Enemic.h"

class PhysBody;

// Limite de objetos actual: 127 (char)
enum class ItemType : char
{
	UNKNOWN = -1,
	COLA,
	YOGUR,
	BIRRA,
	PATATAS,
	CARAMELOS,
	VELOCIDAD,
	TOTAL_ITEMS
};

// Clase generica para objetos. Por defecto es un objeto curativo pero se pueden hacer derivados con otros efectos
class ItemData
{
public:

	ItemData(ItemType type) : type(type), active(true) {}

	virtual ~ItemData() {}

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
	int cantidad;
	int curacion;


	int itemFx;
};

#endif // __ITEMDATA_H__