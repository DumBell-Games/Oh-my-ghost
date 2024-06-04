#ifndef __MUSICACIUDAD_H__
#define __MUSICACIUDAD_H__

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


struct SDL_Texture;
class Physbody;

class MusicaCiudad : public Module
{
public:

	MusicaCiudad(bool startEnabled = false);

	// Destructor
	virtual ~MusicaCiudad();

	// Called before the first frame
	bool Start();

	bool CleanUp();

	// Called before all Updates	

	int ciudadMusic;

};

#endif // __MUSICACIUDAD_H__