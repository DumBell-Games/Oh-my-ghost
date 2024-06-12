#ifndef __TEXTURES_H__
#define __TEXTURES_H__

#include "Module.h"

#include "List.h"

#include "SDL_image/include/SDL_image.h"

#include <memory>

struct SDL_Texture;
struct SDL_Surface;

using shared_texture_t = std::shared_ptr<SDL_Texture>;

struct TextureStruct
{
	SString path;
	shared_texture_t texture;
};

class Textures : public Module
{
public:

	Textures(bool startEnabled = true);

	// Destructor
	virtual ~Textures();

	// Called before render is available
	bool Awake(pugi::xml_node config);

	// Called before the first frame
	bool Start();

	// Called before quitting
	bool CleanUp();

	// Load Texture
	SDL_Texture* const Load(const char* path);
	SDL_Texture* const LoadSurface(SDL_Surface* surface);
	shared_texture_t const LoadSP(const char* path, bool preventDuplicates = false);
	bool UnLoad(SDL_Texture* texture);
	void UnLoadSP(shared_texture_t texture);
	SDL_Texture* Add(SDL_Texture* texture);
	void GetSize(const SDL_Texture* texture, uint& width, uint& height) const;

public:
	List<TextureStruct> noDuplicateTextureList;
	List<SDL_Texture*> textures;
};


#endif // __TEXTURES_H__