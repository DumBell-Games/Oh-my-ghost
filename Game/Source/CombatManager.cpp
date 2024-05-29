#include "CombatManager.h"
#include "GuiControl.h"
#include "GuiManager.h"
#include "InventoryManager.h"
#include "FadeToBlack.h"
#include "App.h"
#include "DebugConsole.h"
#include "Reload.h"

#include "Log.h"
#include "EnumUtils.h"
#include "ItemData.h"
#include "GuiControlButton.h"

#include <string>
#include <vector>

CombatManager::CombatManager(bool startEnabled) : Module(startEnabled)
{
	name.Create("combat");
	needsAwaking = true;
	menuList.push_back(std::vector<GuiControl*>());
	menuList.push_back(std::vector<GuiControl*>());
	menuList.push_back(std::vector<GuiControl*>());
	menuList.push_back(std::vector<GuiControl*>());

	// CODIGO PARA DEBUG, NO DEFINITIVO

	Personatge* p1 = new Personatge("personatge1", 10, 10, 5, 2, "Assets/Animation/Springy/SpringyFantasma.xml");
	p1->atacs.push_back(Atac("Cop de puny1", 10));
	p1->atacs.push_back(Atac("Cop de puny2", 10));
	p1->atacs.push_back(Atac("Cop de puny3", 10));
	p1->atacs.push_back(Atac("Cop de puny4", 10));
	data.allies.push_back(p1);
	Personatge* p2 = new Personatge("personatge2", 5, 30, 2, 1, "Assets/Animation/Springy/SpringyFantasma.xml");
	p2->atacs.push_back(Atac("Puntada de peu1", 15));
	p2->atacs.push_back(Atac("Puntada de peu2", 15));
	p2->atacs.push_back(Atac("Puntada de peu3", 15));
	p2->atacs.push_back(Atac("Puntada de peu4", 15));
	data.allies.push_back(p2);

	// FIN CODIGO PARA DEBUG
}

CombatManager::~CombatManager()
{
	app->console->RemoveCommand("debugcombat");
}

bool CombatManager::PostInit()
{
	app->console->AddCommand("debugcombat", "Inicia un combate con un enemigo de prueba", "debugcombat", [this](std::vector<std::string> args) {
		data.enemy = dummyEnemy = new Personatge("dummy", 1, 10, 1, 10);
		app->reload->QueueReload("combatStart");
		});
	return true;
}

bool CombatManager::Awake(pugi::xml_node config)
{
	posMain.x = config.attribute("mainX").as_int(0);
	posMain.y = config.attribute("mainY").as_int(0);
	posSub.x = config.attribute("subX").as_int(0);
	posSub.y = config.attribute("subY").as_int(0);
	buttonSize.x = config.attribute("buttonW").as_int(0);
	buttonSize.y = config.attribute("buttonH").as_int(0);

	pugi::xml_document layoutDoc;
	pugi::xml_parse_result result = layoutDoc.load_file(config.attribute("uiLayoutPath").as_string());
	if (result && LoadLayout(layoutDoc.child("map"))) {
		LOG("Combat UI layout loaded.");
	}
	else
	{
		LOG("Couldn't load combat menu layout, falling back to original code-generated layout: %s", result.description());
		CreateButtons(config.child("menus"));
	}


	ResetButtonsState();
	combatState = CombatState::START;

	awoken = true;

	return true;
}

