#include "CombatManager.h"
#include "GuiControl.h"
#include "GuiManager.h"
#include "InventoryManager.h"
#include "FadeToBlack.h"
#include "App.h"
#include "DebugConsole.h"
#include "Reload.h"
#include "Map.h"
#include "Video.h"

#include "Log.h"
#include "EnumUtils.h"
#include "ItemData.h"
#include "GuiControlButton.h"

#include <string>
#include <vector>
#include "GuiControlPhysButton.h"
#include "GuiCombatHP.h"

CombatManager::CombatManager(bool startEnabled) : Module(startEnabled)
{
	name.Create("combat");
	needsAwaking = true;
	menuList.push_back(std::vector<GuiControl*>());
	menuList.push_back(std::vector<GuiControl*>());
	menuList.push_back(std::vector<GuiControl*>());
	menuList.push_back(std::vector<GuiControl*>());

	// CODIGO PARA DEBUG, NO DEFINITIVO (supuestamente)

	// Personaje tutorial
	Personatge* p1 = new Personatge("Springy", 10, 10, 5, 2, "Assets/Animation/Springy/SpringyFantasma.xml");
	p1->atacs.push_back(Atac("Cop de puny1", 2, false, "Assets/Screen/Combat/Skill.png", "Skill1"));
	p1->atacs.push_back(Atac("Cop de puny2", 2, false, "Assets/Screen/Combat/Skill.png", "Skill2"));
	p1->atacs.push_back(Atac("Cop de puny3", 2, false, "Assets/Screen/Combat/Skill.png", "Skill1"));
	p1->atacs.push_back(Atac("Cop de puny4", 2, false, "Assets/Screen/Combat/Skill.png", "Skill2"));
	p1->atacs.push_back(Atac("Ultimate", 30, true, "Assets/Screen/Combat/Skill.png", "Assets/Videos/GolpeFinalSpringy.avi"));
	data.allies.push_back(p1);
	// Personaje post-tutorial
	Personatge* p2 = new Personatge("Springy", 5, 30, 2, 1, "Assets/Animation/Springy/SpringyPaloma.xml");
	p2->atacs.push_back(Atac("Puntada de peu1", 5, false, "Assets/Screen/Combat/Skill.png", "Skill1"));
	p2->atacs.push_back(Atac("Puntada de peu2", 5, false, "Assets/Screen/Combat/Skill.png", "Skill2"));
	p2->atacs.push_back(Atac("Puntada de peu3", 5, false, "Assets/Screen/Combat/Skill.png", "Skill1"));
	p2->atacs.push_back(Atac("Puntada de peu4", 5, false, "Assets/Screen/Combat/Skill.png", "Skill2"));
	p2->atacs.push_back(Atac("Ultimate", 30, true, "Assets/Screen/Combat/Skill.png", "Assets/Videos/GolpeFinalSpringy.avi"));
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
		// El enemigo tiene el mismo ataque varias veces porque por ahora la IA no tiene preferencias por determinados ataques, asi que para hacer que no haga casi siempre la ulti se ponen duplicados (si, lo se, esto es un desperdicio de memoria, pero en este caso son menos de 100 bytes por cada ataque)
		data.enemy = dummyEnemy = new Personatge("dummy", 1, 10, 1, 10, "Assets/Animation/Jefe_Astrobark/Astrobark.xml");
		dummyEnemy->atacs.push_back(Atac("Mordisco1", 5, false, "", "Skill1"));
		dummyEnemy->atacs.push_back(Atac("Mordisco2", 5, false, "", "Skill2"));
		dummyEnemy->atacs.push_back(Atac("Mordisco1", 5, false, "", "Skill3"));
		dummyEnemy->atacs.push_back(Atac("Mordisco2", 5, false, "", "Skill4"));
		dummyEnemy->atacs.push_back(Atac("Mordisco1", 5, false, "", "Skill1"));
		dummyEnemy->atacs.push_back(Atac("Mordisco2", 5, false, "", "Skill2"));
		dummyEnemy->atacs.push_back(Atac("Mordisco1", 5, false, "", "Skill3"));
		dummyEnemy->atacs.push_back(Atac("Mordisco2", 5, false, "", "Skill4"));
		dummyEnemy->atacs.push_back(Atac("Ultimate", 30, true, "", "Assets/Videos/GolpeFinalAB.avi"));
		BeginCombat(dummyEnemy, pugi::xml_node(), pugi::xml_node());
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

	//Load default dialogs if none found (debug only?)
	if (app->DebugEnabled())
	{
		if (!startDialogue.start)
			LoadDialog(startDialogue, config.child("startDialogue"));
		if (!endDialogue.start)
			LoadDialog(endDialogue, config.child("endDialogue"));
		if (!defeatDialogue.start)
			LoadDialog(defeatDialogue, config.child("defeatDialogue"));
	}

	// Load UI layout
	pugi::xml_document layoutDoc;
	pugi::xml_parse_result result = layoutDoc.load_file(config.attribute("uiLayoutPath").as_string());
	if (result && LoadLayout(layoutDoc.child("map"))) {
		CreateButtons(config.child("menus"));
		LOG("Combat UI layout loaded.");
	}
	else
	{
		LOG("Couldn't load combat menu layout, falling back to original code-generated layout: %s", result.description());
		CreateButtons(config.child("menus"));

		CreateAbilityButtons(data.allies[data.activeAlly]);
		CreateItemButtons(app->inventory);
		//CreateTeamSwapButtons(menuListNode.child("team"));
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



	// A�ade comando solo disponible durante el combate
	app->console->AddCommand("endcombat", "Termina el combate instantaneamente, como si se hubiera cumplido la condicion de victoria", "endcombat", [this](std::vector<std::string> args) {combatState = CombatState::DIALOG_END; });
	

	// Carga animaciones de los personajes en combate

	enemyAnims = new AnimationSet(data.enemy->animPath.c_str());
	playerAnims = new AnimationSet(data.allies[data.activeAlly]->animPath.c_str());

	enemyAnims->SetDefaultAnimation("IdleCombat");
	playerAnims->SetDefaultAnimation("IdleCombat");
	
	enemyAnims->SetAnimation(enemyAnims->defaultAnimation);
	playerAnims->SetAnimation(playerAnims->defaultAnimation);

	// Arreglo para que funcionen los botones con formas custom (?)
	app->render->camera.x = 0;
	app->render->camera.y = 0;

	LOG("Combat Start!");
	fled = false;

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


	// Render background
	if (!app->DebugEnabled())
		app->render->DrawTexture(bgTexture.get(), 0, 0, nullptr, 1.0F, 0, INT_MAX, INT_MAX, false);

	return true;
}

bool CombatManager::PostUpdate()
{
	// Render characters

	enemyAnims->Render(enemyPos, false, 2);
	playerAnims->Render(playerPos, false, 3);
	enemyAnims->Update();
	playerAnims->Update();
	
	// Render video if applicable

	if (!app->video->isVideoFinished)
		app->video->GrabAVIFrame();

	return true;
}

bool CombatManager::CleanUp()
{
	app->console->RemoveCommand("");

	RELEASE(playerAnims);
	RELEASE(enemyAnims);

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

	if (app->guiManager->guiControlsList.Count()>0)
	for (GuiControl* g : guiElements)
	{
		app->guiManager->DestroyGuiControl(g);
	}
	guiElements.clear();

	for (ListItem<Dialog*>* item = startDialogue.start; item; item = item->next)
	{
		RELEASE(item->data);
	}
	startDialogue.Clear();

	for (ListItem<Dialog*>* item = endDialogue.start; item; item = item->next)
	{
		RELEASE(item->data);
	}
	endDialogue.Clear();

	 // Unpauses the entityManager and allows player movement
	if (((Module*)app->entityManager)->paused)
		((Module*)app->entityManager)->Pause();
	//Unpauses map so it renders again
	if (((Module*)app->map)->paused)
		((Module*)app->map)->Pause();

	// Enemigo debe ser liberado al final para evitar problemas con elementos de GUI que usan valores del propio enemigo
	data.enemy = nullptr;
	RELEASE(dummyEnemy); // Datos usados con el comando "debugcombat"

	return true;
}

void CombatManager::BeginCombat(Personatge* enemy, pugi::xml_node startDialogue, pugi::xml_node endDialogue, pugi::xml_node defeatDialogue, bool canEscape)
{
	data.enemy = enemy;
	LoadDialogs(startDialogue, endDialogue, defeatDialogue);
	app->reload->QueueReload("combatStart");
}

GuiControl* CombatManager::NewButton(GuiControlType type, char menuID, char elementID, const char* text, SDL_Rect bounds, GuiCallback_f onClick, bool independentPtr, SDL_Rect sliderBounds)
{
	GuiControlButton* ret = (GuiControlButton*)app->guiManager->CreateGuiControl(type, elementID, text, bounds, onClick, independentPtr, sliderBounds);
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
			bgTexture = app->tex->LoadSP(path.GetString(), false);
		}
	}

	for (pugi::xml_node objLayer = layoutRoot.child("objectgroup"); objLayer != NULL; objLayer = objLayer.next_sibling("objectgroup"))
	{
		if (strcmp(objLayer.attribute("name").as_string(), "AttackMenu") == 0)
		{
			Personatge* p = data.allies[data.activeAlly];
			// Carga botones de ataque
			int elementID = 0;
			constexpr int menuID = enum2val(Menus::ATTACK);
			for (pugi::xml_node itemNode = objLayer.child("object"); itemNode != NULL && elementID < p->atacs.size(); itemNode = itemNode.next_sibling("object"), elementID++)
			{
				SDL_Rect rect;
				rect.x = itemNode.attribute("x").as_int();
				rect.y = itemNode.attribute("y").as_int();
				rect.w = itemNode.attribute("width").as_int();
				rect.h = itemNode.attribute("height").as_int();

				GuiControlType buttonShape = GuiControlType::PHYSBUTTON_BOX;

				// Otros tipos de forma para botones
				if (itemNode.child("ellipse") != NULL)
					buttonShape = GuiControlType::PHYSBUTTON_CIRCLE;
				// else if()

				SString atkName = (elementID < p->atacs.size()) ? data.allies[data.activeAlly]->atacs[elementID].nom.c_str() : "ERROR";



				Atac* a = &p->atacs[elementID];
				GuiCallback_f func = [this, a](GuiControl* g) {
					LOG("WIP habilidad %s", a->nom.c_str());
					ataqueAliado = a;
					accion = PlayerAction::ATTACK;
					combatState = CombatState::COMBAT; // Al seleccionar el boton de ataque se pasa a ejecutar acciones
					};

				GuiControlPhysButton* newButton = (GuiControlPhysButton*)NewButton(buttonShape, menuID, elementID, atkName.GetString(), rect, func, false);

				Properties properties;
				LoadProperties(itemNode, properties);

				// Load button textures
				Properties::Property* prop;
				SString bgPath;
				SString bgClickedPath;
				SString fgPath;
				if (prop = properties.GetProperty("bgPath"))
					bgPath = prop->strVal.GetString();
				if (prop = properties.GetProperty("bgClickedPath"))
				bgClickedPath = prop->strVal.GetString();

				// Load attack icon
				fgPath = a->iconPath.c_str();

				newButton->Init(bgPath.GetString(), bgClickedPath.GetString(), fgPath.GetString());

				menuList[menuID].push_back(newButton);

			}
		}

		// Carga botones de objetos
		else if (strcmp(objLayer.attribute("name").as_string(), "ItemMenu") == 0)
		{
			pugi::xml_node itemNode = objLayer.child("object");
			posSub.x = itemNode.attribute("x").as_int();
			posSub.y = itemNode.attribute("y").as_int();
			CreateItemButtons(app->inventory);

			MenuDirection dir = (MenuDirection)itemNode.child("properties").child("property").attribute("direction").as_int();

			int i = 0;
			for each (GuiControl* g in menuList[enum2val(Menus::ITEM)])
			{
				g->bounds.x = posSub.x + ((dir == MenuDirection::DOWN_RIGHT || dir == MenuDirection::UP_RIGHT) ? 1 : -1);
				g->bounds.y = posSub.y + i * buttonSize.y * ((dir == MenuDirection::UP_LEFT || dir == MenuDirection::UP_RIGHT) ? -1 : 1);

				((GuiControlPhysButton*)g)->GetPhysBody()->body->SetTransform(b2Vec2(g->bounds.x, g->bounds.y), ((GuiControlPhysButton*)g)->GetPhysBody()->GetRotation());
			}
		}

		// Carga posicion de personajes
		else if (strcmp(objLayer.attribute("name").as_string(), "CharacterPositions") == 0)
		{
			for (pugi::xml_node itemNode = objLayer.child("object"); itemNode != NULL; itemNode = itemNode.next_sibling("object"))
			{
				if (strcmp(itemNode.attribute("name").as_string(), "playerPivot") == 0)
				{
					playerPos.x = itemNode.attribute("x").as_int(playerPos.x);
					playerPos.y = itemNode.attribute("y").as_int(playerPos.y);
				}
				else if (strcmp(itemNode.attribute("name").as_string(), "enemyPivot") == 0)
				{
					enemyPos.x = itemNode.attribute("x").as_int(enemyPos.x);
					enemyPos.y = itemNode.attribute("y").as_int(enemyPos.y);
				}
			}
		}
	
		// Carga posicion de barras de vida
		else if (strcmp(objLayer.attribute("name").as_string(), "CharInfo") == 0)
		{
			int id = 0;
			for (pugi::xml_node itemNode = objLayer.child("object"); itemNode != NULL; itemNode = itemNode.next_sibling("object"))
			{
				SDL_Rect bounds;
				bounds.x = itemNode.attribute("x").as_int();
				bounds.y = itemNode.attribute("y").as_int();
				bounds.w = itemNode.attribute("width").as_int(20);
				bounds.h = itemNode.attribute("height").as_int(20);

				GuiCombatHP* g = (GuiCombatHP*)app->guiManager->CreateGuiControl(GuiControlType::PROGRESS_BAR, id++, "HP", bounds, (Module*)nullptr);
				if (strcmp(itemNode.attribute("name").as_string(), "playerHP") == 0)
				{
					g->trackedValue = &data.allies[data.activeAlly]->salutActual;
					g->maxValue = data.allies[data.activeAlly]->salutTotal;
					
				}
				else if (strcmp(itemNode.attribute("name").as_string(), "enemyHP") == 0)
				{
					g->trackedValue = &data.enemy->salutActual;
					g->maxValue = data.enemy->salutTotal;
				}
					

				guiElements.push_back(g);
			}
		}

	}




	// TODO terminar de importar el layout de combate si falta algo




	return true;
}

