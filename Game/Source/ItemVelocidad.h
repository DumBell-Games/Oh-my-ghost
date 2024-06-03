#ifndef __ITEMVELOCIDAD_H__
#define __ITEMVELOCIDAD_H__

#include "ItemData.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Entity.h"

struct SDL_Texture;

class Velocidad : public Entity
{
public:

	Velocidad();

	virtual ~Velocidad();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

    void OnEndCollision(PhysBody* bodyA, PhysBody* bodyB);

public:
	int cantidadVelocidad = 0;
	bool isPicked = false;

private:

	bool playerContactV;
	shared_texture_t texture;
	const char* texturePath;
	uint texW, texH;
};

#endif // __ITEMVELOCIDAD_H__