bool CombatManager::Start()
{
	// Medidas contra el uso de FadeToBlack()
	if (!awoken)
		Awake(app->GetConfig(this));

	// Se asegura de que el modulo de entidades este pausado para que el jugador no se mueva por el mapa durante el combate
	if (!((Module*)app->entityManager)->paused)
		((Module*)app->entityManager)->Pause();
	// Pausa el modulo de mapa ya que no hace falta mostrar el mapa durante el combate
	if (!((Module*)app->map)->paused)
		((Module*)app->map)->Pause();
	currentMenu = Menus::MAIN;
	currentElement = 0;
	rng = std::mt19937{ std::random_device{}() }; // Resetea el RNG antes de comenzar el combate



	// Añade comando solo disponible durante el combate
	app->console->AddCommand("endcombat", "Termina el combate instantaneamente, como si se hubiera cumplido la condicion de victoria", "endcombat", [this](std::vector<std::string> args) {combatState = CombatState::DIALOG_END; });
	

	// Carga animaciones de los personajes en combate



	LOG("Combat Start!");
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
	case CombatState::COMBAT_ANIM:
	{
		// reproduce y comprueba que han acabado las animaciones de combate antes de continuar
		HandleCombatAnimation();
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
	case CombatState::DO_NOTHING:
	default:
		break;
	}

	return true;
}

bool CombatManager::PostUpdate()
{
	// Render background
	if (!app->DebugEnabled())
		app->render->DrawTexture(backgroundTexture.get(), 0, 0, nullptr, 1.0F, 0, INT_MAX, INT_MAX, false);
	
	// Render UI elements

	return true;
}

bool CombatManager::CleanUp()
{
	app->console->RemoveCommand("");

	data.enemy = nullptr;
	RELEASE(dummyEnemy); // Datos usados con el comando "debugcombat"

	currentMenu = Menus::MAIN;
	currentElement = 0;
	for (std::vector<GuiControl*>& menu : menuList)
	{
		if (app->guiManager->guiControlsList.Count()>0)
		for (GuiControl* g : menu)
		{
			app->guiManager->DestroyGuiControl(g);
		}
		menu.clear();
	}

	 // Unpauses the entityManager and allows player movement
	if (((Module*)app->entityManager)->paused)
		((Module*)app->entityManager)->Pause();
	//Unpauses map so it renders again
	if (((Module*)app->map)->paused)
		((Module*)app->map)->Pause();

	return true;
}

GuiControl* CombatManager::NewButton(char menuID, char elementID, const char* text, SDL_Rect bounds, GuiCallback_f onClick, bool independentPtr, SDL_Rect sliderBounds)
{
	GuiControlButton* ret = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, elementID, text, bounds, onClick, independentPtr, sliderBounds);
	ret->SetOnHover([this, menuID, elementID](GuiControl* g) {currentMenu = (Menus)menuID; currentElement = elementID; });

	return ret;
}

bool CombatManager::LoadLayout(pugi::xml_node layoutRoot)
{
	bool ret = true;

	// Encuentra la imagen de fondo
	for (pugi::xml_node bgLayer = layoutRoot.child("imagelayer"); bgLayer != NULL; bgLayer = bgLayer.next_sibling("imagelayer"))
	{
		if (strcmp(bgLayer.attribute("name").as_string(), "Fondo") == 0)
		{
			SString path = "Assets/Screens/Combat/";
			path += bgLayer.child("image").attribute("source").as_string();
			backgroundTexture = app->tex->LoadSP(path.GetString(), false);
		}
	}


	// TODO terminar de importar el layout de combate




	return false;
}

void CombatManager::CreateButtons(pugi::xml_node menuListNode)
{
	//Monta el menu de arriba a abajo
	std::vector<GuiCallback_f> functions;
	functions.push_back([this](GuiControl* g) {AttackMenu(g); });
	functions.push_back([this](GuiControl* g) {ItemMenu(g); });
	functions.push_back([this](GuiControl* g) {SwapBody(g); });
	functions.push_back([this](GuiControl* g) {Flee(g); });

	//MainMenu
	pugi::xml_node menuItem = menuListNode.child("main");
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
		menuList[enum2val(Menus::MAIN)].push_back(NewButton(0, id, text.GetString(), bounds, func));
	}

	CreateAbilityButtons(data.allies[data.activeAlly]);
	CreateItemButtons(app->inventory);
	CreateTeamSwapButtons(menuListNode.child("team"));

}

