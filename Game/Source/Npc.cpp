#include "Npc.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Npc::Npc() : Entity(EntityType::NPC)
{
	name.Create("Npc");
}

Npc::~Npc() {

}

bool Npc::Awake() {

	//L03: DONE 2: Initialize Player parameters
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturePath").as_string();

	return true;
}

bool Npc::Start() {

	texture = app->tex->Load(texturePath); 
		
	nBody = app->physics->CreateCircle(position.x + 16, position.y + 16, 16, bodyType::KINEMATIC);
	nBody->listener = this;
	nBody->ctype = ColliderType::NPC;

	return true;
}

bool Npc::Update(float dt)
{
			
	app->render->DrawTexture(texture, position.x, position.y);

	b2Transform nBodyPos = nBody->body->GetTransform();
	position.x = METERS_TO_PIXELS(nBodyPos.p.x) - 32 / 2;
	position.y = METERS_TO_PIXELS(nBodyPos.p.y) - 32 / 2;

	return true;
}

bool Npc::CleanUp()
{
	return true;
}

// L07 DONE 6: Define OnCollision function for the player. 
void Npc::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	default:
		break;
	}
}