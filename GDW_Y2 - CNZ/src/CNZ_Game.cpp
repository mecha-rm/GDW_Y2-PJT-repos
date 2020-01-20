#include "CNZ_Game.h"
#include <GLM/gtc/matrix_transform.hpp> // camera modes
#include "cherry/SceneManager.h"

float GetDistance(cherry::Vec3 one, cherry::Vec3 two);
cherry::Vec3 GetUnitDirVec(cherry::Vec3 one, cherry::Vec3 two);
cherry::Vec3 LERP(cherry::Vec3 start, cherry::Vec3 end, float percent);

// constructor
cnz::CNZ_Game::CNZ_Game() : Game() {}

// constructor with window size
// TODO: change 'true' to 'false' so default values aren't used;
// TODO: don't use full screen.
cnz::CNZ_Game::CNZ_Game(float windowWidth, float windowHeight, bool fullScreen) 
	: Game("GDW_Y2 - BnsFt - Codename: ZERO", windowWidth, windowHeight, fullScreen, false, false) {
}

// mouse button has been pressed.
void cnz::CNZ_Game::MouseButtonPressed(GLFWwindow* window, int button)
{
	// checks each button
	switch (button) {
	case GLFW_MOUSE_BUTTON_LEFT:
		this->mbLP = true;
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		break;
	}
}

void cnz::CNZ_Game::MouseButtonReleased(GLFWwindow* window, int button)
{
	// checks each button
	switch (button) {
	case GLFW_MOUSE_BUTTON_LEFT:
		this->mbLR = true;
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		break;
	}
}

// key pressed
void cnz::CNZ_Game::KeyPressed(GLFWwindow* window, int key)
{
	Game* game = (Game*)glfwGetWindowUserPointer(window);

	if (game == nullptr) // if game is 'null', then nothing happens
		return;

	switch (key) // checks the keys
	{
	case GLFW_KEY_SPACE:
		myCamera->SwitchViewMode();
		break;

	case GLFW_KEY_W: // up
		w = true;
		break;
	case GLFW_KEY_S: // down
		s = true;
		break;

	case GLFW_KEY_A: // left
		a = true;
		break;
	case GLFW_KEY_D: // right
		d = true;
		break;

	case GLFW_KEY_LEFT_SHIFT:
		ls = true;
		break;
	}
}

// key held
void cnz::CNZ_Game::KeyHeld(GLFWwindow* window, int key)
{
	Game* game = (Game*)glfwGetWindowUserPointer(window);

	if (game == nullptr) // if game is 'null', then nothing happens
		return;

	// checks key value.
	switch (key)
	{
	case GLFW_KEY_W: // up
		w = true;
		break;
	case GLFW_KEY_S: // down
		s = true;
		break;
	
	case GLFW_KEY_A: // left
		a = true;
		break;
	case GLFW_KEY_D: // right
		d = true;
		break;
	}
}

// key released
void cnz::CNZ_Game::KeyReleased(GLFWwindow* window, int key)
{
	Game* game = (Game*)glfwGetWindowUserPointer(window);

	if (game == nullptr) // if game is 'null', then nothing happens
		return;

	// checks key value.
	switch (key)
	{
	case GLFW_KEY_W: // up
		w = false;
		break;
	case GLFW_KEY_S: // down
		s = false;
		break;

	case GLFW_KEY_A: // left
		a = false;
		break;
	case GLFW_KEY_D: // right
		d = false;
		break;
	
	case GLFW_KEY_LEFT_SHIFT:
		ls = false;
		break;
	}
}

//////////////////TODO: Why is this defined in here??????????????????????? Should it not be in some sort of PhysicsBody file or something?////////////////////////
// returns the physics body of the closest obstacle to the player, in the direction that the player is facing.
// Used later to verify if the player is allowed to dash the full distance in their intended direction
cherry::PhysicsBody* cnz::CNZ_Game::getClosestObstacle()
{
	cherry::PhysicsBody* closestBody = nullptr;
	float cbDist = 0.0f;
	cherry::Vec3 delta;
	float angleFromPlayer;
	float dAngle;
	for (int i = 0; i < obstaclePBs.size(); i++) {
		delta.SetX(obstaclePBs[i]->GetModelPosition().GetX() - playerObj->GetPosition().GetX());
		delta.SetY(obstaclePBs[i]->GetModelPosition().GetY() - playerObj->GetPosition().GetY());

		angleFromPlayer = getXYAngle(delta);
		dAngle = angleFromPlayer - getXYAngle(playerObj->GetDash(playerObj->GetDashDist()));
		
		if (dAngle <= 0.25 && dAngle >= -0.25) { // if angle difference is less than ~15 degrees. 
			if (cbDist == 0.0f) { // if this is the first loop. (we should never get a dist of 0.0f anyway.
				closestBody = obstaclePBs[i];
				cbDist = delta.GetLength();
			}
			else if (delta.GetLength() < cbDist) { // if the current pbody is closer than the last.
				closestBody = obstaclePBs[i];
				cbDist = delta.GetLength();
			}
		}
	}
	return closestBody;
}

vector<cherry::Object*> cnz::CNZ_Game::getEnemiesInDash(cherry::Vec3 dashVec)
{
	vector<cherry::Object*> enemies;

	cherry::Vec3 delta;
	float angleFromPlayer;
	float dAngle;
	float dLen = dashVec.GetLength();

	for (int i = 0; i < enemyPBs.size(); i++) {
		delta.SetX(enemyPBs[i]->GetModelPosition().GetX() - playerObj->GetPosition().GetX());
		delta.SetY(enemyPBs[i]->GetModelPosition().GetY() - playerObj->GetPosition().GetY());

		angleFromPlayer = getXYAngle(delta);
		dAngle = angleFromPlayer - getXYAngle(dashVec);

		if (dAngle <= 0.25 && dAngle >= -0.25) { // if angle difference is less than ~15 degrees. 
			if (delta.GetLength() < dLen) { // if the current pbody is closer than the last.
				enemies.push_back(enemyPBs[i]->GetObject());
			}
		}
	}
	return enemies;
}

