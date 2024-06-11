#include "App.h"
#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "Scene.h"
#include "Map.h"
#include "CombatManager.h"
#include "Reload.h"
#include "Physics.h"
#include "GuiManager.h"
#include "Optick/include/optick.h"
#include "menu.h"
#include "PauseMenu.h"
#include "DebugConsole.h"
#include "EntityManager.h"
#include "TeamScreen.h"
#include "IntroScreen.h"
#include "FadeToBlack.h"
#include "InventoryManager.h"
#include "InventoryScreen.h"
#include "Character_Menu.h"
#include "Video.h"
#include "MusicaCiudad.h"
#include "MusicaCielo.h"
#include "MusicaCasino.h"
#include "MusicaDiscoteca.h"
#include "MusicaMansion.h"
#include "MusicaTaberna.h"
#include "PersonajesScreen.h"
#include "Tienda.h"
#include "ContextoSpringyP1.h"
#include "ContextoSpringySegundaParte.h"
#include "Creditos.h"
#include "Recuerdos.h"
#include "MusicaCombate.h"
#include "MusicaCombateAstroBark.h"
#include "CargaVeterana.h"
#include "CargaAstroBark.h"
#include "CargaAprendiz.h"


#include "DialogManager.h"

#include "Defs.h"
#include "Log.h"

#include <iostream>
#include <sstream>

// Constructor
App::App(int argc, char* args[]) : argc(argc), args(args)
{
	// L1: DONE 3: Measure the amount of ms that takes to execute the App constructor and LOG the result
	Timer timer = Timer();
	startupTime = Timer();
	frameTime = PerfTimer();
	lastSecFrameTime = PerfTimer();

	frames = 0;

	// L3: DONE 1: Add the EntityManager Module to App

	win = new Window(true);
	input = new Input(true);
	render = new Render(true);
	tex = new Textures(true);
	audio = new Audio(true);
	//L07 DONE 2: Add Physics module
	physics = new Physics(false);
	scene = new Scene(false);
	inventory = new InventoryManager(false);
	inventoryScreen = new InventoryScreen(false);
	personajesScreen = new PersonajesScreen(false);
	recuerdos = new Recuerdos(false);
	tienda = new Tienda(false);
	characterMenu = new Character_Menu(false);
	cargaVeterana = new CargaVeterana(false);
	cargaAB = new CargaAB(false);
	cargaAprendiz = new CargaAprendiz(false);
	contextoSpringyP1 = new ContextoSpringyP1(false);
	contextoSpringyP2 = new ContextoSpringySegundaParte(false);
	titlescreen = new TitleScreen(false);
	introScreen = new IntroScreen(false);
	teamScreen = new TeamScreen(true);
	pause = new PauseMenu(false);
	console = new DebugConsole(true);
	map = new Map(false);
	creditos = new Creditos(false);
	combat = new CombatManager(false);
	reload = new Reload(true);
	entityManager = new EntityManager(false);
	guiManager = new GuiManager(true);
	dialogManager = new DialogManager(false);

	fadeToBlack = new FadeToBlack(true);
	video = new Video(false);
	musicaCiudad = new MusicaCiudad(false);
	musicaCombate = new MusicaCombate(false);
	musicaCombateAB = new MusicaCombateAB(false);
	musicaCielo = new MusicaCielo(false);
	musicaCasino = new MusicaCasino(false);
	musicaDisco = new MusicaDiscoteca(false);
	musicaMansion = new MusicaMansion(false);
	musicaTaberna = new MusicaTaberna(false);


	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(win);
	AddModule(input);
	AddModule(tex);
	AddModule(audio);
	//L07 DONE 2: Add Physics module
	AddModule(physics);
	AddModule(map);
	AddModule(entityManager);
	AddModule(combat);
	AddModule(guiManager);
	AddModule(inventory);

	AddModule(scene);
	AddModule(inventoryScreen);
	AddModule(personajesScreen);
	AddModule(recuerdos);
	AddModule(tienda);
	AddModule(characterMenu);
	AddModule(cargaVeterana);
	AddModule(cargaAB);
	AddModule(cargaAprendiz);	
	AddModule(contextoSpringyP1);
	AddModule(contextoSpringyP2);
	AddModule(titlescreen);
	AddModule(introScreen);
	AddModule(teamScreen);
	AddModule(pause);
	AddModule(console);
	AddModule(dialogManager);
	AddModule(fadeToBlack);
	AddModule(video);
	AddModule(musicaCiudad);
	AddModule(musicaCombate);
	AddModule(musicaCombateAB);
	AddModule(musicaCielo);
	AddModule(musicaCasino);
	AddModule(musicaDisco);
	AddModule(musicaMansion);
	AddModule(musicaTaberna);
	AddModule(creditos);

	AddModule(reload);

	// Render last to swap buffer
	AddModule(render);

	LOG("Timer App Constructor: %f", timer.ReadMSec());
}

