#include "Combat.h"

#include "SDL/include/SDL.h"
#include "External/imgui.h"
#include "External/imgui_impl_sdl2.h"

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

// Funció per inicialitzar ImGui
void initImGui(SDL_Window* window, SDL_Renderer* renderer) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
}

// Funció per tancar ImGui
void closeImGui() {
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

// Funció per renderitzar la interfície gràfica d'ImGui
void renderImGui(SDL_Renderer* renderer) {
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	// Aquí pots crear les teves pròpies finestres i widgets amb ImGui
	ImGui::Begin("Informació del Pokémon");
	ImGui::Text("Pokémon: %s", jugadorActual->nom.c_str());
	ImGui::Text("Salut: %d", jugadorActual->salut);
	// Pots mostrar més informació si ho desitges
	ImGui::End();

	// Altres elements de la interfície gràfica aquí

	ImGui::Render();
	ImGui_ImplSDL2_RenderDrawData(ImGui::GetDrawData());
}




// Enumeració efectes d'estat
enum Estat { NORMAL, VENENO, SANGRADO, ATURDIDO };

class Atac {
public:
	string nom;
	int potencia;

	Atac(string _nom, int _potencia) : nom(_nom), potencia(_potencia) {}
};

class Enemic {
public:
	string nom;
	int nivell;
	int salut;
	int atac;
	int defensa;
	int velocitat;
	Estat estat;
	vector<Atac> atacs; // Llista d'atacs que pot realitzar el Enemic

	Enemic(string _nom, int _nivell, int _atac, int _defensa, int _velocitat)
		: nom(_nom), nivell(_nivell), estat(NORMAL), atac(_atac), defensa(_defensa), velocitat(_velocitat) {
		salut = 100; // Init la salut en 100 per defecte
	}

	void rebreDanys(int danys) {
		salut -= danys;
		if (salut < 0) salut = 0;
	}

	void aplicarEfecteEstat() {

		switch (estat)
		{
		case NORMAL:
			break;
		case VENENO:
			salut -= salut * 0.05; // Aplicar danys per efecte d'estat
			if (salut < 0) salut = 0;
			cout << nom << " pateix per l'efecte d'estat i perd salut." << endl;
			break;
		case SANGRADO:
			salut -= salut * 0.05; // Aplicar danys per efecte d'estat
			if (salut < 0) salut = 0;
			cout << nom << " pateix per l'efecte d'estat i perd salut." << endl;
			break;
		case ATURDIDO:
			salut -= salut * 0.05; // Aplicar danys per efecte d'estat
			if (salut < 0) salut = 0;
			cout << nom << " pateix per l'efecte d'estat i perd salut." << endl;
			break;
		default:
			break;
		}

	}

	void atacar(Enemic& oponent, int indexAtac) {
		if (indexAtac < 0 || indexAtac >= atacs.size()) {
			cout << "Índex d'atac no vàlid." << endl;
			return;
		}

		int danys = (atacs[indexAtac].potencia * atac) / oponent.defensa;

		oponent.rebreDanys(danys);
		cout << nom << " fa servir " << atacs[indexAtac].nom << " i fa " << danys << " punts de danys a " << oponent.nom << endl;
	}

	bool operator<(const Enemic& altre) const {
		return velocitat < altre.velocitat;
	}
};

// Declaració de la classe Combat
class Combat {
private:
	Enemic* jugadorActual;
	Enemic* oponentActual;
	vector<Enemic*> equipJugador;
	vector<Enemic*> equipOponent;

public:
	Combat(vector<Enemic*> _equipJugador, vector<Enemic*> _equipOponent) : equipJugador(_equipJugador), equipOponent(_equipOponent) {
		jugadorActual = equipJugador[0];
		oponentActual = equipOponent[0];
	}

	void iniciar() {
		cout << "Comença el combat!" << endl;
		while (!combatFinalitzat()) {
			ordenarEnemics(); // Ordenem els Enemic segons la velocitat
			tornJugador();
			if (combatFinalitzat()) break;
			tornOponent();
		}
		determinarGuanyador();
	}

	void NouCombat() {



		// Creem Enemic per al jugador
		Enemic bulbasaure("Bulbasaure", 10, 7, 5, 6);
		bulbasaure.atacs.push_back(Atac("Placaje", 10));
		bulbasaure.atacs.push_back(Atac("Latigo cepa", 15));

		Enemic squirtle("Squirtle", 12, 6, 7, 5);
		squirtle.atacs.push_back(Atac("Placaje", 10));
		squirtle.atacs.push_back(Atac("Hidrobomba", 15));

		// Creem Enemic per a l'oponent
		Enemic charmander("Charmander", 8, 6, 5, 7);
		charmander.atacs.push_back(Atac("Placaje", 10));
		charmander.atacs.push_back(Atac("Ascuas", 15));

		Enemic pikachu("Pikachu", 10, 5, 6, 7);
		pikachu.atacs.push_back(Atac("Placaje", 10));
		pikachu.atacs.push_back(Atac("Trueno", 20));

		// Creem els equips de combat
		vector<Enemic*> equipJugador = { &bulbasaure, &squirtle };
		vector<Enemic*> equipOponent = { &charmander, &pikachu };

		// Iniciem el combat
		Combat combat(equipJugador, equipOponent);
		combat.iniciar();

		// Inicialitzem ImGui
		initImGui(window, renderer);

		// Bucle principal del joc
		bool running = true;
		while (running) {
			// Processem esdeveniments de SDL
			SDL_Event event;
			while (SDL_PollEvent(&event)) {
				ImGui_ImplSDL2_ProcessEvent(&event);
				if (event.type == SDL_QUIT) {
					running = false;
				}
			}

			// Resta del bucle del joc (actualització, renderitzat, etc.)

			// Renderitzem la interfície gràfica d'ImGui
			renderImGui(renderer);
		}

		// Resta del codi de tancament de SDL i alliberament de recursos

		// Tanquem ImGui
		closeImGui();
	}

private:
	bool combatFinalitzat() {
		return equipJugador.empty() || equipOponent.empty();
	}

	void ordenarEnemics() {
		sort(equipJugador.begin(), equipJugador.end(), [](Enemic* a, Enemic* b) { return *a < *b; });
		sort(equipOponent.begin(), equipOponent.end(), [](Enemic* a, Enemic* b) { return *a < *b; });
	}

	void tornJugador() {
		cout << "El teu torn." << endl;
		mostrarEstat();
		int opcio;
		cout << "Selecciona una opció:" << endl;
		cout << "1. Atacar" << endl;
		cout << "2. Canviar Enemic" << endl;
		cin >> opcio;
		switch (opcio) {
		case 1:
			realitzarAtac(jugadorActual, oponentActual);
			break;
		case 2:
			canviarEnemic(equipJugador, jugadorActual);
			break;
		default:
			cout << "Opció no vàlida. Perds el teu torn." << endl;
		}
	}

	void tornOponent() {
		cout << "Torn de l'oponent." << endl;
		realitzarAtac(oponentActual, jugadorActual);
	}

	void realitzarAtac(Enemic* atacant, Enemic* defensor) {
		atacant->aplicarEfecteEstat();
		int indexAtac;
		cout << "Selecciona un atac:" << endl;
		for (int i = 0; i < atacant->atacs.size(); ++i) {
			cout << i + 1 << ". " << atacant->atacs[i].nom << endl;
		}
		cin >> indexAtac;
		atacant->atacar(*defensor, indexAtac - 1);
		if (defensor->salut <= 0) {
			cout << defensor->nom << " has sigut derrotat." << endl;
			eliminarEnemic(equipOponent, defensor);
			if (!equipOponent.empty()) {
				oponentActual = equipOponent[0];
				cout << "L'oponent treu a " << oponentActual->nom << endl;
			}
		}
	}

	void canviarEnemic(vector<Enemic*>& equip, Enemic*& actual) {
		int indexEnemic;
		cout << "Selecciona un Enemic:" << endl;
		for (int i = 0; i < equip.size(); ++i) {
			cout << i + 1 << ". " << equip[i]->nom << endl;
		}
		cin >> indexEnemic;
		actual = equip[indexEnemic - 1];
		cout << "Has tret a " << actual->nom << endl;
	}

	void eliminarEnemic(vector<Enemic*>& equip, Enemic* Enemic) {
		equip.erase(remove(equip.begin(), equip.end(), Enemic), equip.end());
	}

	void mostrarEstat() {
		cout << "El teu Enemic: " << jugadorActual->nom << " (Salut: " << jugadorActual->salut << ")" << endl;
		cout << "Enemic de l'oponent: " << oponentActual->nom << " (Salut: " << oponentActual->salut << ")" << endl;
	}

	void determinarGuanyador() {
		if (equipJugador.empty()) {
			cout << "Has perdut el combat." << endl;
		}
		else {
			cout << "Has guanyat el combat!" << endl;
		}
	}
};





using namespace std;

// Resta del codi del combat i definicions de les classes Pokémon, Atac, Combat, etc.

