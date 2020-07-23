#include "CNZ_Game.h"
#include <GLM/gtc/matrix_transform.hpp> // camera modes
#include "cherry/scenes/SceneManager.h"
#include "scenes/CNZ_GameplayScene.h"
#include "scenes/CNZ_TitleScene.h"
#include "scenes/CNZ_RankingScene.h"
#include "scenes/CNZ_GameOverScene.h"
#include "cherry/audio/AudioEngine.h"
#include "cherry/Instrumentation.h"

// constructor
cnz::CNZ_Game::CNZ_Game() : Game() {}

// constructor with window size
cnz::CNZ_Game::CNZ_Game(float windowWidth, float windowHeight, bool fullScreen) 
	: Game("GDW_Y2 - Bonus Fruit - Codename: ZERO", windowWidth, windowHeight, fullScreen, false, nullptr, false) 
{
}

// loads content
void cnz::CNZ_Game::LoadContent()
{
	// profiling enabled
	if(PROFILE)
		cherry::ProfilingSession::Start("profiling-game_load_content.json");

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
	gameOverSceneName = "Game Over";

	// Levels
	map1Info = LevelLoadInfo{ "res/loader/legend.csv", "res/loader/map1.csv", "map1" };
	map2Info = LevelLoadInfo{ "res/loader/legend.csv", "res/loader/map2.csv", "map2" };
	map3Info = LevelLoadInfo{ "res/loader/legend.csv", "res/loader/map3.csv", "map3" };
	
	// creating the scenes
	// title
	CNZ_TitleScene* title = new CNZ_TitleScene(titleSceneName);
	RegisterScene(title, false);

	// ranking listing
	CNZ_RankingScene* ranking = new CNZ_RankingScene(rankingSceneName);
	RegisterScene(ranking, false);

	// game over
	CNZ_GameOverScene* gameover = new CNZ_GameOverScene(gameOverSceneName);
	// gameover->SetScore(99999); // used for testing purposes.
	RegisterScene(gameover, false);


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
		// SetCurrentScene(gameover->GetName(), false); // score entry.
	}
	else // load level
	{
		SetCurrentScene(map1Scene->GetName(), false);
	}

	// Audio
	{
		// creates the audio instance
		cherry::AudioEngine& audio = cherry::AudioEngine::GetInstance();

		// Loading All Banks //
		audio.LoadBank("Master");

		// Loading All Events //
		// reloading an event does nothing.
		audio.LoadEvent("arrow");
		audio.LoadEvent("bgm_01");
		audio.LoadEvent("bgm_02");
		audio.LoadEvent("dash");
		audio.LoadEvent("enemy_death");
		audio.LoadEvent("enemy_death_01");
		audio.LoadEvent("enemy_death_02");
		audio.LoadEvent("enemy_death_03");
		audio.LoadEvent("enemy_death_04");
		audio.LoadEvent("footstep");
		audio.LoadEvent("menu_accept");
		audio.LoadEvent("menu_click");
		audio.LoadEvent("new_wave");
		audio.LoadEvent("quip_01");
		audio.LoadEvent("quip_02");
		audio.LoadEvent("quip_03");
		audio.LoadEvent("quip_04");
		audio.LoadEvent("quip_05");
		audio.LoadEvent("shield_hit");
		audio.LoadEvent("timestop");
	
		// bgm positions to turn down audio.
		audio.SetEventPosition("bgm_01", glm::vec3(0.0F, 0.0F, glm::mix(15.0F, 0.0F, 0.5F)));
		audio.SetEventPosition("bgm_02", glm::vec3(0.0F, 0.0F, glm::mix(15.0F, 0.0F, 0.5F)));

		// plays the event.
		audio.PlayEvent("bgm_01");
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

	// profiling enabled
	if (PROFILE)
		cherry::ProfilingSession::End();
}

// unload content
void cnz::CNZ_Game::UnloadContent()
{
	cherry::Game::UnloadContent();
}

// Update function
void cnz::CNZ_Game::Update(float deltaTime)
{
	// calls the main game Update function to go through every object.
	Game::Update(deltaTime);
}