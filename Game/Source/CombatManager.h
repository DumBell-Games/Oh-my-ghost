#pragma once
#include "Module.h"
#include "Enemic.h"

#include "Point.h"
#include "GuiControl.h"

#include "SDL/include/SDL_rect.h"

#include <random>
#include <vector>

class InventoryScreen;
struct SDL_Texture;

enum class CombatState {
	START,
	DIALOG_START,
	MENU,
	COMBAT,
	DIALOG_END,
	END
};

enum class PlayerAction {
	ATTACK,
	ITEM,
	CHANGE,
	NO_ACTION
};

struct CombatData
{
	CombatData() {}

	std::vector<Personatge*> allies;
	int activeAlly = 0;
	Personatge* enemy = nullptr;
};

// Clase de gestion de combate. Para iniciar un combate, rellena los campos de la variable "data"
class CombatManager : public Module
{
public:

	CombatManager(bool startEnabled = false);

	~CombatManager();

	bool Awake(pugi::xml_node config) override;

	bool Start() override;

	bool Update(float dt) override;

	bool PostUpdate() override;

	bool CleanUp() override;

private:

	GuiControl* NewButton(char menuID, char elementID, const char* text, SDL_Rect bounds, GuiCallback_f onClick, SDL_Rect sliderBounds = {0,0,0,0});

	void CreateButtons(pugi::xml_node config);

	void CreateAbilityButtons(Personatge* p);

	void CreateItemButtons(InventoryScreen* inv);

	void CreateTeamSwapButtons();

	bool CombatFinished();

	// Control por mando/teclado (raton va por GUI)

	void HandleStart();

	void HandleStartDialog();

	// Seleccion de accion
	void HandleMenu();

	// Accion elegida
	void HandleCombat();

	void EnemyChoice();

	// Texto de final de combate (dialogo del enemigo, EXP acumulada, etc.)
	void HandleEndDialog();

	// Final de combate, inicio de transicion a mapa (desactivar este modulo y despausar entitymanager)
	void HandleEnd();

	// Realizacion de acciones

	void DoAttack(Personatge* attacker, Personatge* defender, Atac* move);

	void SwapCharacter(int id);

	//Funciones para ser usadas por los botones (menu inicial, las tres primeras abren submenu, la cuarta intenta terminar el combate como accion directa)

	void AttackMenu(GuiControl* ctrl);

	void ItemMenu(GuiControl* ctrl);

	void SwapBody(GuiControl* ctrl);

	void Flee(GuiControl* ctrl);

public:

	// Datos a usar para el combate. Hace falta rellenar este campo antes de activar el modulo
	CombatData data;

private:

	CombatState combatState = CombatState::START;

	std::mt19937 rng;

	// Gestion de menu
	std::vector<std::vector<GuiControl*>*> menuList;
	char currentMenu = 0;
	char currentElement = 0;

	//Menu principal
	std::vector<GuiControl*> bMain;

	//Submenus
	std::vector<GuiControl*> bHabilidades;
	std::vector<GuiControl*> bObjetos;
	std::vector<GuiControl*> bEquipo;

	//Posicion de los menus
	iPoint posMain;
	iPoint posSub;
	iPoint buttonSize;

	// Accion del combate

	PlayerAction accion = PlayerAction::NO_ACTION;

	Atac* ataqueAliado;
	Atac* ataqueEnemigo;



	SDL_Texture* tAliado = nullptr;
	SDL_Texture* tEnemigo = nullptr;

	int turn = 0;

};
