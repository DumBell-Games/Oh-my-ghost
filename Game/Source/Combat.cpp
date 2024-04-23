#include "Combat.h"
#include <iostream>
#include <algorithm>
#include "SDL/include/SDL.h"
#include "External/imgui.h"
#include "External/imgui_impl_sdl2.h"

void Combat::iniciar() {

	// Inicialitza SDL i crea la finestra i el renderitzador
	finestra = SDL_CreateWindow("Combat", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	renderitzador = SDL_CreateRenderer(finestra, -1, SDL_RENDERER_ACCELERATED);

	// Inicialitza ImGui
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(finestra, nullptr);
	ImGui::StyleColorsDark();

	// Resta de la lògica d'inicialització del combat...
	std::cout << "Comença el combat!" << std::endl;
	while (!combatFinalitzat()) {
		tornJugador();
		if (combatFinalitzat()) break;
		tornOponent();
	}
	determinarGuanyador();

	// Tanca ImGui en finalitzar
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	// Neteja SDL
	SDL_DestroyRenderer(renderitzador);
	SDL_DestroyWindow(finestra);
	SDL_Quit();
}

bool Combat::combatFinalitzat() {
	return equipJugador.empty() || equipOponent.empty();
}

void Combat::tornJugador() {
	// Inicia un nou frame de ImGui
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	// Crea una finestra de ImGui
	ImGui::Begin("Torn del jugador");

	// Mostra opcions al jugador amb botons
	if (ImGui::Button("Atacar")) {
		int indexAtac = mostrarAtacs(jugadorActual);
		realitzarAtac(jugadorActual, oponentActual);
	}
	if (ImGui::Button("Revisar inventari")) {
		// Implementació per revisar inventari
	}
	if (ImGui::Button("Veure equip")) {
		// Implementació per veure equip
	}
	if (ImGui::Button("Intentar escapar")) {
		// Implementació per intentar escapar
	}

	ImGui::End();

	// Renderitza la finestra de ImGui
	ImGui::Render();
	//ImGui_ImplSDL2_RenderDrawData(ImGui::GetDrawData());
}

void Combat::tornOponent() {
	realitzarAtac(oponentActual, jugadorActual);
}

void Combat::realitzarAtac(Enemic* atacant, Enemic* defensor) {
	int indexAtac;
	std::cout << "Selecciona un atac:" << std::endl;
	for (int i = 0; i < atacant->atacs.size(); ++i) {
		std::cout << i + 1 << ". " << atacant->atacs[i].nom << std::endl;
	}
	std::cin >> indexAtac;
	atacant->atacar(*defensor, indexAtac - 1);
	if (defensor->salut <= 0) {
		std::cout << defensor->nom << " ha estat derrotat." << std::endl;
		eliminarEnemic(equipOponent, defensor);
		if (!equipOponent.empty()) {
			oponentActual = equipOponent[0];
			std::cout << "L'oponent saca a " << oponentActual->nom << std::endl;
		}
	}
}

void Combat::canviarEnemic(std::vector<Enemic*>& equip, Enemic*& actual) {
	int indexEnemic;
	std::cout << "Selecciona un Enemic:" << std::endl;
	for (int i = 0; i < equip.size(); ++i) {
		std::cout << i + 1 << ". " << equip[i]->nom << std::endl;
	}
	std::cin >> indexEnemic;
	actual = equip[indexEnemic - 1];
	std::cout << "Has sacat a " << actual->nom << std::endl;
}

void Combat::eliminarEnemic(std::vector<Enemic*>& equip, Enemic* Enemic) {
	equip.erase(std::remove(equip.begin(), equip.end(), Enemic), equip.end());
}

void Combat::mostrarEstat() {
	std::cout << "El teu Enemic: " << jugadorActual->nom << " (Salut: " << jugadorActual->salut << ")" << std::endl;
	std::cout << "Enemic de l'oponent: " << oponentActual->nom << " (Salut: " << oponentActual->salut << ")" << std::endl;
}

void Combat::determinarGuanyador() {
	if (equipJugador.empty()) {
		std::cout << "Has perdut el combat." << std::endl;
	}
	else {
		std::cout << "Has guanyat el combat!" << std::endl;
	}
}

int Combat::mostrarAtacs(Enemic* enemic)
{
	// Crea una nova finestra de ImGui per mostrar els atacs disponibles
	ImGui::Begin("Selecciona un atac");

	int indexAtacSeleccionat = -1;
	// Mostra botons per a cada atac
	for (int i = 0; i < enemic->atacs.size(); ++i) {
		if (ImGui::Button(enemic->atacs[i].nom.c_str())) {
			indexAtacSeleccionat = i;
		}
	}

	ImGui::End();

	return indexAtacSeleccionat;
}
