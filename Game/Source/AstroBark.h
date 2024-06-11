#pragma once

#include "Entity.h"
#include "Point.h"
#include "PropertiesStruct.h"
#include "Animation.h"
#include "Enemic.h"

#include "SDL/include/SDL.h"
#include "Box2D/Box2D/Box2D.h"


struct SDL_Texture;



class Astrobark : public Entity
{
public:

	Astrobark();

	virtual ~Astrobark();

	bool Awake();

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool CleanUp();

	// L07 DONE 6: Define OnCollision function for the player. 
	void OnCollision(PhysBody* physA, PhysBody* physB);

	void StartAnims();

	Animation* GetAnimation(SString name);

public:

	//L02: DONE 2: Declare player parameters
	float speed = 0.2f;
	pugi::xml_node config;
	uint texW, texH;
	SString texturePath;

	PhysBody* nBody;
	
	List<Animation*> astroAnims;
	Animation* currentAnim;
	Animation* astroIdle;

	Personatge* astroBark;

	bool playerTouched = false;


};

