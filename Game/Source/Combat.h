#pragma once

#include "Enemic.h"
#include <vector>
#include "Window.h"
#include "Render.h"
#include <random>

// Declaración de la clase Combat
class Combat {

public:
	Combat(std::vector<Enemic*> _equipJugador, std::vector<Enemic*> _equipOponent)
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


private:
	std::random_device rd;
	std::mt19937 generador;
	std::uniform_int_distribution<int> distribucion;

	Enemic* jugadorActual;
	Enemic* oponentActual;
	std::vector<Enemic*> equipJugador;
	std::vector<Enemic*> equipOponent;

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
};
