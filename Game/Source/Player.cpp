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
#include "Window.h"

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");
}

Player::~Player() {

}

void Player::PlayerStartAnims()
{
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
			anim->PushBack({ x,y,w,h });
		}
		springyPalomaAnimatoinList.Add(anim);
	}

	walkR = GetAnimation("walkR");
	walkL = GetAnimation("walkL");
	walkEspalda = GetAnimation("walkEspalda");
	walkFrontal = GetAnimation("walkFrontal");
	idleFrontal = GetAnimation("idleFrontal");
	runR = GetAnimation("runR");
	runL = GetAnimation("runL");
	runEspalda = GetAnimation("runEspalda");
	runFrontal = GetAnimation("runFrontal");


}

bool Player::Awake() {

	//L03: DONE 2: Initialize Player parameters
	position = iPoint(parameters.attribute("x").as_int(), parameters.attribute("y").as_int());

	return true;
}

bool Player::Start() {

	PlayerStartAnims();

	texturePlayer = app->tex->Load(parameters.attribute("texturePath").as_string());
	textureGhost = app->tex->Load(parameters.attribute("ghostTexPath").as_string());
	
	currentTexture = texturePlayer;
	currentAnim = idleFrontal;

	pBody = app->physics->CreateRectangle(position.x + 128, position.y + 128, 128, 256, bodyType::DYNAMIC);
	//haz que el rectangulo no rote
	pBody->body->SetFixedRotation(true);
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
					iPoint targetPos = tt->position;
					targetPos.x += tt->rect.w / 2;
					targetPos.y += tt->rect.h / 2;
					SetPosition(targetPos);
				}
			}
		}
	}

	return true;
}

bool Player::Update(float dt)
{
	currentAnim = idleFrontal;
	currentAnim->Update();

	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN && currentTexture == texturePlayer)
	{
			currentTexture = textureGhost;
			pBody->ctype = ColliderType::GHOST;
	}
	else if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN && currentTexture == textureGhost)
	{
			currentTexture = texturePlayer;
			pBody->ctype = ColliderType::PLAYER;
	}
	//L03: DONE 4: render the player texture and modify the position of the player using WSAD keys and render the texture
	
	fPoint joystick = app->input->GetAxis(MOVE_HORIZONTAL, MOVE_VERTICAL);
	KeyState sprint = app->input->GetButton(BACK);
	float speed = (sprint == KEY_REPEAT) ? 0.5f : 0.2f;

	if (app->input->GetButton(ControlID::UP) == KEY_REPEAT)
	{
		currentAnim = walkEspalda;
		currentAnim->Update();
		if (app->input->GetButton(ControlID::BACK) == KEY_REPEAT)
		{
			currentAnim = runEspalda;
			currentAnim->Update();
		}
	}

	if (app->input->GetButton(ControlID::DOWN) == KEY_REPEAT)
	{
		currentAnim = walkFrontal;
		currentAnim->Update();
		if (app->input->GetButton(ControlID::BACK) == KEY_REPEAT)
		{
			currentAnim = runFrontal;
			currentAnim->Update();
		}
	}

	if (app->input->GetButton(ControlID::LEFT) == KEY_REPEAT)
	{
		currentAnim = walkL;
		currentAnim->Update();
		if (app->input->GetButton(ControlID::BACK) == KEY_REPEAT)
		{
			currentAnim = runL;
			currentAnim->Update();
		}
	}

	if (app->input->GetButton(ControlID::RIGHT) == KEY_REPEAT)
	{
		currentAnim = walkR;
		currentAnim->Update();
		if (app->input->GetButton(ControlID::BACK) == KEY_REPEAT)
		{
			currentAnim = runR;
			currentAnim->Update();
		}
	}
	
	b2Vec2 impulse = b2Vec2_zero;
	
	impulse.x += speed * joystick.x * dt;
	impulse.y += speed * joystick.y * dt;

	pBody->body->SetLinearVelocity(impulse);
	
	b2Transform pBodyPos = pBody->body->GetTransform();
	
	position.x = METERS_TO_PIXELS(pBodyPos.p.x) - 32 / 2;     
	position.y = METERS_TO_PIXELS(pBodyPos.p.y) - 32 / 2;

	app->render->DrawTexture(currentTexture,position.x - 48 ,position.y - 114, &currentAnim->GetCurrentFrame());

	uint w, h;
	app->win->GetWindowSize(w, h);
	app->render->camera.x = (-position.x * app->win->GetScale()) + w / 2;
	app->render->camera.y = (-position.y * app->win->GetScale()) + h / 2;

	
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
		pBody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(newPos.x), PIXEL_TO_METERS(newPos.y)), pBody->GetRotation());
		position = newPos;
	}
}

Animation* Player::GetAnimation(SString name)
{
	for (ListItem<Animation*>* item = springyPalomaAnimatoinList.start; item != nullptr; item = item->next)
	{
		if (item->data != nullptr) {
			if (item->data->name == name) return item->data;
		}
	}
	return nullptr;
}