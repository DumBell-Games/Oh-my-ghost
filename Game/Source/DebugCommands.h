#include <vector>
#include <string>
#include "App.h"
#include "Map.h"


static void WarpTo(std::vector<std::string> args) {
	if (args.size() <= 1) throw std::invalid_argument("Se esperaba un id de mapa");
	if (args.size() >= 3)
		app->map->transitionData.targetDoorID = std::stoi(args[2]);
	app->map->transitionData.mapId = std::stoi(args[1]);
	app->map->ChangeMap(app->map->transitionData.mapId);
}