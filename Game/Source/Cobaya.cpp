#include "Cobaya.h"
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
#include "Player.h"


Cobaya::Cobaya() : Entity(EntityType::COBAYA)
{
	name.Create("cobaya");
}

Cobaya::~Cobaya() {

}

bool Cobaya::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();

	//animations
	

	return true;
}

bool Cobaya::Start() {

	CobayaStartAnims();
 
	texture = app->tex->LoadSP(parameters.attribute("texturePath").as_string(), true);
		
	nBody = app->physics->CreateRectangle(position.x + 128, position.y, 140, 200, bodyType::KINEMATIC);
	//haz que el rectangulo no rote
	nBody->body->SetFixedRotation(true);	
	nBody->listener = this;
	nBody->ctype = ColliderType::COBAYA;




	return true;
}

bool Cobaya::Update(float dt)
{	
	cobayaIdle->Update();
	app->render->DrawTexture(texture.get(), position.x - 50, position.y - 120, &cobayaIdle->GetCurrentFrame());


	b2Transform nBodyPos = nBody->body->GetTransform();
	position.x = METERS_TO_PIXELS(nBodyPos.p.x) - 32 / 2;	
	position.y = METERS_TO_PIXELS(nBodyPos.p.y) - 32 / 2;
	
	

	return true;
}

bool Cobaya::CleanUp()
{
	return true;
}

// L07 DONE 6: Define OnCollision function for the player. 
void Cobaya::OnCollision(PhysBody* physA, PhysBody* physB) {
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

void Cobaya::CobayaStartAnims() {
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
			anim->PushBack({ x,y,w,h }, 10);
		}
		cobayaAnims.Add(anim);
	}

	cobayaIdle = GetAnimation("cobayaIdle");
}

Animation* Cobaya::GetAnimation(SString name)
{
	for (ListItem<Animation*>* item = cobayaAnims.start; item != nullptr; item = item->next)
	{
		if (item->data != nullptr) {
			if (item->data->name == name) return item->data;
		}
	}
	return nullptr;
}