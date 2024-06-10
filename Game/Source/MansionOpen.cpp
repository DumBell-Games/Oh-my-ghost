#include "MansionOpen.h"
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

MansionOpen::MansionOpen() : Entity(EntityType::MANSIONOPEN)
{
	name.Create("mansionOpen");
}

MansionOpen::~MansionOpen() {

}

bool MansionOpen::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	return true;
}

bool MansionOpen::Start() {

	nBody = app->physics->CreateRectangleSensor(position.x + 64, position.y + 128, 108, 207, bodyType::KINEMATIC);
	//haz que el rectangulo no rote
	nBody->listener = this;
	nBody->ctype = ColliderType::MANSIONKEY;


	return true;
}

bool MansionOpen::PreUpdate()
{
	return true;
}

bool MansionOpen::Update(float dt)
{	
	/*if (playerTouched)
	{
		nBody->body->SetTransform(b2Vec2(position.x + 256, position.y), 0);
	}
	*/
	return true;
}

bool MansionOpen::CleanUp()
{
	return true;
}

// L07 DONE 6: Define OnCollision function for the player. 
void MansionOpen::OnCollision(PhysBody* physA, PhysBody* physB) {
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
		break;
	default:
		break;
	}
}

//out collision
void MansionOpen::OutCollision(PhysBody* physA, PhysBody* physB) {
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
		break;
	default:
		break;
	}
}