void CombatManager::CreateButtons(pugi::xml_node menuListNode)
{
	//Monta el menu de arriba a abajo
	std::vector<GuiCallback_f> functions;
	functions.push_back([this](GuiControl* g) {AttackMenu(g); });
	functions.push_back([this](GuiControl* g) {ItemMenu(g); });
	//functions.push_back([this](GuiControl* g) {SwapBody(g); });
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
		menuList[enum2val(Menus::MAIN)].push_back(NewButton(GuiControlType::PHYSBUTTON_BOX, 0, id, text.GetString(), bounds, func));
	}


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
		GuiControlPhysButton* newButton = (GuiControlPhysButton*)NewButton(GuiControlType::PHYSBUTTON_CIRCLE, 1, i, a->nom.c_str(), bounds, func);

		menuList[enum2val(Menus::ATTACK)].push_back(newButton);
	}
	//TODO a�adir boton para volver atras
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
			GuiControlPhysButton* newButton = (GuiControlPhysButton*)NewButton(GuiControlType::PHYSBUTTON_BOX, 3, i, item->name.c_str(), bounds, func);


			menuList[enum2val(Menus::ITEM)].push_back(newButton);
			i++;
		}
	}
	//TODO a�adir boton para volver atras
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
			GuiControlPhysButton* newButton = (GuiControlPhysButton*)NewButton(GuiControlType::PHYSBUTTON_BOX, 3, i, p->nom.c_str(), bounds, func);
			//newButton->SetRotation(45.0f);


			menuList[enum2val(Menus::TEAM)].push_back(newButton);
		}
	}
	//TODO a�adir boton para volver atras
}

