#include "Player.h"
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
#include "TransitionTrigger.h"

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");
}

Player::~Player() {

}

bool Player::Awake() {

	//L03: DONE 2: Initialize Player parameters
	position = iPoint(parameters.attribute("x").as_int(), parameters.attribute("y").as_int());

	return true;
}

bool Player::Start() {

	texture = app->tex->Load(parameters.attribute("texturePath").as_string());

	pBody = app->physics->CreateCircle(position.x + 32, position.y + 32, 16, bodyType::DYNAMIC);
	pBody->listener = this;
	pBody->ctype = ColliderType::PLAYER;

	//initialize audio effect
	pickCoinFxId = app->audio->LoadFx(parameters.attribute("coinfxpath").as_string());

	
	TransitionData& tData = app->map->transitionData;
	if (tData.targetDoorID >= 0) {
		for (ListItem<Entity*>* item = app->entityManager->entities.start; item; item = item->next)
		{
			if (item->data && item->data->type == EntityType::TRANSITION) {
				TransitionTrigger* tt = (TransitionTrigger*)item->data;
				if (tt->id == tData.targetDoorID) {
					SetPosition(tt->position);
				}
			}
		}
	}

	return true;
}

bool Player::Update(float dt)
{
	//L03: DONE 4: render the player texture and modify the position of the player using WSAD keys and render the texture

	fPoint joystick = app->input->GetAxis(MOVE_HORIZONTAL, MOVE_VERTICAL);
	KeyState sprint = app->input->GetButton(BACK);
	float speed = (sprint == KEY_REPEAT) ? 0.5f : 0.2f;

	b2Vec2 impulse = b2Vec2_zero;
	
	impulse.x += speed * joystick.x * dt;
	impulse.y += speed * joystick.y * dt;

	pBody->body->SetLinearVelocity(impulse);
	
	b2Transform pBodyPos = pBody->body->GetTransform();
	
	position.x = METERS_TO_PIXELS(pBodyPos.p.x) - 32 / 2;     
	position.y = METERS_TO_PIXELS(pBodyPos.p.y) - 32 / 2;

	app->render->DrawTexture(texture,position.x,position.y);

	return true;
}

bool Player::CleanUp()
{
	return true;
}

// L07 DONE 6: Define OnCollision function for the player. 
void Player::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		break;
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		app->audio->PlayFx(pickCoinFxId);
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	default:
		break;
	}
}

bool Player::LoadState(pugi::xml_node& node)
{

	return true;
}

bool Player::SaveState(pugi::xml_node& node)
{
	
	return true;
}

void Player::SetPosition(iPoint newPos)
{
	if (pBody) {
		//pBody->body->SetTransform(b2Vec2(position.x, position.y), pBody->GetRotation()); // TODO descomentar esto una vez resuelto el problema de posicion de los triggers
		position = newPos;
	}
}
