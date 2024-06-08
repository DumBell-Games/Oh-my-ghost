#include "Sisebuto.h"
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

Sisebuto::Sisebuto() : Entity(EntityType::SISEBUTO)
{
	name.Create("sisebuto");
}

Sisebuto::~Sisebuto() {

}

bool Sisebuto::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();

	//animations
	

	return true;
}

bool Sisebuto::Start() {

	StartAnims();
 
	texture = app->tex->LoadSP(parameters.attribute("texturePath").as_string(), true);

	nBody = app->physics->CreateRectangle(position.x + 50, position.y + 50, 243, 154, bodyType::KINEMATIC);
	//haz que el rectangulo no rote
	nBody->body->SetFixedRotation(true);
	nBody->listener = this;
	nBody->ctype = ColliderType::SISEBUTO;

	blockSegismunda = app->physics->CreateRectangle(5643, 1538, 257, 257, bodyType::STATIC);
	blockSegismunda->body->SetFixedRotation(true);
	blockSegismunda->ctype = ColliderType::WALL;

	currentAnim = sisebutoIdle;

	return true;
}

bool Sisebuto::PreUpdate()
{
	return true;
}

bool Sisebuto::Update(float dt)
{	
	currentAnim->Update();
	app->render->DrawTexture(texture.get(), position.x - 120, position.y - 120, &currentAnim->GetCurrentFrame());

	b2Transform nBodyPos = nBody->body->GetTransform();
	position.x = METERS_TO_PIXELS(nBodyPos.p.x) - 32 / 2;
	position.y = METERS_TO_PIXELS(nBodyPos.p.y) - 32 / 2;

	if (playerTouched)
	{
		app->scene->BirraRemoved();
		blockSegismunda->body->SetTransform(b2Vec2_zero, 0.0f);
		playerTouched = false;
	}

	return true;
}

bool Sisebuto::CleanUp()
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
	for (ListItem<Animation*>* item = sisebutoAnims.start; item != nullptr; item = item->next)
	{
		if (item->data != nullptr)
		{
			delete item->data;
			item->data = nullptr;
		}
	}
	sisebutoAnims.Clear();
	return true;
}

// L07 DONE 6: Define OnCollision function for the player. 
void Sisebuto::OnCollision(PhysBody* physA, PhysBody* physB) {
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

void Sisebuto::StartAnims() {
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
		sisebutoAnims.Add(anim);
	}

	sisebutoIdle = GetAnimation("sisebutoIdle");
}
	

Animation* Sisebuto::GetAnimation(SString name)
{
	for (ListItem<Animation*>* item = sisebutoAnims.start; item != nullptr; item = item->next)
	{
		if (item->data != nullptr) {
			if (item->data->name == name) return item->data;
		}
	}
	return nullptr;
}