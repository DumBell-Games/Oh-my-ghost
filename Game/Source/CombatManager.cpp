#include "CombatManager.h"
#include "GuiControl.h"
#include "GuiManager.h"
#include "InventoryScreen.h"
#include "FadeToBlack.h"

#include "Log.h"
#include "App.h"

CombatManager::CombatManager(bool startEnabled) : Module(startEnabled)
{
	name.Create("combat");
	needsAwaking = true;
	menuList.push_back(&bMain);
	menuList.push_back(&bHabilidades);
	menuList.push_back(&bObjetos);
	menuList.push_back(&bEquipo);
}

CombatManager::~CombatManager()
{
}

bool CombatManager::Awake(pugi::xml_node config)
{
	CreateButtons(config.child("buttons"));

	return true;
}

bool CombatManager::Start()
{
	// Se asegura de que el modulo de entidades este pausado para que el jugador no se mueva por el mapa durante el combate
	if (!app->entityManager->paused)
		app->entityManager->Pause();
	currentMenu = 0;
	currentElement = 0;
	LOG("Combat Start!");
	rng = std::mt19937{ std::random_device{}() }; // Resetea el RNG antes de comenzar el combate
	return true;
}

bool CombatManager::Update(float dt)
{
	switch (combatState)
	{
	case CombatState::START:
	{
		HandleStart();
		break;
	}
	case CombatState::DIALOG_START:
	{
		HandleStartDialog();
		break;
	}
	case CombatState::MENU:
	{
		HandleMenu();
		break;
	}
	case CombatState::COMBAT:
	{
		HandleCombat();
		break;
	}
	case CombatState::DIALOG_END:
	{
		HandleEndDialog();
		break;
	}
	case CombatState::END:
	{
		HandleEnd();
		break;
	}
	default:
		break;
	}

	return true;
}

bool CombatManager::PostUpdate()
{

	return true;
}

bool CombatManager::CleanUp()
{
	for (std::vector<GuiControl*>* menu : menuList)
	{
		for (GuiControl* g : *menu)
		{
			app->guiManager->DestroyGuiControl(g);
		}
		menu->clear();
	}
	//No hace falta vaciar menuList ya que es simplemente una agrupación de 4 variables dentro del mismo modulo

	app->fadeToBlack->FadeToBlackTransition(this, (Module*)app->entityManager);
	return true;
}

void CombatManager::CreateButtons(pugi::xml_node config)
{
	//Monta el menu de arriba a abajo
	std::vector<GuiOnClick_f> functions;
	functions.push_back([this](GuiControl* g) {AttackMenu(this, g); });
	functions.push_back([this](GuiControl* g) {ItemMenu(this, g); });
	functions.push_back([this](GuiControl* g) {SwapBody(this, g); });
	functions.push_back([this](GuiControl* g) {Flee(this, g); });

	//MainMenu
	pugi::xml_node menuItem = config.child("main");
	for (pugi::xml_node buttonItem = menuItem.child("button"); buttonItem != NULL; buttonItem = buttonItem.next_sibling("button"))
	{
		int id = buttonItem.attribute("id").as_int(-1);
		SString text = buttonItem.attribute("text").as_string("CHECK CFG");
		SDL_Rect bounds;
		bounds.x = buttonItem.attribute("x").as_int(0);
		bounds.y = buttonItem.attribute("y").as_int(0);
		bounds.w = buttonItem.attribute("w").as_int(128);
		bounds.h = buttonItem.attribute("h").as_int(128);
		GuiOnClick_f func;
		if (id >= 0 && id < functions.size())
			func = functions[id];
		bMain.push_back(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, id, text.GetString(), bounds, func));
	}

}

void CombatManager::CreateAbilityButtons(Personatge* p)
{
	int i = 0;
	for (Atac& a : p->atacs)
	{
		GuiOnClick_f func = [this,&a](GuiControl* g) {
			LOG("WIP habilidad");
			ataqueAliado = &a;
			combatState = CombatState::COMBAT;
			};
		bHabilidades.push_back(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, i++, a.nom.c_str(), {posSub.x,posSub.y,128,32}, func));
	}
}

void CombatManager::CreateItemButtons(InventoryScreen* inv)
{
}

bool CombatManager::CombatFinished()
{
	bool ret = false;
	for (Personatge* p : data.allies)
	{
		ret = ret && p->salutActual <= 0;
	}
	ret = data.enemy->salutActual <= 0;
	return ret;
}

bool CombatManager::TriggerCombat()
{
	return false;
}

void CombatManager::HandleStart()
{
}

void CombatManager::HandleStartDialog()
{
}

void CombatManager::HandleMenu()
{

}

void CombatManager::HandleCombat()
{
	// TODO eleccion de accion enemiga, uso de objetos, etc.
	EnemyChoice();
	Personatge* ally = data.allies[data.activeAlly];
	Personatge* enemy = data.enemy;

	//Check accion aliada, usa objeto o ataca
	switch (accion)
	{
	case PlayerAction::ATTACK:
	{
		if (ataqueAliado) {
			DoAttack(ally, enemy, ataqueAliado);
		}
		ataqueAliado = nullptr;
		break;
	}
	case PlayerAction::ITEM:
	{
		break;
	}
	case PlayerAction::CHANGE:
	{
		break;
	}
	case PlayerAction::NO_ACTION:
	default:
		break;
	}

	// TODO implementar orden de turnos
	if (ataqueEnemigo) {
		DoAttack(enemy, ally, ataqueEnemigo);
	}

	//Check animacion completada, si no se ha terminado no pases de este punto y espera al siguiente frame

	// Accion completada, resetea las variables para nuevo turno
	ataqueAliado = ataqueEnemigo = nullptr;

	if (CombatFinished())
		combatState = CombatState::DIALOG_END;
	else combatState = CombatState::MENU;
}

void CombatManager::EnemyChoice()
{
}

void CombatManager::HandleEndDialog()
{
}

void CombatManager::HandleEnd()
{
}

void CombatManager::DoAttack(Personatge* attacker, Personatge* defender, Atac* move)
{
	//TODO añadir precision (posibilidad de fallar el ataque)
	float attackerDefMult = 1, attackerAtkMult = 1;
	float defenderDefMult = 1, defenderAtkMult = 1;
	bool attackInterrupted = false;
	// Si los estados alterados que tengan el atacante y el atacado afectan al ataque y la defensa respectivamente, se aplican los multiplicadores adecuados. Tambien pueden interrumpir el ataque (paralisis)
	if (attacker->activeStatus)
		attackInterrupted = attacker->activeStatus->ApplyToAttack(attackerDefMult, attackerAtkMult);
	if (defender->activeStatus)
		defender->activeStatus->ApplyToAttack(defenderDefMult, defenderAtkMult);
	if (!attackInterrupted) {
		int finalAtk = ((float)(attacker->atac * move->potencia * attackerAtkMult) / (float)(25 * defender->defensa * defenderDefMult)); // damage = [(atk*pow*atkMultiplier)/(25*def*defMultiplier)]
		defender->rebreDanys(finalAtk);
	}

}

void AttackMenu(GuiControl* ctrl, CombatManager* manager)
{
}

void AttackMenu(CombatManager* manager, GuiControl* ctrl)
{
}

void ItemMenu(CombatManager* manager, GuiControl* ctrl)
{
}

void SwapBody(CombatManager* manager, GuiControl* ctrl)
{
}

void Flee(CombatManager* manager, GuiControl* ctrl)
{
}
