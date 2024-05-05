#include "Combat.h"
#include <iostream>
#include <algorithm>
#include "SDL/include/SDL.h"
#include "App.h"
#include "Textures.h"
#include "Window.h"

void Combat::Iniciar() {
	std::cout << "Comença el combat!" << std::endl;

	font = app->render->font;
	renderitzador = app->render->renderer;

	// Inicialitzar el generador de números aleatoris amb una llavor
	std::mt19937 generadorTemp(rd());
	generador = generadorTemp;

	// Definir la distribució (pels 4 atacs)
	std::uniform_int_distribution<int> distribucionTemp(1, 4);
	distribucion = distribucionTemp;

	// Temp -> referencia de textura per parametre	
	text_pika = app->tex->Load("../Output/Assets/Textures/Pika.png");	
	text_dia = app->tex->Load("../Output/Assets/Textures/Dia.png");
	text_background = app->tex->Load("../Output/Assets/Textures/BackgroundCombat2.png");

	rectWin = { 0,0, (int)app->win->width, (int)app->win->height };
	rectPika = { 200,400, 200,200 };
	rectDia = { 600,200, 300,300 };

	while (!CombatFinalitzat()) {
		TornJugador();
		if (CombatFinalitzat()) break;
	}

}

bool Combat::CombatFinalitzat() {
	return equipJugador[0]->salutActual <= 0 || equipOponent[0]->salutActual <= 0;
}

void Combat::TornJugador() {
	bool jugant = true;
	while (jugant) {
		SDL_Event event;



		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				jugant = false;
			}
			else if (event.type == SDL_KEYDOWN) {
				if (b_Idle) {
					// Opcions precombat
					switch (event.key.keysym.sym) {
					case SDLK_1:
						std::cout << "Has premut la tecla 1 - Atacar" << std::endl;
						b_Idle = false;
						b_Atacar = true;
						break;
					case SDLK_2:
						std::cout << "Has premut la tecla 2 - Inventari" << std::endl;
						b_Idle = false;
						b_Inventari = true;
						break;
					case SDLK_3:
						std::cout << "Has premut la tecla 3 - Canviar" << std::endl;
						b_Idle = false;
						b_Canviar = true;
						break;
					case SDLK_4:
						std::cout << "Has premut la tecla 4 - Fugir" << std::endl;
						jugant = false;
						break;
					case SDLK_5:
						std::cout << "Has premut la tecla 5 - Proves filtre red" << std::endl;
						jugant = false;
						break;
					case SDLK_6:
						std::cout << "Has premut la tecla 6 - Proves vida" << std::endl;
						equipJugador[jugadorActiu]->salutActual -= 020;
						app->render->aplicarFiltreVermell(text_pika, &rectPika);
						jugant = false;
						break;
					case SDLK_b:
						std::cout << "---> BACK" << std::endl;
						b_Idle = true;
						b_Atacar = false;
						b_Inventari = false;
						b_Canviar = false;
						break;
					default:
						break;
					}
				}
				else if (b_Atacar) {
					// Selecionar atac
					switch (event.key.keysym.sym) {
					case SDLK_1:
						std::cout << "Ataca1" << std::endl;
						RealitzarAtac(1);
						break;
					case SDLK_2:
						std::cout << "Ataca2" << std::endl;
						RealitzarAtac(2);
						break;
					case SDLK_3:
						std::cout << "Ataca3" << std::endl;
						RealitzarAtac(3);
						break;
					case SDLK_4:
						std::cout << "Ataca4" << std::endl;
						RealitzarAtac(4);
					break; case SDLK_b:
						std::cout << "---> BACK" << std::endl;
						b_Idle = true;
						b_Atacar = false;
						break;
					default:
						break;
					}
				}
				else if (b_Inventari)
				{
					// Selecionar item
					switch (event.key.keysym.sym) {
					case SDLK_1:
						std::cout << "Item1" << std::endl;
						break;
					case SDLK_2:
						std::cout << "Item2" << std::endl;
						break;
					case SDLK_3:
						std::cout << "Item3" << std::endl;
						break;
					case SDLK_4:
						std::cout << "Item4" << std::endl;
					break; case SDLK_b:
						std::cout << "---> BACK" << std::endl;
						b_Idle = true;
						b_Inventari = false;
						break;
					default:
						break;
					}
				}
			}
		}

		NetejarFinestra();

		// Diferents renders

		// Background		
		SDL_RenderCopy(app->render->renderer, text_background, nullptr, &rectWin);

		// Entitats combat		
		SDL_RenderCopy(app->render->renderer, text_pika, nullptr, &rectPika);
		SDL_RenderCopy(app->render->renderer, text_dia, nullptr, &rectDia);

		//Botons
		if (b_Atacar) { RenderitzarBotonsCombat(); }
		else if (b_Inventari) { RenderitzarBotonsInventari(); }
		else if (b_Canviar) { RenderitzarBotonsCanviar(); }
		else { RenderitzarBotonsPreCombat(); }

		// Jugador i enemic
		renderitzarBarraDeVida(posVidaPlayer.x, posVidaPlayer.y, ampladaBarraVida, alcadaBarraVida, equipJugador[jugadorActiu]->salutActual, equipJugador[jugadorActiu]->salutTotal);
		renderitzarBarraDeVida(posVidaEnemic.x, posVidaEnemic.y, ampladaBarraVida, alcadaBarraVida, equipOponent[enemicActiu]->salutActual, equipOponent[enemicActiu]->salutTotal);

		MostrarCanvis();
	}
}

