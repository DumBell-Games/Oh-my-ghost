#ifndef __DIALOG_H__
#define __DIALOG_H__

#include "Point.h"
#include "Input.h"
#include "Render.h"
#include "List.h"
#include "Textures.h"


enum class DialogType
{
	TEXT,
	CHOOSE
};

enum class DialogEvent : bool
{
	NO_EVENT,
	TIENESDINERO,
	PAGARBIRRA,
	BIRRAOBTENIDA,
	HASHABLADOCONSISEBUTO,
	ARCADEDESBLOQUEADO,
	TARGETAOBTENIDA,
	VIPCOMPLETADO,
	SUPERVIPCOMPLETADO,
	CASINODESBLOQUEADO,
	CRUPIER2DESBLOQUEADA,
	MANSIONDESBLOQUEADA,
	ENCOMBATE,
	HASGANADO,
	HASPERDIDO,
	HASMUERTO

};

class Dialog
{
public:

	Dialog(std::string sentence) : sentence(sentence) {}

	~Dialog()
	{
		CleanUp();
	}

	virtual bool CleanUp()
	{

		ListItem<Dialog*>* item;
		Dialog* pDialog = nullptr;

		for (item = options1.start; item != NULL; item = item->next)
		{
			pDialog = item->data;
			RELEASE(pDialog);
		}
		options1.Clear();

		for (item = options2.start; item != NULL; item = item->next)	
		{
			pDialog = item->data;
			RELEASE(pDialog);
		}
		options2.Clear();

		return true;
	}




public:

	//Definir el tipo del dialogo, si es normal o de elecciones
	DialogType type;

	DialogEvent dEvent;
	
	//Frase del dialogo
	std::string sentence;

	//Nombre de quien realiza el dialogo
	std::string name;

	int id;
	int mapId;

	shared_texture_t background_tex = nullptr;

	//Texturas
	shared_texture_t face_tex = nullptr;
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