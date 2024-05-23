#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
#include "Physics.h"
#include "Reload.h"

#include <sstream>
#include <string.h>

#include "Defs.h"
#include "Log.h"

#include <math.h>
#include "SDL_image/include/SDL_image.h"
#include "DebugConsole.h"

Map::Map(bool startEnabled) : Module(startEnabled), mapLoaded(false)
{
    name.Create("map");
}

// Destructor
Map::~Map()
{}

// Called before render is available
bool Map::Awake(pugi::xml_node config)
{
    LOG("Loading Map Parser");
    bool ret = true;

    path = config.attribute("path").as_string();

    for (pugi::xml_node& item = config.child("map"); item != NULL; item = item.next_sibling("map"))
    {
        mapNames.Add(item.attribute("file").as_string("invalidMap"));
    }
    if (mapNames.Count() == 0) {
        LOG("No maps in config. Make sure filepaths are in config");
        ret = false;
    }

    return ret;
}

bool Map::Start() {
    app->console->AddCommand("warpto", "Teletransporta al jugador al mapa (y opcionalmente entrada) especificados", "warpto mapId [doorId]", [this](std::vector<std::string> args) {WarpTo(this, args); });
    //Calls the functon to load the map, make sure that the filename is assigned

    SString mapPath = path;
    mapPath += mapNames[currentMap];
    bool ret = Load(mapPath);

    return ret;
}

bool Map::Update(float dt)
{
    if (mapLoaded == false)
        return false;

    const SDL_Rect& camRect = app->render->camera;
    SDL_Rect bounds;
    bounds.x = -camRect.x;// +camRect.w / 2;
    bounds.y = -camRect.y;// +camRect.h / 2;
    bounds.w = camRect.w;
    bounds.h = camRect.h;
    LOG("rawcamX:%i\nrawcamY:%i", camRect.x, camRect.y);
    LOG("boundsX:%i\nboundsY:%i", bounds.x, bounds.y);

    {
        iPoint tmp = WorldToMap(bounds.x, bounds.y);
        bounds.x = tmp.x;
        bounds.y = tmp.y;
        tmp = WorldToMap(bounds.w, bounds.h);
        bounds.w = tmp.x;
        bounds.h = tmp.y;
        LOG("mapX:%i\nmapY:%i", bounds.x, bounds.y);
    }


    ListItem<MapLayer*>* mapLayerItem;
    mapLayerItem = mapData.maplayers.start;

    while (mapLayerItem != NULL) {

        if (mapLayerItem->data->properties.GetProperty("Draw") != NULL && mapLayerItem->data->properties.GetProperty("Draw")->value) {

            for (int x = 0; x < mapLayerItem->data->width && x-2 < bounds.x + bounds.w; x++)
            {
                if (x < bounds.x)
                    continue;

                for (int y = 0; y < mapLayerItem->data->height && y-2 < bounds.y + bounds.h; y++)
                {
                    if (y < bounds.y)
                        continue;

                    int gid = mapLayerItem->data->Get(x, y);
                    TileSet* tileset = GetTilesetFromTileId(gid);

                    if (!tileset) continue;

                    SDL_Rect r = tileset->GetTileRect(gid);
                    iPoint pos = MapToWorld(x, y);

                    app->render->DrawTexture(tileset->texture,
                        pos.x,
                        pos.y,
                        &r);
                }
            }
        }
        mapLayerItem = mapLayerItem->next;

    }


    return true;
}

iPoint Map::MapToWorld(int x, int y) const
{
    iPoint ret;

    ret.x = x * mapData.tileWidth;
    ret.y = y * mapData.tileHeight;

    return ret;
}

iPoint Map::WorldToMap(int x, int y)
{
    iPoint ret(0, 0);

    ret.x = x / mapData.tileWidth;
    ret.y = y / mapData.tileHeight;

    return ret;
}