void Combat::RealitzarAtac(int numAtacJugador) {
	// TODO: fer i ajustar la formula de dany



	// Atac enemic (1-4)
	int atacAleatoriEnemic = distribucion(generador);

	if (JugadorEsMesRapid()) {
		// Ataca jugador primer
		if (equipJugador[jugadorActiu]->atacs[numAtacJugador].potencia > equipOponent[enemicActiu]->defensa)
			equipOponent[enemicActiu]->salutActual -=
			equipJugador[jugadorActiu]->atacs[numAtacJugador].potencia - equipOponent[enemicActiu]->defensa;

		// Si enemic esta viu ataca 
		if (equipOponent[enemicActiu]->salutActual > 0) {
			if (equipOponent[enemicActiu]->atacs[atacAleatoriEnemic].potencia > equipJugador[jugadorActiu]->defensa)
				equipJugador[jugadorActiu]->salutActual -=
				equipOponent[enemicActiu]->atacs[atacAleatoriEnemic].potencia - equipJugador[jugadorActiu]->defensa;
		}
	}
	else {
		// Per tant ataca enemic primer				
		if (equipOponent[enemicActiu]->atacs[atacAleatoriEnemic].potencia > equipJugador[jugadorActiu]->defensa)
			equipJugador[jugadorActiu]->salutActual -=
			equipOponent[enemicActiu]->atacs[atacAleatoriEnemic].potencia - equipJugador[jugadorActiu]->defensa;

		if (equipJugador[jugadorActiu]->salutActual > 0) {
			if (equipJugador[jugadorActiu]->atacs[numAtacJugador].potencia > equipOponent[enemicActiu]->defensa)
				equipOponent[enemicActiu]->salutActual -=
				equipJugador[jugadorActiu]->atacs[numAtacJugador].potencia - equipOponent[enemicActiu]->defensa;
		}
	}
}

bool Combat::JugadorEsMesRapid() {
	if (equipJugador[jugadorActiu]->velocitat >= equipOponent[enemicActiu]->velocitat)
		return true;

	return false;
}

bool Combat::QuedenMesEnemics() {
	int sizeEquipOponent = equipOponent.size();



	return true;
}

void Combat::CanviarEnemic() {

}

void Combat::MostrarEstat() {
	//std::cout << "El teu Enemic: " << jugadorActual->nom << " (Salut: " << jugadorActual->salut << ")" << std::endl;
	//std::cout << "Enemic de l'oponent: " << oponentActual->nom << " (Salut: " << oponentActual->salut << ")" << std::endl;
}

void Combat::NetejarFinestra() {
	// Esborra la finestra
	SDL_SetRenderDrawColor(app->render->renderer, 0, 0, 0, 255);
	SDL_RenderClear(app->render->renderer);
}

void Combat::MostrarCanvis() {
	// Mostra els canvis en la finestra
	SDL_RenderPresent(app->render->renderer);
}

void Combat::RenderitzarBotonsPreCombat() {
	SDL_Color color = { 255, 255, 255, 255 }; // Color blanc

	// Dibuixa el botó "Atacar"
	SDL_Rect atacarRect = { 50, 50, 200, 50 };
	SDL_SetRenderDrawColor(renderitzador, 0, 0, 255, 255); // Color blau
	SDL_RenderFillRect(renderitzador, &atacarRect);
	RenderitzarText("Atacar", 80, 65, color);

	// Dibuixa el botó "Inventari"
	SDL_Rect inventariRect = { 50, 150, 200, 50 };
	SDL_SetRenderDrawColor(renderitzador, 0, 0, 255, 255); // Color blau
	SDL_RenderFillRect(renderitzador, &inventariRect);
	RenderitzarText("Inventari", 70, 165, color);

	// Dibuixa el botó "Canviar"
	SDL_Rect canviarRect = { 50, 250, 200, 50 };
	SDL_SetRenderDrawColor(renderitzador, 0, 0, 255, 255); // Color blau
	SDL_RenderFillRect(renderitzador, &canviarRect);
	RenderitzarText("Canviar", 80, 265, color);

	// Dibuixa el botó "Fugir"
	SDL_Rect fugirRect = { 50, 350, 200, 50 };
	SDL_SetRenderDrawColor(renderitzador, 0, 0, 255, 255); // Color blau
	SDL_RenderFillRect(renderitzador, &fugirRect);
	RenderitzarText("Fugir", 90, 365, color);
}

