#pragma once

#include "Entity.h"
#include "Point.h"
#include "PropertiesStruct.h"
#include "Animation.h"

#include "SDL/include/SDL.h"
#include "Box2D/Box2D/Box2D.h"

struct SDL_Texture;

class MansionOpen : public Entity
{
public:

	MansionOpen();

	virtual ~MansionOpen();

	bool Awake();

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool CleanUp();

	// L07 DONE 6: Define OnCollision function for the player. 
	void OnCollision(PhysBody* physA, PhysBody* physB);

	//out collision
	void OutCollision(PhysBody* physA, PhysBody* physB);

	void StartAnims();

	Animation* GetAnimation(SString name);

public:

	//L02: DONE 2: Declare player parameters
	float speed = 0.2f;
	pugi::xml_node config;
	uint texW, texH;
	SString texturePath;

	PhysBody* nBody;
	PhysBody* blockDespacho;
	
	List<Animation*> astroAnims;
	Animation* currentAnim;
	Animation* astroIdle;

	bool playerTouched = false;


};
