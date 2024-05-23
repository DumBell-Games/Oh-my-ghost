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

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	// L07 DONE 6: Define OnCollision function for the player. 
	void OnCollision(PhysBody* physA, PhysBody* physB);

	bool LoadState(pugi::xml_node& node);

	bool SaveState(pugi::xml_node& node);

	void SetPosition(iPoint newPos) override;

	void PlayerStartAnims();

	Animation* GetAnimation(SString name);

	

public:

	//L02: DONE 2: Declare player parameters
	float speed = 0.2f;
	SDL_Texture* texturePlayer = NULL;
	SDL_Texture* textureGhost = NULL;
	SDL_Texture* currentTexture = NULL;
	
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

	bool casinoIN = false;
	bool casinoOUT = false;
	bool tabernaIN = false;
	bool tabernaOUT = false;
	bool arcadeIN = false;
	bool arcadeOUT = false;
	bool palomaTouched = false;

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

	Timer timeToIdle;

	bool cervezaPicked = false;
	
};

#endif // __PLAYER_H__