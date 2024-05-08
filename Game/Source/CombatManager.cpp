#include "CombatManager.h"
#include "GuiControl.h"

#include "Log.h"

CombatManager::CombatManager(bool startEnabled) : Module(startEnabled)
{
	name.Create("combat");
	needsAwaking = true;
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

	LOG("Combat Start!");
	srand(time(0));
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
	return true;
}

void CombatManager::CreateButtons(pugi::xml_node config)
{
	for (pugi::xml_node buttonItem = config.child("button"); buttonItem != NULL; buttonItem = buttonItem.next_sibling("button"))
	{

	}
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
}

void CombatManager::HandleEndDialog()
{
}

void CombatManager::HandleEnd()
{
}

void AttackMenu(GuiControl* ctrl)
{
}
