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
#include "InventoryManager.h"
#include "ItemData.h"
#include "Map.h"


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
		ItemData* i = app->inventory->GetItemByType(ItemType::COLA);
		if (i)	return i->cantidad;
		else	return cola.cantidadCola;
	}

	int ColaPicked() {
		ItemData* i = app->inventory->GetItemByType(ItemType::COLA);
		if (i) i->cantidad++;
		cola.cantidadCola++;
		return GetColaQuantity();
	}

	int YogurPicked() {
		ItemData* i = app->inventory->GetItemByType(ItemType::YOGUR);
		if (i) i->cantidad++;
		return yogur.cantidadYogur++;
	}

	int GetYogurQuantity(){
		ItemData* i = app->inventory->GetItemByType(ItemType::YOGUR);
		if (i)	return i->cantidad;
		else	return yogur.cantidadYogur;
	}

	int BirraPicked() {
		ItemData* i = app->inventory->GetItemByType(ItemType::BIRRA);
		if (i) i->cantidad++;
		return birra.cantidadBirra++;
	}
		
	int BirraRemoved() {
		ItemData* i = app->inventory->GetItemByType(ItemType::BIRRA);
		if (i && i->cantidad > 0) i->cantidad--;
		return birra.cantidadBirra > 0 ? birra.cantidadBirra-- : 0;
	}                      //condicion ? si es verdadero : si es falso


	int GetBirraQuantity(){
		ItemData* i = app->inventory->GetItemByType(ItemType::BIRRA);
		if (i)	return i->cantidad;
		else	return birra.cantidadBirra;
	}

	int CaramelosPicked() {
		ItemData* i = app->inventory->GetItemByType(ItemType::CARAMELOS);
		if (i) i->cantidad++;
		return caramelos.cantidadCaramelos++;
	}

	int GetCaramelosQuantity(){
		ItemData* i = app->inventory->GetItemByType(ItemType::CARAMELOS);
		if (i)	return i->cantidad;
		else	return caramelos.cantidadCaramelos;
	}

	int PatatasPicked() {
		ItemData* i = app->inventory->GetItemByType(ItemType::PATATAS);
		if (i) i->cantidad++;
		return patatas.cantidadPatatas++;
	}

	int GetPatatasQuantity(){
		ItemData* i = app->inventory->GetItemByType(ItemType::PATATAS);
		if (i)	return i->cantidad;
		else	return patatas.cantidadPatatas;
	}

	int VelocidadPicked() {
		ItemData* i = app->inventory->GetItemByType(ItemType::VELOCIDAD);
		if (i) i->cantidad++;
		return velocidad.cantidadVelocidad++;
	}

	int GetVelocidadQuantity(){
		ItemData* i = app->inventory->GetItemByType(ItemType::VELOCIDAD);
		if (i)	return i->cantidad;
		else	return velocidad.cantidadVelocidad;
	}

	int GetMonedasQuantity() {
		ItemData* i = app->inventory->GetItemByType(ItemType::MONEDAS);
		if (i) return i->cantidad;
		else return cantidadMonedas;
	}

	int BestiaGift() {
		ItemData* i = app->inventory->GetItemByType(ItemType::MONEDAS);
		if (i) return i->cantidad++;
		return cantidadMonedas += 20;
	}

	iPoint Scene::GetPlayerTilePosition() {
		return app->map->WorldToMap(player->position.x, player->position.y);
	}

public:
	int cityFx;
	int ciudadMusic;
	int cieloMusic;
	int cantidadMonedas = 0;


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

	const char* currentMusic;
	
	const char* tabernaMusic;
	const char* mansionMusic;
	const char* arcadeMusic;
	

	bool fullscreen = false;
};

#endif // __SCENE_H__