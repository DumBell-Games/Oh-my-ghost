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
	posMain.x = config.attribute("mainX").as_int(0);
	posMain.y = config.attribute("mainY").as_int(0);
	posSub.x = config.attribute("subX").as_int(0);
	posSub.y = config.attribute("subY").as_int(0);
	buttonSize.x = config.attribute("buttonW").as_int(0);
	buttonSize.y = config.attribute("buttonH").as_int(0);

	// CODIGO PARA DEBUG, NO DEFINITIVO

	Personatge* p1 = new Personatge("personatge1", 10, 1, 5, 2);
	p1->atacs.push_back(Atac("Cop de puny1", 10));
	p1->atacs.push_back(Atac("Cop de puny2", 10));
	p1->atacs.push_back(Atac("Cop de puny3", 10));
	p1->atacs.push_back(Atac("Cop de puny4", 10));
	data.allies.push_back(p1);
	Personatge* p2 = new Personatge("personatge2", 5, 3, 2, 1);
	p2->atacs.push_back(Atac("Puntada de peu1", 15));
	p2->atacs.push_back(Atac("Puntada de peu2", 15));
	p2->atacs.push_back(Atac("Puntada de peu3", 15));
	p2->atacs.push_back(Atac("Puntada de peu4", 15));
	data.allies.push_back(p2);

	// FIN CODIGO PARA DEBUG

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
	//No hace falta vaciar menuList ya que es simplemente una agrupaci�n de 4 variables dentro del mismo modulo

	return true;
}

GuiControl* CombatManager::NewButton(char menuID, char elementID, const char* text, SDL_Rect bounds, GuiCallback_f onClick, SDL_Rect sliderBounds)
{
	GuiControlButton* ret = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, elementID, text, bounds, onClick, sliderBounds);
	ret->SetOnHover([this, &menuID, &elementID](GuiControl* g) {currentMenu = menuID; currentElement = elementID; });

	return ret;
}

void CombatManager::CreateButtons(pugi::xml_node config)
{
	//Monta el menu de arriba a abajo
	std::vector<GuiCallback_f> functions;
	functions.push_back([this](GuiControl* g) {AttackMenu(g); });
	functions.push_back([this](GuiControl* g) {ItemMenu(g); });
	functions.push_back([this](GuiControl* g) {SwapBody(g); });
	functions.push_back([this](GuiControl* g) {Flee(g); });

	//MainMenu
	pugi::xml_node menuItem = config.child("main");
	SDL_Rect bounds{0,0,0,0};
	bounds.x = posMain.x;
	bounds.w = buttonSize.x;
	bounds.h = buttonSize.y;
	for (pugi::xml_node buttonItem = menuItem.child("button"); buttonItem != NULL; buttonItem = buttonItem.next_sibling("button"))
	{
		int id = buttonItem.attribute("id").as_int(-1);
		SString text = buttonItem.attribute("text").as_string("CHECK CFG");
		
		bounds.y = posMain.y + id * (bounds.h + 16);
		GuiCallback_f func;
		if (id >= 0 && id < functions.size())
			func = functions[id];
		bMain.push_back(NewButton(0, id, text.GetString(), bounds, func));
	}

	CreateAbilityButtons(data.allies[data.activeAlly]);
	CreateItemButtons(nullptr);

}

void CombatManager::CreateAbilityButtons(Personatge* p)
{
	SDL_Rect bounds{posSub.x,0,buttonSize.x,buttonSize.y};
	int i = 0;
	for (Atac& a : p->atacs)
	{
		bounds.y = posSub.y + i * (bounds.h + 16);
		GuiCallback_f func = [this,&a](GuiControl* g) {
			LOG("WIP habilidad %s",a.nom);
			ataqueAliado = &a;
			combatState = CombatState::COMBAT; // Al seleccionar el boton de ataque se pasa a ejecutar acciones
			};
		// Crea el boton en la posicion del submenu, la posicion 'y' depende del indice de este boton
		bHabilidades.push_back(NewButton(1, i++, a.nom.c_str(), bounds, func));
	}
}

void CombatManager::CreateItemButtons(InventoryScreen* inv)
{
	// Crea los botones para el uso de objetos, el codigo es similar al de los botones de habilidades
	LOG("WIP itemButtons");
}

void CombatManager::CreateTeamSwapButtons()
{
	SDL_Rect bounds{ posSub.x,posSub.y,buttonSize.x,buttonSize.y };
	for (size_t i = 0; i < data.allies.size(); i++)
	{
		Personatge* p = data.allies[i];
		if (p) {
			bounds.y = posSub.y + i * (bounds.h + 16);
			GuiCallback_f func = [this, &i](GuiControl* g) {
				SwapCharacter(i);
				combatState = CombatState::COMBAT;
			};
			bEquipo.push_back(NewButton(3, i, p->nom.c_str(), bounds, func));
		}
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

void CombatManager::HandleStart()
{
	// Activar dialogo de inicio

	// Una vez creado el dialogo, pasa al siguiente estado (para esperar a que se cierre el dialogo)
	combatState = CombatState::DIALOG_START;
}

void CombatManager::HandleStartDialog()
{
	// Una vez terminado el dialogo de inicio, pasa a HandleMenu() asignando el valor correcto a la variable combatState

	if (true)
		combatState = CombatState::MENU;
}

void CombatManager::HandleMenu()
{
	// Control de botones con mando y teclado
	int i = 0, j = 0;
	for (std::vector<GuiControl*>* menu : menuList)
	{
		for (GuiControl* g : *menu)
		{
			if (i == currentMenu && j == currentElement) {

			}
			else
				g->state = GuiControlState::NORMAL;
			j++;
		}
		i++;
	}

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
	data.enemy = nullptr;
	app->fadeToBlack->FadeToBlackTransition(this, (Module*)app->entityManager);
}

void CombatManager::DoAttack(Personatge* attacker, Personatge* defender, Atac* move)
{
	//TODO a�adir precision (posibilidad de fallar el ataque)
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

void CombatManager::SwapCharacter(int id)
{
	if (id >=0 && id < data.allies.size())
		data.activeAlly = id;
}

void CombatManager::AttackMenu(GuiControl* ctrl)
{
}

void CombatManager::ItemMenu(GuiControl* ctrl)
{
}

void CombatManager::SwapBody(GuiControl* ctrl)
{
}

void CombatManager::Flee(GuiControl* ctrl)
{
}