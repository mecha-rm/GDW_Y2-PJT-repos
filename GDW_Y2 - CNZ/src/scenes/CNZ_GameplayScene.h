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

//Enemy Sub-classes
#include "..\EnemyTypes/Enemies.h"

namespace cnz
{
	struct AnimStruct { // easier to batch add frames with a morphanim this way.
		int numFrames;
		std::string basePath;
		float animTime;
		cherry::MorphAnimation* anim;
	};

	class CNZ_GameplayScene : public cherry::GameplayScene
	{
	public:
		CNZ_GameplayScene(std::string legendPath, std::string levelPath, std::string sceneName);

		// called when the scene is being opened (equivalent to Game::LoadContent).
		// this is only called when the game is set to use this scene.
		void OnOpen() override;

		// called when the scene is being closed.
		void OnClose() override;

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
		void MapSceneObjectsToGame();
		
		// returns 'true' if physics bodies are visible.
		// do note that this checks if they are on overall; individual objects can enable their own without this returning true.
		bool GetVisiblePhysicsBodies() const;

		// sets whether the bodies should be visible or not.
		void SetVisiblePhysicsBodies(bool visible);

		// update loop
		void Update(float deltaTime) override;

	private:
		
		Level map; // the map
		int curWave = 0; //Current enemy wave

		cherry::Image* pauseMenu = nullptr;

		// the GameplayScene already has the object list and light list.
		// these should probably be removed, but it was kept in for ease of use.
		// cherry::ObjectList* objList = nullptr;
		// cherry::LightList* tempList = nullptr;

		// default material
		cherry::SamplerDesc description; // texture description 
		cherry::TextureSampler::Sptr sampler; // texture sampler
		cherry::Material::Sptr matStatic = nullptr; // the static material
		cherry::Material::Sptr matDynamic = nullptr; // the dynamic material


		cnz::Player* playerObj = nullptr; // object for the player.

		//Weapons
		Obstacle* bow = nullptr;
		Obstacle* katana = nullptr;
		Obstacle* spear = nullptr;

		//Props
		Obstacle* drum = nullptr;
		Obstacle* dumpster = nullptr;
		Obstacle* lamp_Center = nullptr;
		Obstacle* lamp_Corner = nullptr;
		Obstacle* lamp_Side = nullptr;
		Obstacle* manhole = nullptr;
		Obstacle* road = nullptr;
		Obstacle* piller = nullptr;


		//Load enemies // Create multiple enemies using these with only loading one .obj
		cnz::Sentry* sentry = nullptr; //Sentry enemy : Bowman
		cnz::Oracle* oracle = nullptr; //Oracle enemy : Polearmsman
		cnz::Marauder* marauder = nullptr; //Marauder enemy : Swordsman
		cnz::Bastion* bastion = nullptr; //Bastion enemy : Shield guy
		cnz::Mechaspider* mechaspider = nullptr; //Mechaspider enemy : Land mine
		Projectile* arrowBase = nullptr;

		std::vector<cnz::Obstacle*> obstacles; // vector of every non moving object in the game. Non moving, for now.
		std::vector<std::vector<string>> enemyGroups; //2D Vector of enemy groups [which group][what enemy in the group]
		std::vector<Enemy*> enemyList; //2D Vector of enemy groups [which group][what enemy in the group]

		std::vector<Projectile*> projList; //list of projectiles
		std::vector<float> projTimeList; //list of projectile timers

		cherry::Skybox skyboxObj;
		cherry::Object* indArrow;

		// Animations
		std::vector<cnz::AnimStruct*> animList;

		cherry::MorphAnimation* indArrowAnim;
		cnz::AnimStruct playerCharging = { 26, "res/objects/anims/player/Attack_Charge/One_Charge_0000", 1.08f, new cherry::MorphAnimation() };
		cnz::AnimStruct playerCharged = { 1, "res/objects/anims/player/Attack_Charged/One_Charge_0000", 0.01f, new cherry::MorphAnimation() };
		cnz::AnimStruct playerDashing = { 26, "res/objects/anims/player/Attack_Dash/One_Dash_0000", 0.27f, new cherry::MorphAnimation() };
		cnz::AnimStruct playerRun_F = { 24, "res/objects/anims/player/Run_F/One_RF_0000", 1.0f, new cherry::MorphAnimation() };
		cnz::AnimStruct playerRun_B = { 24, "res/objects/anims/player/Run_B/One_RB_0000", 1.0f, new cherry::MorphAnimation() };
		cnz::AnimStruct playerRun_L = { 24, "res/objects/anims/player/Run_L/One_RL_0000", 1.0f, new cherry::MorphAnimation() };
		cnz::AnimStruct playerRun_R = { 24, "res/objects/anims/player/Run_R/One_RR_0000", 1.0f, new cherry::MorphAnimation() };

		cnz::Player* testObj = nullptr; // object for the player.
		cherry::Object* indicatorObj = nullptr; // object for the dash indicator.
		bool mbLP = false, mbLR = false;


		vector<int> enemyLocationi;
		vector<int> enemyLocationj;

		float camLerpPercent = 0.0f;

		glm::vec3 testPlayPos = glm::vec3(0, 0, 0);
		cherry::Vec3 playerPrevPos;
		cherry::Vec3 playerSpawn;

		// player movement
		bool w = false;
		bool a = false;
		bool s = false;
		bool d = false;
		bool f = false;
		bool ls = false;
		bool spaceP = false;
		bool spaceR = false;
		bool cw = true;
		bool ca = true;
		bool cs = true;
		bool cd = true;
		bool paused = false;
		bool restart = false;

		int kills = 0;
		int lives = 3;
		int curGroup = -1;

		// camera
		bool debugMode = true; // allows for debug mode.

		// if 'true', the physics bodies are shown.
		bool showPBs = false;

	protected:

	};
}