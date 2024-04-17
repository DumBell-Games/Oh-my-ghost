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

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");
}

Player::~Player() {

}

bool Player::Awake() {

	//L03: DONE 2: Initialize Player parameters
	position = iPoint(config.attribute("x").as_int(), config.attribute("y").as_int());

	return true;
}

bool Player::Start() {

	texture = app->tex->Load(config.attribute("texturePath").as_string());

	pBody = app->physics->CreateCircle(position.x + 32, position.y + 32, 16, bodyType::DYNAMIC);
	pBody->listener = this;
	pBody->ctype = ColliderType::PLAYER;

	//initialize audio effect
	pickCoinFxId = app->audio->LoadFx(config.attribute("coinfxpath").as_string());


	return true;
}

bool Player::Update(float dt)
{
	//L03: DONE 4: render the player texture and modify the position of the player using WSAD keys and render the texture

	fPoint joystick = app->input->GetAxis(MOVE_HORIZONTAL, MOVE_VERTICAL);
	KeyState sprint = app->input->GetButton(BACK);
	float speed = (sprint == KEY_REPEAT) ? 0.5f : 0.2f;

	b2Vec2 impulse = b2Vec2_zero;
	b2Vec2 vel = b2Vec2(0, -GRAVITY_Y);

	position.x += speed * joystick.x * dt;
	position.y += speed * joystick.y * dt;

	b2Transform pBodyPos = pBody->body->GetTransform();
	position.x = METERS_TO_PIXELS(pBodyPos.p.x) - 32 / 2;
	position.y = METERS_TO_PIXELS(pBodyPos.p.y) - 32 / 2;
	
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		impulse.x -= acceleration;
		vel = b2Vec2(speed * dt, -GRAVITY_Y);

		if(app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			position.x += -0.3*dt;
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		impulse.x += acceleration;
		vel = b2Vec2(-speed * dt, -GRAVITY_Y);
		if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			position.x += 0.3*dt;
	}

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		impulse.y -= acceleration;
		vel = b2Vec2(0, speed * dt);
		if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			position.y += -0.3 * dt;

	}

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		impulse.y += acceleration;
		vel = b2Vec2(0, -speed * dt);
		if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			position.y += 0.3 * dt;
	}
	pBody->body->ApplyLinearImpulse(impulse, pBody->body->GetPosition(), false);
	pBody->body->SetLinearVelocity(b2Clamp(pBody->body->GetLinearVelocity(), -vel, vel));

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