// Destructor
App::~App()
{
	// Release modules
	ListItem<Module*>* item = modules.end;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.Clear();
}

void App::AddModule(Module* module)
{
	module->Init();
	modules.Add(module);
}

Module* App::GetModule(const char* name)
{
	Module* ret = nullptr;
	for (ListItem<Module*>* item = modules.start; item; item = item->next)
	{
		if (item->data->name == name) {
			ret = item->data;
			break;
		}
	}
	return ret;
}

pugi::xml_node App::GetConfig(const Module& module)
{
	return configFile.child("config").child(module.name.GetString());
}

// Called before render is available
bool App::Awake()
{
	// L1: DONE 3: Measure the amount of ms that takes to execute the Awake and LOG the result
	Timer timer = Timer();
	
	bool ret = true;

	for (ListItem<Module*>* item = modules.start; item != NULL && ret == true; item = item->next)
	{
		ret = item->data->PostInit();
	}

	if (ret)
		ret = LoadConfig();

	if(ret == true)
	{
		// L04: DONE 3: Read the title from the config file and set the windows title 
		// substitute "Video Game Template" string from the value of the title in the config file
		// also read maxFrameDuration 
		gameTitle.Create(configFile.child("config").child("app").child("title").child_value());
		win->SetTitle(gameTitle.GetString());
		maxFrameDuration = configFile.child("config").child("app").child("maxFrameDuration").attribute("value").as_int();

		ListItem<Module*>* item;
		item = modules.start;

		while(item != NULL && ret == true)
		{
			// L04: DONE 4: Add a new argument to the Awake method to receive a pointer to an xml node.
			// If the section with the module name exists in config.xml, fill the pointer with the valid xml_node
			// that can be used to read all variables for that module.
			// Send nullptr if the node does not exist in config.xml
			if (item->data->active || !item->data->needsAwaking)
				ret = item->data->Awake(configFile.child("config").child(item->data->name.GetString()));
			item = item->next;
		}
	}

	LOG("Timer App Awake(): %f", timer.ReadMSec());

	return ret;
}

// Called before the first frame
bool App::Start()
{
	// L1: DONE 3: Measure the amount of ms that takes to execute the App Start() and LOG the result
	Timer timer = Timer();

	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;

	while(item != NULL && ret == true)
	{
		if (item->data->active)
			ret = item->data->Start();
		item = item->next;
	}

	LOG("Timer App Start(): %f", timer.ReadMSec());

	return ret;
}