// Get relative Tile rectangle
SDL_Rect TileSet::GetTileRect(int gid) const
{
    SDL_Rect rect = { 0 };
    int relativeIndex = gid - firstgid;

    rect.w = tileWidth;
    rect.h = tileHeight;
    rect.x = margin + (tileWidth + spacing) * (relativeIndex % columns);
    rect.y = margin + (tileWidth + spacing) * (relativeIndex / columns);

    return rect;
}

TileSet* Map::GetTilesetFromTileId(int gid) const
{
    if (gid == 0) return nullptr;
    ListItem<TileSet*>* item = mapData.tilesets.start;
    TileSet* set = NULL;

    while (item)
    {
        set = item->data;
        if (gid < (item->data->firstgid + item->data->tilecount))
        {
            break;
        }
        item = item->next;
    }

    return set;
}

// Called before quitting
bool Map::CleanUp()
{
    Unload();

    return true;
}

// Load new map
bool Map::Load(SString mapFileName)
{
    bool ret = true;

    pugi::xml_document mapFileXML;
    pugi::xml_parse_result result = mapFileXML.load_file(mapFileName.GetString());

    if (result == NULL)
    {
        LOG("Could not load map xml file %s. pugi error: %s", mapFileName.GetString(), result.description());
        ret = false;
    }

    if (ret == true)
    {
        ret = LoadMap(mapFileXML);
    }

    if (ret == true)
    {
        ret = LoadTileSet(mapFileXML);
    }

    if (ret == true)
    {
        pathfinding = new PathFinding();
        ret = LoadAllLayers(mapFileXML.child("map"));
    }

    if (ret == true)
    {
        ret = LoadAllObjects(mapFileXML.child("map"));
    }

    if (ret == true)
    {
        LOG("Successfully parsed map XML file :%s", mapFileName.GetString());
        LOG("width : %d height : %d", mapData.width, mapData.height);
        LOG("tile_width : %d tile_height : %d", mapData.tileWidth, mapData.tileHeight);

        LOG("Tilesets----");

        ListItem<TileSet*>* tileset;
        tileset = mapData.tilesets.start;

        while (tileset != NULL) {
            LOG("name : %s firstgid : %d", tileset->data->name.GetString(), tileset->data->firstgid);
            LOG("tile width : %d tile height : %d", tileset->data->tileWidth, tileset->data->tileHeight);
            LOG("spacing : %d margin : %d", tileset->data->spacing, tileset->data->margin);
            tileset = tileset->next;
        }

        LOG("Layers----");

        ListItem<MapLayer*>* mapLayer;
        mapLayer = mapData.maplayers.start;

        while (mapLayer != NULL) {
            LOG("id : %d name : %s", mapLayer->data->id, mapLayer->data->name.GetString());
            LOG("Layer width : %d Layer height : %d", mapLayer->data->width, mapLayer->data->height);
            mapLayer = mapLayer->next;
        }

    }

    if (mapFileXML) mapFileXML.reset();

    mapLoaded = ret;

    return ret;
}

bool Map::Unload()
{
    LOG("Unloading map");

    ListItem<TileSet*>* item;
    item = mapData.tilesets.start;

    while (item != NULL)
    {
        app->tex->UnLoad(item->data->texture);
        RELEASE(item->data);
        item = item->next;
    }
    mapData.tilesets.Clear();

    // Remove all layers
    ListItem<MapLayer*>* layerItem;
    layerItem = mapData.maplayers.start;

    while (layerItem != NULL)
    {
        RELEASE(layerItem->data);
        layerItem = layerItem->next;
    }

    mapData.maplayers.Clear();

    RELEASE(pathfinding);

    app->console->RemoveCommand("warpto");

    return true;
}

// If parameter is -1 just reloads the current map
bool Map::ChangeMap(int id)
{
    currentMap = ((id <= -1) ? currentMap : id);

    return app->reload->QueueReload("loadMap");
}

bool Map::LoadMap(pugi::xml_node mapFile)
{
    bool ret = true;
    pugi::xml_node map = mapFile.child("map");

    if (map == NULL)
    {
        LOG("Error parsing map xml file: Cannot find 'map' tag.");
        ret = false;
    }
    else
    {
        //Load map general properties
        mapData.height = map.attribute("height").as_int();
        mapData.width = map.attribute("width").as_int();
        mapData.tileHeight = map.attribute("tileheight").as_int();
        mapData.tileWidth = map.attribute("tilewidth").as_int();
        mapData.type = MAPTYPE_UNKNOWN;
    }

    return ret;
}