void Combat::RenderitzarBotonsCombat() {
	int posYRecuadre = -50;
	int posYText = -50;

	SDL_Color color = { 255, 255, 255, 255 }; // Color blanc

	for (int i = 0; i < equipJugador[0]->atacs.size(); i++) {
		posYRecuadre += 100;
		posYText += 100;
		SDL_Rect atacarRect = { 50, posYRecuadre, 200, 50 };
		SDL_SetRenderDrawColor(renderitzador, 255, 0, 0, 255); // Color blau
		SDL_RenderFillRect(renderitzador, &atacarRect);
		RenderitzarText(equipJugador[0]->atacs[i].nom, 80, posYText, color);
	}
}

void Combat::RenderitzarBotonsInventari() {
	// Llista items

	SDL_Color color = { 255, 255, 255, 255 }; // Color blanc

	// Dibuixa el botó "Atacar"
	SDL_Rect atacarRect = { 50, 50, 200, 50 };
	SDL_SetRenderDrawColor(renderitzador, 0, 255, 0, 255); // Color verd
	SDL_RenderFillRect(renderitzador, &atacarRect);
	RenderitzarText("Item1", 80, 65, color);

	// Dibuixa el botó "Inventari"
	SDL_Rect inventariRect = { 50, 150, 200, 50 };
	SDL_SetRenderDrawColor(renderitzador, 0, 255, 0, 255); // Color verd
	SDL_RenderFillRect(renderitzador, &inventariRect);
	RenderitzarText("Item2", 70, 165, color);

	// Dibuixa el botó "Canviar"
	SDL_Rect canviarRect = { 50, 250, 200, 50 };
	SDL_SetRenderDrawColor(renderitzador, 0, 255, 0, 255); // Color verd
	SDL_RenderFillRect(renderitzador, &canviarRect);
	RenderitzarText("Item3", 80, 265, color);

	// Dibuixa el botó "Fugir"
	SDL_Rect fugirRect = { 50, 350, 200, 50 };
	SDL_SetRenderDrawColor(renderitzador, 0, 255, 0, 255); // Color verd
	SDL_RenderFillRect(renderitzador, &fugirRect);
	RenderitzarText("Item4", 90, 365, color);
}

void Combat::RenderitzarBotonsCanviar() {
	// Llista de cossos poseits
}

void Combat::RenderitzarText(const std::string& text, int x, int y, SDL_Color color) {
	SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderitzador, surface);
	int texW, texH;
	SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
	SDL_Rect dstRect = { x, y, texW, texH };
	SDL_RenderCopy(renderitzador, texture, NULL, &dstRect);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
}

void Combat::renderitzarBarraDeVida(int x, int y, int amplada, int alcada, float vidaActual, float vidaTotal)
{
	// Calcular el percentatge de vida actual
	float percentatgeVida = (vidaActual / vidaTotal) * 100.0f;

	// Calcular la longitud de la barra de vida en funció del percentatge
	int longitudBarra = (percentatgeVida / 100.0f) * amplada;

	// Definir el color de la barra de vida (verd per a la vida restant i vermell per a la vida perduda)
	SDL_Color colorVidaRestant = { 0, 255, 0, 255 }; // Verd
	SDL_Color colorVidaPerduda = { 255, 0, 0, 255 }; // Vermell

	// Renderitzar la barra de vida
	SDL_Rect rectangleVidaRestant = { x, y, longitudBarra, alcada };
	SDL_SetRenderDrawColor(renderitzador, colorVidaRestant.r, colorVidaRestant.g, colorVidaRestant.b, colorVidaRestant.a);
	SDL_RenderFillRect(renderitzador, &rectangleVidaRestant);

	SDL_Rect rectangleVidaPerduda = { x + longitudBarra, y, amplada - longitudBarra, alcada };
	SDL_SetRenderDrawColor(renderitzador, colorVidaPerduda.r, colorVidaPerduda.g, colorVidaPerduda.b, colorVidaPerduda.a);
	SDL_RenderFillRect(renderitzador, &rectangleVidaPerduda);
}
