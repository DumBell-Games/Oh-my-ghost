#include "TiendaOpen.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Map.h"
#include "Tienda.h"
#include "Entity.h"
#include "EntityManager.h"

TiendaOpen::TiendaOpen() : Entity(EntityType::TIENDAOPEN)
{
	name.Create("tiendaOpen");
}

TiendaOpen::~TiendaOpen() {

}

bool TiendaOpen::Awake() {

	return true;
}

bool TiendaOpen::Start() {

	nBody = app->physics->CreateRectangle(8450 + 128, 1566, 485, 126, bodyType::KINEMATIC);
	//haz que el rectangulo no rote
	nBody->body->SetFixedRotation(true);
	nBody->ctype = ColliderType::TIENDAOPEN;

	return true;
}

bool TiendaOpen::PreUpdate()
{
	return true;
}

bool TiendaOpen::Update(float dt)
{	
	if (playerTouched)
	{
		app->tienda->Enable();
		app->entityManager->Pause();
		playerTouched = false;
	}

	return true;
}

bool TiendaOpen::CleanUp()
{
	return true;
}

// L07 DONE 6: Define OnCollision function for the player. 
void TiendaOpen::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	case ColliderType::PLAYER:
		LOG("Collision PLAYER");
		playerTouched = true;
		break;
	default:
		break;
	}
}

