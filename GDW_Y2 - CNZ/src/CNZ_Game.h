#pragma once
#include "cherry/Game.h"
#include "Player.h"
#include "Enemies.h"
#include "cherry/PhysicsBody.h"
#include <ctime>

//Enemy Sub-classes
#include "EnemyTypes/Bastion.h"
#include "EnemyTypes/Marauder.h"
#include "EnemyTypes/Mechaspider.h"
#include "EnemyTypes/Oracle.h"
#include "EnemyTypes/Sentry.h"

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

		// called when a mouse button has been pressed
		virtual void MouseButtonPressed(GLFWwindow* window, int button);

		// called when a mouse button has been pressed
		virtual void MouseButtonReleased(GLFWwindow* window, int button);

		// called when a key has been pressed; overloaded from the game's keyPressed
		virtual void KeyPressed(GLFWwindow* window, int key);

		// called when a key is being held down; overloaded from the game's keyHeld
		virtual void KeyHeld(GLFWwindow* window, int key);

		// called when a key has been released; overloaded from the game's keyReleased
		virtual void KeyReleased(GLFWwindow* window, int key);

		// get closest obstacle within a certain degrees of where the player is facing
		cherry::PhysicsBody* getClosestObstacle();

		// gets a list of enemies within a certain degrees of where the player is facing
		// we require the dash vector so that we can use a smaller one if the player is dashing towards an obstacle that
		// would case the dash to end when they collide with that obstacle.
		vector<cherry::Object*> getEnemiesInDash(cherry::Vec3 dashVec);

		// get the angle at which a Vec3 is facing in X and Y axis. 
		// can also be used to find the angle between two positions by getting passing in their difference
		float getXYAngle(cherry::Vec3 vec);

		void spawnEnemyGroup(int i);

	protected:

		// overwritten function for loading in game content.
		virtual void LoadContent();

		// overwritten from the game class's data.
		virtual void Update(float deltaTime);

	private:
		
		int curWave = 0; //Current enemy wave

		cnz::Player * playerObj = nullptr; // object for the player.

		//Load enemies // Create multiple enemies using these with only loading one .obj
		cnz::Enemies* sentry = nullptr; //Sentry enemy : Bowman
		cnz::Enemies* oracle = nullptr; //Oracle enemy : Polearmsman
		cnz::Enemies* marauder = nullptr; //Marauder enemy : Swordsman
		cnz::Enemies* bastion = nullptr; //Bastion enemy : Shield guy
		cnz::Enemies* mechaspider = nullptr; //Mechaspider enemy : Land mine
		cherry::Object* arrowBase = nullptr;

		std::vector<std::vector<Enemies*>> enemyGroups; //2D Vector of enemy groups [which group][what enemy in the group]

		cnz::Player* testObj = nullptr; // object for the player.
		bool mbLP = false, mbLR = false;

		vector<cherry::PhysicsBody*> obstaclePBs;
		vector<cherry::PhysicsBody*> enemyPBs;

		float camLerpPercent = 0.0f;

		glm::vec3 testPlayPos = glm::vec3(0, 0, 0);
		cherry::Vec3 playerPrevPos;

		// player movement
		bool w = false;
		bool a = false;
		bool s = false;
		bool d = false;
		bool cw = true;
		bool ca = true;
		bool cs = true;
		bool cd = true;

		// camera
		bool debugMode = true; // allows for debug mode.

	};
}
