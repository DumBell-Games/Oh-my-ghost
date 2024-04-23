#include "Combat.h"
#include <iostream>
#include <algorithm>
#include "SDL/include/SDL.h"
#include "App.h"

void Combat::iniciar() {	
	std::cout << "Comença el combat!" << std::endl;
	while (!combatFinalitzat()) {
		tornJugador();
		if (combatFinalitzat()) break;
	}
	determinarGuanyador();
}

bool Combat::combatFinalitzat() {
	return equipJugador.empty() || equipOponent.empty();
}

void Combat::tornJugador() {
	bool jugant = true;
	while (jugant) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				jugant = false;
			}
		}

		// Esborra la finestra
		SDL_SetRenderDrawColor(app->render->renderer, 0, 0, 0, 255);
		SDL_RenderClear(app->render->renderer);

		// Renderitza els botons
		renderitzarBotons(app->render->renderer, app->render->font);

		// Mostra els canvis en la finestra
		SDL_RenderPresent(app->render->renderer);
	}
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
	return 0;
}

void Combat::renderitzarBotons(SDL_Renderer* renderer, TTF_Font* font)
{
	SDL_Color color = { 255, 255, 255, 255 }; // Color blanc

	// Dibuixa el botó "Atacar"
	SDL_Rect atacarRect = { 50, 50, 200, 50 };
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Color blau
	SDL_RenderFillRect(renderer, &atacarRect);
	renderitzarText(renderer, font, "Atacar", 80, 65, color);

	// Dibuixa el botó "Inventari"
	SDL_Rect inventariRect = { 50, 150, 200, 50 };
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Color blau
	SDL_RenderFillRect(renderer, &inventariRect);
	renderitzarText(renderer, font, "Inventari", 70, 165, color);

	// Dibuixa el botó "Canviar"
	SDL_Rect canviarRect = { 50, 250, 200, 50 };
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Color blau
	SDL_RenderFillRect(renderer, &canviarRect);
	renderitzarText(renderer, font, "Canviar", 80, 265, color);

	// Dibuixa el botó "Fugir"
	SDL_Rect fugirRect = { 50, 350, 200, 50 };
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Color blau
	SDL_RenderFillRect(renderer, &fugirRect);
	renderitzarText(renderer, font, "Fugir", 90, 365, color);
}

void Combat::renderitzarText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int x, int y, SDL_Color color)
{
	SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	int texW, texH;
	SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
	SDL_Rect dstRect = { x, y, texW, texH };
	SDL_RenderCopy(renderer, texture, NULL, &dstRect);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
}