bool Map::LoadTileSet(pugi::xml_node mapFile) {

    bool ret = true;

    pugi::xml_node tileset;
    for (tileset = mapFile.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
    {
        TileSet* set = new TileSet();

        set->name = tileset.attribute("name").as_string();
        set->firstgid = tileset.attribute("firstgid").as_int();
        set->margin = tileset.attribute("margin").as_int();
        set->spacing = tileset.attribute("spacing").as_int();
        set->tileWidth = tileset.attribute("tilewidth").as_int();
        set->tileHeight = tileset.attribute("tileheight").as_int();
        set->columns = tileset.attribute("columns").as_int();
        set->tilecount = tileset.attribute("tilecount").as_int();

        SString texPath = path;
        texPath += tileset.child("image").attribute("source").as_string();
        set->texture = app->tex->Load(texPath.GetString());

        if (tileset.child("tile").child("animation")) {
			LoadAnimation(tileset.child("tile"), set);
		}
        else
        {
            mapData.tilesets.Add(set);
        }
    }

    return ret;
}

bool Map::LoadAnimation(pugi::xml_node node, TileSet* tileset)
{
    bool ret = true;
    Animation* anim = new Animation();

    anim->name = tileset->name;
   

    for (pugi::xml_node frame = node.child("animation").child("frame"); frame && ret; frame = frame.next_sibling("frame"))
    {
		int id = frame.attribute("tileid").as_int();
        int duration = frame.attribute("duration").as_int();
        int tilesPerRow = tileset->columns;
        int x = (id % tilesPerRow) * tileset->tileWidth;
        int y = (id / tilesPerRow) * tileset->tileHeight;
        anim->PushBack({ x, y, tileset->tileWidth, tileset->tileHeight }, 0);
	}

    mapData.animations.Add(anim);

    return ret;
}

bool Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
    bool ret = true;

    //Load the attributes
    layer->id = node.attribute("id").as_int();
    layer->name = node.attribute("name").as_string();
    layer->width = node.attribute("width").as_int();
    layer->height = node.attribute("height").as_int();

    LoadProperties(node, layer->properties);

    //Reserve the memory for the data 
    uint size = layer->width * layer->height;
    layer->data = new uint[size];
    memset(layer->data, 0, size);

    //Iterate over all the tiles and assign the values
    pugi::xml_node tile;
    uint i = 0;
    for (tile = node.child("data").child("tile"); tile && ret && i < size; tile = tile.next_sibling("tile"))
    {
        layer->data[i] = tile.attribute("gid").as_uint();
        i++;
    }

    if (layer->name == "Navigation") {
        navigationLayer = layer;
        pathfinding->SetNavigationMap(layer->width, layer->height, layer->data);
    }

    return ret;
}

bool Map::LoadAllLayers(pugi::xml_node mapNode) {
    bool ret = true;

    for (pugi::xml_node layerNode = mapNode.child("layer"); layerNode && ret; layerNode = layerNode.next_sibling("layer"))
    {
        //Load the layer
        MapLayer* mapLayer = new MapLayer();
        ret = LoadLayer(layerNode, mapLayer);

        //add the layer to the map
        mapData.maplayers.Add(mapLayer);
    }

    return ret;
}

bool Map::LoadAllObjects(pugi::xml_node mapNode) {
    bool ret = true;

    for (pugi::xml_node objGroupNode = mapNode.child("objectgroup"); objGroupNode && ret; objGroupNode = objGroupNode.next_sibling("objectgroup"))
    {
        Properties prop;
        LoadProperties(objGroupNode, prop);

        for (pugi::xml_node objNode = objGroupNode.child("object"); objNode && ret; objNode = objNode.next_sibling("object"))
        {
            // If object is an entity and the entity type is defined, load as entity
            Properties::Property* p = prop.GetProperty("entity");
            if (p != nullptr && p->intVal > 0) {
                LoadEntity(objGroupNode, objNode, p->intVal);
            }
            else if (objNode.child("ellipse")) {
                LoadCircle(objGroupNode, objNode);
            }
            else if (objNode.child("polygon")) {
                LoadPolygon(objGroupNode, objNode);
            }
            else {
                LoadRectangle(objGroupNode, objNode);
            }
        }
    }

    return ret;
}

