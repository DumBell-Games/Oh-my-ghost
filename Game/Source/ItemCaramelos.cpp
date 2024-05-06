#include "ItemCaramelos.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Caramelos::Caramelos() : Entity(EntityType::CARAMELOS)
{
	name.Create("caramelos");
}

Caramelos::~Caramelos() {}

bool Caramelos::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	
	//cargar las carater�sticas que tiene el objeto en cuestion (cola) 
	curacion = 10;
	//cargariamos descripci�n desde el documento xml
	descripcion = "Caramelos: Bebida rica con sabor dulce que restaura 10 puntos de salud";
	//cargariamos el tipo de objeto desde el documento xml
	tipo = "Cura";
	//cargar sonido del objeto
	itemFx = app->audio->LoadFx("Assets/Audio/Fx/cola.wav");
	//cargar la textura desde el xml
	texturePath = parameters.attribute("texturePath").as_string();

	return true;
}

bool Caramelos::Start() {

	//initilize textures

	//haz que el ibody se asigne al objeto
	ibody = app->physics->CreateCircle(position.x + 32, position.y + 32, 24, bodyType::STATIC);
	ibody->ctype = ColliderType::CARAMELOS;
	ibody->listener = this;
	texture = app->tex->Load(texturePath);

	
	return true;
}

bool Caramelos::Update(float dt)
{
	app->render->DrawTexture(texture, position.x, position.y);
	if (playerContactC)
	{
		if (app->input->GetButton(ControlID::CONFIRM) == KEY_DOWN)
		{
			app->physics->DestroyBody(ibody);
			app->tex->UnLoad(texture);
			app->entityManager->DestroyEntity(this);
		}
	}

	return true;
}

bool Caramelos::CleanUp()
{
	return true;
}


//on collision with player
void Caramelos::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	//SWITCH CASE PARA LOS DIFERENTES TIPOS DE COLISIONES
	switch (bodyB->ctype)
	{
		case ColliderType::PLAYER:
			//LOG COLA PLAYER
			LOG("COLA PLAYER");
			playerContactC = true;
		break;

	}

}