#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Box2D/Box2D/Box2D.h"
#include "Animation.h"
#include "Timer.h"

struct SDL_Texture;

class Player : public Entity
{
public:

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	// L07 DONE 6: Define OnCollision function for the player. 
	void OnCollision(PhysBody* physA, PhysBody* physB);

	//out collision
	void OutCollision(PhysBody* physA, PhysBody* physB);

	bool LoadState(pugi::xml_node& node);

	bool SaveState(pugi::xml_node& node);

	void SetPosition(iPoint newPos) override;

	void PlayerStartAnims();

	Animation* GetAnimation(SString name);

	

public:
	
	int ciudadMusic;

	bool canMove = true;

	//L02: DONE 2: Declare player parameters
	float speed = 0.2f;
	shared_texture_t texturePlayer = NULL;
	shared_texture_t textureGhost = NULL;
	shared_texture_t currentTexture = NULL;
	
	uint texW, texH;

	float acceleration = 0.2f;

	//Audio fx
	int pickCoinFxId;

	PhysBody* pBody;
	PhysBody* casinoIn;
	PhysBody* casinoOut;
	PhysBody* tabernaIn;
	PhysBody* tabernaOut;
	PhysBody* arcadeIn;
	PhysBody* arcadeOut;
	PhysBody* cieloOut;
	PhysBody* mansionIn;
	PhysBody* mansionOut;
	PhysBody* despachoIn;
	PhysBody* despachoOut;
	PhysBody* tiendaIn;


    PhysBody* piso2Disco;
    PhysBody* piso3Disco;
    PhysBody* casinoDesbloqueado;
	PhysBody* mansionOpen;
	PhysBody* piso2;
	PhysBody* blockDespacho;
	PhysBody* blockCielo;

	bool casinoIN = false;
	bool casinoOUT = false;
	bool tabernaIN = false;
	bool tabernaOUT = false;
	bool arcadeIN = false;
	bool arcadeOUT = false;
	bool palomaTouched = false;
	bool cieloOUT = false;
	bool mansionIN = false;
	bool mansionOUT = false;
	bool despachoIN = false;
	bool despachoOUT = false;
	bool ciudadIN = false;
	bool ciudadOUT = false;
	bool tiendaIN = false;
	bool tiendaOUT = false;

	bool conserjeTouched = false;
	

	List<Animation*> springyAnimationList;

	Animation* currentAnim;

	Animation* walkFrontal;
	Animation* walkEspalda;
	Animation* walkR;
	Animation* walkL;
	Animation* runR;
	Animation* runL;
	Animation* runFrontal;
	Animation* runEspalda;
	Animation* die;
	Animation* manoDivina;
	Animation* torbellino;
	Animation* cambioCuerpo;
	Animation* idleFrontal;
	Animation* vomito;

	Animation* walkFrontalF;
	Animation* walkEspaldaF;
	Animation* walkRF;
	Animation* walkLF;
	Animation* runRF;
	Animation* runLF;
	Animation* runFrontalF;
	Animation* runEspaldaF;
	Animation* dieF;
	Animation* manoDivinaF;
	Animation* torbellinoF;
	Animation* cambioCuerpoF;
	Animation* idleFrontalF;
	
	bool cervezaPicked = false;
	bool bestiaTouched = false;
	bool aprendizTouched = false;
	bool veteranaTouched = false;
	bool llavePicked = false;

	Timer openTiendaTime;

	int vomitoFx;
	int entradaEdificioFx;
	
};

#endif // __PLAYER_H__