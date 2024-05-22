#ifndef __ITEMDATA_H__
#define __ITEMDATA_H__

#include "Point.h"
#include "SString.h"
#include "Input.h"
#include "Render.h"
#include "Enemic.h"
#include "EnumUtils.h"

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

	virtual bool Init();

	virtual bool CleanUp()
	{
		return true;
	}

	virtual bool LoadState(pugi::xml_node& node)
	{
		cantidad = node.attribute("amount").as_int(0);
		return true;
	}

	virtual bool SaveState(pugi::xml_node& node)
	{
		node.append_attribute("type").set_value(enum2val(type));
		node.append_attribute("amount").set_value(cantidad);
		return true;
	}

	virtual void UseOn(Personatge* p)
	{
		p->salutActual += curacion;
		if (p->salutActual > p->salutTotal) p->salutActual = p->salutTotal;
		cantidad--;
	}



public:

	bool active = true;
	
	SString name;
	ItemType type;
	pugi::xml_node parameters; 
	SString descripcion;
	int cantidad;
	int curacion;

	SDL_Texture* texture;

	int itemFx;
};

#endif // __ITEMDATA_H__