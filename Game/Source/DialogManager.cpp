#include "DialogManager.h"
#include "Player.h"
#include "Item.h"
#include "App.h"
#include "Textures.h"
#include "Scene.h"
#include "Window.h"

#include "Defs.h"
#include "Log.h"
#include "SDL_ttf/include/SDL_ttf.h"

DialogManager::DialogManager() : Module()
{
	name.Create("dialogmanager");
}

// Destructor
DialogManager::~DialogManager()
{}

// Called before render is available
bool DialogManager::Awake(pugi::xml_node config)
{
	LOG("Loading Dialog Manager");
	bool ret = true;

	background_tex_path = config.child("textures").child("background_dialog").attribute("texturepath").as_string();


	return ret;

}

bool DialogManager::Start() {

	bool ret = true; 

	uint windowW, windowH;
	app->win->GetWindowSize(windowW, windowH);
	
	//Calculo del tamaño de la caja de texto
	textBoundWidth = windowW - dialogMargin[1] - dialogMargin[3];
	

	indexText = 1;


	background_tex = app->tex->Load(background_tex_path.c_str());

	return ret;
}

// Called before quitting
bool DialogManager::CleanUp()
{
	bool ret = true;
	ListItem<Dialog*>* item;
	item = dialogues.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	dialogues.Clear();

	return ret;
}

Dialog* DialogManager::CreateDialog(pugi::xml_node itemNode, std::string name, const char* faceTexturePath, const char* font)
{
	//Dialogo a crear
	Dialog* dialog = new Dialog(itemNode.attribute("text").as_string());
	dialog->name = name;
	dialog->name = itemNode.attribute("name").as_string(dialog->name.c_str());
	dialog->face_tex = app->tex->Load(itemNode.attribute("facetexturepath").as_string(faceTexturePath));
	dialog->font = FontSelector(itemNode.attribute("font").as_string(font));
	

	const char* type = itemNode.attribute("type").as_string("");

	if (strcmp(type, "choose") == 0) {

		dialog->type = DialogType::CHOOSE;

		//Options1
		dialog->option1 = itemNode.child("option1").attribute("text").as_string();
		for (pugi::xml_node optionNode = itemNode.child("option1").child("sentence"); optionNode; optionNode = optionNode.next_sibling("sentence")) {
			Dialog* dialogOp1 = CreateDialog(optionNode, name, faceTexturePath, font);
			dialog->options1.Add(dialogOp1);
		}

		//Options2
		dialog->option2 = itemNode.child("option2").attribute("text").as_string();
		for (pugi::xml_node optionNode = itemNode.child("option2").child("sentence"); optionNode; optionNode = optionNode.next_sibling("sentence")) {
			Dialog* dialogOp2 = CreateDialog(optionNode, name, faceTexturePath, font);
			dialog->options2.Add(dialogOp2);
		}
	}

	return dialog;
}

bool DialogManager::AddDialog(Dialog* dialog)
{
	dialogues.Add(dialog);
	return true;
}

