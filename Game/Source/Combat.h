#pragma once

#include "Enemic.h"
#include <vector>
#include "Window.h"
#include "Render.h"

// Declaración de la clase Combat
class Combat {

public:
	Combat(std::vector<Enemic*> _equipJugador, std::vector<Enemic*> _equipOponent)
		: equipJugador(_equipJugador), equipOponent(_equipOponent) {
		jugadorActual = equipJugador[0];
		oponentActual = equipOponent[0];
	}

	void iniciar();

private:
	bool combatFinalitzat();
	void tornJugador();
	void realitzarAtac(Enemic* atacant, Enemic* defensor);
	void canviarEnemic(std::vector<Enemic*>& equip, Enemic*& actual);
	void eliminarEnemic(std::vector<Enemic*>& equip, Enemic* Enemic);
	void mostrarEstat();
	void determinarGuanyador();
	void renderitzarBotonsPreCombat(SDL_Renderer* renderer, TTF_Font* font);
	void renderitzarBotonsCombat(SDL_Renderer* renderer, TTF_Font* font);
	void renderitzarBotonsInventari(SDL_Renderer* renderer, TTF_Font* font);
	void renderitzarBotonsCanviar(SDL_Renderer* renderer, TTF_Font* font);
	void renderitzarText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int x, int y, SDL_Color color);

private:
	Enemic* jugadorActual;
	Enemic* oponentActual;
	std::vector<Enemic*> equipJugador;
	std::vector<Enemic*> equipOponent;

	SDL_Window* finestra;
	SDL_Renderer* renderitzador;

	// Mini maquina d'estats
	bool b_Idle = true;
	bool b_Atacar = false;
	bool b_Inventari = false;
	bool b_Canviar = false;
};
