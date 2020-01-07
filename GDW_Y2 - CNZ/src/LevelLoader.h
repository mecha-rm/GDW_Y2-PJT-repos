#pragma once
#include <string>
#include <map>
#include <vector>
#include <iterator>
#include "CSV.h"

class Level {
public:
	Level();
	Level(std::string legendPath, std::string levelPath);

	std::map<std::string, std::string> legend; // legend lookup table
	std::vector<std::vector<std::string>> map; // 2d vector of strings, the scene map

	std::string sceneName; // optional scene ID. Should be set automatically by the game if it is not set by the level itself. If not set by level, set to ""
	// int obstacleNum; // a count of the obstacles in the scene. Maybe unneeded.
	std::string levelPath; // path to the level CSV
	std::string legendPath; // path to the legend CSV
	bool isActiveScene; // if the scene is loaded

	// load legend into map
	bool loadLegend(std::string legendPath);

	// load level into 2d vector
	bool loadLevel(std::string levelPath);

	// get level size
	std::vector<int> getMapSize(CSV level);

	// get level symbols loaded into 2d vector called map
	std::vector<std::vector<std::string>> getMap(CSV level);


private:

};