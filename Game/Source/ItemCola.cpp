#include "ItemCola.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Cola::Cola() : Inventory(ItemType::COLA)
{
	name.Create("cola");
}

Cola::~Cola() {}

bool Cola::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	
	//cargar las caraterísticas que tiene el objeto en cuestion (cola) 
	curacion = 10;
	//cargariamos descripción desde el documento xml
	descripcion = "Cola: Bebida rica con sabor dulce que restaura 10 puntos de salud";
	//cargariamos el tipo de objeto desde el documento xml
	tipo = "Cura";
	//cargar sonido del objeto
	itemFx = app->audio->LoadFx("Assets/Audio/Fx/cola.wav");
	//cargar la textura desde el xml
	texturePath = parameters.attribute("texturePath").as_string();

	return true;
}

bool Cola::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);

	
	return true;
}

bool Cola::Update(float dt)
{
	app->render->DrawTexture(texture, position.x, position.y);

	return true;
}

bool Cola::CleanUp()
{
	return true;
}