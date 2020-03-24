#include "CNZ_Game.h"
#include <GLM/gtc/matrix_transform.hpp> // camera modes
#include "cherry/scenes/SceneManager.h"
#include "scenes/CNZ_GameplayScene.h"
#include "scenes/CNZ_TitleScene.h"
#include "scenes/CNZ_RankingScene.h"

// constructor
cnz::CNZ_Game::CNZ_Game() : Game() {}

// constructor with window size
cnz::CNZ_Game::CNZ_Game(float windowWidth, float windowHeight, bool fullScreen) 
	: Game("GDW_Y2 - Bonus Fruit - Codename: ZERO", windowWidth, windowHeight, fullScreen, false, nullptr, false) {
}


// loads content
void cnz::CNZ_Game::LoadContent()
{
	// if 'true', then the menu is loaded
	bool loadMenu = true; // if 'true', it loads in from the menu.

	// 30 frames per second
	FPS = 30;

	// GL debug output
	glEnable(GL_DEBUG_OUTPUT);

	Game::LoadContent(); // calls the load content
	
	// Menus
	titleSceneName = "Title Screen";
	rankingSceneName = "Ranking";

	// Levels
	map1Info = LevelLoadInfo{ "res/loader/legend.csv", "res/loader/map1.csv", "map1" };
	map2Info = LevelLoadInfo{ "res/loader/legend.csv", "res/loader/map2.csv", "map2" };
	map3Info = LevelLoadInfo{ "res/loader/legend.csv", "res/loader/map3.csv", "map3" };
	
	// creating the scenes
	CNZ_TitleScene* title = new CNZ_TitleScene(titleSceneName);
	RegisterScene(title, false);

	// creating the scenes
	CNZ_RankingScene* ranking = new CNZ_RankingScene(rankingSceneName);
	RegisterScene(ranking, false);


	// CNZ_GameplayScene* map1Scene = new CNZ_GameplayScene(map1Info.legendPath, map1Info.levelPath, map1Info.sceneName);
	// maps
	CNZ_GameplayScene* map1Scene = new CNZ_GameplayScene(map1Info);
	RegisterScene(map1Scene, false);

	CNZ_GameplayScene* map2Scene = new CNZ_GameplayScene(map2Info);
	RegisterScene(map2Scene, false);

	CNZ_GameplayScene* map3Scene = new CNZ_GameplayScene(map3Info);
	RegisterScene(map3Scene, false);

	// generates the source objects.
	Level::GenerateSources();

	if (loadMenu) // load menu
	{
		SetCurrentScene(title->GetName(), false);
	}
	else // load level
	{
		SetCurrentScene(map1Scene->GetName(), false);
	}


	// Title Scene
	// if(loadMenu)
	// {
	// 	CNZ_TitleScene* title = new CNZ_TitleScene("Title Screen");
	// 
	// 	// loads in the first map
	// 	CNZ_GameplayScene* map1Scene = new CNZ_GameplayScene(map1Info.legendPath, map1Info.levelPath, map1Info.sceneName);
	// 	title->destScene = map1Scene->GetName(); // destination
	// 
	// 	RegisterScene(title, true);
	// 	// RegisterScene(map1Scene, true); // this is the current scene.
	// 	RegisterScene(map1Scene, false); // this is the current scene. 
	// }
	// else
	// {
	// 	// loads in the first map
	// 	// map 1
	// 	// CNZ_GameplayScene* mapScene = new CNZ_GameplayScene(map1Info.legendPath, map1Info.levelPath, map1Info.sceneName);
	// 
	// 	// map 2
	// 	CNZ_GameplayScene* mapScene = new CNZ_GameplayScene(map2Info.legendPath, map2Info.levelPath, map2Info.sceneName);
	// 
	// 	RegisterScene(mapScene, true);
	// }
}

// Update function
void cnz::CNZ_Game::Update(float deltaTime)
{
	// calls the main game Update function to go through every object.
	Game::Update(deltaTime);
}