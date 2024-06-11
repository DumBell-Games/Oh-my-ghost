#include "AstroBark.h"
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
#include "CombatManager.h"
#include "Video.h"
#include "MusicaCombateAstroBark.h"
#include "MusicaMansion.h"
#include "CargaAstroBark.h"
#include "Creditos.h"
#include "MusicaCinematicaFinal.h"


Astrobark::Astrobark() : Entity(EntityType::ASTROBARK)
{
	name.Create("djbunny");
}

Astrobark::~Astrobark() {

}

bool Astrobark::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();

	//animations
	

	return true;
}

bool Astrobark::Start() {

	StartAnims();
 
	texture = app->tex->LoadSP(parameters.attribute("texturePath").as_string(), true);

	nBody = app->physics->CreateRectangle(position.x, position.y, 237, 218, bodyType::KINEMATIC);
	//haz que el rectangulo no rote
	nBody->body->SetFixedRotation(true);
	nBody->listener = this;
	nBody->ctype = ColliderType::NPC;

	astroBark = new Personatge("AstroBark", 1, 10, 2, 10, "Assets/Animation/Jefe_Astrobark/Astrobark.xml");
	astroBark->atacs.push_back(Atac("Ladrido", 5, false, "", "Skill1"));
	astroBark->atacs.push_back(Atac("Mordiscazo", 5, false, "", "Skill2"));
	astroBark->atacs.push_back(Atac("Jaur�a", 5, false, "", "Skill3"));
	astroBark->atacs.push_back(Atac("Lamerse las heridas", 5, false, "", "Skill4"));
	astroBark->atacs.push_back(Atac("LA RABIA", 30, true, "", "Assets/Videos/GolpeFinalAB.avi"));
	astroBark->atacs.push_back(Atac("LA RABIA", 30, true, "", "Assets/Videos/GolpeFinalAB.avi"));

	currentAnim = astroIdle;

	return true;
}

bool Astrobark::Update(float dt)
{	
	if (astroBark->salutActual> 0)
	{
		currentAnim->Update();
		app->render->DrawTexture(texture.get(), position.x - 128, position.y - 128, &currentAnim->GetCurrentFrame());

		b2Transform nBodyPos = nBody->body->GetTransform();
		position.x = METERS_TO_PIXELS(nBodyPos.p.x) - 32 / 2;
		position.y = METERS_TO_PIXELS(nBodyPos.p.y) - 32 / 2;
		
		if (playerTouched) {
			app->cargaAB->Enable();
			if (astroBark->salutActual > 0) {
				app->combat->BeginCombat(astroBark, parameters.child("astrobarkCombatIN"), parameters.child("astrobarkCombatEND"), pugi::xml_node(), false);
			}
			app->musicaCombateAB->Enable();
			app->musicaMansion->Disable();
		
			
			playerTouched = false;
		}
	}


	return true;
}

bool Astrobark::PostUpdate()
{

	if (!videoPlayed && astroBark->salutActual <= 0)
	{
		app->musicaCombateAB->Disable();
		app->musicaCFinal->Enable();
		if (!videoTriggered)
		{
			app->video->Initialize("Assets/Videos/OhMyGhost_Cinematica_Context_1.avi");
			videoTriggered = true;
		}
		else
		{
			if (!app->video->isVideoFinished)
			{
				app->video->GrabAVIFrame();
			}
			else
			{
				app->video->CloseAVI();
				videoPlayed = true;
				app->creditos->Enable();
			}
		}
		
	}

	return true;
}

bool Astrobark::CleanUp()
{
	for (ListItem<Animation*>* item = astroAnims.start; item; item = item->next)
	{
		RELEASE(item->data);
	}
	astroAnims.Clear();

	RELEASE(astroBark);
	return true;
}

// L07 DONE 6: Define OnCollision function for the player. 
void Astrobark::OnCollision(PhysBody* physA, PhysBody* physB) {
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

void Astrobark::StartAnims() {
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
		astroAnims.Add(anim);
	}

	astroIdle = GetAnimation("astroIdle");
}
	

Animation* Astrobark::GetAnimation(SString name)
{
	for (ListItem<Animation*>* item = astroAnims.start; item != nullptr; item = item->next)
	{
		if (item->data != nullptr) {
			if (item->data->name == name) return item->data;
		}
	}
	return nullptr;
}