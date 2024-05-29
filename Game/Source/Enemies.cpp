#include "Enemies.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "CombatManager.h"
#include "Reload.h"

Enemy::Enemy() : Entity(EntityType::ENEMY) , enemyData("PH",0,0,0,0, "Assets/Animation/Aprendiz_Astrobark/Aprendiz.xml")
{
	name.Create("Enemy");
}

Enemy::~Enemy() {

}

bool Enemy::Awake() {

	//L03: DONE 2: Initialize Player parameters
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturePath").as_string();
	

	//Carga de datos de enemigo para combate
	Properties properties;
	LoadProperties(parameters, properties);
	if (properties.list.Count()> 0)
	{
		Properties::Property* p;
		if (p = properties.GetProperty("animPath")) enemyData.animPath = p->strVal.GetString();
	}

	return true;
}

bool Enemy::Start() {

	texture = app->tex->Load(texturePath);

	eBody = app->physics->CreateCircle(position.x + 16, position.y + 16, 16, bodyType::KINEMATIC);
	eBody->listener = this;
	eBody->ctype = ColliderType::ENEMY;

	return true;
}

bool Enemy::Update(float dt)
{
	if (enemyData.salutActual <= 0)
		active = false;

	app->render->DrawTexture(texture, position.x, position.y);

	b2Transform nBodyPos = eBody->body->GetTransform();
	position.x = METERS_TO_PIXELS(nBodyPos.p.x) - 32 / 2;
	position.y = METERS_TO_PIXELS(nBodyPos.p.y) - 32 / 2;

	return true;
}

bool Enemy::CleanUp()
{
	if (app->combat->data.enemy == &enemyData)
		app->combat->data.enemy = nullptr;
	return true;
}

// L07 DONE 6: Define OnCollision function for the player. 
void Enemy::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		break;
	case ColliderType::PLAYER:
	{
		if (enemyData.salutActual > 0 && !app->reload->activePreset) {
			app->combat->data.enemy = &enemyData;
			app->reload->QueueReload("combatStart");
		}
		break;
	}
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	default:
		break;
	}
}