void CombatManager::CreateAbilityButtons(Personatge* p)
{
	SDL_Rect bounds{posSub.x,0,buttonSize.x,buttonSize.y};
	
	//AttackMenu
	for (int i = 0; i < p->atacs.size(); i++)
	{
		bounds.y = posSub.y + (i * (bounds.h + 16));
		Atac* a = &p->atacs[i];
		GuiCallback_f func = [this,a](GuiControl* g) {
			LOG("WIP habilidad %s",a->nom.c_str());
			ataqueAliado = a;
			accion = PlayerAction::ATTACK;
			combatState = CombatState::COMBAT; // Al seleccionar el boton de ataque se pasa a ejecutar acciones
			};
		// Crea el boton en la posicion del submenu, la posicion 'y' depende del indice de este boton
		menuList[enum2val(Menus::ATTACK)].push_back(NewButton(1, i, a->nom.c_str(), bounds, func));
	}
	//TODO añadir boton para volver atras
}

void CombatManager::CreateItemButtons(InventoryManager* inv)
{
	// Crea los botones para el uso de objetos, el codigo es similar al de los botones de habilidades y cambio de personaje
	SDL_Rect bounds{ posSub.x,posSub.y,buttonSize.x,buttonSize.y };

	//ItemMenu
	int i = 0;
	for (ListItem<ItemData*>* listItem = inv->items.start; listItem; listItem = listItem->next)
	{
		if (listItem->data && listItem->data->cantidad > 0) {
			ItemData* item = listItem->data;
			bounds.y = posSub.y + i * (bounds.h + 16);
			GuiCallback_f func = [this, item](GuiControl* g) {
				objetoAliado = item;
				accion = PlayerAction::ITEM;
				combatState = CombatState::COMBAT;
				};
			menuList[enum2val(Menus::ITEM)].push_back(NewButton(3, i, item->name.GetString(), bounds, func));
			i++;
		}
	}
	//TODO añadir boton para volver atras
}

void CombatManager::CreateTeamSwapButtons(pugi::xml_node menuItem)
{
	SDL_Rect bounds{ posSub.x,posSub.y,buttonSize.x,buttonSize.y };

	//BodyChangeMenu
	for (size_t i = 0; i < data.allies.size(); i++)
	{
		Personatge* p = data.allies[i];
		if (p) {
			bounds.y = posSub.y + i * (bounds.h + 16);
			GuiCallback_f func = [this, i](GuiControl* g) {
				nuevoAliadoActivo = i;
				accion = PlayerAction::CHANGE;
				combatState = CombatState::COMBAT;
			};
			menuList[enum2val(Menus::TEAM)].push_back(NewButton(3, i, p->nom.c_str(), bounds, func));
		}
	}
	//TODO añadir boton para volver atras
}

bool CombatManager::CombatFinished()
{
	bool ret = false;
	// Gameover si todo el equipo aliado ha sido derrotado
	for (Personatge* p : data.allies)
	{
		ret = ret && p->salutActual <= 0;
	}
	//victoria si enemigo ha sido derrotado
	ret |= data.enemy->salutActual <= 0;
	return ret || fled;
}

