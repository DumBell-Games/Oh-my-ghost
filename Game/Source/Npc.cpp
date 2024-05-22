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


Paloma::Paloma() : Entity(EntityType::PALOMA)
{
	name.Create("paloma");
}

Paloma::~Paloma() {

}

bool Paloma::Awake() {

	//L03: DONE 2: Initialize Player parameters
	Properties p;
	LoadProperties(parameters, p);
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = p.GetProperty("texturePath")->strVal;

	//animations
	

	return true;
}

bool Paloma::Start() {

	PalomaStartAnims();

	texture = app->tex->Load(texturePath.GetString()); 
		
	nBody = app->physics->CreateRectangle(position.x + 128, position.y + 128, 128, 256, bodyType::KINEMATIC);
	//haz que el rectangulo no rote
	nBody->body->SetFixedRotation(true);	
	nBody->listener = this;
	nBody->ctype = ColliderType::NPC;

	return true;
}

bool Paloma::Update()
{			
	app->render->DrawTexture(texture, position.x - 48, position.y - 114, &palomaIdle->GetCurrentFrame());
	
	b2Transform nBodyPos = nBody->body->GetTransform();
	position.x = METERS_TO_PIXELS(nBodyPos.p.x) - 32 / 2;
	position.y = METERS_TO_PIXELS(nBodyPos.p.y) - 32 / 2;

	return true;
}

bool Paloma::CleanUp()
{
	return true;
}

// L07 DONE 6: Define OnCollision function for the player. 
void Paloma::OnCollision(PhysBody* physA, PhysBody* physB) {
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

void Paloma::PalomaStartAnims() {
	for (pugi::xml_node animNode = parameters.child("animation"); animNode; animNode = animNode.next_sibling())
	{
		Animation* anim = new Animation();

		anim->name = animNode.attribute("name").as_string();
		anim->speed = animNode.attribute("speed").as_float();
		anim->loop = animNode.attribute("loop").as_bool();
		anim->pingpong = animNode.attribute("pingpong").as_bool();

		for (pugi::xml_node frameNode = animNode.child("frame"); frameNode; frameNode = frameNode.next_sibling())
		{
			int x = frameNode.attribute("x").as_int();
			int y = frameNode.attribute("y").as_int();
			int w = frameNode.attribute("w").as_int();
			int h = frameNode.attribute("h").as_int();
			anim->PushBack({ x,y,w,h }, 4);
		}
		palomaAnims.Add(anim);
	}

	palomaIdle = GetAnimation("palomaIdle");
}

Animation* Paloma::GetAnimation(SString name)
{
	for (ListItem<Animation*>* item = palomaAnims.start; item != nullptr; item = item->next)
	{
		if (item->data != nullptr) {
			if (item->data->name == name) return item->data;
		}
	}
	return nullptr;
}