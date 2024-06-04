#ifndef __MUSICACIELO_H__
#define __MUSICACIELO_H__

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

class MusicaCielo : public Module
{
public:

	MusicaCielo(bool startEnabled = false);

	// Destructor
	virtual ~MusicaCielo();

	// Called before the first frame
	bool Start();

	bool CleanUp();

	// Called before all Updates	

	int cieloMusic;

};

#endif // __MUSICACIELO_H__