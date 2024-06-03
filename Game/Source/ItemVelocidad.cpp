#include "ItemVelocidad.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Velocidad::Velocidad() : Entity(EntityType::VELOCIDAD)
{
	name.Create("velocidad");
}

Velocidad::~Velocidad() {}

bool Velocidad::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	
	//cargar las caraterísticas que tiene el objeto en cuestion (cola) 
	curacion = 10;
	//cargariamos descripción desde el documento xml
	descripcion = "Velocidad: Bebida rica con sabor dulce que restaura 10 puntos de salud";
	//cargariamos el tipo de objeto desde el documento xml
	tipo = "Cura";
	//cargar sonido del objeto
	itemFx = app->audio->LoadFx("Assets/Audio/Fx/cola.wav");
	//cargar la textura desde el xml
	texturePath = parameters.attribute("texturePath").as_string();

	return true;
}

bool Velocidad::Start() {

	//initilize textures

	//haz que el ibody se asigne al objeto
	ibody = app->physics->CreateCircle(position.x + 32, position.y + 32, 24, bodyType::STATIC);
	ibody->ctype = ColliderType::VELOCIDAD;
	ibody->listener = this;
	texture = app->tex->LoadSP(texturePath, true);

	
	return true;
}

bool Velocidad::Update(float dt)
{
	app->render->DrawTexture(texture.get(), position.x, position.y);

	if (playerContactV)
	{
		if (app->input->GetButton(ControlID::CONFIRM) == KEY_DOWN)
		{
			app->physics->DestroyBody(ibody);
			app->tex->UnLoadSP(texture);
			app->entityManager->DestroyEntity(this);
			app->scene->VelocidadPicked();
		}
	}
	
	return true;
}


bool Velocidad::CleanUp()
{
	return true;
}

//on collision with player
void Velocidad::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	//SWITCH CASE PARA LOS DIFERENTES TIPOS DE COLISIONES
	switch (bodyB->ctype)
	{
		case ColliderType::PLAYER:
			//LOG COLA PLAYER
			LOG("COLA PLAYER");
			playerContactV = true;
		break;

	}

}

void Velocidad::OnEndCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	//SWITCH CASE PARA LOS DIFERENTES TIPOS DE COLISIONES
	switch (bodyB->ctype)
	{
	case ColliderType::PLAYER:
		//LOG COLA PLAYER
		LOG("COLA PLAYER");
		playerContactV = false;
		break;

	}

}