// test func to get angle in xy axes of a vec. use only when you stored delta x, delta y and delta z in a vec3.
// this doesnt make sense to use on a position, but rather only on a difference in position
float cnz::CNZ_Game::getXYAngle(cherry::Vec3 vec)
{
	return atanf(vec.GetX() / vec.GetY());
}

void cnz::CNZ_Game::spawnEnemyGroup(int i = -1)
{
	int percent = rand() % 100;
	
	int count = 0;

	if (i == -1) {
		if (percent < 50) {
			i = rand() % 6;
		}
		else if (percent < 80) {
			i = rand() % 13;
		}
		else if (percent < 96 && curWave > 5) {
			i = rand() % 18;
		}
		else if (percent < 100 && curWave > 8) {
			i = rand() % 20;
		}
		else {
			i = rand() % 13;
		}
	}
	
	cout << i << endl;
	curGroup = i;

	int n = enemyGroups[i].size();

	for (int j = 0; j < n; j++) {

		enemyGroups[i][j]->SetRotation(cherry::Vec3(0, 0, 0), true);
		enemyGroups[i][j]->SetPosition(cherry::Vec3(0 + count * 5, -10 + abs(count) * -5, 0));
		enemyGroups[i][j]->AddPhysicsBody(new cherry::PhysicsBodyBox(enemyGroups[i][j]->GetPosition(), enemyGroups[i][j]->GetPBodySize()));
		enemyPBs.push_back(enemyGroups[i][j]->GetPhysicsBodies()[0]);
		enemyGroups[i][j]->alive = true;
		enemyGroups[i][j]->SetRotationXDegrees(90);
		//enemyGroups[i][j]->SetRotationZDegrees(180);
		AddObjectToScene(enemyGroups[i][j]);

		if (j % 2 == 0) {
			count++;
		}
		count *= -1;
	}
}

