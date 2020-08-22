#pragma once

// TODO: organize variables and change accessibility parameters.

// use these instead so that they can be updated with new objects easier.
// #define ORIGIN_SYMBOL "Origin"
// 
// #define WALL_SYMBOL "Wall"
// #define WALL_FILE "res/objects/GDW_1_Y2 - Wall Tile.obj"
// 
// #define DUMPSTER_SYMBOL "Dumpster"
// #define DUMPSTER_FILE "res/objects/props/Dumpster.obj"
// 
// #define LAMP_POST_SYMBOL "Lamp post"
// #define LAMP_POST_FILE "res/objects/props/Lamp_Side.obj"
// 
// #define LAMP_POST_CORNER_SYMBOL "Lamp post corner"
// #define LAMP_POST_CORNER_FILE "res/objects/props/Lamp_Corner.obj"
// 
// #define LAMP_POST_CORNER_SYMBOL "Lamp post middle"
// #define LAMP_POST_CORNER_FILE "res/objects/props/Lamp_Center.obj"
// 
// #define BARREL_SYMBOL "Barrel"
// #define BARREL_FILE "res/objects/props/drum.obj"
//  
// #define KATANA_SYMBOL "Katana"
// #define KATANA_FILE "res/objects/weapons/katana.obj"
// 
// #define PILLAR_SYMBOL "Pillar"
// #define PILLAR_FILE "res/objects/GDW_1_Y2 - Pillar.obj"
// 
// #define MANHOLE_SYMBOL "Manhole cover"
// #define MANHOLE_FILE "res/objects/props/manhole.obj"
// 
// #define MANHOLE_SYMBOL "Road"
// #define MANHOLE_FILE "res/objects/props/Road.obj"
// 
// #define SIDEWALK_SYMBOL "Sidewalk"
// #define SIDEWALK_FILE "res/objects/props/sidewalk.obj"

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
#include "EnemyTypes/Enemies.h"
#include "Obstacle.h"
#include "cherry/physics/PhysicsBody.h"
#include "Projectile.h"

namespace cnz
{
	// struct for level information.
	struct LevelLoadInfo
	{
		// the legend path for the level.
		std::string legendPath = "";

		// the level path for the level file.
		std::string levelPath = "";

		// the name of the level's scene.
		std::string sceneName = "";

		// the map number.
		int mapNumber = 0;
	};

	class Level {
	public:
		Level() = default;

		Level(std::string legendPath, std::string levelPath, std::string sceneName, int mapNumber);

		// return scene name as string
		std::string GetSceneName() const;

		// gets the number of the map
		int GetMapNumber() const;

		// load legend into map
		bool LoadLegend(std::string legendPath);

		// load level into 2d vector
		bool LoadLevel(std::string levelPath);

		// get level size
		std::vector<int> GetMapSize(CSV level);

		// get level symbols loaded into 2d vector called map
		std::vector<std::vector<std::string>> GetMap(CSV level);

		// gets the map row count (y-axis)
		int GetMapRowCount() const;

		// gets the map column count (x-axis)
		int GetMapColumnCount() const;

		// gets the map minimum limit
		const glm::vec2 GetMapMinimumLimit() const;

		// gets the map maximum limit
		const glm::vec2 GetMapMaximumLimit() const;

		// gets the map limits
		const glm::vec4 GetMapLimits() const;

		// generates the objects for the level, and returns them.
		// if objects have already been generated, the list is cleared.
		// do note that the level loader does NOT delete the objects it generates. 
		// They are put into an objectlist upon creation, so that should be deleted instead.
		std::vector<cherry::Object*> GenerateObjects();

		// generates the default objects and puts them into the scene.
		std::vector<cherry::Object*> GenerateDefaults();

		// generates the source objects. This should only be called once.
		// these objects are put in a random scene
		static void GenerateSources();

		// gets the objects for the scene. 
		// the vector will be empty if GenerateObjects() has not been called yet.
		std::vector<cherry::Object*> GetObjects() const;

