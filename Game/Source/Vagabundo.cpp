#include "Vagabundo.h"
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

Vagabundo::Vagabundo() : Entity(EntityType::VAGABUNDO)
{
	name.Create("vagabundo");
}

Vagabundo::~Vagabundo() {

}

bool Vagabundo::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();

	//animations
	

	return true;
}

bool Vagabundo::Start() {

	StartAnims();
 
	texture = app->tex->LoadSP(parameters.attribute("texturePath").as_string(), true);

	nBody = app->physics->CreateRectangle(position.x + 50, position.y, 122, 193, bodyType::KINEMATIC);
	//haz que el rectangulo no rote
	nBody->body->SetFixedRotation(true);
	nBody->listener = this;
	nBody->ctype = ColliderType::NPC;

	currentAnim = vagabundoIdle;

	return true;
}

bool Vagabundo::PreUpdate()
{
	return true;
}

bool Vagabundo::Update(float dt)
{	
	currentAnim->Update();
	app->render->DrawTexture(texture.get(), position.x - 50, position.y - 100, &currentAnim->GetCurrentFrame());

	b2Transform nBodyPos = nBody->body->GetTransform();
	position.x = METERS_TO_PIXELS(nBodyPos.p.x) - 32 / 2;
	position.y = METERS_TO_PIXELS(nBodyPos.p.y) - 32 / 2;

	return true;
}

bool Vagabundo::CleanUp()
{
	// Liberar la textura
	if (texture)
	{
		app->tex->UnLoad(texture.get());
		texture.reset();
	}

	// Liberar el cuerpo físico
	if (nBody)
	{
		app->physics->DestroyBody(nBody);
		nBody = nullptr;
	}

	// Liberar animaciones
	for (ListItem<Animation*>* item = vagabundoAnims.start; item != nullptr; item = item->next)
	{
		if (item->data != nullptr)
		{
			delete item->data;
			item->data = nullptr;
		}
	}
	vagabundoAnims.Clear();
	return true;
}

// L07 DONE 6: Define OnCollision function for the player. 
void Vagabundo::OnCollision(PhysBody* physA, PhysBody* physB) {
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

void Vagabundo::StartAnims() {
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
		vagabundoAnims.Add(anim);
	}

	vagabundoIdle = GetAnimation("vagabundoIdle");
}
	

Animation* Vagabundo::GetAnimation(SString name)
{
	for (ListItem<Animation*>* item = vagabundoAnims.start; item != nullptr; item = item->next)
	{
		if (item->data != nullptr) {
			if (item->data->name == name) return item->data;
		}
	}
	return nullptr;
}