char CombatManager::CombatResult()
{
	bool alliesDefeated = true;
	bool enemiesDefeated = true;

	for (Personatge* p : data.allies)
	{
		alliesDefeated &= p->salutActual <= 0;
	}
	enemiesDefeated &= data.enemy->salutActual <= 0;

	return -((char)alliesDefeated)+((char)enemiesDefeated);
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
	int currentMenuInt = enum2val(currentMenu);
	if (app->input->GetButton(ControlID::UP) == KeyState::KEY_DOWN)
	{
		// Si intenta subir desde el primer elemento, va hasta el ultimo sumando la cantidad de elementos
		if (--currentElement < 0) currentElement += menuList[currentMenuInt].size();
	}
	else if (app->input->GetButton(ControlID::DOWN) == KEY_DOWN)
	{
		// Si intenta bajar desde el ultimo elemento, vuelve al inicio restando la cantidad de elementos
		if (++currentElement >= menuList[currentMenuInt].size()) currentElement -= menuList[currentMenuInt].size();
	}

	bool notified = false;
	char i = 0, j = 0;
	for (std::vector<GuiControl*>& menu : menuList)
	{
		j = 0;
		for (GuiControl* g : menu)
		{
			// Si es el elemento seleccionado, lo marca como tal, los otros elementos del mismo menu se quedan en su estado actual
			if ((Menus)i == currentMenu) {
				if (j == currentElement) {
					g->state = (app->input->GetButton(ControlID::CONFIRM) == KEY_REPEAT) ? GuiControlState::PRESSED : GuiControlState::FOCUSED;
					if (!notified && app->input->GetButton(ControlID::CONFIRM) == KEY_UP)
					{
						g->NotifyMouseClick();
						notified = true;
					}
				}
				else
					g->state = GuiControlState::NORMAL;
			}
			// Los otros menus se vuelven invisibles, menos el principal que simplemente se impide hacer clic
			else
				g->state = (i == 0) ? GuiControlState::NON_CLICKABLE : GuiControlState::DISABLED;
			j++;
		}
		i++;
	}

}

void CombatManager::HandleCombat()
{
	//Esconde los menus
	ResetButtonsState();
	currentMenu = Menus::MAIN;
	currentElement = 0;
	HandleMenu();


	// TODO eleccion de accion enemiga, uso de objetos, etc.
	EnemyChoice();
	Personatge* ally = data.allies[data.activeAlly];
	Personatge* enemy = data.enemy;

	//Check accion aliada, usa objeto o ataca
	switch (accion)
	{
	case PlayerAction::ATTACK:
	{
		if (ataqueAliado)
		{
			DoAttack(ally, enemy, ataqueAliado);
			// Activar animacion de ataque aqui
		}
		ataqueAliado = nullptr;
		break;
	}
	case PlayerAction::ITEM:
	{
		if (objetoAliado)
		{
			UseItem(data.allies[data.activeAlly], objetoAliado);
			// Activar animacion de objeto aqui
		}
		break;
	}
	case PlayerAction::CHANGE:
	{
		if (nuevoAliadoActivo > -1)
		{
			SwapCharacter(nuevoAliadoActivo);
		}
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

	combatState = CombatState::COMBAT_ANIM;
}

void CombatManager::EnemyChoice()
{
}

void CombatManager::HandleCombatAnimation()
{
	if (CombatFinished())
		combatState = CombatState::DIALOG_END;
	else combatState = CombatState::MENU;
}

void CombatManager::HandleEndDialog()
{
	combatState = CombatState::END;
}

void CombatManager::HandleEnd()
{
	data.enemy = nullptr;
	app->fadeToBlack->FadeToBlackTransition(this, (Module*)app->entityManager);
	combatState = CombatState::DO_NOTHING;
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

void CombatManager::UseItem(Personatge* target, ItemData* item)
{
	item->UseOn(target);
}

void CombatManager::SwapCharacter(int id)
{
	if (id >=0 && id < data.allies.size())
		data.activeAlly = id;
}

void CombatManager::AttackMenu(GuiControl* ctrl)
{
	currentMenu = Menus::ATTACK;
	currentElement = 0;
	ResetButtonsState();
}

void CombatManager::ItemMenu(GuiControl* ctrl)
{
	currentMenu = Menus::ITEM;
	currentElement = 0;
	ResetButtonsState();
}

void CombatManager::SwapBody(GuiControl* ctrl)
{
	currentMenu = Menus::TEAM;
	currentElement = 0;
	ResetButtonsState();
}

void CombatManager::Flee(GuiControl* ctrl)
{
	currentMenu = Menus::MAIN;
	currentElement = 0;
	fled = true; // NOTE % posibilidad de huir?
	combatState = CombatState::COMBAT_ANIM;
}

void CombatManager::ResetButtonsState()
{
	for (std::vector<GuiControl*>& menu : menuList)
		for (GuiControl* g : menu)
			g->state = GuiControlState::NORMAL;
}
