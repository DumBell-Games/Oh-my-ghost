#include "OsoCarpintero.h"
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

Oso::Oso() : Entity(EntityType::OSO)
{
	name.Create("oso");
}

Oso::~Oso() {

}

bool Oso::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();

	//animations
	

	return true;
}

bool Oso::Start() {

	StartAnims();
 
	texture = app->tex->LoadSP(parameters.attribute("texturePath").as_string(), true);

	nBody = app->physics->CreateRectangle(position.x + 128, position.y + 128, 128, 211, bodyType::KINEMATIC);
	//haz que el rectangulo no rote
	nBody->body->SetFixedRotation(true);
	nBody->listener = this;
	nBody->ctype = ColliderType::NPC;

	currentAnim = osoIdle;

	return true;
}

bool Oso::PreUpdate()
{
	return true;
}

bool Oso::Update(float dt)
{	
	currentAnim->Update();
	app->render->DrawTexture(texture.get(), position.x - 100, position.y - 100, &currentAnim->GetCurrentFrame());

	b2Transform nBodyPos = nBody->body->GetTransform();
	position.x = METERS_TO_PIXELS(nBodyPos.p.x) - 32 / 2;
	position.y = METERS_TO_PIXELS(nBodyPos.p.y) - 32 / 2;

	return true;
}

bool Oso::CleanUp()
{
	return true;
}

// L07 DONE 6: Define OnCollision function for the player. 
void Oso::OnCollision(PhysBody* physA, PhysBody* physB) {
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

void Oso::StartAnims() {
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
        osoAnims.Add(anim);
	}

    osoIdle = GetAnimation("osoIdle");
}
	

Animation* Oso::GetAnimation(SString name)
{
	for (ListItem<Animation*>* item = osoAnims.start; item != nullptr; item = item->next)
	{
		if (item->data != nullptr) {
			if (item->data->name == name) return item->data;
		}
	}
	return nullptr;
}