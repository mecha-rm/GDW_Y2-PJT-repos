#include "CNZ_Game.h"
#include <GLM/gtc/matrix_transform.hpp> // camera modes
#include "cherry/scenes/SceneManager.h"
#include "scenes/CNZ_GameplayScene.h"
#include "scenes/CNZ_TitleScene.h"

// constructor
cnz::CNZ_Game::CNZ_Game() : Game() {}

// constructor with window size
cnz::CNZ_Game::CNZ_Game(float windowWidth, float windowHeight, bool fullScreen) 
	: Game("GDW_Y2 - Bonus Fruit - Codename: ZERO", windowWidth, windowHeight, fullScreen, false, nullptr, false) {
}


// loads content
void cnz::CNZ_Game::LoadContent()
{
	// 30 frames per second
	FPS = 30;

	// GL debug output
	glEnable(GL_DEBUG_OUTPUT);

	Game::LoadContent(); // calls the load content
	
	LevelLoadInfo map1Info{ "res/loader/legend.csv", "res/loader/map1.csv", "map1" };
						 
	// Title Scene
	if(false)
	{
		CNZ_TitleScene* title = new CNZ_TitleScene("Title Screen");

		// loads in the first map
		CNZ_GameplayScene* map1Scene = new CNZ_GameplayScene(map1Info.legendPath, map1Info.levelPath, map1Info.sceneName);
		title->destScene = map1Scene->GetName(); // destination

		RegisterScene(title, true);
		// RegisterScene(map1Scene, true); // this is the current scene.
		RegisterScene(map1Scene, false); // this is the current scene. 
	}
	else
	{
		// loads in the first map
		CNZ_GameplayScene* map1Scene = new CNZ_GameplayScene(map1Info.legendPath, map1Info.levelPath, map1Info.sceneName);

		RegisterScene(map1Scene, true);
	}

	// generates source objects.
	Level::GenerateSources();
}

// Update function
void cnz::CNZ_Game::Update(float deltaTime)
{
	// calls the main game Update function to go through every object.
	Game::Update(deltaTime);
}