// Called each loop iteration
bool App::Update()
{
	//L16 TODO 2: Add the Optick macro to mark the beginning of the main loop
	OPTICK_FRAME("Main Loop");

	bool ret = !quit;
	PrepareUpdate();

	if(input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if(ret == true)
		ret = PreUpdate();

	if(ret == true)
		ret = DoUpdate();

	if(ret == true)
		ret = PostUpdate();

	FinishUpdate();
	return ret;
}

// Load config from XML file
bool App::LoadConfig()
{
	bool ret = true;

	// L04: DONE 2: Load config.xml file using load_file() method from the xml_document class
	// If the result is ok get the main node of the XML
	// else, log the error
	// check https://pugixml.org/docs/quickstart.html#loading

	pugi::xml_parse_result result = configFile.load_file("config.xml");
	if (result)
	{
		LOG("config.xml parsed without errors");
	}
	else
	{
		LOG("Error loading config.xml: %s",result.description());
	}

	return ret;
}

// ---------------------------------------------
void App::PrepareUpdate()
{
	// L16 TODO 3: Add OPTICK macro to functions to include in the profiling
	OPTICK_EVENT();
	frameTime.Start();
}

// ---------------------------------------------
void App::FinishUpdate()
{
	// L16 TODO 3: Add OPTICK macro to functions to include in the profiling
	OPTICK_EVENT();

	// This is a good place to call Load / Save functions

	// L02: DONE 1: Cap the framerate of the gameloop
	// L02: DONE 2: Measure accurately the amount of time SDL_Delay() actually waits compared to what was expected
	
	double currentDt = frameTime.ReadMs();
	if (maxFrameDuration > 0 && currentDt < maxFrameDuration) {
		uint32 delay = (uint32) (maxFrameDuration - currentDt);

		PerfTimer delayTimer = PerfTimer();
		SDL_Delay(delay);
		//LOG("We waited for %I32u ms and got back in %f ms",delay,delayTimer.ReadMs());
	}
	

    // L1: DONE 4: Calculate:
	// Amount of frames since startup
	frameCount++;

	// Amount of time since game start (use a low resolution timer)
	secondsSinceStartup = startupTime.ReadSec();
	
	// Amount of ms took the last update (dt)
	dt = (float) frameTime.ReadMs();

	// Amount of frames during the last second
	lastSecFrameCount++;

	// Average FPS for the whole game life
	if (lastSecFrameTime.ReadMs() > 1000) {
		lastSecFrameTime.Start();
		averageFps = (averageFps + lastSecFrameCount) / 2;
		framesPerSecond = lastSecFrameCount; 
		lastSecFrameCount = 0;
	}


	// Shows the time measurements in the window title
	// check sprintf formats here https://cplusplus.com/reference/cstdio/printf/
	static char title[256];
	sprintf_s(title, 256, "%s: Av.FPS: %.2f Last sec frames: %i Last dt: %.3f Time since startup: %I32u Frame Count: %I64u ", 
		gameTitle.GetString(), averageFps, framesPerSecond, dt, secondsSinceStartup, frameCount);

	app->win->SetTitle(title);

	if (loadRequest) {
		loadRequest = false;
		LoadFromFile();
	}

	if (saveRequest) {
		saveRequest = false;
		SaveFromFile();
	}
}

// Call modules before each loop iteration
bool App::PreUpdate()
{
	// L16 TODO 3: Add OPTICK macro to functions to include in the profiling
	OPTICK_EVENT();

	bool ret = true;

	ListItem<Module*>* item;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool App::DoUpdate()
{
	// L16 TODO 3: Add OPTICK macro to functions to include in the profiling
	OPTICK_EVENT();

	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool App::PostUpdate()
{
	// L16 TODO 3: Add OPTICK macro to functions to include in the profiling
	OPTICK_EVENT();

	bool ret = true;
	ListItem<Module*>* item;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PostUpdate();
	}

	return ret;
}

// Called before quitting
bool App::CleanUp()
{
	// L1: DONE 3: Measure the amount of ms that takes to execute the App CleanUp() and LOG the result
	Timer timer = Timer();

	bool ret = true;
	ListItem<Module*>* item;
	item = modules.end;

	while(item != NULL && ret == true)
	{
		if (item->data->isEnabled)
			ret = item->data->CleanUp();
		item = item->prev;
	}

	LOG("Timer App CleanUp(): %f", timer.ReadMSec());

	return ret;
}

// ---------------------------------------
int App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* App::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* App::GetTitle() const
{
	return gameTitle.GetString();
}

// ---------------------------------------
const char* App::GetOrganization() const
{
	return organization.GetString();
}

// L14: TODO 1: Implement the methods LoadRequest() and SaveRequest() to request and call the Load / Save the game state at the end of the frame
// The real execution of load / save will be implemented in TODO 5 and 7

// Request a save data in an XML file 
bool App::LoadRequest() {

	bool ret = true;
	loadRequest = true;
	return ret;
}

// Request to load data from XML file 
bool App::SaveRequest() {
	bool ret = true;
	saveRequest = true;
	return true;
}


// L03: TODO 5: Implement the method LoadFromFile() to actually load a xml file
// then call all the modules to load themselves
bool App::LoadFromFile() {

	bool ret = true;

	pugi::xml_document saveFile;
	pugi::xml_parse_result result = saveFile.load_file("save_game.xml");

	if (result)
	{
		LOG("save_game.xml parsed without errors");

		// Iterates all modules and call the load of each with the part of the XML node that corresponds to the module
		ListItem<Module*>* item;
		item = modules.start;

		while (item != NULL && ret == true)
		{
			ret = item->data->LoadState(saveFile.child("game_state").child(item->data->name.GetString()));
			item = item->next;
		}

	}
	else
	{
		LOG("Error loading save_game.xml: %s", result.description());
		ret = false;
	}



	return ret;

}

// L03: TODO 7: Implement the xml save method SaveToFile() for current state
// check https://pugixml.org/docs/quickstart.html#modify
bool App::SaveFromFile() {

	bool ret = true;

	pugi::xml_document saveFile;
	pugi::xml_node gameState = saveFile.append_child("game_state");

	// Iterates all modules and call the save of each with the part of the XML node that corresponds to the module
	ListItem<Module*>* item;
	item = modules.start;

	while (item != NULL && ret == true)
	{
		pugi::xml_node module = gameState.append_child(item->data->name.GetString());
		ret = item->data->SaveState(module);
		item = item->next;
	}

	ret = saveFile.save_file("save_game.xml");

	if (ret) LOG("Saved");
	else LOG("Error saving game state");

	return ret;

}
