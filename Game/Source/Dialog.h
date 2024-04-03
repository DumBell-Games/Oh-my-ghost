#ifndef __DIALOG_H__
#define __DIALOG_H__

#include "Point.h"
#include "Input.h"
#include "Render.h"
#include "List.h"


enum class DialogType
{
	TEXT,
	CHOOSE
};

class Dialog
{
public:

	Dialog(std::string sentence) : sentence(sentence) {}

	virtual bool CleanUp()
	{

		ListItem<Dialog*>* item;
		Dialog* pDialog = nullptr;

		for (item = options1.start; item != NULL; item = item->next)
		{
			pDialog = item->data;
			SDL_DestroyTexture(pDialog->face_tex);
		}
		options1.Clear();

		for (item = options2.start; item != NULL; item = item->next)
		{
			pDialog = item->data;
			SDL_DestroyTexture(pDialog->face_tex);
		}
		options2.Clear();

		return true;
	}




public:

	//Definir el tipo del dialogo, si es normal o de elecciones
	DialogType type;
	
	//Frase del dialogo
	std::string sentence;

	//Nombre de quien realiza el dialogo
	std::string name;

	//Texturas
	SDL_Texture* face_tex = nullptr;
	const char* face_tex_path;

	//El texto de las opciones
	std::string option1;
	std::string option2;

	//El resultado al escoger dichas opciones
	List<Dialog*> options1;
	List<Dialog*> options2;

	TTF_Font* font;
	
};

#endif // __DIALOG_H__