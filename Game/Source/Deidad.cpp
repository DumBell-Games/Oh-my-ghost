#include "Deidad.h"
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

Deidad::Deidad() : Entity(EntityType::CONSERJE)
{
	name.Create("deidad");
}

Deidad::~Deidad() {

}

bool Deidad::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();

	//animations
	

	return true;
}

bool Deidad::Start() {

	StartAnims();
 
	texture = app->tex->LoadSP(parameters.attribute("texturePath").as_string(), true);
		
	currentAnim = deidadIdle;

	return true;
}

bool Deidad::Update(float dt)
{	
	currentAnim->Update();
	app->render->DrawTexture(texture.get(), position.x, position.y, &currentAnim->GetCurrentFrame());
	

	return true;
}

bool Deidad::CleanUp()
{
	return true;
}

// L07 DONE 6: Define OnCollision function for the player. 
void Deidad::OnCollision(PhysBody* physA, PhysBody* physB) {
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

void Deidad::StartAnims() {
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
		deidadAnims.Add(anim);
	}

	deidadIdle = GetAnimation("deidadIdle");
}
	

Animation* Deidad::GetAnimation(SString name)
{
	for (ListItem<Animation*>* item = deidadAnims.start; item != nullptr; item = item->next)
	{
		if (item->data != nullptr) {
			if (item->data->name == name) return item->data;
		}
	}
	return nullptr;
}