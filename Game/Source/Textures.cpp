#include "App.h"
#include "Render.h"
#include "Textures.h"

#include "Defs.h"
#include "Log.h"

//#pragma comment(lib, "../Game/Source/External/SDL_image/libx86/SDL2_image.lib")

Textures::Textures(bool startEnabled) : Module(startEnabled)
{
	name.Create("textures");
}

// Destructor
Textures::~Textures()
{}

// Called before render is available
bool Textures::Awake(pugi::xml_node config)
{
	LOG("Init Image library");
	bool ret = true;

	// Load support for the PNG image format
	int flags = IMG_INIT_PNG;
	int init = IMG_Init(flags);

	if((init & flags) != flags)
	{
		LOG("Could not initialize Image lib. IMG_Init: %s", IMG_GetError());
		ret = false;
	}

	return ret;
}

// Called before the first frame
bool Textures::Start()
{
	LOG("start textures");
	bool ret = true;
	return ret;
}

// Called before quitting
bool Textures::CleanUp()
{
	LOG("Freeing textures and Image library");
	ListItem<SDL_Texture*>* item;

	for(item = textures.start; item != NULL; item = item->next)
	{
		SDL_DestroyTexture(item->data);
	}

	textures.Clear();
	noDuplicateTextureList.Clear();
	IMG_Quit();
	return true;
}

// Load new texture from file path
SDL_Texture* const Textures::Load(const char* path)
{
	SDL_Texture* texture = NULL;
	SDL_Surface* surface = NULL;

	if (strcmp(path,"") != 0) // Strings vacias crashean
		surface = IMG_Load(path);

	if(surface == NULL)
	{
		LOG("Could not load surface with path: %s. IMG_Load: %s", path, IMG_GetError());
	}
	else
	{
		texture = LoadSurface(surface);
		SDL_FreeSurface(surface);
	}

	return texture;
}

// Unload texture
bool Textures::UnLoad(SDL_Texture* texture)
{
	ListItem<SDL_Texture*>* item;

	for(item = textures.start; item != NULL; item = item->next)
	{
		if(texture == item->data)
		{
			SDL_DestroyTexture(item->data);
			textures.Del(item);
			return true;
		}
	}

	return false;
}

// No elimina realmente la textura, pero si los demás punteros son eliminados, la textura se eliminará
void Textures::UnLoadSP(shared_texture_t texture)
{
	for (ListItem<TextureStruct>* item = noDuplicateTextureList.start; item; item = item->next)
	{
		if (item->data.texture == texture) {
			noDuplicateTextureList.Del(item);
		}
	}
}

// Translate a surface into a texture
SDL_Texture* const Textures::LoadSurface(SDL_Surface* surface)
{
	SDL_Texture* texture = SDL_CreateTextureFromSurface(app->render->renderer, surface);

	if(texture == NULL)
	{
		LOG("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
	}
	else
	{
		textures.Add(texture);
	}

	return texture;
}

// Carga la textura como un smart pointer, lo cual significa que la textura no sera eliminada hasta que se borre la ultima referencia a ella. Proporciona el parametro preventDuplicates como true para guardar una referencia en el modulo para evitar duplicados (debera liberarse manualmente en ese caso)
shared_texture_t const Textures::LoadSP(const char* path, bool preventDuplicates)
{
	if (preventDuplicates && strcmp(path, "") != 0)
	for (ListItem<TextureStruct>* item = noDuplicateTextureList.start; item; item = item->next)
	{
		if (item->data.path == path)
			return item->data.texture;
	}

	shared_texture_t tex = shared_texture_t(Load(path), 
		[](SDL_Texture* t) {app->tex->UnLoad(t); });
	if (preventDuplicates && tex.get())
		noDuplicateTextureList.Add({ SString(path), tex });

	return tex;
}

// Retrieve size of a texture
void Textures::GetSize(const SDL_Texture* texture, uint& width, uint& height) const
{
	SDL_QueryTexture((SDL_Texture*)texture, NULL, NULL, (int*) &width, (int*) &height);
}
