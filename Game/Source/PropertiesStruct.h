#pragma once

#include "SString.h"
#include <SDL/include/SDL_pixels.h>
#include "List.h"

struct Properties
{
	struct Property
	{
		SString name;
		bool value;
		int intVal;
		float floatVal;
		SString strVal;
		SDL_Color colorVal;

	};

	~Properties()
	{
		//...
		ListItem<Property*>* item;
		item = list.start;

		while (item != NULL)
		{
			RELEASE(item->data);
			item = item->next;
		}

		list.Clear();
	}

	Property* GetProperty(const char* name);

	List<Property*> list;
};