// loads content
void cnz::CNZ_Game::LoadContent()
{
	// srand(time(NULL)); // move to Game.h

	bool levelLoading = true;

	Game::LoadContent(); // calls the load content

	if (levelLoading) { // load all levels here, set main menu scene here. Change scenes in Update based on certain conditions where the level should change.
		//// LOAD LEVELS
		Level map1 = Level("res/loader/legend.csv", "res/loader/map1.csv", "map1");

		//// ADD SCENE NAMES TO SCENE LIST
		// set scene name as string, or keep using levelName->getSceneName();
		scenes.push_back(map1.getSceneName());

		//// CREATE SCENE OBJECT LISTS
		// objManager->AddSceneObjectList(sceneName);
		objManager->AddSceneObjectList(map1.getSceneName());

		//// GET SCENE OBJECT LISTS
		// objList = objManager->GetSceneObjectListByName(sceneName);
		objList = objManager->GetSceneObjectListByName(map1.getSceneName());

		//// PUT OBJECTS INT SCENE OBJECT LIST
		for (int i = 0; i < map1.objList.size(); i++) {
			objList->AddObject(&map1.objList[i]);
		}

		//// REGISTER SCENES
		// SceneManager::RegisterScene(sceneName);
		cherry::SceneManager::RegisterScene(map1.getSceneName());

		//// SET CURRENT SCENE
		// SceneManager::SetCurrentScene(sceneName);
		cherry::SceneManager::SetCurrentScene(map1.getSceneName());

		//// Stuff I dont need to change for now (stays as is in Game.cpp)
		// lightManager->AddSceneLightList(sceneName);
		// lightList = lightManager->GetSceneLightList(sceneName);
		// for all lights:
		// lightList->AddLight(new Light(sceneName, Vec3(), Vec3(), Vec3(), float, float, float, float);
		// material stuff that happens in Game.cpp
	}

	else { // for testing, loads a level for testing collision, showing all objects and test paths and such
		playerObj = new Player("res/objects/hero/charactoereee.obj", GetCurrentScene(), matStatic); // creates the player.
		testObj = new Player("res/objects/monkey.obj", GetCurrentScene(), matStatic); // creates the not player.

		sentry = new Enemies("res/objects/enemies/Enemy_Bow.obj", GetCurrentScene(), matStatic);
		oracle = new Enemies("res/objects/enemies/Enemy_Spear.obj", GetCurrentScene(), matStatic);
		marauder = new Enemies("res/objects/enemies/Enemy_Sword.obj", GetCurrentScene(), matStatic);
		bastion = new Enemies("res/objects/weapons/shield.obj", GetCurrentScene(), matStatic);
		mechaspider = new Enemies("res/objects/enemies/Spider.obj", GetCurrentScene(), matStatic);
		arrowBase = new Projectile("res/objects/weapons/arrow.obj");

		// arena obstacles
		Obstacle* wall1 = new Obstacle("res/objects/GDW_1_Y2_-_Wall_Tile.obj", GetCurrentScene(), cherry::Vec3(10, 2, 2));
		Obstacle* wall2 = new Obstacle("res/objects/GDW_1_Y2_-_Wall_Tile.obj", GetCurrentScene(), cherry::Vec3(2, 2, 2));
		Obstacle* wall3 = new Obstacle("res/objects/GDW_1_Y2_-_Wall_Tile.obj", GetCurrentScene(), cherry::Vec3(2, 2, 2));
		Obstacle* wall4 = new Obstacle("res/objects/GDW_1_Y2_-_Wall_Tile.obj", GetCurrentScene(), cherry::Vec3(2, 2, 2));
		Obstacle* wall5 = new Obstacle("res/objects/GDW_1_Y2_-_Wall_Tile.obj", GetCurrentScene(), cherry::Vec3(2, 2, 2));
		Obstacle* wall6 = new Obstacle("res/objects/GDW_1_Y2_-_Wall_Tile.obj", GetCurrentScene(), cherry::Vec3(2, 2, 2));
		Obstacle* wall7 = new Obstacle("res/objects/GDW_1_Y2_-_Wall_Tile.obj", GetCurrentScene(), cherry::Vec3(2, 2, 2));


		bow = new Obstacle("res/objects/weapons/bow.obj", GetCurrentScene(), false);
		katana = new Obstacle("res/objects/weapons/katana.obj", GetCurrentScene(), false);
		spear = new Obstacle("res/objects/weapons/spear.obj", GetCurrentScene(), false);
		obstacles.push_back(bow);
		obstacles.push_back(katana);
		obstacles.push_back(spear);

		drum = new Obstacle("res/objects/props/drum.obj", GetCurrentScene(), false);
		dumpster = new Obstacle("res/objects/props/Dumpster.obj", GetCurrentScene(), false);
		lamp_Center = new Obstacle("res/objects/props/Lamp_Center.obj", GetCurrentScene(), false);
		lamp_Corner = new Obstacle("res/objects/props/Lamp_Corner.obj", GetCurrentScene(), false);
		lamp_Side = new Obstacle("res/objects/props/Lamp_Side.obj", GetCurrentScene(), false);
		manhole = new Obstacle("res/objects/props/manhole.obj", GetCurrentScene(), false);
		piller = new Obstacle("res/objects/GDW_1_Y2 - Pillar.obj", GetCurrentScene(), false);
		road = new Obstacle("res/objects/props/Road.obj", GetCurrentScene(), false);
		obstacles.push_back(drum);
		obstacles.push_back(dumpster);
		obstacles.push_back(lamp_Center);
		obstacles.push_back(lamp_Corner);
		obstacles.push_back(lamp_Side);
		obstacles.push_back(manhole);
		obstacles.push_back(piller);
		obstacles.push_back(road);

		for (int i = 0; i < 20; i++) {
			enemyGroups.push_back(std::vector<Enemies*>());
		}

		//Easy
		enemyGroups[0].push_back(new Marauder(marauder, GetCurrentScene()));
		enemyGroups[0].push_back(new Marauder(marauder, GetCurrentScene()));
		enemyGroups[0].push_back(new Sentry(sentry, GetCurrentScene()));

		//Easy
		enemyGroups[1].push_back(new Mechaspider(mechaspider, GetCurrentScene()));
		enemyGroups[1].push_back(new Mechaspider(mechaspider, GetCurrentScene()));
		enemyGroups[1].push_back(new Mechaspider(mechaspider, GetCurrentScene()));
		enemyGroups[1].push_back(new Mechaspider(mechaspider, GetCurrentScene()));
		enemyGroups[1].push_back(new Mechaspider(mechaspider, GetCurrentScene()));

		//Easy
		enemyGroups[2].push_back(new Bastion(bastion, GetCurrentScene()));
		enemyGroups[2].push_back(new Mechaspider(mechaspider, GetCurrentScene()));
		enemyGroups[2].push_back(new Mechaspider(mechaspider, GetCurrentScene()));
		enemyGroups[2].push_back(new Mechaspider(mechaspider, GetCurrentScene()));

		//Easy
		enemyGroups[3].push_back(new Marauder(marauder, GetCurrentScene()));
		enemyGroups[3].push_back(new Marauder(marauder, GetCurrentScene()));
		enemyGroups[3].push_back(new Oracle(oracle, GetCurrentScene()));
		enemyGroups[3].push_back(new Oracle(oracle, GetCurrentScene()));

		//Easy
		enemyGroups[4].push_back(new Marauder(marauder, GetCurrentScene()));
		enemyGroups[4].push_back(new Marauder(marauder, GetCurrentScene()));
		enemyGroups[4].push_back(new Marauder(marauder, GetCurrentScene()));
		enemyGroups[4].push_back(new Marauder(marauder, GetCurrentScene()));
		enemyGroups[4].push_back(new Marauder(marauder, GetCurrentScene()));

		//Easy
		enemyGroups[5].push_back(new Bastion(bastion, GetCurrentScene()));
		enemyGroups[5].push_back(new Bastion(bastion, GetCurrentScene()));

		//Medium
		enemyGroups[6].push_back(new Sentry(sentry, GetCurrentScene()));
		enemyGroups[6].push_back(new Sentry(sentry, GetCurrentScene()));
		enemyGroups[6].push_back(new Oracle(oracle, GetCurrentScene()));

		//Medium
		enemyGroups[7].push_back(new Marauder(marauder, GetCurrentScene()));
		enemyGroups[7].push_back(new Sentry(sentry, GetCurrentScene()));
		enemyGroups[7].push_back(new Oracle(oracle, GetCurrentScene()));
		enemyGroups[7].push_back(new Bastion(bastion, GetCurrentScene()));

		//Medium
		enemyGroups[8].push_back(new Oracle(oracle, GetCurrentScene()));
		enemyGroups[8].push_back(new Oracle(oracle, GetCurrentScene()));
		enemyGroups[8].push_back(new Bastion(bastion, GetCurrentScene()));
		enemyGroups[8].push_back(new Mechaspider(mechaspider, GetCurrentScene()));

		//Medium
		enemyGroups[9].push_back(new Sentry(sentry, GetCurrentScene()));
		enemyGroups[9].push_back(new Bastion(bastion, GetCurrentScene()));
		enemyGroups[9].push_back(new Bastion(bastion, GetCurrentScene()));

		//Medium
		enemyGroups[10].push_back(new Oracle(oracle, GetCurrentScene()));
		enemyGroups[10].push_back(new Oracle(oracle, GetCurrentScene()));
		enemyGroups[10].push_back(new Oracle(oracle, GetCurrentScene()));
		enemyGroups[10].push_back(new Oracle(oracle, GetCurrentScene()));

		//Medium
		enemyGroups[11].push_back(new Marauder(marauder, GetCurrentScene()));
		enemyGroups[11].push_back(new Marauder(marauder, GetCurrentScene()));
		enemyGroups[11].push_back(new Marauder(marauder, GetCurrentScene()));
		enemyGroups[11].push_back(new Marauder(marauder, GetCurrentScene()));
		enemyGroups[11].push_back(new Marauder(marauder, GetCurrentScene()));
		enemyGroups[11].push_back(new Marauder(marauder, GetCurrentScene()));
		enemyGroups[11].push_back(new Marauder(marauder, GetCurrentScene()));
		enemyGroups[11].push_back(new Mechaspider(mechaspider, GetCurrentScene()));

		//Medium
		enemyGroups[12].push_back(new Sentry(sentry, GetCurrentScene()));
		enemyGroups[12].push_back(new Sentry(sentry, GetCurrentScene()));
		enemyGroups[12].push_back(new Bastion(bastion, GetCurrentScene()));
		enemyGroups[12].push_back(new Bastion(bastion, GetCurrentScene()));
		enemyGroups[12].push_back(new Bastion(bastion, GetCurrentScene()));

		//Hard
		enemyGroups[13].push_back(new Sentry(sentry, GetCurrentScene()));
		enemyGroups[13].push_back(new Sentry(sentry, GetCurrentScene()));
		enemyGroups[13].push_back(new Sentry(sentry, GetCurrentScene()));
		enemyGroups[13].push_back(new Sentry(sentry, GetCurrentScene()));
		enemyGroups[13].push_back(new Sentry(sentry, GetCurrentScene()));

		//Hard
		enemyGroups[14].push_back(new Sentry(sentry, GetCurrentScene()));
		enemyGroups[14].push_back(new Oracle(oracle, GetCurrentScene()));
		enemyGroups[14].push_back(new Oracle(oracle, GetCurrentScene()));
		enemyGroups[14].push_back(new Oracle(oracle, GetCurrentScene()));
		enemyGroups[14].push_back(new Bastion(bastion, GetCurrentScene()));

		//Hard
		enemyGroups[15].push_back(new Marauder(marauder, GetCurrentScene()));
		enemyGroups[15].push_back(new Marauder(marauder, GetCurrentScene()));
		enemyGroups[15].push_back(new Oracle(oracle, GetCurrentScene()));
		enemyGroups[15].push_back(new Oracle(oracle, GetCurrentScene()));
		enemyGroups[15].push_back(new Oracle(oracle, GetCurrentScene()));
		enemyGroups[15].push_back(new Mechaspider(mechaspider, GetCurrentScene()));
		enemyGroups[15].push_back(new Mechaspider(mechaspider, GetCurrentScene()));
		enemyGroups[15].push_back(new Mechaspider(mechaspider, GetCurrentScene()));

		//Hard
		enemyGroups[16].push_back(new Bastion(bastion, GetCurrentScene()));
		enemyGroups[16].push_back(new Bastion(bastion, GetCurrentScene()));
		enemyGroups[16].push_back(new Mechaspider(mechaspider, GetCurrentScene()));
		enemyGroups[16].push_back(new Mechaspider(mechaspider, GetCurrentScene()));
		enemyGroups[16].push_back(new Mechaspider(mechaspider, GetCurrentScene()));
		enemyGroups[16].push_back(new Mechaspider(mechaspider, GetCurrentScene()));
		enemyGroups[16].push_back(new Mechaspider(mechaspider, GetCurrentScene()));
		enemyGroups[16].push_back(new Mechaspider(mechaspider, GetCurrentScene()));
		enemyGroups[16].push_back(new Mechaspider(mechaspider, GetCurrentScene()));
		enemyGroups[16].push_back(new Mechaspider(mechaspider, GetCurrentScene()));
		enemyGroups[16].push_back(new Mechaspider(mechaspider, GetCurrentScene()));
		enemyGroups[16].push_back(new Mechaspider(mechaspider, GetCurrentScene()));

		//Hard
		enemyGroups[17].push_back(new Marauder(marauder, GetCurrentScene()));
		enemyGroups[17].push_back(new Marauder(marauder, GetCurrentScene()));
		enemyGroups[17].push_back(new Marauder(marauder, GetCurrentScene()));
		enemyGroups[17].push_back(new Marauder(marauder, GetCurrentScene()));
		enemyGroups[17].push_back(new Sentry(sentry, GetCurrentScene()));
		enemyGroups[17].push_back(new Sentry(sentry, GetCurrentScene()));
		enemyGroups[17].push_back(new Bastion(bastion, GetCurrentScene()));
		enemyGroups[17].push_back(new Bastion(bastion, GetCurrentScene()));
		enemyGroups[17].push_back(new Bastion(bastion, GetCurrentScene()));

		//Insane
		enemyGroups[18].push_back(new Sentry(sentry, GetCurrentScene()));
		enemyGroups[18].push_back(new Sentry(sentry, GetCurrentScene()));
		enemyGroups[18].push_back(new Sentry(sentry, GetCurrentScene()));
		enemyGroups[18].push_back(new Oracle(oracle, GetCurrentScene()));
		enemyGroups[18].push_back(new Oracle(oracle, GetCurrentScene()));
		enemyGroups[18].push_back(new Oracle(oracle, GetCurrentScene()));
		enemyGroups[18].push_back(new Bastion(bastion, GetCurrentScene()));
		enemyGroups[18].push_back(new Bastion(bastion, GetCurrentScene()));
		enemyGroups[18].push_back(new Bastion(bastion, GetCurrentScene()));

		//Insane
		enemyGroups[19].push_back(new Marauder(marauder, GetCurrentScene()));
		enemyGroups[19].push_back(new Marauder(marauder, GetCurrentScene()));
		enemyGroups[19].push_back(new Sentry(sentry, GetCurrentScene()));
		enemyGroups[19].push_back(new Sentry(sentry, GetCurrentScene()));
		enemyGroups[19].push_back(new Sentry(sentry, GetCurrentScene()));
		enemyGroups[19].push_back(new Oracle(oracle, GetCurrentScene()));
		enemyGroups[19].push_back(new Oracle(oracle, GetCurrentScene()));
		enemyGroups[19].push_back(new Bastion(bastion, GetCurrentScene()));
		enemyGroups[19].push_back(new Mechaspider(mechaspider, GetCurrentScene()));
		enemyGroups[19].push_back(new Mechaspider(mechaspider, GetCurrentScene()));
		enemyGroups[19].push_back(new Mechaspider(mechaspider, GetCurrentScene()));

		// big wall bois
		obstacles.push_back(wall1);
		obstacles.push_back(wall2);
		obstacles.push_back(wall3);
		obstacles.push_back(wall4);
		obstacles.push_back(wall5);
		obstacles.push_back(wall6);
		obstacles.push_back(wall7);

		// rotations
		playerObj->SetRotation(cherry::Vec3(0, 0, 0), true);
		playerObj->SetRotationXDegrees(90);
		playerObj->SetRotationZDegrees(180);
		testObj->SetRotation(cherry::Vec3(0, 0, 0), true);

		wall1->SetRotation(cherry::Vec3(90, 0, 180), true); // top wall
		wall2->SetRotation(cherry::Vec3(90, 0, 0), true); // bottom wall
		wall3->SetRotation(cherry::Vec3(90, 0, 0), true); // bottom wall
		wall4->SetRotation(cherry::Vec3(90, 0, 90), true); // right wall
		wall5->SetRotation(cherry::Vec3(90, 0, 90), true); // right wall
		wall6->SetRotation(cherry::Vec3(90, 0, 270), true); // left wall
		wall7->SetRotation(cherry::Vec3(90, 0, 270), true); // left wall

		// positions
		testObj->SetPosition(cherry::Vec3(0, -5, 0));
		wall1->SetPosition(cherry::Vec3(15, -1, 0));
		wall2->SetPosition(cherry::Vec3(15, 14, 0));
		wall3->SetPosition(cherry::Vec3(8, 14, 0));
		wall4->SetPosition(cherry::Vec3(4, 10, 0));
		wall5->SetPosition(cherry::Vec3(4, 3, 0));
		wall6->SetPosition(cherry::Vec3(19, 10, 0));
		wall7->SetPosition(cherry::Vec3(19, 3, 0));

		// scale. if needed.


		// attach pbody
		playerObj->AddPhysicsBody(new cherry::PhysicsBodyBox(playerObj->GetPosition(), playerObj->getPBodySize()));
		testObj->AddPhysicsBody(new cherry::PhysicsBodyBox(testObj->GetPosition(), testObj->getPBodySize()));
		wall1->AddPhysicsBody(new cherry::PhysicsBodyBox(wall1->GetPosition(), wall1->getPBodySize()));
		wall2->AddPhysicsBody(new cherry::PhysicsBodyBox(wall2->GetPosition(), wall2->getPBodySize()));
		wall3->AddPhysicsBody(new cherry::PhysicsBodyBox(wall3->GetPosition(), wall3->getPBodySize()));
		wall4->AddPhysicsBody(new cherry::PhysicsBodyBox(wall4->GetPosition(), wall4->getPBodySize()));
		wall5->AddPhysicsBody(new cherry::PhysicsBodyBox(wall5->GetPosition(), wall5->getPBodySize()));
		wall6->AddPhysicsBody(new cherry::PhysicsBodyBox(wall6->GetPosition(), wall6->getPBodySize()));
		wall7->AddPhysicsBody(new cherry::PhysicsBodyBox(wall7->GetPosition(), wall7->getPBodySize()));

		// set pbody pos and maybe rotation for static objects
		//testObj->GetPhysicsBodies()[0]->SetModelPosition(testObj->GetPosition());
		wall1->GetPhysicsBodies()[0]->SetModelPosition(wall1->GetPosition());
		wall2->GetPhysicsBodies()[0]->SetModelPosition(wall2->GetPosition());
		wall3->GetPhysicsBodies()[0]->SetModelPosition(wall3->GetPosition());
		wall4->GetPhysicsBodies()[0]->SetModelPosition(wall4->GetPosition());
		wall5->GetPhysicsBodies()[0]->SetModelPosition(wall5->GetPosition());
		wall6->GetPhysicsBodies()[0]->SetModelPosition(wall6->GetPosition());
		wall7->GetPhysicsBodies()[0]->SetModelPosition(wall7->GetPosition());

		// debug draw pbdody
		wall1->GetPhysicsBodies()[0]->SetVisible(true);
		wall2->GetPhysicsBodies()[0]->SetVisible(true);
		wall3->GetPhysicsBodies()[0]->SetVisible(true);
		wall4->GetPhysicsBodies()[0]->SetVisible(true);
		wall5->GetPhysicsBodies()[0]->SetVisible(true);
		wall6->GetPhysicsBodies()[0]->SetVisible(true);
		wall7->GetPhysicsBodies()[0]->SetVisible(true);

		playerObj->GetPhysicsBodies()[0]->SetVisible(true);

		// Path stuff
		cherry::Path testPath = cherry::Path();
		testPath.AddNode(1.0f, 1.0f, 0.0f);
		testPath.AddNode(0.0f, 5.0f, 0.0f);
		testPath.AddNode(-2.0f, 5.0f, 0.0f);
		testPath.AddNode(-3.0f, 7.0f, 0.0f);
		testPath.AddNode(-6.0f, 8.0f, 0.0f);
		testPath.AddNode(-6.0f, 6.0f, 0.0f);
		testPath.AddNode(-2.0f, 4.0f, 0.0f);
		testPath.AddNode(1.0f, 1.0f, 0.0f);

		testPath.SetIncrementer(0.5f);
		testPath.SetInterpolationMode(1);

		testObj->SetPath(testPath, true);

		//Number corresponds with enemygroups first index
		spawnEnemyGroup(19);

		// add objects
		AddObjectToScene(playerObj);
		AddObjectToScene(testObj);
		int x = -27;
		for (int i = 0; i < obstacles.size(); i++) {
			AddObjectToScene(obstacles[i]);

			if (i > 0 && i < 11) {
				obstacles[i]->SetRotationXDegrees(90);
				obstacles[i]->SetRotationZDegrees(180);
				obstacles[i]->SetPosition(x, -40, 0);
			}
			x += 5;
		}

		road->SetRotationXDegrees(90);
		road->SetRotationZDegrees(180);
		road->SetPosition(0, -30, -1);
		manhole->SetPosition(manhole->GetPosition().GetX(), manhole->GetPosition().GetY(), -1);

		if (!playerObj->setDrawPBody(true)) {
			std::cout << "Ruhroh... Couldn't set drawPBody on playerObj!" << std::endl;
		}

		// add pbs to correct list for collisions
		// enemy PBs are added to the list in spawnEnemyGroup.
		for (int i = 0; i < obstacles.size(); i++) {
			auto temp = obstacles[i]->GetPhysicsBodies();
			if (temp.size() != 0) {
				obstaclePBs.push_back(temp[0]);
			}
			else {
				// cout << "obstacle " << i << " did not have a pb attached. accident?" << endl;
			}
		}

		//// setting up the camera
		myCamera->SetPosition(glm::vec3(playerObj->GetPosition().GetX(), playerObj->GetPosition().GetY() + 5.0f, playerObj->GetPosition().GetZ() + 20.0f));
		//myCamera->LookAt(glm::vec3(0));

		//// sets the camera to perspective mode for the scene.
		//// myCamera->SetPerspectiveMode(glm::perspective(glm::radians(60.0f), 1.0f, 0.01f, 1000.0f));
		////myCamera->SetPerspectiveMode(glm::perspective(glm::radians(60.0f), 1.0f, 0.01f, 1000.0f));
		myCamera->SetPerspectiveMode(glm::perspective(glm::radians(60.0f), 1.0f, 0.01f, 1000.0f));
		myCamera->LookAt(playerObj->GetPositionGLM());

		//// myCamera->SetPerspectiveMode(glm::perspective(glm::radians(10.0f), 1.0f, 0.01f, 1000.0f));

		//// sets the orthographic mode values. False is passed so that the camera starts in perspective mode.
		//myCamera->SetOrthographicMode(glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.0f, 100.0f), false);
		//// this is kind of useless in our game so it's commented out. No point wasting resources on it.
	}
}

