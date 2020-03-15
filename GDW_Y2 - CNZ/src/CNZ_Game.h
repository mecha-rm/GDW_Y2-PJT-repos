#pragma once
#include "cherry/Game.h"
#include "Player.h"
#include "EnemyTypes/Enemies.h"
#include "Obstacle.h"
#include "cherry/physics/PhysicsBody.h"
#include "Projectile.h"
#include "cherry/animate/Path.h"
#include <ctime>
#include "LevelLoader.h"

//Enemy Sub-classes
#include "EnemyTypes/Enemies.h"

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

		// overwritten from the game class's data.
		virtual void Update(float deltaTime) override;

	protected:


	private:
	};
}