bool DialogManager::ShowDialog(Dialog* dialog)
{
	//Mostrar fondo
	app->render->DrawTexture(background_tex, dialogPosition.x, dialogPosition.y, 0, 0);

	std::string actualText = dialog->sentence.substr(0, indexText);
	

	//Inicializando las variables para las texturas
	SDL_Texture* textTexture = nullptr;
	SDL_Texture* textNameTexture = nullptr;
	SDL_Texture* options1NameTexture = nullptr;
	SDL_Texture* options2NameTexture = nullptr;

	//Setting de las variables
	int _textBoundWidth;
	iPoint _dialogPosition;

	if (dialog->face_tex != nullptr) {
		_textBoundWidth = textBoundWidth - faceTextureSize.x;
		_dialogPosition.x = dialogMargin[3] + dialogPosition.x + faceTextureSize.x;
		_dialogPosition.y = dialogMargin[0] + dialogPosition.y;
	}
	else {
		_textBoundWidth = textBoundWidth;
		_dialogPosition.x = dialogMargin[3] + dialogPosition.x;
		_dialogPosition.y = dialogMargin[0] + dialogPosition.y;
	}

	if (dialog->type == DialogType::CHOOSE) {
		_textBoundWidth = optionsPosition.x - faceTextureSize.x - 10; //CAMBIAR PARA AJUSTAR LAS OPCINES A LA DERECHA
	}

	//Textura dialogo
	textTexture = CreateTextTexture(dialog->font, actualText.c_str(), textColor, _textBoundWidth);
	app->render->DrawTexture(textTexture, _dialogPosition.x, _dialogPosition.y, 0, 0);
	
	//Imagen del personaje
	if (dialog->face_tex != nullptr) {
		app->render->DrawTexture(dialog->face_tex, dialogMargin[3] + dialogPosition.x, dialogMargin[0] + dialogPosition.y, 0, 0);
	}



	//Nombre personaje
	textNameTexture = CreateTextTexture(app->render->primary_font, dialog->name.c_str(), textColor, textNameBoundWidth);
	app->render->DrawTexture(textNameTexture, dialogMargin[3]+ dialogPosition.x + namePosition.x, dialogMargin[0] + dialogPosition.y + namePosition.y, 0, 0);


	//Opciones
	if (dialog->type == DialogType::CHOOSE) {

		//Textura opcion1
		options1NameTexture = CreateTextTexture(app->render->primary_font, dialog->option1.c_str(), (optionSelected == 1) ? optionSelectedColor : optionColor, optionsBoundWidth);
		app->render->DrawTexture(options1NameTexture, dialogMargin[3] + dialogPosition.x + optionsPosition.x, dialogMargin[0] + dialogPosition.y + optionsDistanceBetween, 0, 0);

		//Textura opcion2
		options2NameTexture = CreateTextTexture(app->render->primary_font, dialog->option2.c_str(), (optionSelected == 2) ? optionSelectedColor : optionColor, optionsBoundWidth);
		app->render->DrawTexture(options2NameTexture, dialogMargin[3] + dialogPosition.x + optionsPosition.x, dialogMargin[0] + dialogPosition.y + optionsDistanceBetween*2, 0, 0);
	}


	

	//Optimizacion de la memoria
	SDL_DestroyTexture(textTexture);
	SDL_DestroyTexture(textNameTexture);
	SDL_DestroyTexture(options1NameTexture);
	SDL_DestroyTexture(options2NameTexture);




	//Añadir caracter segun el tiempo
	if (actualText.size() < dialog->sentence.size()) {

		if (charTimer.ReadMSec() >= charTimeMS) {
			indexText++;
			charTimer.Start();
		}
		
		return false;
	}

	return true;
}

SDL_Texture* DialogManager::CreateTextTexture(TTF_Font* font, const char* text, SDL_Color color, int textBoundWidth)
{
	SDL_Surface* textSurface = nullptr;
	SDL_Texture* textTexture = nullptr;

	

	textSurface = TTF_RenderUTF8_Blended_Wrapped(font, text, color, textBoundWidth);
	textTexture = SDL_CreateTextureFromSurface(app->render->renderer, textSurface);

	SDL_FreeSurface(textSurface);

	return textTexture;
}

TTF_Font* DialogManager::FontSelector(const char* font)
{

	if (std::strcmp(font, "primary") == 0) {
		return app->render->primary_font;

	}
	else if (std::strcmp(font, "secondary") == 0) {
		return app->render->secondary_font;
	}


	//Por defecto
	return app->render->primary_font;
}

bool DialogManager::Update(float dt) {

	bool ret = true;

	//Para saber si hay algun dialogo en funcionamiento
	isPlaying = (dialogues.Count() > 0);

	if (isPlaying) { //Entonces mostrar dialogos
		

		Dialog* actualDialog = dialogues.At(0)->data;
		bool dialogFinished = ShowDialog(actualDialog);



		//Gestionar la opcion seleccionada
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {
			optionSelected = 1;
		}
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) {
			optionSelected = 2;
		}


		//Siguiente dialogo
		if (dialogFinished && app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && actualDialog->type != DialogType::CHOOSE) {
			
			
			indexText = 1;
			dialogues.Del(dialogues.At(0));
			
			


		}
		//Gestion de las opciones
		else if (dialogFinished && app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && optionSelected != 0 && actualDialog->type == DialogType::CHOOSE) {
		
			
			if (optionSelected == 1) {
				dialogues.InsertAfter(0, actualDialog->options1);
			}
			else if (optionSelected == 2) {
				dialogues.InsertAfter(0, actualDialog->options2);
			}
			

			//Reiniciar varialbes de dialogo y quitar el dialogo actual de su lista
			optionSelected = 0;
			indexText = 1;
			dialogues.Del(dialogues.At(0));

		}
		//Terminar el dialogo empezado
		else if (!dialogFinished && app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && indexText > 2) {
			indexText = 999;
		}
		


	}
	else {
		//Reiniciar variables mientras no este algun dialogo en marcha
		indexText = 1;
		optionSelected = 0;
	}

	return ret;

}