// Update function
void cnz::CNZ_Game::Update(float deltaTime)
{
	this->playerPrevPos = playerObj->GetPosition();

	float moveInc = -10.0F; // the movement incrementer.

	vector<cherry::PhysicsBody*> playerObstacleCollisions; // obstacle PBs with which the player's PB is colliding with
	vector<cherry::PhysicsBody*> playerEnemyCollisions;

	// find all obstacles the player is colliding with
	for (int i = 0; i < obstaclePBs.size(); i++) {
		bool collision = cherry::PhysicsBody::Collision(playerObj->GetPhysicsBodies()[0], obstaclePBs[i]);
		if (collision) {
			playerObstacleCollisions.push_back(obstaclePBs[i]);
		}
		else {
			//std::cout << i << " is not a collision." << std::endl;
		}
	}

	// find all enemies the player is colliding with
	for (int i = 0; i < enemyPBs.size(); i++) {
		bool collision = cherry::PhysicsBody::Collision(playerObj->GetPhysicsBodies()[0], enemyPBs[i]);
		if (collision) {
			playerEnemyCollisions.push_back(enemyPBs[i]);
		}
	}

	// find all projectiles that player is colliding with
	for (int i = 0; i < projectilePBs.size(); i++) {
		projectilePBs[i]->SetModelPosition(projectilePBs[i]->GetObject()->GetPosition()); // update pb

		bool collision = cherry::PhysicsBody::Collision(playerObj->GetPhysicsBodies()[0], projectilePBs[i]);
		if (collision) { // check for collision and delete
			auto tempObj = projectilePBs[i]->GetObject();
			tempObj->RemovePhysicsBody(projectilePBs[i]);
			tempObj->SetPosition(1000, 1000, 1000);
			DeleteObjectFromScene(projectilePBs[i]->GetObject());
		}
	}

	cs = true;
	cw = true;
	ca = true;
	cd = true;

	// check what directions the player can move in based on its collisions with obstacles in the scene.
	if (playerObstacleCollisions.size() != 0) { // allow movement only in directions oposite of the collision (CUBES ONLY)
		// std::cout << "There are " << playerObstacleCollisions.size() << " playerObj collisions this update!" << std::endl;
		for (int i = 0; i < playerObstacleCollisions.size(); i++) {
			cherry::Vec3 dP = playerObstacleCollisions[i]->GetModelPosition() - playerObj->GetPosition();
			if (fabsf(dP.GetX()) < fabsf(dP.GetY())) { // this is why its cubes only
				if ((playerObstacleCollisions[i]->GetModelPosition().GetY() - playerObj->GetPosition().GetY()) >= 0) { // above the object
					cs = false;
				}
				else if ((playerObstacleCollisions[i]->GetModelPosition().GetY() - playerObj->GetPosition().GetY()) <= 0) { // below the object
					cw = false;
				}
			}
			else if (fabsf(dP.GetX()) > fabsf(dP.GetY())) { // this is the same thing, also why its cube only.
				if ((playerObstacleCollisions[i]->GetModelPosition().GetX() - playerObj->GetPosition().GetX()) >= 0) { // right of the object
					ca = false;
				}
				else if ((playerObstacleCollisions[i]->GetModelPosition().GetX() - playerObj->GetPosition().GetX()) <= 0) { // left of the object
					cd = false;
				}
			}
		}
	}
	
	// side checks
	//if ((playerObstacleCollisions[i]->GetModelPosition().GetX() - playerObj->GetPosition().GetX()) >= 0) // right of the object
	//if ((playerObstacleCollisions[i]->GetModelPosition().GetX() - playerObj->GetPosition().GetX()) <= 0) // left of the object
	//if ((playerObstacleCollisions[i]->GetModelPosition().GetY() - playerObj->GetPosition().GetY()) >= 0) // above the object
	//if ((playerObstacleCollisions[i]->GetModelPosition().GetY() - playerObj->GetPosition().GetY()) <= 0) // below the object

	// moving the player.
	if (w && cw) { // up
		playerObj->SetPosition(playerObj->GetPosition() + cherry::Vec3(0.0F, moveInc * deltaTime, 0.0F));
	}
	if (s && cs) { // down
		playerObj->SetPosition(playerObj->GetPosition() + cherry::Vec3(0.0F, -moveInc * deltaTime, 0.0F));
	}
	if (a && ca) { // left
		playerObj->SetPosition(playerObj->GetPosition() + cherry::Vec3(-moveInc * deltaTime, 0.0F, 0.0F));
	}
	if (d && cd) { // right
		playerObj->SetPosition(playerObj->GetPosition() + cherry::Vec3(moveInc * deltaTime, 0.0F, 0.0F));
	}
	
	playerObj->UpdateAngle(myCamera, GetCursorPosX(), GetCursorPosY(), GetWindowWidth(), GetWindowHeight());
	playerObj->SetRotation(cherry::Vec3(90.0f, 0.0f, playerObj->GetDegreeAngle() - 90), true);
	
	// dodge code
	if (ls) {
		cherry::Vec3 temp;
		if (w) {
			temp.SetY(temp.GetY() - 1.0f);
		}
		if (s) {
			temp.SetY(temp.GetY() + 1.0f);
		}
		if (a) {
			temp.SetX(temp.GetX() + 1.0f);
		}
		if (d) {
			temp.SetX(temp.GetX() - 1.0f);
		}

		playerObj->SetPosition(playerObj->GetPosition() + temp * 2);

		ls = false;
	}
	
	// dash code
	if (playerObj->GetDashTime() >= 1.0f && mbLR == true) // if dash timer is above 1.0 and left mouse has been released, do the dash
	{
		cherry::Vec3 dashVec = playerObj->GetDash(playerObj->GetDashDist());
		float tempDist = dashVec.GetLength();
		playerObj->SetDash(true);
		playerObj->SetDashTime(0.0f);

		cherry::PhysicsBody* closestObstacle = getClosestObstacle();
		if (closestObstacle == nullptr) {
			vector<cherry::Object*> enemiesInRange = getEnemiesInDash(dashVec);
			for (int i = 0; i < enemiesInRange.size(); i++) {
				cherry::Object* curEnemy = enemiesInRange[i];
				int epbvSize = enemyPBs.size();
				for (int j = 0; j < epbvSize; j++) {
					if (enemiesInRange[i]->GetPhysicsBodies()[0] == enemyPBs[j]) {
						enemyPBs.erase(enemyPBs.begin() + j);
						epbvSize -= 1;
					}
				}
				
				enemiesInRange[i]->RemovePhysicsBody(enemiesInRange[i]->GetPhysicsBodies()[0]);
				DeleteObjectFromScene(enemiesInRange[i]);
				kills++;
				cout << kills << endl;
			}
			playerObj->SetPosition(playerObj->GetPosition() + dashVec);
		}
		else {
			cherry::Vec3 dP = closestObstacle->GetModelPosition() - playerObj->GetPosition();
			cherry::Vec3 dPN;
			
			if (dP.GetLength() < tempDist) {
				if (fabsf(dP.GetX()) > fabsf(dP.GetY())) {
					float tempX = 0;
					if (dP.GetX() < 0) {
						tempX = dP.GetX() + ((playerObj->GetPBodySize().GetX() / 4) + (closestObstacle->GetObject()->GetPBodySize().GetX() / 4));
					}
					else {
						tempX = dP.GetX() - ((playerObj->GetPBodySize().GetX() / 4) + (closestObstacle->GetObject()->GetPBodySize().GetX() / 4));
					}
					float angle = getXYAngle(dP);
					float tempY = tempX / tanf(angle);
					dPN.SetX(tempX);
					dPN.SetY(tempY);
				}
				else {
					float tempY = 0;
					if (dP.GetY() < 0) {
						tempY = dP.GetY() + ((playerObj->GetPBodySize().GetY() / 4) + (closestObstacle->GetObject()->GetPBodySize().GetY() / 4));
					}
					else {
						tempY = dP.GetY() - ((playerObj->GetPBodySize().GetY() / 4) + (closestObstacle->GetObject()->GetPBodySize().GetY() / 4));
					}
					float angle = getXYAngle(dP);
					float tempX = tempY * tanf(angle);
					dPN.SetX(tempX);
					dPN.SetY(tempY);
				}

				vector<cherry::Object*> enemiesInRange = getEnemiesInDash(dPN);
				for (int i = 0; i < enemiesInRange.size(); i++) {
					cherry::Object* curEnemy = enemiesInRange[i];
					int epbvSize = enemyPBs.size();
					for (int j = 0; j < epbvSize; j++) {
						if (enemiesInRange[i]->GetPhysicsBodies()[0] == enemyPBs[j]) {
							enemyPBs.erase(enemyPBs.begin() + j);
							epbvSize -= 1;
						}
					}

					enemiesInRange[i]->RemovePhysicsBody(enemiesInRange[i]->GetPhysicsBodies()[0]);
					DeleteObjectFromScene(enemiesInRange[i]);
					kills++;
					cout << kills << endl;
				}
				playerObj->SetPosition(playerObj->GetPosition() + dPN);
			}
			else {
				vector<cherry::Object*> enemiesInRange = getEnemiesInDash(dashVec);
				for (int i = 0; i < enemiesInRange.size(); i++) {
					cherry::Object* curEnemy = enemiesInRange[i];
					int epbvSize = enemyPBs.size();
					for (int j = 0; j < epbvSize; j++) {
						if (enemiesInRange[i]->GetPhysicsBodies()[0] == enemyPBs[j]) {
							enemyPBs.erase(enemyPBs.begin() + j);
							epbvSize -= 1;
						}
					}

					enemiesInRange[i]->RemovePhysicsBody(enemiesInRange[i]->GetPhysicsBodies()[0]);
					DeleteObjectFromScene(enemiesInRange[i]);
					kills++;
					cout << kills << endl;
				}
				playerObj->SetPosition(playerObj->GetPosition() + dashVec);
			}
		}
		
	}
	else if (mbLP == true && mbLR == false) // before dash, while left mouse is being held
	{
		playerObj->SetDashTime(playerObj->GetDashTime() + 1.25f * deltaTime);
		//std::cout << playerObj->GetDashTime() << std::endl;
	}
	else if (mbLP == true && mbLR == true) { // left mouse has been released, reset dash timer
		playerObj->SetDashTime(0.0f);
		//Logger::GetLogger()->info(this->dashTime);
		this->mbLP = false;
		this->mbLR = false;
	}

	// Path update
	testObj->Update(deltaTime);
	
	//Enemy AI
	for (int i = 0; i < enemyGroups.size(); i++) {
		for (int j = 0; j < enemyGroups[i].size(); j++) {
			//Look at player
			enemyGroups[i][j]->UpdateAngle(enemyGroups[i][j]->GetPosition(), playerObj->GetPosition());
			enemyGroups[i][j]->SetRotation(cherry::Vec3(90.0f, 0.0f, enemyGroups[i][j]->GetDegreeAngle() - 90), true);

			if (enemyGroups[i][j]->WhoAmI() == "Sentry" && enemyGroups[i][j]->attacking == false && enemyGroups[i][j]->alive == true) {
				if (GetDistance(playerObj->GetPosition(), enemyGroups[i][j]->GetPosition()) < 10.0f) {
					//Spawn projectiles
					enemyGroups[i][j]->attacking = true;
					projList.push_back(new Projectile(arrowBase));
					projTimeList.push_back(0);
					//projList[projList.size() - 1]->AddPhysicsBody(new cherry::PhysicsBodyBox(enemyGroups[i][j]->GetPosition(), enemyGroups[i][j]->GetPBodySize()));
					//projectilePBs.push_back(projList[projList.size() - 1]->GetPhysicsBodies()[0]);
					projList[projList.size() - 1]->SetWhichGroup(i);
					projList[projList.size() - 1]->SetWhichEnemy(j);
					projList[projList.size() - 1]->active = true;
					projList[projList.size() - 1]->SetPosition(enemyGroups[i][j]->GetPosition());
					projList[projList.size() - 1]->SetDirVec(GetUnitDirVec(projList[projList.size() - 1]->GetPosition(), playerObj->GetPosition()));
					AddObjectToScene(projList[projList.size() - 1]);
				}
				else {
					//Move towards player				
					enemyGroups[i][j]->SetPosition(enemyGroups[i][j]->GetPosition() + (GetUnitDirVec(enemyGroups[i][j]->GetPosition(), playerObj->GetPosition()) * 50.0f * deltaTime));
				}
			}
			enemyGroups[i][j]->Update(deltaTime);
		}
	}

	//Update Projectiles
	for (int i = 0; i < projList.size(); i++) {
		if (projList[i]->active == true) {
			projList[i]->SetPosition(projList[i]->GetPosition() + (projList[i]->GetDirectionVec() * (100.0f * deltaTime)));
			projTimeList[i]++;
			if (projTimeList[i] >= 60 * 5) {
				enemyGroups[projList[i]->GetWhichGroup()][projList[i]->GetWhichEnemy()]->attacking = false;
				projList[i]->active = false;
				projList[i]->SetPosition(cherry::Vec3(1000, 1000, 1000));
				//DeleteObjectFromScene(projList[i]);
				projList.erase(projList.begin() + i);
				projTimeList.erase(projTimeList.begin() + i);
			}
		}
	}

	//// update physics bodies
	// player PB
	playerObj->GetPhysicsBodies()[0]->SetModelPosition(playerObj->GetPosition());
	// enemy PBs
	for (int i = 0; i < enemyPBs.size(); i++) {
		enemyPBs[i]->SetModelPosition(enemyPBs[i]->GetObject()->GetPosition());
	}
	// test PB
	//testObj->GetPhysicsBodies()[0]->SetModelPosition(testObj->GetPosition());


	// camera position update code
	if (myCamera->GetPosition().x != playerObj->GetPosition().GetX() || myCamera->GetPosition().y != playerObj->GetPosition().GetY() + 5.0f) {
		if (!playerObj->IsDashing()) {
			goto notDashing;
		}
		
		if (camLerpPercent >= 1.0f) {
			camLerpPercent = 0.0f;
		}
		camLerpPercent += 0.01f;

		glm::vec3 temp;
		glm::vec2 xyCam;
		glm::vec2 xyPla;
		cherry::Vec2 xyCur;

		xyCam.x = myCamera->GetPosition().x;
		xyCam.y = myCamera->GetPosition().y;

		xyPla.x = playerObj->GetPosition().GetX();
		xyPla.y = playerObj->GetPosition().GetY() + 5.0f;

		xyCur = cherry::Vec2::Lerp(xyCam, xyPla, camLerpPercent);

		temp.x = xyCur.GetX();
		temp.y = xyCur.GetY();
		temp.z = 20.0f;
		
		myCamera->SetPosition(temp);
	}
	else {
		camLerpPercent = 0.0f;
		playerObj->SetDash(false);
	notDashing:
		myCamera->SetPosition(cherry::Vec3(playerObj->GetPosition().GetX(), playerObj->GetPosition().GetY() + 5.0f, 20.0f));
	}

	// calls the main game Update function to go through every object.
	Game::Update(deltaTime);
}

