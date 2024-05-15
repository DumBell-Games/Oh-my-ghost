#pragma once
#include "Module.h"
#include "SDL/include/SDL.h"
#include "Window.h"
#include "GuiControlButton.h"
#include "GuiManager.h"
#include "GuiControl.h"
#include "Timer.h"
#include "Enemic.h"

class Character_Menu : public Module
{
public:
	Character_Menu(bool start = true);
	virtual ~Character_Menu();

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	/*int Getnivel() {
		return player.nivell;
	}*/

public:
	int screenWidth;
	int screenHeight;

	SDL_Texture* character_Menu;

	SString nivel;
	SString vida_maxima;
	SString vida_actual;
	SString ataque;
	SString defensa;
	SString velocidad;
	SString mana_maxima;
	SString mana_actual;
	SString experiencia_total;
	SString experiencia_siguiente;

	/*Personatge player;*/

	//Solo de prueba, para nada estara asi para la entrega
	int nive = 4;
	int vidaM = 570;
	int vidaA = 370;
	int ataqu = 40;
	int defens = 60;
	int velocida = 30;
	int manaM = 50;
	int manaA = 40;
	int experienciaT = 48939;
	int experienciaS = 5896;
};