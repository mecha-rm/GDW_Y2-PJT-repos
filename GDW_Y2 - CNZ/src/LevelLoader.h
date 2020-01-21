#pragma once
#include <string>
#include <map>
#include <vector>
#include <iterator>
#include "CSV.h"
#include "cherry/objects/Object.h"
#include "cherry/Material.h" 
#include "cherry/Game.h" // so that we can call AddObjectToScene

// object types so that we are able to create the objects in the loader
#include "Player.h"
#include "Enemies.h"
#include "Obstacle.h"
#include "cherry/PhysicsBody.h"
#include "Projectile.h"

class Level {
public:
	Level();
	Level(std::string legendPath, std::string levelPath, std::string sceneName);

	std::map<std::string, std::string> legend; // legend lookup table
	std::vector<std::vector<std::string>> map; // 2d vector of strings, the scene map
	std::vector<cherry::Object*> objList;	   // list of pointers to objects

	std::string sceneName; // optional scene ID. Should be set automatically by the game if it is not set by the level itself. If not set by level, set to ""
	// int obstacleNum; // a count of the obstacles in the scene. Maybe unneeded.
	std::string levelPath; // path to the level CSV
	std::string legendPath; // path to the legend CSV
	bool isActiveScene; // if the scene is loaded


	// return scene name as string
	std::string getSceneName();

	// load legend into map
	bool loadLegend(std::string legendPath);

	// load level into 2d vector
	bool loadLevel(std::string levelPath);

	// get level size
	std::vector<int> getMapSize(CSV level);

	// get level symbols loaded into 2d vector called map
	std::vector<std::vector<std::string>> getMap(CSV level);


	// return vector of all objects in level
	std::vector<cherry::Object*> getObjects();

	// return properties of object at specific location on map
	std::vector<float> getObjProps(int y, int x);

private:
	// the m_Scene material
	cherry::Material::Sptr matStatic; // the static material
	cherry::Material::Sptr matDynamic; // the dynamic material

	const float cellOffset = 6.25f;
};