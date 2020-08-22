#pragma once
// gameplay scene for Codename: Zero

#include "..\cherry/scenes/GameplayScene.h"

#include "..\Player.h"
#include "..\EnemyTypes/Enemies.h"
#include "..\Obstacle.h"
#include "..\cherry/physics/PhysicsBody.h"
#include "..\Projectile.h"
#include "..\cherry/animate/Path.h"
#include "..\LevelLoader.h"
#include "..\AnimationLoader.h"
#include "..\cherry/post/KernelLayer.h"

//Enemy Sub-classes
#include "..\EnemyTypes/Enemies.h"
#include <cherry\objects\Text.h>

namespace cnz
{
	// struct AnimStruct { // easier to batch add frames with a morphanim this way.
	// 	int numFrames;
	// 	std::string basePath;
	// 	float animTime;
	// 	cherry::MorphAnimation* anim;
	// };

	class CNZ_GameplayScene : public cherry::GameplayScene
	{
	public:
		// constructor
		CNZ_GameplayScene(std::string legendPath, std::string levelPath, std::string sceneName, int mapNumber);

		// constructor - takes information from info provided.
		CNZ_GameplayScene(const LevelLoadInfo& info);

		// called when the scene is being opened (equivalent to Game::LoadContent).
		// this is only called when the game is set to use this scene.
		void OnOpen() override;

		// called when the scene is being closed.
		void OnClose() override;

		// generates a new instance of the cnz gameplay scene.
		virtual Scene* GenerateNewInstance() const override;

		// mouse button pressed
		void MouseButtonPressed(GLFWwindow* window, int button) override;

		// mouse button released
		void MouseButtonReleased(GLFWwindow* window, int button) override;


		// called when a key has been pressed
		void KeyPressed(GLFWwindow* window, int key) override;

		// called when a key is being held down
		void KeyHeld(GLFWwindow* window, int key) override;

		// called when a key has been released
		void KeyReleased(GLFWwindow* window, int key) override;

		// get closest obstacle within a certain degrees of where the player is facing
		cherry::PhysicsBody* GetClosestObstacle();

		// gets a list of enemies within a certain degrees of where the player is facing
		// we require the dash vector so that we can use a smaller one if the player is dashing towards an obstacle that
		// would case the dash to end when they collide with that obstacle.

		//Returns true of an specified enemy is within the dash of the player
		bool GetEnemiesInDash(cherry::Vec3 dashVec, cnz::Enemy* enemy, cnz::Player* player);

		// get the angle at which a Vec3 is facing in X and Y axis. 
		// can also be used to find the angle between two positions by getting passing in their difference
		float GetXYAngle(cherry::Vec3 vec);

		// spawns an enemy
		void SpawnEnemyGroup(int i = -1);

		// pass in scene name. Should be called on scene switch. Will overwrite game's object lists and objects and physics body lists with objects from new scene.
		// if 'true' is passed, then the level is loaded from the file. If false is passed, then the default load is used.
		void MapSceneObjectsToGame(bool loadFromFile = true);
		
		// loads the enemy strings
		static void LoadEnemyGroups();

		// returns the player object. Set to 'nullptr' if the player isn't available.
		cnz::Player* const GetPlayer() const;

		// returns 'true' if physics bodies are visible.
		// do note that this checks if they are on overall; individual objects can enable their own without this returning true.
		bool GetVisiblePhysicsBodies() const;

		// sets whether the bodies should be visible or not.
		void SetVisiblePhysicsBodies(bool visible);

		// updates the player's score string
		void UpdateScore();

		// updates the time stop string
		void UpdateTimeStop();

		// update loop
		void Update(float deltaTime) override;

	private:
		// function called for regular gameplay.
		// void GameplayUpdate();
		// 
		// // function called for pause menu.
		// void PauseUpdate();

		Level map; // the map
		int curWave = 0; // current enemy wave

		// the pause menu
		cherry::Image* pauseMenu = nullptr;

		// TODO: add in exit key, back key, and change pausee button.
		// cherry::Text* pauseMenuLine1;

		// the GameplayScene already has the object list and light list.
		// these should probably be removed, but it was kept in for ease of use.
		// cherry::ObjectList* objList = nullptr;
		// cherry::LightList* tempList = nullptr;

		// default material
		cherry::SamplerDesc description; // texture description 
		cherry::TextureSampler::Sptr sampler; // texture sampler
		cherry::Material::Sptr matStatic = nullptr; // the static material
		cherry::Material::Sptr matDynamic = nullptr; // the dynamic material

		// object for the player.
		cnz::Player* playerObj = nullptr; 

		// loads enemies 
		// Create multiple enemies using these with only loading one .obj
		cnz::Sentry* sentry = nullptr; //Sentry enemy : Bowman
		cnz::Oracle* oracle = nullptr; //Oracle enemy : Polearmsman
		cnz::Marauder* marauder = nullptr; //Marauder enemy : Swordsman
		cnz::Bastion* bastion = nullptr; //Bastion enemy : Shield guy
		cnz::Mechaspider* mechaspider = nullptr; //Mechaspider enemy : Land mine
		Projectile* arrowBase = nullptr;
		//  obstacle list.
		std::vector<cnz::Obstacle*> obstacles; // vector of every non moving object in the game. Non moving, for now.

