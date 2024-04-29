#ifndef __ITEMCOLA_H__
#define __ITEMCOLA_H__

#include "Inventory.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class Cola : public Inventory
{
public:

	Cola();
	virtual ~Cola();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

public:

	bool isPicked = false;

private:

	SDL_Texture* texture;
	const char* texturePath;
	uint texW, texH;
};

#endif // __ITEMCOLA_H__