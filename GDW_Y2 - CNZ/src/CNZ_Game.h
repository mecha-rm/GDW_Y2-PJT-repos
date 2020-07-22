/*
 * Name: Bonus Fruit
 * Date: 07/20/2020
 * Description: game file for Codename: ZERO
 * References:
*/

#pragma once

// macros
#define RANKING_FILE "res/data/ranking.txt"
#define RANKING_SCORES 10
#define NAME_CHAR_LIMIT 16
#define SCORE_DIGIT_LIMIT 8

#include "cherry/Game.h"
#include "Player.h"
#include "EnemyTypes/Enemies.h"
#include "Obstacle.h"
#include "cherry/physics/PhysicsBody.h"
#include "Projectile.h"
#include "cherry/animate/Path.h"
#include "LevelLoader.h"

//Enemy Sub-classes
#include "EnemyTypes/Enemies.h"

#include <ctime>

// inherits from the game class.
namespace cnz
{
	// class for the codename zero game.
	class CNZ_Game : public cherry::Game
	{
	public:
		// creates the game with default values for the screen size
		CNZ_Game();

		// creates a game window with the provided screen size
		CNZ_Game(float windowWidth, float windowHeight, bool fullScreen);

		// overwritten function for loading in game content.
		virtual void LoadContent() override;

		// overwritten unload content.
		virtual void UnloadContent() override;

		// overwritten from the game class's data.
		virtual void Update(float deltaTime) override;

		// Audio Engine
		//cherry::AudioEngine& audioEngine = cherry::AudioEngine::GetInstance();

		// name of the title scene.
		std::string titleSceneName = "";

		// name of the score scene.
		std::string rankingSceneName = "";

		// game over scene name.
		std::string gameOverSceneName = "";

		// maps; do not make changes.
		LevelLoadInfo map1Info;
		LevelLoadInfo map2Info;
		LevelLoadInfo map3Info;

	protected:


	private:

		// profiles game (performance check)
		const bool PROFILE = false;

	};
}
