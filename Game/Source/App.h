#ifndef __APP_H__
#define __APP_H__

#include "Module.h"
#include "List.h"
#include "PerfTimer.h"
#include "Timer.h"
#include "EntityManager.h"

#include "PugiXml/src/pugixml.hpp"

// L03: DONE 1: Add the EntityManager Module to App

// Modules
class Window;
class Input;
class Render;
class Textures;
class Audio;
class Scene;
class EntityManager;
class Map;
//L07 DONE 2: Add Physics module
class Physics;
class GuiManager;

class App
{
public:

	// Constructor
	App(int argc, char* args[]);

	// Destructor
	virtual ~App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

	// L14: TODO 1: Declare the methods LoadRequest() and SaveRequest() to request and call the Load / Save the game state at the end of the frame

	// Request a save data in an XML file 
	bool LoadRequest();

	// Request to load data from XML file 
	bool SaveRequest();

private:

	// Load config file
	bool LoadConfig();

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	// Reads XML file and loads the data
	bool LoadFromFile();

	// Sace XML file with modules data
	bool SaveFromFile();


public:

	// L03: DONE 1: Add the EntityManager Module to App

	// Modules
	Window* win;
	Input* input;
	Render* render;
	Textures* tex;
	Audio* audio;
	Scene* scene;
	EntityManager* entityManager;
	Map* map;
	//L07 DONE 2: Add Physics module
	Physics* physics;
	GuiManager* guiManager;

private:

	int argc;
	char** args;
	SString gameTitle;
	SString organization;

	List<Module *> modules;

	// L04: DONE 2 - Create a variable to load and store the XML file in memory
	// xml_document to store the config file
	pugi::xml_document configFile;
	
	uint frames;
	float dt;

	// L1: DONE 4: Calculate some timing measures
    // required variables are provided:
	Timer startupTime;
	PerfTimer frameTime;
	PerfTimer lastSecFrameTime;

	uint64 frameCount = 0;
	uint32 framesPerSecond = 0;
	uint32 lastSecFrameCount = 0;

	float averageFps = 0.0f;
	uint32 secondsSinceStartup = 0;

	//L02 DONE 1: Set the maximun frame duration in miliseconds.
	uint32 maxFrameDuration = 16;

	//
	bool loadRequest = false;
	bool saveRequest = false;

};

extern App* app;

#endif	// __APP_H__