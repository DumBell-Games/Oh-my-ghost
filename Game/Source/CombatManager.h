#pragma once
#include "Module.h"
#include "Enemic.h"

#include "Point.h"
#include "GuiControl.h"
#include "ItemData.h"

#include "Textures.h"

#include "SDL/include/SDL_rect.h"

#include <random>
#include <vector>
#include "AnimationSet.h"

#include "Dialog.h"
#include "DialogManager.h"

class InventoryManager;
struct SDL_Texture;

enum class Menus : char {
	MAIN,
	ATTACK,
	ITEM,
	TEAM,
	MENU_COUNT
};

enum class CombatState {
	START,
	DIALOG_START,
	MENU,
	COMBAT,
	COMBAT_ANIM,
	DIALOG_END,
	END,
	DO_NOTHING
};

enum class PlayerAction {
	ATTACK,
	ITEM,
	CHANGE,
	NO_ACTION
};

enum class MenuDirection
{
	DOWN_RIGHT,
	DOWN_LEFT,
	UP_RIGHT,
	UP_LEFT
};

struct TurnStep
{
	~TurnStep()
	{
		for (Dialog*& item : dialogs)
		{
			RELEASE(item);
		}
		dialogs.clear();
	}

	bool started = false;
	int allyAnimationID = -1;
	int enemyAnimationID = -1;
	std::vector<Dialog*> dialogs;
	bool video = false;
	std::string videoPath = "";
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

	bool PostInit() override;

	bool Awake(pugi::xml_node config) override;

	bool Start() override;

	bool Update(float dt) override;

	bool PostUpdate() override;

	bool CleanUp() override;

	//Inicia un combate con los parametros proporcionados
	void BeginCombat(Personatge* enemy, pugi::xml_node startDialogue, pugi::xml_node endDialogue);

private:

	GuiControl* NewButton(GuiControlType type, char menuID, char elementID, const char* text, SDL_Rect bounds, GuiCallback_f onClick, bool independentPtr = false, SDL_Rect sliderBounds = {0,0,0,0});

	bool LoadLayout(pugi::xml_node layoutRoot);

	void CreateButtons(pugi::xml_node menuListNode);

	void CreateAbilityButtons(Personatge* p);

	void CreateItemButtons(InventoryManager* inv);

	void CreateTeamSwapButtons(pugi::xml_node menuItem);

	bool CombatFinished();

	// Devuelve el balance entre enemigos y aliados derrotados. 0 = empate, 1 = victoria, -1 = derrota
	char CombatResult();


	// Control por mando/teclado (raton va por GUI)

	// Crea los elementos de UI que proporcionan informacion de los personajes en pantalla e inicia el dialogo de inicio de combate
	void HandleStart();

	// Espera a que se termine el dialogo para pasar a la siguiente parte del combate
	void HandleStartDialog();

	// Seleccion de accion
	void HandleMenu();

	// Accion elegida
	void HandleCombat();

	// Genera la acción del enemigo
	void EnemyChoice();

	// Reproduce las animaciones de los ataques ejecutados y vuelve al menu o acaba el combate dependiendo de la condicion de victoria/derrota
	void HandleCombatAnimation();

	// Texto de final de combate (dialogo del enemigo, EXP acumulada, etc.)
	void HandleEndDialog();

	// Final de combate, inicio de transicion a mapa (desactivar este modulo y despausar entitymanager)
	void HandleEnd();


	// Realizacion de acciones

	bool DoAttack(Personatge* attacker, Personatge* defender, Atac* move);

	void UseItem(Personatge* target, ItemData* item);

	void SwapCharacter(int id);

	//Funciones para ser usadas por los botones (menu inicial, las tres primeras abren submenu, la cuarta intenta terminar el combate como accion directa)

	void AttackMenu(GuiControl* ctrl);

	void ItemMenu(GuiControl* ctrl);

	void SwapBody(GuiControl* ctrl);

	void Flee(GuiControl* ctrl);

	void ResetButtonsState(GuiControlState state = GuiControlState::NORMAL);

	// Dialog functions

	void LoadDialogs(pugi::xml_node startDialog, pugi::xml_node endDialog);

	void LoadDialog(List<Dialog*>& list, pugi::xml_node dialogNode);

public:

	// Datos a usar para el combate. Hace falta rellenar este campo antes de activar el modulo
	CombatData data;

private:

	CombatState combatState = CombatState::START;

	std::mt19937 rng;

	// Elementos generales de UI
	std::vector<GuiControl*> guiElements;

	// Gestion de menu
	std::vector<std::vector<GuiControl*>> menuList;
	Menus currentMenu = Menus::MAIN;
	char currentElement = 0;

	//Posicion de los menus
	iPoint posMain;
	iPoint posSub;
	iPoint buttonSize;

	// Dialogos

	List<Dialog*> startDialogue;
	List<Dialog*> endDialogue;

	// Accion del combate

	PlayerAction accion = PlayerAction::NO_ACTION;

	Atac* ataqueAliado = nullptr;
	ItemData* objetoAliado = nullptr;
	int nuevoAliadoActivo = -1;
	Atac* ataqueEnemigo = nullptr;

	std::vector<TurnStep*> turnResults;

	int currentTurnStep = 0;

	// Animation sets and positions (+ defaults if no layout found)

	iPoint playerPos = { 200,1080 };
	iPoint enemyPos = { 1300, 300 };

	AnimationSet* playerAnims = nullptr;
	AnimationSet* enemyAnims = nullptr;

	// Texturas de combate

	shared_texture_t bgTexture = nullptr;
	shared_texture_t atkMenuTexture = nullptr;

	int turn = 0;

	bool fled = false;

	// DEBUG VARS
	Personatge* dummyEnemy = nullptr;


	friend ItemData;
};

