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
			anim->PushBack({ x,y,w,h }, 4);
		}
		springyAnimationList.Add(anim);
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
	cambioCuerpo = GetAnimation("cambioCuerpo");

	walkRF = GetAnimation("walkRF");
	walkLF = GetAnimation("walkLF");
	walkEspaldaF = GetAnimation("walkEspaldaF");
	walkFrontalF = GetAnimation("walkFrontalF");
	idleFrontalF = GetAnimation("idleFrontalF");
	runRF = GetAnimation("runRF");
	runLF = GetAnimation("runLF");
	runEspaldaF = GetAnimation("runEspaldaF");
	runFrontalF = GetAnimation("runFrontalF");
	cambioCuerpoF = GetAnimation("cambioCuerpoF");

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
	
	currentTexture = textureGhost;
	currentAnim = idleFrontal;

	pBody = app->physics->CreateRectangle(position.x + 128, position.y, 128, 30, bodyType::DYNAMIC);
	casinoIn = app->physics->CreateRectangleSensor(6332 + 128, 754 + 64, 192, 126, bodyType::KINEMATIC);
	casinoIn->ctype = ColliderType::CASINOIN;
	casinoOut = app->physics->CreateRectangleSensor(1793 + 128, 12542 + 64, 256, 64, bodyType::KINEMATIC);
	casinoOut->ctype = ColliderType::CASINOOUT;

	tabernaIn = app->physics->CreateRectangleSensor(4289 + 128, 1588 + 64, 192, 126, bodyType::KINEMATIC);
	tabernaIn->ctype = ColliderType::TABERNAIN;
	tabernaOut = app->physics->CreateRectangleSensor(7939 + 128, 12159 + 64, 256, 128, bodyType::KINEMATIC);
	tabernaOut->ctype = ColliderType::TABERNAOUT;

	arcadeIn = app->physics->CreateRectangleSensor(3009 + 128, 4645 + 64, 192, 126, bodyType::KINEMATIC);
	arcadeIn->ctype = ColliderType::ARCADEIN;
	arcadeOut = app->physics->CreateRectangleSensor(14849 + 128, 9600 + 64, 256, 128, bodyType::KINEMATIC);
	arcadeOut->ctype = ColliderType::ARCADEOUT;



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
	if (currentTexture == texturePlayer && currentAnim != cambioCuerpo && currentAnim != cambioCuerpoF)
	{
		currentAnim = idleFrontal;
	}
	else if (currentTexture == textureGhost && currentAnim != cambioCuerpo && currentAnim != cambioCuerpoF)
	{
		currentAnim = idleFrontalF;
	}

	currentAnim->Update();
	if (currentAnim->HasFinished() && currentAnim == cambioCuerpo && currentTexture == texturePlayer)
	{
		currentTexture = textureGhost;
		currentAnim = idleFrontalF;
		pBody->ctype = ColliderType::GHOST;
	}
	if (currentAnim->HasFinished() && currentAnim == cambioCuerpoF && currentTexture == textureGhost)
	{
		currentTexture = texturePlayer;
		currentAnim = idleFrontal;
		pBody->ctype = ColliderType::PLAYER;
	}

	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN && currentTexture == texturePlayer)
	{
		currentAnim = cambioCuerpo;
		currentAnim->Reset();
		currentAnim->Update();
	
	}
	else if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN && currentTexture == textureGhost)
	{
		currentAnim = cambioCuerpoF;
		currentAnim->Reset();
		currentAnim->Update();
		
	}
	//L03: DONE 4: render the player texture and modify the position of the player using WSAD keys and render the texture
	
	fPoint joystick = app->input->GetAxis(MOVE_HORIZONTAL, MOVE_VERTICAL);
	KeyState sprint = app->input->GetButton(BACK);
	float speed = (sprint == KEY_REPEAT) ? 0.5f : 0.2f;

	if (joystick.x != 0 || joystick.y != 0)
	{
		timeToIdle.Start();
	}

	if (app->input->GetButton(ControlID::UP) == KEY_REPEAT)
	{
		if (currentTexture == texturePlayer)
		{
			currentAnim = walkEspalda;
			currentAnim->Update();
			if (app->input->GetButton(ControlID::BACK) == KEY_REPEAT)
			{
				currentAnim = runEspalda;
				currentAnim->Update();
			}
		}
		if (currentTexture == textureGhost)
		{
			currentAnim = walkEspaldaF;
			currentAnim->Update();
			if (app->input->GetButton(ControlID::BACK) == KEY_REPEAT)
			{
				currentAnim = runEspaldaF;
				currentAnim->Update();
			}
		}
	}

	if (app->input->GetButton(ControlID::DOWN) == KEY_REPEAT)
	{
		if (currentTexture == texturePlayer)
		{
			currentAnim = walkFrontal;
			currentAnim->Update();
			if (app->input->GetButton(ControlID::BACK) == KEY_REPEAT)
			{
				currentAnim = runFrontal;
				currentAnim->Update();
			}
		}
		if (currentTexture == textureGhost)
		{
			currentAnim = walkFrontalF;
			currentAnim->Update();
			if (app->input->GetButton(ControlID::BACK) == KEY_REPEAT)
			{
				currentAnim = runFrontalF;
				currentAnim->Update();
			}
		}
	}

	if (app->input->GetButton(ControlID::LEFT) == KEY_REPEAT)
	{
		if (currentTexture == texturePlayer)
		{
			currentAnim = walkL;
			currentAnim->Update();
			if (app->input->GetButton(ControlID::BACK) == KEY_REPEAT)
			{
				currentAnim = runL;
				currentAnim->Update();
			}
		}
		if (currentTexture == textureGhost)
		{
			currentAnim = walkLF;
			currentAnim->Update();
			if (app->input->GetButton(ControlID::BACK) == KEY_REPEAT)
			{
				currentAnim = runLF;
				currentAnim->Update();
			}
		}
	}

	if (app->input->GetButton(ControlID::RIGHT) == KEY_REPEAT)
	{
		if (currentTexture == texturePlayer)
		{
			currentAnim = walkR;
			currentAnim->Update();
			if (app->input->GetButton(ControlID::BACK) == KEY_REPEAT)
			{
				currentAnim = runR;
				currentAnim->Update();
			}
		}
		if (currentTexture == textureGhost)
		{
			currentAnim = walkRF;
			currentAnim->Update();
			if (app->input->GetButton(ControlID::BACK) == KEY_REPEAT)
			{
				currentAnim = runRF;
				currentAnim->Update();
			}
		}
	}
	
	b2Vec2 impulse = b2Vec2_zero;
	
	impulse.x += speed * joystick.x * dt;
	impulse.y += speed * joystick.y * dt;

	pBody->body->SetLinearVelocity(impulse);
	
	b2Transform pBodyPos = pBody->body->GetTransform();
	
	position.x = METERS_TO_PIXELS(pBodyPos.p.x) - 32 / 2;     
	position.y = METERS_TO_PIXELS(pBodyPos.p.y) - 32 / 2;

	app->render->DrawTexture(currentTexture,position.x - 48 ,position.y - 110, &currentAnim->GetCurrentFrame());

	uint w, h;
	app->win->GetWindowSize(w, h);
	app->render->camera.x = (-position.x * app->win->GetScale()) + w / 2;
	app->render->camera.y = (-position.y * app->win->GetScale()) + h / 2;
	
	if (casinoIN || app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		pBody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(1900), PIXEL_TO_METERS(12488)), NULL);
		casinoIN = false;
	}
	if (casinoOUT)
	{
		pBody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(6420), PIXEL_TO_METERS(374)), NULL);
		casinoOUT = false;
	}
	if (arcadeIN || app->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
	{
		pBody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(14972), PIXEL_TO_METERS(9512)), NULL);
		arcadeIN = false;
	}
	if (arcadeOUT)
	{
		pBody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(3104), PIXEL_TO_METERS(4934)), NULL);
		arcadeOUT = false;
	}
	if (tabernaIN || app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		pBody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(8060), PIXEL_TO_METERS(12104)), NULL);
		tabernaIN = false;
	}
	if (tabernaOUT)
	{
		pBody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(4393), PIXEL_TO_METERS(1878)), NULL);
		tabernaOUT = false;
	}
	
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
	case ColliderType::CASINOIN:
		LOG("Collision CASINOIN");
		casinoIN = true;
		casinoOUT = false;
		break;
	case ColliderType::CASINOOUT:
		LOG("Collision CASINOOUT");
		casinoOUT = true;
		casinoIN = false;
		
		break;
	case ColliderType::ARCADEIN:
		LOG("Collision ARCADEIN");
		arcadeIN = true;
		arcadeOUT = false;
		break;
	case ColliderType::ARCADEOUT:
		LOG("Collision ARCADEOUT");
		arcadeOUT = true;
		arcadeIN = false;
		break;
	case ColliderType::TABERNAIN:
		LOG("Collision TABERNAIN");
		tabernaIN = true;
		tabernaOUT = false;
		break;
	case ColliderType::TABERNAOUT:
		LOG("Collision TABERNAOUT");
		tabernaOUT = true;
		tabernaIN = false;
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
	for (ListItem<Animation*>* item = springyAnimationList.start; item != nullptr; item = item->next)
	{
		if (item->data != nullptr) {
			if (item->data->name == name) return item->data;
		}
	}
	return nullptr;
}

