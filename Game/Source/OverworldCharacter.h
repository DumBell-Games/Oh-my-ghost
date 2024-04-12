#pragma once
#include "Entity.h"
#include <functional>

// Clase abstracta para el comportamiento de NPCs
class Behaviour {
	virtual bool Action() = 0;
};

enum NPCState {
	IDLE,
	STATE_COUNT
};

class OverworldCharacter :
    public Entity
{
public:
	virtual bool Awake()
	{
		return true;
	}

	virtual bool Start()
	{
		return true;
	}

	virtual bool Update(float dt)
	{
		return true;
	}

	virtual bool Render()
	{
		return true;
	}

	virtual bool CleanUp()
	{
		return true;
	}

public:


	std::vector<std::shared_ptr<Behaviour>> behaviourmap;

};

