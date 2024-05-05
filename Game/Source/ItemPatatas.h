#ifndef __ITEMPATATAS_H__
#define __ITEMPATATAS_H__

#include "Inventory.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Entity.h"

struct SDL_Texture;

class Patatas : public Entity
{
public:

	Patatas();

	virtual ~Patatas();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:

	bool isPicked = false;

private:

	void PlayerContact();

private:
	bool playerContactP;
	SDL_Texture* texture;
	const char* texturePath;
	uint texW, texH;
};

#endif // __ITEMPATATAS_H__