bool CombatManager::CombatFinished()
{
	bool ret = false;
	// Gameover si todo el equipo aliado ha sido derrotado
	/*for (Personatge* p : data.allies)
	{
		ret = ret && p->salutActual <= 0;
	}*/

	//Derrota si el jugador ha sido derrotado
	ret = data.allies[data.activeAlly]->salutActual <= 0;
	//victoria si enemigo ha sido derrotado
	ret |= data.enemy->salutActual <= 0;
	return ret || fled;
}

char CombatManager::CombatResult()
{
	bool alliesDefeated = true;
	bool enemiesDefeated = true;

	// Codigo para cuando haya multiples cuerpos en combate
	/*for (Personatge* p : data.allies)
	{
		alliesDefeated &= p->salutActual <= 0;
	}*/

	//Chequeo para un solo cuerpo
	alliesDefeated = data.allies[data.activeAlly]->salutActual <= 0;

	enemiesDefeated &= data.enemy->salutActual <= 0;

	return -((char)alliesDefeated)+((char)enemiesDefeated);
}

void CombatManager::HandleStart()
{
	// Activar dialogo de inicio
	ListItem<Dialog*>* item;
	Dialog* pDialog = nullptr;

	for (item = startDialogue.start; item != NULL; item = item->next)
	{
		pDialog = item->data;
		app->dialogManager->AddDialog(pDialog);
	}

	// Una vez creado el dialogo, pasa al siguiente estado (para esperar a que se cierre el dialogo)
	combatState = CombatState::DIALOG_START;
}

