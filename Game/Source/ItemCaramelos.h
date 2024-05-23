#ifndef __ITEMCARAMELOS_H__
#define __ITEMCARAMELOS_H__

#include "Inventory.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Entity.h"

struct SDL_Texture;

class Caramelos : public Entity
{
public:

	Caramelos();

	virtual ~Caramelos();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

	void OnEndCollision(PhysBody* bodyA, PhysBody* bodyB);


public:
	int cantidadCaramelos = 0;

	bool isPicked = false;


private:

	bool playerContactC;
	SDL_Texture* texture;
	const char* texturePath;
	uint texW, texH;
};

#endif // __ITEMCARAMELOS_H__