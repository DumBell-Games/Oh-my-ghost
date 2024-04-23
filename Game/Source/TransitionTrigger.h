#pragma once
#include "Entity.h"
#include "Map.h"

class TransitionTrigger : public Entity
{
public:

	TransitionTrigger();

	virtual ~TransitionTrigger();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	// L07 DONE 6: Define OnCollision function for the player. 
	void OnCollision(PhysBody* physA, PhysBody* physB);

private:
	
	TransitionData transition;
};