void CombatManager::HandleStartDialog()
{
	// Una vez terminado el dialogo de inicio, pasa a HandleMenu() asignando el valor correcto a la variable combatState

	if (!app->dialogManager->isPlaying)
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
	else if (app->input->GetButton(ControlID::BACK) == KEY_DOWN || app->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN)
	{
		// Vuelve al menu principal de combate
		currentMenu = Menus::MAIN;
		currentElement = 0;
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
				g->state = GuiControlState::DISABLED;//(i == 0) ? GuiControlState::NON_CLICKABLE : GuiControlState::DISABLED;
			j++;
		}
		i++;
	}

}

void CombatManager::HandleCombat()
{
	//Esconde los menus. Menu de ataque siempre es visible
	ResetButtonsState(GuiControlState::DISABLED);
	for (GuiControl* m : menuList[enum2val(Menus::ATTACK)])
	{	m->state = GuiControlState::NON_CLICKABLE;	}
	currentMenu = Menus::MAIN;
	currentElement = 0;
	HandleMenu();


	// TODO eleccion de accion enemiga, uso de objetos, etc.
	EnemyChoice();
	Personatge* ally = data.allies[data.activeAlly];
	Personatge* enemy = data.enemy;

	// Crea primer bloque de texto + animacion
	currentTurnStep = 0;
	turnResults.push_back(new TurnStep());
	//Check accion aliada, usa objeto o ataca
	switch (accion)
	{
	case PlayerAction::ATTACK:
	{
		if (ataqueAliado)
		{
			if (DoAttack(ally, enemy, ataqueAliado))
			{
				if (ataqueAliado->isUltimate)
				{
					turnResults.push_back(new TurnStep());

					turnResults.back()->video = true;
					turnResults.back()->videoPath = ataqueAliado->animationID;

					turnResults.push_back(new TurnStep());
				}
				else
				{
					// Solo asigna animacion a aliado si no es ataque ultimate
					turnResults.back()->allyAnimationID = playerAnims->GetAnimationId(ataqueAliado->animationID);
				}
				turnResults.back()->enemyAnimationID = enemyAnims->GetAnimationId("Hurt");
			}
		}
		ataqueAliado = nullptr;
		break;
	}
	case PlayerAction::ITEM:
	{
		if (objetoAliado)
		{
			UseItem(data.allies[data.activeAlly], objetoAliado);
			// Activar animacion de objeto aqui (?)
		}
		break;
	}
	case PlayerAction::CHANGE:
	{
		// Esta parte del codigo no est� en uso
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

	// Si el enemigo ha sido derrotado, no puede atacar
	if (data.enemy->salutActual>0)
	{
		// Crea segundo bloque de texto + animacion
		turnResults.push_back(new TurnStep());

		// TODO implementar orden de turnos
		if (ataqueEnemigo) {
			if (DoAttack(enemy, ally, ataqueEnemigo))
			{
				if (ataqueEnemigo->isUltimate)
				{
					turnResults.push_back(new TurnStep());

					turnResults.back()->video = true;
					turnResults.back()->videoPath = ataqueEnemigo->animationID;

					turnResults.push_back(new TurnStep());
				}
				else
				{
					// Solo asigna animacion a enemigo si no es ataque ultimate
					turnResults.back()->enemyAnimationID = enemyAnims->GetAnimationId(ataqueEnemigo->animationID);
				}
				turnResults.back()->allyAnimationID = playerAnims->GetAnimationId("Hurt");
			}
		}
	}

	// Accion completada, resetea las variables para nuevo turno
	ataqueAliado = ataqueEnemigo = nullptr;

	combatState = CombatState::COMBAT_ANIM;
}

void CombatManager::EnemyChoice()
{
	if (data.enemy->atacs.size() == 0) return;


	int selectedAttack = rng() % data.enemy->atacs.size();
	ataqueEnemigo = &(data.enemy->atacs[abs(selectedAttack)]);
}

void CombatManager::HandleCombatAnimation()
{
	//Check animacion completada
	if (turnResults.size() > 0)
	{
		bool stepFinished = false;
		TurnStep* currentStep = turnResults[currentTurnStep];

		// El primer frame en el que se accede a esta funcion cada turno se activan las animaciones y el dialogo
		if (!currentStep->started)
		{
			if (!currentStep->video)
			{
				playerAnims->SetAnimation(currentStep->allyAnimationID);
				enemyAnims->SetAnimation(currentStep->enemyAnimationID);

				for (Dialog*& item : currentStep->dialogs)
				{
					app->dialogManager->AddDialog(item);
				}
			}
			else
			{
				// Ataques ultimate tienen su propio TurnStep con solamente el video
				app->video->Initialize(currentStep->videoPath.c_str());
			}

			currentStep->started = true;
		}
		else
		{
			if (!currentStep->video)
			{
				bool playerAnimFinished = playerAnims->activeAnimation == playerAnims->defaultAnimation;
				bool enemyAnimFinished = enemyAnims->activeAnimation == enemyAnims->defaultAnimation;

				if (!app->dialogManager->isPlaying && (playerAnimFinished) && (enemyAnimFinished))
				{
					stepFinished = true;
				}
			}
			else
			{
				// Comprueba si ha terminado el video
				stepFinished = app->video->isVideoFinished;
				if (stepFinished)
					app->video->CloseAVI();
			}
		}
		
		// Limpia la lista de animaciones y dialogos una vez se han reproducido todos, indicando fin de turno
		if (stepFinished)
		{
			// Incrementa el contador. Si ha salido fuera del vector, indica que ya se han terminado de reproducir animaciones y dialogos de turno
			if (++currentTurnStep >= turnResults.size())
			{
				for (TurnStep*& i : turnResults)
				{
					RELEASE(i);
				}
				turnResults.clear();
				currentTurnStep = 0;
			}
		}
	}
	else
	{
		playerAnims->SetAnimation("IdleCombat");
		enemyAnims->SetAnimation("IdleCombat");

		// Fin de turno. Si ha terminado el combate muestra dialogo de fin de combate, en caso contrario vuelve a los menus
		if (CombatFinished())
		{
			if (!fled)
			{
				char combatResult = CombatResult();
				if (combatResult <= 0) playerAnims->SetAnimation("Dead", false);
				if (combatResult >= 0) enemyAnims->SetAnimation("Dead", false);

				// Si el jugador ha sido derrotado muestra el dialogo de derrota, incluso si el enemigo tambien fue derrotado
				ListItem<Dialog*>* item = (combatResult > 0) ? endDialogue.start : defeatDialogue.start;
				Dialog* pDialog = nullptr;

				for (; item != NULL; item = item->next)
				{
					pDialog = item->data;
					app->dialogManager->AddDialog(pDialog);
				}
			}
			else
			{
				// Borra el texto que iba a reproducirse originalmente y sustituye por "has huido del combate"
				for (ListItem<Dialog*>* item = endDialogue.start; item; item = item->next)
				{
					RELEASE(item->data);
				}
				endDialogue.Clear();

				endDialogue.Add(app->dialogManager->CreateSimpleDialog("Has huido del combate..."));
			}

			combatState = CombatState::DIALOG_END;
		}
		else combatState = CombatState::MENU;
	}
}

void CombatManager::HandleEndDialog()
{
	bool playerAnimFinished = playerAnims->GetCurrent().HasFinished() || playerAnims->activeAnimation == playerAnims->defaultAnimation;
	bool enemyAnimFinished = enemyAnims->GetCurrent().HasFinished() || enemyAnims->activeAnimation == enemyAnims->defaultAnimation;

	if (!app->dialogManager->isPlaying && playerAnimFinished && enemyAnimFinished)
		combatState = CombatState::END;
}

void CombatManager::HandleEnd()
{
	data.enemy = nullptr;
	app->fadeToBlack->FadeToBlackTransition(this, (Module*)app->entityManager);
	combatState = CombatState::DO_NOTHING;
}

// Calcula el da�o efectuado, devuelve si el ataque se ha efectuado o no
bool CombatManager::DoAttack(Personatge* attacker, Personatge* defender, Atac* move)
{
	//TODO a�adir precision (posibilidad de fallar el ataque)
	float attackerDefMult = 1, attackerAtkMult = 1;
	float defenderDefMult = 1, defenderAtkMult = 1;
	bool attackInterrupted = false;
	// Si los estados alterados que tengan el atacante y el atacado afectan al ataque y la defensa respectivamente, se aplican los multiplicadores adecuados. Tambien pueden interrumpir el ataque (paralisis)
	if (attacker->activeStatus)
		attackInterrupted = attacker->activeStatus->ApplyToAttack(attackerDefMult, attackerAtkMult);
	
	// La interrupcion de ataque no aplica por parte del objetivo
	if (defender->activeStatus)
		defender->activeStatus->ApplyToAttack(defenderDefMult, defenderAtkMult);
	if (!attackInterrupted) {

		int finalAtk = ((float)(attacker->atac * move->potencia * attackerAtkMult) / (float)(5 * defender->defensa * defenderDefMult)); // damage = [(atk*pow*atkMultiplier)/(5*def*defMultiplier)]

		defender->rebreDanys(finalAtk);
		// Crea una linea de dialogo para mostrar al jugador el ataque que ha lanzado el personaje
		turnResults.back()->dialogs.push_back(app->dialogManager->CreateSimpleDialog(attacker->nom + " usa " + move->nom + "!"));

		// Comprueba si el objetivo tiene un estado alterado, si no lo tiene, comprueba si se aplica estado alterado, si se aplica, se guarda en los datos de personaje y a�ade el mensaje a la cola
		if (defender->activeStatus.get() == nullptr && (rng() % 100) < move->probEstat)
		{
			defender->activeStatus = std::unique_ptr<StatusEffect>(move->status);
			turnResults.back()->dialogs.push_back(app->dialogManager->CreateSimpleDialog(move->status->GetApplicationMsg()));
		}
	}
	else
	{
		turnResults.back()->dialogs.push_back(app->dialogManager->CreateSimpleDialog(attacker->nom + " no pudo atacar."));
	}
	return !attackInterrupted;

}

void CombatManager::UseItem(Personatge* target, ItemData* item)
{
	turnResults.back()->dialogs.push_back(app->dialogManager->CreateSimpleDialog(item->name+" usado sobre "+target->nom));
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
	fled = true && canFlee; // NOTE % posibilidad de huir?
	combatState = CombatState::COMBAT_ANIM;
}

void CombatManager::ResetButtonsState(GuiControlState state)
{
	for (std::vector<GuiControl*>& menu : menuList)
		for (GuiControl* g : menu)
			g->state = state;
}

void CombatManager::LoadDialogs(pugi::xml_node startDialog, pugi::xml_node endDialog, pugi::xml_node defeatDialog)
{
	LoadDialog(startDialogue, startDialog);
	LoadDialog(endDialogue, endDialog);
	LoadDialog(defeatDialogue, defeatDialog);
}

void CombatManager::LoadDialog(List<Dialog*>& list, pugi::xml_node dialogNode)
{

	for (pugi::xml_node itemNode = dialogNode.child("sentence"); itemNode; itemNode = itemNode.next_sibling("sentence"))
	{
		SString faceTexturePath = itemNode.attribute("faceTexturePath").as_string();
		list.Add(app->dialogManager->CreateDialogs(itemNode, itemNode.attribute("name").as_string(), faceTexturePath.GetString(), "primary", 0, app->map->currentMap));
	}

}