		// gets the obstacles for the map. 
		// this vector will be empty if GenerateObjects() has not been called, or if there were no obstacles in the map.
		std::vector<cnz::Obstacle *> GetObstacles() const;

		// returns 'true' if the objects have been generated at least once.
		bool GetObjectsGenerated() const;

		// gets the player object.
		Player* GetPlayerObject() const;

		// gets the player's spawn position
		const cherry::Vec3 GetPlayerSpawnPosition() const;

		// gets the index in the map array that has the player's spawn position
		const glm::ivec2 GetPlayerSpawnIndex() const;

		// return properties of object at specific location on map
		std::vector<float> GetObjectProps(int y, int x);

		// hacky fix for rotated physics bodies. When getting PB size based on mesh mins and maxes, they are all rotated 90 deg on one axis
		cherry::Vec3 UnFlipVec3(cherry::Vec3 vecToFlip);

		std::map<std::string, std::string> legend; // legend lookup table
		std::vector<std::vector<std::string>> map; // 2d vector of strings, the scene map

		std::vector<cherry::Object*> objects;	   // list of pointers to objects
		std::vector<cnz::Obstacle*> obstacles; // list of pointers to obstacles

		// player object.
		Player* playerObj = nullptr;

		// optional scene ID. Should be set automatically by the game if it is not set by the level itself. If not set by level, set to ""
		std::string sceneName = "";
		
		// int obstacleNum; // a count of the obstacles in the scene. Maybe unneeded.
		std::string levelPath = ""; // path to the level CSV

		std::string legendPath = ""; // path to the legend CSV

		// the minimum and maximum of the map.
		glm::vec2 mapMinimum;
		glm::vec2 mapMaximum;

		// the number of the map.
		int mapNumber = 0;

		// initialization in cpp file.
		static const float CELL_OFFSET;

		// not really needed.
		// bool isActiveScene; // if the scene is loaded

		// source player object; copy this object for faster object construction.
		static Player* sourcePlayer;

		// source sentry object; copy this object for faster object construction.
		static Sentry* sourceSentry;

		// source oracle object; copy this object for faster object construction.
		static Oracle* sourceOracle;

		// source marauder object; copy this object for faster object construction.
		static Marauder* sourceMarauder;

		// source bastion object; copy this object for faster object construction.
		static Bastion* sourceBastion;

		// source mechaspider object; copy this object for faster object construction.
		static Mechaspider* sourceSpider;

		// source arrow base
		static Projectile* sourceArrow;

		// static Projectile* sourceProjectile;


	private:
		// becomes 'true' if the objects have been generated at least once.
		bool objectsGenerated = false;

		// the m_Scene material
		cherry::Material::Sptr matStatic; // the static material
		cherry::Material::Sptr matDynamic; // the dynamic material

		// the player's spawn point
		cherry::Vec3 playerSpawn{};
		glm::ivec2 playerSpawnIndex{};

		// TODO: I don't think these are being used properly.
		// Obstacles
		Obstacle* wall = nullptr;
		Obstacle* dumpster = nullptr;
		Obstacle* lampPost = nullptr;
		Obstacle* lampPostCorner = nullptr;
		Obstacle* lampPostMiddle = nullptr;
		Obstacle* barrel = nullptr;
		Obstacle* katana = nullptr;
		Obstacle* pillar = nullptr;
		Obstacle* manHoleCover = nullptr;
		Obstacle* road = nullptr;
		Obstacle* sidewalk = nullptr;
		Obstacle* building = nullptr;
		Obstacle* concrete = nullptr;
		Obstacle* metalBox = nullptr;
		Obstacle* woodenBox = nullptr;
		Obstacle* floodLight = nullptr;
		Obstacle* concretePillar = nullptr;
		Obstacle* shelves = nullptr;
		Obstacle* edgeGround = nullptr;
		Obstacle* bench = nullptr;
		Obstacle* tent = nullptr;

		// symbols
		static const std::string DIVIDER_SYM;
		static const std::string TFORM_SYM;
	};
}