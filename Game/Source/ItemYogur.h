#ifndef __ITEMYOGUR_H__
#define __ITEMYOGUR_H__

#include "Inventory.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Entity.h"

struct SDL_Texture;

class Yogur : public Entity
{
public:

    Yogur();

	virtual ~Yogur();

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
	bool playerContactY;
	SDL_Texture* texture;
	const char* texturePath;
	uint texW, texH;
};

#endif // __ITEMYOGUR_H__