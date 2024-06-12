#include "ItemData.h"

#include "App.h"
#include "Textures.h"

#include "CombatManager.h"

bool ItemData::Init ()
{
	name = parameters.attribute("name").as_string("Macguffin usado");
	descripcion.Create(parameters.attribute("description").as_string("Objeto que no recuerdas por qué tienes en tu inventario. (Objeto bug, reporta al desarrollador a menos de que hayas modificado el archivo de guardado"));
	curacion = parameters.attribute("healing").as_int(0);
	texture = app->tex->Load(parameters.attribute("texturePath").as_string());

	return true;
}

std::string ItemData::UseOn(Personatge* p)
{
	int preHeal = p->salutActual;
	p->salutActual += curacion;
	if (p->salutActual > p->salutTotal) p->salutActual = p->salutTotal;
	cantidad--;
	return p->nom+" se ha curado "+std::to_string(p->salutActual-preHeal);
}