bool Map::LoadEntity(pugi::xml_node objGroupNode, pugi::xml_node objNode, char entityType)
{
    Entity* entity = app->entityManager->CreateEntity(static_cast<EntityType>(entityType), objNode);
    // TODO change all past this line until return to be in each of the entities instead (custom initialization)
    int x = objNode.attribute("x").as_int();
    int y = objNode.attribute("y").as_int();
    if (entity == nullptr) {
        LOG("Entity of type %c could not be created at (%i,%i)", entityType, x, y);
        return true;
    }
    iPoint pos(x, y);
    entity->SetPosition(pos);

    return true;
}

bool Map::LoadRectangle(pugi::xml_node objGroupNode, pugi::xml_node objNode)
{
    bool ret = true;

    int id = objNode.attribute("id").as_int();
    float x = objNode.attribute("x").as_float();
    float y = objNode.attribute("y").as_float();
    float width = objNode.attribute("width").as_float();
    float height = objNode.attribute("height").as_float();

    PhysBody* object = app->physics->CreateRectangle(x + width / 2, y + height / 2, width, height, STATIC);
    object->ctype = ColliderType::PLATFORM;
    
    return ret;
}

bool Map::LoadCircle(pugi::xml_node objGroupNode, pugi::xml_node objNode)
{
    bool ret = true;

    int id = objNode.attribute("id").as_int();
    float x = objNode.attribute("x").as_float();
    float y = objNode.attribute("y").as_float();
    float width = objNode.attribute("radius").as_float();
    float height = objNode.attribute("height").as_float();

    float radius = (width + height) / 2; // TODO cambiar esto para elipses no regulares (nueva función de creación de elipses en el entitymanager?)

    PhysBody* object = app->physics->CreateCircle(x + radius, y + radius, radius, STATIC);
    object->ctype = ColliderType::PLATFORM;

    return true;
}

bool Map::LoadPolygon(pugi::xml_node objGroupNode, pugi::xml_node objNode)
{
    bool ret = true;

    float x = objNode.attribute("x").as_float();
    float y = objNode.attribute("y").as_float();

    for (pugi::xml_node polyNode = objNode.child("polygon"); polyNode && ret; polyNode = polyNode.next_sibling("polygon"))
    {
        SString pStr = polyNode.attribute("points").as_string();
        std::vector<SString> coords = pStr.GetWords(' ');
        std::vector<int> intPoints;

        for (SString item : coords)
        {
            std::vector<SString> coord = item.GetWords(',');
            if (coord.size() >= 2) {
                intPoints.push_back(std::stof(coord[0].GetString()));
                intPoints.push_back(std::stof(coord[1].GetString()));
            }
        }

        int* pointsArray = intPoints.data();
        int numPoints = intPoints.size();

        PhysBody* object = app->physics->CreateChain(x, y, pointsArray, numPoints, STATIC);
        object->ctype = ColliderType::PLATFORM;
    }
    return true;
}

static void WarpTo(Map* map, std::vector<std::string> args) {
    if (args.size() <= 1) throw std::invalid_argument("Se esperaba un id de mapa");
    if (args.size() >= 3)
        map->transitionData.targetDoorID = std::stoi(args[2]);
    map->transitionData.mapId = std::stoi(args[1]);
    map->ChangeMap(map->transitionData.mapId);
}

Animation* Map::GetAnimByName(SString name) {
    ListItem<Animation*>* item = mapData.animations.start;
    Animation* set = NULL;

    while (item) {
        set = item->data;
        if (item->data->name == name)
        {
            return set;
        }
        item = item->next;
    }
    return set;
}