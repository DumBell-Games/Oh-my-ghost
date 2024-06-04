#ifndef __MUSICATABERNA_H__
#define __MUSICATABERNA_H__

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

class MusicaTaberna : public Module
{
public:

    MusicaTaberna(bool startEnabled = false);

	// Destructor
	virtual ~MusicaTaberna();

	// Called before the first frame
	bool Start();

	bool CleanUp();

	// Called before all Updates	

	int tabernaMusic;

};

#endif // __MUSICACIELO_H__