//Get Distance Between two Vectors in xy axis
float GetDistance(cherry::Vec3 one, cherry::Vec3 two) {
	float x = two.GetX() - one.GetX();
	float y = two.GetY() - one.GetY();

	return sqrt(pow(x, 2) + pow(y, 2));
}

//Gets unit direction vector between two vectors
cherry::Vec3 GetUnitDirVec(cherry::Vec3 one, cherry::Vec3 two) {
	cherry::Vec3 newVec = two - one;
	newVec.SetZ(0.0f);
	float temp = (newVec.GetX() * newVec.GetX() + newVec.GetY() * newVec.GetY());

	return newVec / temp;
}

//Lerp between two vectors in xy axis
cherry::Vec3 LERP(cherry::Vec3 start, cherry::Vec3 end, float percent) {

	glm::vec3 temp;
	glm::vec2 xyStart;
	glm::vec2 xyEnd;
	cherry::Vec2 xyCur;

	xyStart.x = start.GetX();
	xyStart.y = start.GetY();

	xyEnd.x = end.GetX();
	xyEnd.y = end.GetY() + 5.0f;

	xyCur = cherry::Vec2::Lerp(xyStart, xyEnd, percent);

	temp.x = xyCur.GetX();
	temp.y = xyCur.GetY();
	temp.z = 20.0f;

	return temp;
}
