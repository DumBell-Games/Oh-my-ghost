#include "ItemBirra.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Birra::Birra() : Entity(EntityType::BIRRA)
{
	name.Create("birra");
}

Birra::~Birra() {}

bool Birra::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	
	//cargar las caraterísticas que tiene el objeto en cuestion (cola) 
	curacion = 10;
	//cargariamos descripción desde el documento xml
	descripcion = "Birra: Bebida rica con sabor dulce que restaura 10 puntos de salud";
	//cargariamos el tipo de objeto desde el documento xml
	tipo = "Cura";
	//cargar sonido del objeto
	itemFx = app->audio->LoadFx("Assets/Audio/Fx/cola.wav");
	//cargar la textura desde el xml
	texturePath = parameters.attribute("texturePath").as_string();

	return true;
}

bool Birra::Start() {

	//initilize textures

	//haz que el ibody se asigne al objeto
	ibody = app->physics->CreateCircle(position.x + 32, position.y + 32, 24, bodyType::STATIC);
	ibody->ctype = ColliderType::BIRRA;
	ibody->listener = this;
	texture = app->tex->Load(texturePath);

	
	return true;
}

bool Birra::Update(float dt)
{
	app->render->DrawTexture(texture, position.x, position.y);


	if (playerContactB)
	{
		if (app->input->GetButton(ControlID::CONFIRM) == KEY_DOWN)
		{
			app->physics->DestroyBody(ibody);
			app->tex->UnLoad(texture);
			app->entityManager->DestroyEntity(this);
			app->scene->BirraPicked();
			app->scene->GetPlayer()->cervezaPicked = true;
		}
	}

	return true;
}

bool Birra::CleanUp()
{
	return true;
}

//on collision with player
void Birra::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	//SWITCH CASE PARA LOS DIFERENTES TIPOS DE COLISIONES
	switch (bodyB->ctype)
	{
		case ColliderType::PLAYER:
			LOG("BIRRA PLAYER");
			playerContactB = true;
		break;

	}

}

void Birra::OnEndCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	//SWITCH CASE PARA LOS DIFERENTES TIPOS DE COLISIONES
	switch (bodyB->ctype)
	{
	case ColliderType::PLAYER:
			LOG("BIRRA OUT PLAYER");
			playerContactB = false;
		break;

	}

}