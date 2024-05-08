#pragma once
#include "Module.h"
#include <vector>
#include "Enemic.h"

class GuiControl;
struct SDL_Texture;

enum class CombatState {
	START,
	DIALOG_START,
	MENU,
	COMBAT,
	DIALOG_END,
	END
};

struct CombatData
{
	CombatData() {}

	std::vector<Personatge*> allies;
	int activeAlly = 0;
	Personatge* enemy = nullptr;
};


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

	void CreateButtons(pugi::xml_node config);

	bool CombatFinished();

	bool TriggerCombat();

	void HandleStart();

	void HandleStartDialog();

	void HandleMenu();

	void HandleCombat();

	void HandleEndDialog();

	void HandleEnd();


public:

	CombatData data;

private:

	CombatState combatState = CombatState::START;

	//Menu principal
	std::vector<GuiControl*> bMain;

	//Submenus
	std::vector<GuiControl*> bHabilidades;
	std::vector<GuiControl*> bObjetos;
	std::vector<GuiControl*> bEquipo;

	SDL_Texture* tAliado = nullptr;
	SDL_Texture* tEnemigo = nullptr;

	int turn = 0;

	//Funciones para ser usadas por los botones

	friend void AttackMenu(GuiControl* ctrl);
};

