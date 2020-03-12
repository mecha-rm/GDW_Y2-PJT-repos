#include "CNZ_Game.h"
#include <GLM/gtc/matrix_transform.hpp> // camera modes
#include "cherry/scenes/SceneManager.h"
#include "scenes/CNZ_GameplayScene.h"

float GetDistance(cherry::Vec3 one, cherry::Vec3 two);
cherry::Vec3 GetUnitDirVec(cherry::Vec3 one, cherry::Vec3 two);
cherry::Vec3 LERP(cherry::Vec3 start, cherry::Vec3 end, float percent);

// constructor
cnz::CNZ_Game::CNZ_Game() : Game() {}

// constructor with window size
cnz::CNZ_Game::CNZ_Game(float windowWidth, float windowHeight, bool fullScreen) 
	: Game("GDW_Y2 - Bonus Fruit - Codename: ZERO", windowWidth, windowHeight, fullScreen, false, nullptr, false) {
}


// loads content
void cnz::CNZ_Game::LoadContent()
{
	// GL debug output
	glEnable(GL_DEBUG_OUTPUT);

	Game::LoadContent(); // calls the load content
	
	LevelLoadInfo map1Info{ "res/loader/legend.csv", "res/loader/map1.csv", "map1" };
						 
	// loads in the first map
	CNZ_GameplayScene* map1Scene = new CNZ_GameplayScene(map1Info.legendPath, map1Info.levelPath, map1Info.sceneName);
	RegisterScene(map1Scene, true); // this is the current scene.
}

// Update function
void cnz::CNZ_Game::Update(float deltaTime)
{
	// calls the main game Update function to go through every object.
	Game::Update(deltaTime);
}