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
	void tornOponent();
	void realitzarAtac(Enemic* atacant, Enemic* defensor);
	void canviarEnemic(std::vector<Enemic*>& equip, Enemic*& actual);
	void eliminarEnemic(std::vector<Enemic*>& equip, Enemic* Enemic);
	void mostrarEstat();
	void determinarGuanyador();
	int Combat::mostrarAtacs(Enemic* enemic);

private:
	Enemic* jugadorActual;
	Enemic* oponentActual;
	std::vector<Enemic*> equipJugador;
	std::vector<Enemic*> equipOponent;

	SDL_Window* finestra;
	SDL_Renderer* renderitzador;
};
