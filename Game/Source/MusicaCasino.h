#ifndef __MUSICACASINO_H__
#define __MUSICACASINO_H__

#include "Module.h"
#include "Player.h"
#include "GuiControl.h"
#include "GuiControlButton.h"
#include "ItemCola.h"
#include "ItemBirra.h"
#include "ItemCaramelos.h"
#include "ItemPatatas.h"
#include "ItemVelocidad.h"
#include "ItemYogur.h"
#include "InventoryManager.h"
#include "ItemData.h"


struct _Mix_Music;
struct Mix_Chunk;


struct SDL_Texture;
class Physbody;

class MusicaCasino : public Module
{
public:

	MusicaCasino(bool startEnabled = false);

	// Destructor
	virtual ~MusicaCasino();

	// Called before the first frame
	bool Start();

	bool CleanUp();

	// Called before all Updates	

	int casinoMusic;

};

#endif // __MUSICACASINO_H__