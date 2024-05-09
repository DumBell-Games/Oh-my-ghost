#ifndef __ITEMBIRRA_H__
#define __ITEMBIRRA_H__

#include "Inventory.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Entity.h"

struct SDL_Texture;

class Birra : public Entity
{
public:

	Birra();

	virtual ~Birra();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

	void OnEndCollision(PhysBody* bodyA, PhysBody* bodyB);

public:

	bool isPicked = false;

private:

	bool playerContactB;
	SDL_Texture* texture;
	const char* texturePath;
	uint texW, texH;
};

#endif // __ITEMBIRRA_H__