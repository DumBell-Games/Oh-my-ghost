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
#include "Map.h"


Npc::Npc() : Entity(EntityType::NPC)
{
	name.Create("Npc");
}

Npc::~Npc() {

}

bool Npc::Awake() {

	//L03: DONE 2: Initialize Player parameters
	Properties p;
	LoadProperties(parameters, p);
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = p.GetProperty("texturePath")->strVal;

	//animations
	

	return true;
}

bool Npc::Start() {

	palomaIdle = app->map->GetAnimByName("Paloma_SpriteSheet");
	palomaIdle->PushBack({7806, 5761, 128, 256}, 4);

	texture = app->tex->Load(texturePath.GetString()); 
		
	nBody = app->physics->CreateRectangle(position.x + 128, position.y + 128, 128, 256, bodyType::KINEMATIC);
	//haz que el rectangulo no rote
	nBody->body->SetFixedRotation(true);	
	nBody->listener = this;
	nBody->ctype = ColliderType::NPC;

	return true;
}

bool Npc::Update(float dt)
{			
	app->render->DrawTexture(palomaIdle->texture, position.x - 48, position.y - 114, &palomaIdle->GetCurrentFrame(), 1.0f, SDL_FLIP_NONE);
	palomaIdle->Update();

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