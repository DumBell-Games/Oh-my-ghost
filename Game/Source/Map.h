#ifndef __MAP_H__
#define __MAP_H__


#include "Module.h"
#include "List.h"
#include "Point.h"
#include "PropertiesStruct.h"
#include "Pathfinding.h"

#include "PugiXml\src\pugixml.hpp"

// Ignore Terrain Types and Tile Types for now, but we want the image!
struct TileSet
{
	SString	name;
	int	firstgid;
	int margin;
	int	spacing;
	int	tileWidth;
	int	tileHeight;
	int columns;
	int tilecount;

	SDL_Texture* texture;
	SDL_Rect GetTileRect(int gid) const;
};

//  We create an enum for map type, just for convenience,
// NOTE: Platformer game will be of type ORTHOGONAL
enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};

struct MapLayer
{
	SString	name;
	int id;
	int x;
	int y;
	int width;
	int height;
	uchar* data;

	Properties properties;

	MapLayer() : data(NULL)
	{}

	~MapLayer()
	{
		RELEASE(data);
	}

	inline uchar Get(int x, int y) const
	{
		return data[(y * width) + x];
	}
};

struct MapData
{
	int width;
	int	height;
	int	tileWidth;
	int	tileHeight;
	List<TileSet*> tilesets;
	MapTypes type;

	List<MapLayer*> maplayers;

	iPoint GetMapSize() const { return { width * tileWidth,height * tileHeight }; }
};

class Map : public Module
{
public:

	Map(bool _startEnabled = true);

	// Destructor
	virtual ~Map();

	// Called before render is available
	bool Awake(pugi::xml_node config);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(SString mapFileName);

	bool ChangeMap(int id);

	iPoint MapToWorld(int x, int y) const;
	iPoint Map::WorldToMap(int x, int y);

private:

	bool LoadMap(pugi::xml_node mapFile);
	bool LoadTileSet(pugi::xml_node mapFile);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadAllLayers(pugi::xml_node mapNode);
	TileSet* GetTilesetFromTileId(int gid) const;
	bool LoadProperties(pugi::xml_node& node, Properties& properties);
	bool LoadAllObjects(pugi::xml_node mapNode);

	bool LoadEntity(pugi::xml_node objGroupNode, pugi::xml_node objNode, char entityType);

	bool LoadRectangle(pugi::xml_node objGroupNode, pugi::xml_node objectNode);
	bool LoadCircle(pugi::xml_node objGroupNode, pugi::xml_node objectNode);
	bool LoadPolygon(pugi::xml_node objGroupNode, pugi::xml_node objectNode);

	bool Unload();

public:

	MapData mapData;
	SString path;
	PathFinding* pathfinding;
	MapLayer* navigationLayer;

private:

	bool mapLoaded;
	int blockedGid = 49; // TODO asignar el Gid correcto

	List<SString> mapNames;
	int currentMap = 0;
};

#endif // __MAP_H__