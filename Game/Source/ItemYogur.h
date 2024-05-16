#ifndef __ITEMYOGUR_H__
#define __ITEMYOGUR_H__

#include "ItemData.h"
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

    void OnEndCollision(PhysBody* bodyA, PhysBody* bodyB);

public:
	int cantidadYogur = 0;
	bool isPicked = false;

private:

	void PlayerContact();

private:
	bool playerContactY;
	SDL_Texture* texture;
	SString texturePath;
	uint texW, texH;
};

#endif // __ITEMYOGUR_H__