		// NEW
		int wave = 1; // wave number
		int waveEnemyCount = 5; // starting enemy number for spawn
		const int WAVE_ENEMY_COUNT_MAX = 20; // maximum enemy count.

		// TODO: remove this, sicne the enemy list is the same for everything.
		static std::vector<std::vector<string>> enemyGroups; //2D Vector of enemy groups [which group][what enemy in the group]
		
		// becomes 'true' when the groups are loaded in.
		// since this is static, groups only need to be loaded once.
		static bool groupsLoaded;

		std::vector<Enemy*> enemyList; //2D Vector of enemy groups [which group][what enemy in the group] 

		std::vector<Projectile*> projList; //list of projectiles
		// std::vector<float> projTimeList; //list of projectile timers (put inside projectile objects)

		// skybox object.
		cherry::Skybox skyboxObj;

		// indicator arrow for the dash.
		cherry::Object* indicatorObj = nullptr; // object for the dash indicator.

		// camera lerp bool, percentage, incrementer, and camera starting position.
		bool camLerping = false; // if the camera is lerping and not just snapping to the player.
		float camLerpPercent = 0.0f;
		const float CAM_LERP_INC = 5.0F; // originally 0.05F
		glm::vec3 camLerpStartPos = glm::vec3(0); // the camera's start position when lerping.

		// the camera's position on the z-axis. This does not change.
		const glm::vec3 CAMERA_POS_OFFSET{ 0.0F, 5.0F, 20.0F };

		// glm::vec3 testPlayPos = glm::vec3(0, 0, 0);
		cherry::Vec3 playerPrevPos;
		cherry::Vec3 playerSpawn;

		// the player light (follows the player)
		cherry::Light* playerLight; // player light
		glm::vec3 playerLightOffset; // light offset from player position

		/*
		 * trial numbers
		 *** (-0.0488586426, -2.58236694, -3.05355835)
		 *** (0.356292725, -3.05722046, -3.05355835)
		 *** (0.305786133, -2.34268188, -1.60046387)
		 *** (0.356292725, -3.05722046, -6.89315796)
		 */
		const glm::vec3 PLAYER_LIGHT_OFFSET_BASE{ 0.356292725, -3.05722046, -6.89315796 };
		glm::ivec3 playerLightDir; // light direction (0, 0, 0)

		// allows the player to control the light.
		const bool LIGHT_CONTROL = true;
		const glm::vec3 LIGHT_OFFSET_INC {9.0F, 9.0F, 9.0F};

		// player movement
		bool w = false;
		bool a = false;
		bool s = false;
		bool d = false;
		bool f = false;
		bool leftShift = false;

		// dash trigger
		// mouse buttons - left and right
		bool mouseLeft = false, mouseRight = false;
		bool spaceBar = false;

		// stops the player from moving through solid objects.
		bool cw = true;
		bool ca = true;
		bool cs = true;
		bool cd = true;

		// paused bool, restart bool, and exit bool
		bool paused = false; // pause game
		bool restart = false; // restart game
		bool exitMap = false; // returns back to main menu

		// becomes 'true' when the player is invincible.
		bool isInvincible = false;
		float invincibleCountdown = 0.0F;
		static const float INVINCIBLE_TIME_MAX;

		// used for freezing enemies.
		// counts down to 0 when freeze is activated, and counts up to maximum when cooldown is active.
		float timeStopTimer = 0.0F;
		bool timeStopActive = false; // the time stop effect is active.

		// the time decrement rate and increment rate.
		float timeDecRate = 5.0F; // used for counting down time
		float timeIncRate = 2.5F; // used for charging

		// time stop maximum value and cooldown maximum.
		// the chage happens automatically.
		static const float TIME_STOP_MAX;
		static const int TIME_STOP_DISPLAY_DIGITS_MAX;

		// time stop text, and bool for showing it.
		cherry::Text* timeStopText;
		bool showTimeStopText = true; // the game might run poorly if this is used, so maybe comment it out?
		
		// number of kills
		int kills = 0;

		// the amount of lives
		int lives = 3;

		// life icons
		cherry::Image* lifeIcon_1 = nullptr;
		cherry::Image* lifeIcon_2 = nullptr;
		cherry::Image* lifeIcon_3 = nullptr;

		// the current group
		int curGroup = -1;

		// score
		int score = 0;

		// the maximum amount of digits
		// static const int DIGITS_MAX;

		// score text.
		cherry::Text* scoreText = nullptr;
		bool showScore = true;

		// if 'true', the physics bodies are shown.
		bool showPBs = false;

		// set to 'true' if post processing should be used.
		bool postProcess = true;

		// edge detection object
		cherry::Kernel3Layer edgeDetect;

		// profile the game
		const bool PROFILE = false;

	protected:

	};
}