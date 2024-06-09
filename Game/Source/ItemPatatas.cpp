#include "ItemPatatas.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Patatas::Patatas() : Entity(EntityType::PATATAS)
{
	name.Create("patatas");
}

Patatas::~Patatas() {}

bool Patatas::Awake() {

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

bool Patatas::Start() {

	//initilize textures

	

	//haz que el ibody se asigne al objeto
	ibody = app->physics->CreateCircle(position.x + 32, position.y + 32, 24, bodyType::STATIC);
	ibody->ctype = ColliderType::PATATAS;
	ibody->listener = this;
	texture = app->tex->LoadSP(texturePath, true);

	
	return true;
}

bool Patatas::Update(float dt)
{
	
	app->render->DrawTexture(texture.get(), position.x, position.y);

	if (playerContactP)
	{
		if (app->input->GetButton(ControlID::CONFIRM) == KEY_DOWN)
		{
			app->physics->DestroyBody(ibody);
			app->tex->UnLoadSP(texture);
			app->entityManager->DestroyEntity(this);
			app->scene->PatatasPicked();
		}
	}

	return true;
}

bool Patatas::CleanUp()
{
	// Liberar la textura
	if (texture)
	{
		app->tex->UnLoad(texture.get());
		
	}

	// Liberar el cuerpo físico
	if (ibody)
	{
		app->physics->DestroyBody(ibody);
		ibody = nullptr;
	}
	return true;
}


//on collision with player
void Patatas::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	//SWITCH CASE PARA LOS DIFERENTES TIPOS DE COLISIONES
	switch (bodyB->ctype)
	{
		case ColliderType::PLAYER:
			//LOG COLA PLAYER
			LOG("PATATAS PLAYER");
			playerContactP = true;
		break;

	}

}

void Patatas::OnEndCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	//SWITCH CASE PARA LOS DIFERENTES TIPOS DE COLISIONES
	switch (bodyB->ctype)
	{
	case ColliderType::PLAYER:
		//LOG COLA PLAYER
		LOG("PATATAS PLAYER");
		playerContactP = false;
		break;

	}

}