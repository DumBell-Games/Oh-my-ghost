#ifndef __DIALOGMANAGER_H__
#define __DIALOGMANAGER_H__

#include "Module.h"
#include "Dialog.h"
#include "Timer.h"
#include "List.h"
#include <vector>
#include "SDL_ttf/include/SDL_ttf.h"

class DialogManager : public Module
{
public:

	DialogManager(bool startEnabled = true);

	// Destructor
	virtual ~DialogManager();

	// Called before render is available
	bool Awake(pugi::xml_node config);

	// Called after Awake
	bool Start();

	// Called every frame
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	//Funcion para crear dialogos
	Dialog* CreateDialog(pugi::xml_node itemNode, std::string name = "", const char* faceTexturePath = "", const char* font = "primary", int id = 0, int mapID = 0);
	//Funcion para a�adir a la lista de dialogos
	bool AddDialog(Dialog* dialog);

	void EventManager(DialogEvent actualEvent, DialogEvent eventToActivate);
	
private:
	//Funcion que se encarga de mostrar los dialogos
	bool ShowDialog(Dialog* dialog);

	SDL_Texture* CreateTextTexture(TTF_Font* font, const char* text, SDL_Color color, int textBoundWidth);

	TTF_Font* FontSelector(const char* font);


public:

	List<Dialog*> dialogues;

	bool isPlaying;


private:

	//Colores
	SDL_Color textColor = { 255,255,255,255 };
	SDL_Color optionSelectedColor = { 200, 200, 200, 255 };
	SDL_Color optionColor = { 23, 23, 23, 255 };
	

	//Posiciones
	std::vector<int> dialogMargin = { 130, 50, 0, 50 };
	iPoint dialogPosition = { 130 , 415 }; //{ 0 , 375 }
	iPoint namePosition = { 40 , -75 };
	iPoint optionsPosition = { 900 , 300 };
	int optionsDistanceBetween = 50;

	//Dimensiones
	int textBoundWidth;
	int textNameBoundWidth = 200;
	int optionsBoundWidth = 350;
	iPoint faceTextureSize = { 250, 250 };
	

	//Config
	int charTimeMS = 10;


	//Para el funcionamiento

	SDL_Texture* background_tex;
	std::string background_tex_path;
	int indexText;
	Timer charTimer;
	int optionSelected = 0;

	DialogEvent actualEvent;

	bool dineroObtenido;
	bool pagarBirra;
	bool birraObtenida;
	bool sisebutoHablado;



};

#endif // __DIALOGMANAGER_H__
