#pragma once

#include "Enemic.h"
#include <vector>
#include "App.h"
#include "Window.h"
#include "Render.h"
#include <random>

struct Posicio
{
	int x;
	int y;
};

// Declaración de la clase Combat
class Combat {

public:
	Combat(std::vector<Personatge*> _equipJugador, std::vector<Personatge*> _equipOponent)
		: equipJugador(_equipJugador), equipOponent(_equipOponent) {
		jugadorActual = equipJugador[0];
		oponentActual = equipOponent[0];
	}

	void Iniciar();

private:
	bool CombatFinalitzat();
	void TornJugador();
	void RealitzarAtac(int numAtacJugador);

	bool JugadorEsMesRapid();
	bool QuedenMesEnemics();
	void CanviarEnemic();
	void MostrarEstat();

	void NetejarFinestra();
	void MostrarCanvis();
	void RenderitzarBotonsPreCombat();
	void RenderitzarBotonsCombat();
	void RenderitzarBotonsInventari();
	void RenderitzarBotonsCanviar();
	void RenderitzarText(const std::string& text, int x, int y, SDL_Color color);
	void renderitzarBarraDeVida(int x, int y, int amplada, int alcada, float vidaActual, float vidaTotal);


private:
	std::random_device rd;
	std::mt19937 generador;
	std::uniform_int_distribution<int> distribucion;

	Personatge* jugadorActual;
	Personatge* oponentActual;
	std::vector<Personatge*> equipJugador;
	std::vector<Personatge*> equipOponent;

	SDL_Window* finestra;
	SDL_Renderer* renderitzador;
	TTF_Font* font;

	// Mini maquina d'estats
	bool b_Idle = true;
	bool b_Atacar = false;
	bool b_Inventari = false;
	bool b_Canviar = false;

	// Personatge/enemic actiu
	int jugadorActiu = 0; // Cos/poder actiu
	int enemicActiu = 0;

	SDL_Texture* text_pika;
	SDL_Texture* text_dia;
	SDL_Texture* text_background;

	Posicio posVidaPlayer{100,100};
	Posicio posVidaEnemic{600,100};

	int ampladaBarraVida = 200;
	int alcadaBarraVida = 20;

	SDL_Rect rectWin;
	SDL_Rect rectPika;
	SDL_Rect rectDia;
};
