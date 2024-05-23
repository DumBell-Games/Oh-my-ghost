#ifndef __SCENE_H__
#define __SCENE_H__

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


struct SDL_Texture;
class Physbody;

class Scene : public Module
{
public:

	Scene(bool startEnabled = true);

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake(pugi::xml_node config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Return the player position
	iPoint GetPLayerPosition();

	// Handles multiple Gui Event methods
	bool OnGuiMouseClickEvent(GuiControl* control);

	Player* GetPlayer() {
		return player;
	}

	PhysBody* GetPlayerPbody()
	{
		return player->pBody;
	}

	int GetColaQuantity(){
		return cola.cantidadCola;
	}

	int ColaPicked() {
		return cola.cantidadCola++;
	}

	int YogurPicked() {
		return yogur.cantidadYogur++;
	}

	int GetYogurQuantity(){
		return yogur.cantidadYogur;
	}

	int BirraPicked() {
		return birra.cantidadBirra++;
	}

	int GetBirraQuantity(){
		return birra.cantidadBirra;
	}

	int CaramelosPicked() {
		return caramelos.cantidadCaramelos++;
	}

	int GetCaramelosQuantity(){
		return caramelos.cantidadCaramelos;
	}

	int PatatasPicked() {
		return patatas.cantidadPatatas++;
	}

	int GetPatatasQuantity(){
		return patatas.cantidadPatatas;
	}

	int VelocidadPicked() {
		return velocidad.cantidadVelocidad++;
	}

	int GetVelocidadQuantity(){
		return velocidad.cantidadVelocidad;
	}


public:
	int cityFx;


private:
	SDL_Texture* img;

	SDL_Texture* patatasQuestTex;
	
	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;
	SDL_Texture* mouseTileTex = nullptr;

	//L03: DONE 3b: Declare a Player attribute
	Player* player;
	Cola cola;
	Yogur yogur;
	Patatas patatas;
	Caramelos caramelos;
	Birra birra;
	Velocidad velocidad;


	// L15: TODO 2: Declare a GUI Control Button 
	GuiControlButton* gcButtom;



	bool fullscreen = false;
};

#endif // __SCENE_H__