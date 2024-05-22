#include "ItemData.h"

#include "App.h"
#include "Textures.h"

bool ItemData::Init()
{
	name.Create(parameters.attribute("name").as_string("Macguffin usado"));
	descripcion.Create(parameters.attribute("description").as_string("Objeto que no recuerdas por qué tienes en tu inventario. (Objeto bug, reporta al desarrollador a menos de que hayas modificado el archivo de guardado"));
	curacion = parameters.attribute("healing").as_int(0);
	texture = app->tex->Load(parameters.attribute("texturePath").as_string());

	return true;
}
