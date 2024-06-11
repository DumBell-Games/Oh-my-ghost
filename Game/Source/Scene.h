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
#include "TiendaOpen.h"
#include "Map.h"
#include "Tienda.h"
#include "FadeToBlack.h"
#include "Physics.h" 
#include "MusicaCiudad.h"


struct SDL_Texture;
class PhysBody;

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

	int GetChuchesQuantity() {
		ItemData* i = app->inventory->GetItemByType(ItemType::CHUCHES);
		if (i) return i->cantidad;
		else return cantidadChuches;
	}

	int ChuchesPicked() {
		ItemData* i = app->inventory->GetItemByType(ItemType::CHUCHES);
		if (i) i->cantidad++;
		return cantidadChuches++;
	}

	int GetPastelQuantity() {
		ItemData* i = app->inventory->GetItemByType(ItemType::PASTEL);
		if (i) return i->cantidad;
		else return cantidadPastel;
	}

	int PastelPicked() {
		ItemData* i = app->inventory->GetItemByType(ItemType::PASTEL);
		if (i) i->cantidad++;
		return cantidadPastel++;
	}

	int GetHeladoQuantity() {
		ItemData* i = app->inventory->GetItemByType(ItemType::HELADO);
		if (i) return i->cantidad;
		else return cantidadHelado;
	}

	int HeladoPicked() {
		ItemData* i = app->inventory->GetItemByType(ItemType::HELADO);
		if (i) i->cantidad++;
		return cantidadHelado++;
	}
	
	int RestarCincoMonedas() {
		ItemData* i = app->inventory->GetItemByType(ItemType::MONEDAS);
		if (i) return i->cantidad;
		else return cantidadMonedas = cantidadMonedas - 5;
	}

	int RestarDiezMonedas() {
		ItemData* i = app->inventory->GetItemByType(ItemType::MONEDAS);
		if (i) return i->cantidad;
		else return cantidadMonedas =  cantidadMonedas - 10;
	}
	
	int RestarVeinteMonedas() {
		ItemData* i = app->inventory->GetItemByType(ItemType::MONEDAS);
		if (i) return i->cantidad;
		else return cantidadMonedas = cantidadMonedas - 20;
	}

	int BestiaGift() {
		ItemData* i = app->inventory->GetItemByType(ItemType::MONEDAS);
		if (i) return i->cantidad++;
		return cantidadMonedas += 20;
	}

	iPoint Scene::GetPlayerTilePosition() {
		return app->map->WorldToMap(player->position.x, player->position.y);
	}

	void Scene::OpenTienda(){
		app->musicaCiudad->Disable();
		PlayerQuieto();
		app->tienda->Enable();
	}

	void Scene::CloseTienda() {
		app->musicaCiudad->Enable();
		app->fadeToBlack->FadeToBlackTransition((Module*) app->tienda, (Module*) app->scene, 0.0f);
	}

	void Scene::PlayerQuieto() {
		return player->pBody->body->SetType(b2_staticBody);
	}

	void Scene::PlayerDinamico() {
		return player->pBody->body->SetType(b2_dynamicBody);
	}

public:
	int cityFx;
	int ciudadMusic;
	int cieloMusic;
	int cantidadMonedas = 0;
	int cantidadChuches = 0;
	int cantidadPastel = 0;
	int cantidadHelado = 0;


private:
	SDL_Texture* img;

	SDL_Texture* patatasQuestTex;
	
	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;
	SDL_Texture* mouseTileTex = nullptr;

	//L03: DONE 3b: Declare a Player attribute
	TiendaOpen* tiendaOpen;
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