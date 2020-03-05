#include "CNZ_GameplayScene.h"
#include "..\CNZ_Game.h"

// Forward Declares
// Get Distance Between two Vectors in xy axis
float GetDistance(cherry::Vec3, cherry::Vec3);
//Gets unit direction vector between two vectors
cherry::Vec3 GetUnitDirVec(cherry::Vec3, cherry::Vec3);
//Lerp between two vectors in xy axis
cherry::Vec3 LERP(cherry::Vec3, cherry::Vec3, float);


// constructor
cnz::CNZ_GameplayScene::CNZ_GameplayScene(std::string legendPath, std::string levelPath, std::string sceneName)
	: cherry::GameplayScene(sceneName)
{
	// creating the map
	map = Level(legendPath, levelPath, sceneName);
}

// LoadContent for the scene
void cnz::CNZ_GameplayScene::OnOpen()
{
	using namespace cherry;
	cherry::GameplayScene::OnOpen();

	CNZ_Game* game = (CNZ_Game*)CNZ_Game::GetRunningGame();
	Camera::Sptr myCamera = game->myCamera;

	bool levelLoading = true;

	description = SamplerDesc();

	description.MinFilter = MinFilter::LinearMipNearest;
	description.MagFilter = MagFilter::Linear;
	description.WrapS = description.WrapT = WrapMode::Repeat;

	// TODO: make linear and NearestMipNearest different variables?
	// called 'Linear' in the original code
	sampler = std::make_shared<TextureSampler>(description);
	
	// initial lights 
	// TODO: take out initial lights
	LightManager::CreateSceneLightList(GetName());
	lightList = LightManager::GetSceneLightListByName(game->GetCurrentSceneName()); // getting the light list

	lightList->AddLight(new Light(GetName(), Vec3(-7.0F, 0.0F, 0.0F), Vec3(1.0F, 0.1F, 0.1F),
		Vec3(0.1F, 1.0F, 0.4F), 0.4F, 0.2F, 250.0F, 0.15F));

	lightList->AddLight(new Light(GetName(), Vec3(7.0F, 0.0F, 0.0F), Vec3(0.1, 0.1F, 1.0F),
		Vec3(0.2F, 0.7F, 0.9F), 0.3F, 0.5F, 256.0F, 0.15F));

	lightList->AddLight(new Light(GetName(), Vec3(0.0F, 7.0F, 0.0F), Vec3(0.3, 0.9F, 0.1F),
		Vec3(0.8F, 0.2F, 0.95F), 0.9F, 0.7F, 100.0F, 0.85F));

	matStatic = lightList->GenerateMaterial(STATIC_VS, STATIC_FS, sampler);
	matDynamic = lightList->GenerateMaterial(DYNAMIC_VS, DYNAMIC_FS, sampler);

	
	if (levelLoading) { // load all levels here, set main menu scene here. Change scenes in Update based on certain conditions where the level should change.
		//// LOAD LEVELS
		// the level has already been registered, and set as the main scene.
		
		// the first map
		// Level map = Level("res/loader/legend.csv", "res/loader/map.csv", "map");

		//// ADD SCENE NAMES TO SCENE LIST
		// set scene name as string, or keep using levelName->GetSceneName();
		// cherry::ObjectManager::CreateSceneObjectList(map.GetSceneName());

		//// CREATE SCENE OBJECT LISTS
		// cherry::ObjectManager::CreateSceneObjectList(sceneName);
		// cherry::ObjectManager::CreateSceneObjectList(map.GetSceneName());

		//// GET SCENE OBJECT LISTS
		objList = objectList; // inherited
		
		// do this in Update() as well, and only here for start scene

		//// REGISTER SCENES
		// SceneManager::RegisterScene(sceneName);
		// cherry::SceneManager::RegisterScene(map.GetSceneName());

		//// SET CURRENT SCENE
		// game->SetCurrentScene(map.GetSceneName(), true);
		// cherry::SceneManager::SetCurrentScene(map.GetSceneName());

		//// PUT OBJECTS IN OBJECT LIST
		std::vector<cherry::Object*> mapobjList = map.GetObjects();

		//// ADD OBJECTS TO SCENE
		for (int i = 0; i < mapobjList.size(); i++) {
			game->AddObjectToScene(mapobjList[i]);
		}

		//// Temp static light
		tempList = lightList; // inherited

		tempList->AddLight(new Light(GetName(), Vec3(0, 0, 4), Vec3(1.0f, 1.0f, 1.0f), Vec3(0.5f, 0.5f, 0.5f), 0.1f, 0.7f, 0.6f, 1.0f / 100.0f));
		// make for loop apply light to every object's material

		// only do this for starting scene, and do for current scene right after scene switch
		MapSceneObjectsToGame(map.GetSceneName());

		//// Stuff I dont need to change for now (stays as is in Game.cpp)
		// lightManager->AddSceneLightList(sceneName);
		// lightList = lightManager->GetSceneLightList(sceneName);
		// for all lights:
		// lightList->AddLight(new Light(sceneName, Vec3(), Vec3(), Vec3(), float, float, float, float);
		// material stuff that happens in Game.cpp

		//Skybox stuff
		skyboxObj = new cherry::Skybox("res/images/cubemaps/oSky.jpg", "res/images/cubemaps/oSky.jpg", 
									   "res/images/cubemaps/oFloor.jpg", "res/images/cubemaps/oSky.jpg", 
									   "res/images/cubemaps/oSky.jpg", "res/images/cubemaps/oSky.jpg");
		skyboxObj->AddSkyboxToScene(game->GetCurrentScene());
		game->SetSkybox(*skyboxObj, GetName());

		//Jonah Load Enemy Stuff
		sentry = new Sentry(GetName());
		oracle = new Oracle(GetName());
		marauder = new Marauder(GetName());
		bastion = new Bastion(GetName());
		mechaspider = new Mechaspider(GetName());
		arrowBase = new Projectile("res/objects/weapons/arrow.obj", GetName());

		for (int i = 0; i < 20; i++) {
			enemyGroups.push_back(std::vector<string>());
		}

		//Easy
		enemyGroups[0].push_back("marauder");
		enemyGroups[0].push_back("marauder");
		enemyGroups[0].push_back("sentry");

		//Easy
		enemyGroups[1].push_back("mechaspider");
		enemyGroups[1].push_back("mechaspider");
		enemyGroups[1].push_back("mechaspider");
		enemyGroups[1].push_back("mechaspider");
		enemyGroups[1].push_back("mechaspider");

		//Easy
		enemyGroups[2].push_back("bastion");
		enemyGroups[2].push_back("mechaspider");
		enemyGroups[2].push_back("mechaspider");
		enemyGroups[2].push_back("mechaspider");

		//Easy
		enemyGroups[3].push_back("marauder");
		enemyGroups[3].push_back("marauder");
		enemyGroups[3].push_back("oracle");
		enemyGroups[3].push_back("oracle");

		//Easy
		enemyGroups[4].push_back("marauder");
		enemyGroups[4].push_back("marauder");
		enemyGroups[4].push_back("marauder");
		enemyGroups[4].push_back("marauder");
		enemyGroups[4].push_back("marauder");

		//Easy
		enemyGroups[5].push_back("bastion");
		enemyGroups[5].push_back("bastion");

		//Medium
		enemyGroups[6].push_back("sentry");
		enemyGroups[6].push_back("sentry");
		enemyGroups[6].push_back("oracle");

		//Medium
		enemyGroups[7].push_back("marauder");
		enemyGroups[7].push_back("sentry");
		enemyGroups[7].push_back("oracle");
		enemyGroups[7].push_back("bastion");

		//Medium
		enemyGroups[8].push_back("oracle");
		enemyGroups[8].push_back("oracle");
		enemyGroups[8].push_back("bastion");
		enemyGroups[8].push_back("mechaspider");

		//Medium
		enemyGroups[9].push_back("sentry");
		enemyGroups[9].push_back("bastion");
		enemyGroups[9].push_back("bastion");

		//Medium
		enemyGroups[10].push_back("oracle");
		enemyGroups[10].push_back("oracle");
		enemyGroups[10].push_back("oracle");
		enemyGroups[10].push_back("oracle");

		//Medium
		enemyGroups[11].push_back("marauder");
		enemyGroups[11].push_back("marauder");
		enemyGroups[11].push_back("marauder");
		enemyGroups[11].push_back("marauder");
		enemyGroups[11].push_back("marauder");
		enemyGroups[11].push_back("marauder");
		enemyGroups[11].push_back("marauder");
		enemyGroups[11].push_back("mechaspider");

		//Medium
		enemyGroups[12].push_back("sentry");
		enemyGroups[12].push_back("sentry");
		enemyGroups[12].push_back("bastion");
		enemyGroups[12].push_back("bastion");
		enemyGroups[12].push_back("bastion");

		//Hard
		enemyGroups[13].push_back("sentry");
		enemyGroups[13].push_back("sentry");
		enemyGroups[13].push_back("sentry");
		enemyGroups[13].push_back("sentry");
		enemyGroups[13].push_back("sentry");

		//Hard
		enemyGroups[14].push_back("sentry");
		enemyGroups[14].push_back("oracle");
		enemyGroups[14].push_back("oracle");
		enemyGroups[14].push_back("oracle");
		enemyGroups[14].push_back("bastion");

		//Hard
		enemyGroups[15].push_back("marauder");
		enemyGroups[15].push_back("marauder");
		enemyGroups[15].push_back("oracle");
		enemyGroups[15].push_back("oracle");
		enemyGroups[15].push_back("oracle");
		enemyGroups[15].push_back("mechaspider");
		enemyGroups[15].push_back("mechaspider");
		enemyGroups[15].push_back("mechaspider");

		//Hard
		enemyGroups[16].push_back("bastion");
		enemyGroups[16].push_back("bastion");
		enemyGroups[16].push_back("mechaspider");
		enemyGroups[16].push_back("mechaspider");
		enemyGroups[16].push_back("mechaspider");
		enemyGroups[16].push_back("mechaspider");
		enemyGroups[16].push_back("mechaspider");
		enemyGroups[16].push_back("mechaspider");
		enemyGroups[16].push_back("mechaspider");
		enemyGroups[16].push_back("mechaspider");
		enemyGroups[16].push_back("mechaspider");
		enemyGroups[16].push_back("mechaspider");

		//Hard
		enemyGroups[17].push_back("marauder");
		enemyGroups[17].push_back("marauder");
		enemyGroups[17].push_back("marauder");
		enemyGroups[17].push_back("marauder");
		enemyGroups[17].push_back("sentry");
		enemyGroups[17].push_back("sentry");
		enemyGroups[17].push_back("bastion");
		enemyGroups[17].push_back("bastion");
		enemyGroups[17].push_back("bastion");

		//Insane
		enemyGroups[18].push_back("sentry");
		enemyGroups[18].push_back("sentry");
		enemyGroups[18].push_back("sentry");
		enemyGroups[18].push_back("oracle");
		enemyGroups[18].push_back("oracle");
		enemyGroups[18].push_back("oracle");
		enemyGroups[18].push_back("bastion");
		enemyGroups[18].push_back("bastion");
		enemyGroups[18].push_back("bastion");

		//Insane
		enemyGroups[19].push_back("marauder");
		enemyGroups[19].push_back("marauder");
		enemyGroups[19].push_back("sentry");
		enemyGroups[19].push_back("sentry");
		enemyGroups[19].push_back("sentry");
		enemyGroups[19].push_back("oracle");
		enemyGroups[19].push_back("oracle");
		enemyGroups[19].push_back("bastion");
		enemyGroups[19].push_back("mechaspider");
		enemyGroups[19].push_back("mechaspider");
		enemyGroups[19].push_back("mechaspider");

		// adds all the enemies to the scene.
		// for (int i = 0; i < enemyGroups.size(); i++)
		// 	for (int j = 0; j < enemyGroups[i].size(); j++)
		// 		AddObjectToScene(enemyGroups[i][j]);

		//Number corresponds with enemygroups first index
		SpawnEnemyGroup(19);

		indArrowAnim = new MorphAnimation();
		indArrowAnim->AddFrame(new MorphAnimationFrame("res/objects/Arrow_Start.obj", 2.0F));
		indArrowAnim->AddFrame(new MorphAnimationFrame("res/objects/Arrow_End.obj", 2.0F));
		
		indArrow = new Object("res/objects/Arrow_Start.obj", GetName(), matDynamic, false, true);
		indArrow->SetRotationXDegrees(90);
		indArrow->AddAnimation(indArrowAnim);
		//AddObjectToScene(indArrow);

		indicatorObj = new Object("res/objects/Arrow_End.obj", GetName(), matStatic, false, false); // creates indicator for dash being ready
		indicatorObj->SetRotationXDegrees(90);
		//indicatorObj->AddPhysicsBody(new cherry::PhysicsBodyBox(indicatorObj->GetPosition(), indicatorObj->GetPBodySize()));
		game->AddObjectToScene(indicatorObj);

		//// setting up the camera
		myCamera->SetPosition(glm::vec3(playerObj->GetPosition().GetX(), playerObj->GetPosition().GetY() + 5.0f, playerObj->GetPosition().GetZ() + 20.0f));
		//myCamera->LookAt(glm::vec3(0));

		//// sets the camera to perspective mode for the scene.
		//// myCamera->SetPerspectiveMode(glm::perspective(glm::radians(60.0f), 1.0f, 0.01f, 1000.0f));
		////myCamera->SetPerspectiveMode(glm::perspective(glm::radians(60.0f), 1.0f, 0.01f, 1000.0f));
		myCamera->SetPerspectiveMode(glm::radians(60.0f), 1.0f, 0.01f, 1000.0f);
		myCamera->LookAt(playerObj->GetPositionGLM());

		//// myCamera->SetPerspectiveMode(glm::perspective(glm::radians(10.0f), 1.0f, 0.01f, 1000.0f));

		//// sets the orthographic mode values. False is passed so that the camera starts in perspective mode.
		//myCamera->SetOrthographicMode(glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.0f, 100.0f), false);
		//// this is kind of useless in our game so it's commented out. No point wasting resources on it.


		
	}

	else { // for testing, loads a level for testing collision, showing all objects and test paths and such
		playerObj = new Player("res/objects/hero/charactoereee.obj", GetName()); // creates the player.
		testObj = new Player("res/objects/monkey.obj", GetName()); // creates the not player.


		// arena obstacles
		Obstacle* wall1 = new Obstacle("res/objects/GDW_1_Y2 - Wall Tile.obj", GetName(), cherry::Vec3(10, 2, 2));
		Obstacle* wall2 = new Obstacle("res/objects/GDW_1_Y2 - Wall Tile.obj", GetName(), cherry::Vec3(2, 2, 2));
		Obstacle* wall3 = new Obstacle("res/objects/GDW_1_Y2 - Wall Tile.obj", GetName(), cherry::Vec3(2, 2, 2));
		Obstacle* wall4 = new Obstacle("res/objects/GDW_1_Y2 - Wall Tile.obj", GetName(), cherry::Vec3(2, 2, 2));
		Obstacle* wall5 = new Obstacle("res/objects/GDW_1_Y2 - Wall Tile.obj", GetName(), cherry::Vec3(2, 2, 2));
		Obstacle* wall6 = new Obstacle("res/objects/GDW_1_Y2 - Wall Tile.obj", GetName(), cherry::Vec3(2, 2, 2));
		Obstacle* wall7 = new Obstacle("res/objects/GDW_1_Y2 - Wall Tile.obj", GetName(), cherry::Vec3(2, 2, 2));


		bow = new Obstacle("res/objects/weapons/bow.obj", GetName(), false);
		katana = new Obstacle("res/objects/weapons/katana.obj", GetName(), false);
		spear = new Obstacle("res/objects/weapons/spear.obj", GetName(), false);
		obstacles.push_back(bow);
		obstacles.push_back(katana);
		obstacles.push_back(spear);

		drum = new Obstacle("res/objects/props/drum.obj", GetName(), false);
		dumpster = new Obstacle("res/objects/props/Dumpster.obj", GetName(), false);
		lamp_Center = new Obstacle("res/objects/props/Lamp_Center.obj", GetName(), false);
		lamp_Corner = new Obstacle("res/objects/props/Lamp_Corner.obj", GetName(), false);
		lamp_Side = new Obstacle("res/objects/props/Lamp_Side.obj", GetName(), false);
		manhole = new Obstacle("res/objects/props/manhole.obj", GetName(), false);
		piller = new Obstacle("res/objects/GDW_1_Y2 - Pillar.obj", GetName(), false);
		road = new Obstacle("res/objects/props/Road.obj", GetName(), false);
		obstacles.push_back(drum);
		obstacles.push_back(dumpster);
		obstacles.push_back(lamp_Center);
		obstacles.push_back(lamp_Corner);
		obstacles.push_back(lamp_Side);
		obstacles.push_back(manhole);
		obstacles.push_back(piller);
		obstacles.push_back(road);

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
		playerObj->AddPhysicsBody(new cherry::PhysicsBodyBox(playerObj->GetPosition(), playerObj->GetPBodySize()));
		testObj->AddPhysicsBody(new cherry::PhysicsBodyBox(testObj->GetPosition(), testObj->GetPBodySize()));
		wall1->AddPhysicsBody(new cherry::PhysicsBodyBox(wall1->GetPosition(), wall1->GetPBodySize()));
		wall2->AddPhysicsBody(new cherry::PhysicsBodyBox(wall2->GetPosition(), wall2->GetPBodySize()));
		wall3->AddPhysicsBody(new cherry::PhysicsBodyBox(wall3->GetPosition(), wall3->GetPBodySize()));
		wall4->AddPhysicsBody(new cherry::PhysicsBodyBox(wall4->GetPosition(), wall4->GetPBodySize()));
		wall5->AddPhysicsBody(new cherry::PhysicsBodyBox(wall5->GetPosition(), wall5->GetPBodySize()));
		wall6->AddPhysicsBody(new cherry::PhysicsBodyBox(wall6->GetPosition(), wall6->GetPBodySize()));
		wall7->AddPhysicsBody(new cherry::PhysicsBodyBox(wall7->GetPosition(), wall7->GetPBodySize()));

		// set pbody pos and maybe rotation for static objects
		//testObj->GetPhysicsBodies()[0]->SetLocalPosition(testObj->GetPosition());
		wall1->GetPhysicsBodies()[0]->SetLocalPosition(wall1->GetPosition());
		wall2->GetPhysicsBodies()[0]->SetLocalPosition(wall2->GetPosition());
		wall3->GetPhysicsBodies()[0]->SetLocalPosition(wall3->GetPosition());
		wall4->GetPhysicsBodies()[0]->SetLocalPosition(wall4->GetPosition());
		wall5->GetPhysicsBodies()[0]->SetLocalPosition(wall5->GetPosition());
		wall6->GetPhysicsBodies()[0]->SetLocalPosition(wall6->GetPosition());
		wall7->GetPhysicsBodies()[0]->SetLocalPosition(wall7->GetPosition());

		// debug draw pbdody
		wall1->GetPhysicsBodies()[0]->SetVisible(showPBs);
		wall2->GetPhysicsBodies()[0]->SetVisible(showPBs);
		wall3->GetPhysicsBodies()[0]->SetVisible(showPBs);
		wall4->GetPhysicsBodies()[0]->SetVisible(showPBs);
		wall5->GetPhysicsBodies()[0]->SetVisible(showPBs);
		wall6->GetPhysicsBodies()[0]->SetVisible(showPBs);
		wall7->GetPhysicsBodies()[0]->SetVisible(showPBs);

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

		// add objects
		game->AddObjectToScene(playerObj);
		game->AddObjectToScene(testObj);
		int x = -27;
		for (int i = 0; i < obstacles.size(); i++) {
			game->AddObjectToScene(obstacles[i]);

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
	}

	/*for (int i = 0; i < objList->GetObjectCount(); i++)
	{
		objList->GetObjectByIndex(i)->SetVisible(true);
	}*/

	game->GetSceneLightList()->Update(0);
}

// called when the scene is being closed.
void cnz::CNZ_GameplayScene::OnClose()
{
	// TODO: delete other pointers
	cherry::GameplayScene::OnClose();
}

// mouse button has been pressed.
void cnz::CNZ_GameplayScene::MouseButtonPressed(GLFWwindow* window, int button)
{
	// checks each button
	switch (button) {
	case GLFW_MOUSE_BUTTON_LEFT:
		//this->mbLP = true;
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		break;
	}
}

void cnz::CNZ_GameplayScene::MouseButtonReleased(GLFWwindow* window, int button)
{
	// checks each button
	switch (button) {
	case GLFW_MOUSE_BUTTON_LEFT:
		//this->mbLR = true;
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		break;
	}
}

// key pressed
void cnz::CNZ_GameplayScene::KeyPressed(GLFWwindow* window, int key)
{
	switch (key) // checks the keys
	{
	case GLFW_KEY_SPACE:
		//myCamera->SwitchViewMode();
		spaceP = true;
		mbLP = true;
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
	case GLFW_KEY_F: // right
		f = true;
		break;
	case GLFW_KEY_LEFT_SHIFT:
		ls = true;
		break;
	}
}

// key held
void cnz::CNZ_GameplayScene::KeyHeld(GLFWwindow* window, int key)
{
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
void cnz::CNZ_GameplayScene::KeyReleased(GLFWwindow* window, int key)
{
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
	case GLFW_KEY_F: // right
		f = false;
		break;

	case GLFW_KEY_LEFT_SHIFT:
		ls = false;
		break;
	case GLFW_KEY_SPACE:
		mbLR = true;
		spaceR = true;
		break;
	}
}

//// TODO: Why is this defined in here? Should it not be in some sort of PhysicsBody file or something?
//// ^ no particular reason. It could also be defined in the Player class. It doesn't have anything to do with PhysicsBodies specifically
//// so it should not be defined there IMO.
// returns the physics body of the closest obstacle to the player, in the direction that the player is facing.
// Used later to verify if the player is allowed to dash the full distance in their intended direction
cherry::PhysicsBody* cnz::CNZ_GameplayScene::GetClosestObstacle()
{
	cherry::PhysicsBody* closestBody = nullptr;
	float cbDist = 0.0f;
	cherry::Vec3 delta;
	float angleFromPlayer;
	float dAngle;
	for (int i = 0; i < obstacles.size(); i++) {
		delta.SetX(obstacles[i]->GetPhysicsBodies()[0]->GetLocalPosition().GetX() - playerObj->GetPosition().GetX());
		delta.SetY(obstacles[i]->GetPhysicsBodies()[0]->GetLocalPosition().GetY() - playerObj->GetPosition().GetY());

		angleFromPlayer = GetXYAngle(delta);
		dAngle = angleFromPlayer - GetXYAngle(playerObj->GetDash(playerObj->GetDashDist()));

		if (dAngle <= 0.25 && dAngle >= -0.25) { // if angle difference is less than ~15 degrees. 
			if (cbDist == 0.0f) { // if this is the first loop. (we should never get a dist of 0.0f anyway.
				closestBody = obstacles[i]->GetPhysicsBodies()[0];
				cbDist = delta.GetLength();
			}
			else if (delta.GetLength() < cbDist) { // if the current pbody is closer than the last.
				closestBody = obstacles[i]->GetPhysicsBodies()[0];
				cbDist = delta.GetLength();
			}
		}
	}
	return closestBody;
}

bool cnz::CNZ_GameplayScene::GetEnemiesInDash(cherry::Vec3 dashVec, cnz::Enemy* enemy, cnz::Player* player)
{
	cherry::Vec3 delta;
	float angleFromPlayer;
	float dAngle;
	float dLen = dashVec.GetLength();

	delta.SetX(enemy->GetPhysicsBodies()[0]->GetWorldPosition().GetX() - player->GetPosition().GetX());
	delta.SetY(enemy->GetPhysicsBodies()[0]->GetWorldPosition().GetY() - player->GetPosition().GetY());

	angleFromPlayer = GetXYAngle(delta);
	dAngle = angleFromPlayer - GetXYAngle(dashVec);

	if (dAngle <= 0.25 && dAngle >= -0.25) { // if angle difference is less than ~15 degrees. 
		if (delta.GetLength() < dLen) { // if the current pbody is closer than the last.
			return true;
		}
	}
	return false;
}

// test func to get angle in xy axes of a vec. use only when you stored delta x, delta y and delta z in a vec3.
// this doesnt make sense to use on a position, but rather only on a difference in position
float cnz::CNZ_GameplayScene::GetXYAngle(cherry::Vec3 vec)
{
	return atanf(vec.GetX() / vec.GetY());
}

void cnz::CNZ_GameplayScene::SpawnEnemyGroup(int i = -1)
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
		if (enemyGroups[i][j] == "sentry") {
			enemyList.push_back(new Sentry(sentry, GetName()));
		}
		else if (enemyGroups[i][j] == "bastion") {
			enemyList.push_back(new Bastion(bastion, GetName()));
		}
		else if (enemyGroups[i][j] == "oracle") {
			enemyList.push_back(new Oracle(oracle, GetName()));
		}
		else if (enemyGroups[i][j] == "marauder") {
			enemyList.push_back(new Marauder(marauder, GetName()));
		}
		else if (enemyGroups[i][j] == "mechaspider") {
			enemyList.push_back(new Mechaspider(mechaspider, GetName()));
		}

		enemyList[enemyList.size() - 1]->SetRotation(cherry::Vec3(0, 0, 0), true);
		enemyList[enemyList.size() - 1]->SetPosition(cherry::Vec3(0 + count * 5, -10 + abs(count) * -5, 0));
		enemyList[enemyList.size() - 1]->alive = true;
		enemyList[enemyList.size() - 1]->SetRotationXDegrees(90);
		enemyList[enemyList.size() - 1]->AddPhysicsBody(new cherry::PhysicsBodyBox(enemyList[enemyList.size() - 1]->GetPosition(), enemyList[enemyList.size() - 1]->GetPBodySize()));
		enemyList[enemyList.size() - 1]->GetPhysicsBodies()[0]->SetLocalPosition(cherry::Vec3(0, 0, 1));
		objectList->AddObject(enemyList[enemyList.size() - 1]);

		if (j % 2 == 0) {
			count++;
		}
		count *= -1;
	}
}

void cnz::CNZ_GameplayScene::MapSceneObjectsToGame(std::string sceneName) {

	objList = cherry::ObjectManager::GetSceneObjectListByName(sceneName);
	std::vector<cherry::Object*> allSceneObjects = objList->GetObjects();
	std::string curObjStr;

	this->obstacles.clear();


	for (int i = 0; i < allSceneObjects.size(); i++) {
		curObjStr = allSceneObjects[i]->ToString();

		if (curObjStr.find("GDW_1_Y2 - Tile Sets (MAS_1 - ASN03 - Texturing).blend") != std::string::npos) { // wall
			//std::cout << "its a wall" << std::endl;
			this->obstacles.push_back(allSceneObjects[i]);
			//cherry::Material::Sptr mat = allSceneObjects[i]->GetMaterial();
			//tempList->ApplyLights(mat, tempList->GetLightCount());
		}
		else if (curObjStr.find("charactoereee.blend") != std::string::npos) { // player
			//std::cout << "its a player" << std::endl;
			this->playerObj = (cnz::Player*)allSceneObjects[i];
			this->playerObj->SetPosition(allSceneObjects[i]->GetPosition());
			this->playerObj->SetRotation(cherry::Vec3(0, 0, 0), true);
			this->playerObj->SetRotationXDegrees(90);
			this->playerObj->SetRotationZDegrees(180);
			this->playerObj->AddPhysicsBody(new cherry::PhysicsBodyBox(playerObj->GetPosition(), playerObj->GetPBodySize()));
		}
		else if (curObjStr.find("Dumpster.blend") != std::string::npos) { // dumpster
			//std::cout << "its a dumpster" << std::endl;
			this->obstacles.push_back(allSceneObjects[i]);
			//cherry::Material::Sptr mat = allSceneObjects[i]->GetMaterial();
			//tempList->ApplyLights(mat, tempList->GetLightCount());
		}
		else if (curObjStr.find("Lamp_Side.blend") != std::string::npos) { // lamp post
			//std::cout << "its a lamp post" << std::endl;
			this->obstacles.push_back(allSceneObjects[i]);
			tempList->AddLight(new cherry::Light(GetName(), allSceneObjects[i]->GetPosition(), cherry::Vec3(1.0f, 1.0f, 1.0f), cherry::Vec3(0.5f, 0.5f, 0.5f), 0.1f, 0.7f, 0.6f, 1.0f / 100.0f));

			//cherry::Material::Sptr mat = allSceneObjects[i]->GetMaterial();
			//tempList->ApplyLights(mat, tempList->GetLightCount());
		}
		else if (curObjStr.find("Lamp_Corner.blend") != std::string::npos) { // lamp post corner
		//	std::cout << "its a lamp post corner" << std::endl;
			this->obstacles.push_back(allSceneObjects[i]);
			tempList->AddLight(new cherry::Light(GetName(), allSceneObjects[i]->GetPosition(), cherry::Vec3(1.0f, 1.0f, 1.0f), cherry::Vec3(0.5f, 0.5f, 0.5f), 0.1f, 0.7f, 0.6f, 1.0f / 100.0f));

			//cherry::Material::Sptr mat = allSceneObjects[i]->GetMaterial();
			//tempList->ApplyLights(mat, tempList->GetLightCount());
		}
		else if (curObjStr.find("Lamp_Center.blend") != std::string::npos) { // lamp post middle
			//std::cout << "its a lamp post middle" << std::endl;
			this->obstacles.push_back(allSceneObjects[i]);
			tempList->AddLight(new cherry::Light(GetName(), allSceneObjects[i]->GetPosition(), cherry::Vec3(1.0f, 1.0f, 1.0f), cherry::Vec3(0.5f, 0.5f, 0.5f), 0.1f, 0.7f, 0.6f, 1.0f / 100.0f));

			//cherry::Material::Sptr mat = allSceneObjects[i]->GetMaterial();
			//tempList->ApplyLights(mat, tempList->GetLightCount());
		}
		else if (curObjStr.find("drum.blend") != std::string::npos) { // barrel
			//std::cout << "its a barrel" << std::endl;
			this->obstacles.push_back(allSceneObjects[i]);
			//cherry::Material::Sptr mat = allSceneObjects[i]->GetMaterial();
			//tempList->ApplyLights(mat, tempList->GetLightCount());
		}
		else if (curObjStr.find("katana.blend") != std::string::npos) { // katana
			//std::cout << "its a katana" << std::endl;
			this->obstacles.push_back(allSceneObjects[i]);
			//cherry::Material::Sptr mat = allSceneObjects[i]->GetMaterial();
			//tempList->ApplyLights(mat, tempList->GetLightCount());
		}
		else if (curObjStr.find("GDW_1_Y2 - Pillar.blend") != std::string::npos) { // pillar
			//std::cout << "its a pillar" << std::endl;
			this->obstacles.push_back(allSceneObjects[i]);
			//cherry::Material::Sptr mat = allSceneObjects[i]->GetMaterial();
			//tempList->ApplyLights(mat, tempList->GetLightCount());
		}
		else if (curObjStr.find("manhole.blend") != std::string::npos) { // manhole
			//std::cout << "its a manhole" << std::endl;
			this->obstacles.push_back(allSceneObjects[i]);
			//cherry::Material::Sptr mat = allSceneObjects[i]->GetMaterial();
			//tempList->ApplyLights(mat, tempList->GetLightCount());
		}
		else if (curObjStr.find("Road.blend") != std::string::npos) { // road
			//std::cout << "its a road" << std::endl;
			this->obstacles.push_back(allSceneObjects[i]);
			//cherry::Material::Sptr mat = allSceneObjects[i]->GetMaterial();
			//tempList->ApplyLights(mat, tempList->GetLightCount());
		}
		else if (curObjStr.find("sidewalk.blend") != std::string::npos) { // sidewalk
			//std::cout << "its a sidewalk" << std::endl;
			this->obstacles.push_back(allSceneObjects[i]);
			//cherry::Material::Sptr mat = allSceneObjects[i]->GetMaterial();
			//tempList->ApplyLights(mat, tempList->GetLightCount());
		}
	}
	tempList->Update(0.0f);
	allSceneObjects.clear(); // clear up some memory since all of these pointers are added to other lists
}

// update loop
void cnz::CNZ_GameplayScene::Update(float deltaTime)
{
	CNZ_Game* game = (CNZ_Game*)cherry::Game::GetRunningGame();
	cherry::Camera::Sptr myCamera = game->myCamera;

	//GLenum test = glGetError();
	this->playerPrevPos = playerObj->GetPosition(); // store previous position

	if (showPBs) { // show player and projectile physics bodies if showPBs
		playerObj->GetPhysicsBodies()[0]->SetVisible(true);
		for (int i = 0; i < projList.size(); i++) {
			projList[i]->GetPhysicsBodies()[0]->SetVisible(true);
		}
	}

	float moveInc = -10.0F; // the movement incrementer.

	vector<cherry::PhysicsBody*> playerObstacleCollisions; // obstacle PBs with which the player's PB is colliding with
	vector<cherry::PhysicsBody*> playerEnemyCollisions; // enemy PBs with which the player's PB is colliding with

	// find all obstacles the player is colliding with
	for (int i = 0; i < obstacles.size(); i++) {
		bool collision = false;
		if (showPBs) { // shows obstacle physics bodies if showPBs
			obstacles[i]->GetPhysicsBodies()[0]->SetVisible(true);
		}
		collision = cherry::PhysicsBody::Collision(playerObj->GetPhysicsBodies()[0], obstacles[i]->GetPhysicsBodies()[0]);
		if (collision) {
			playerObstacleCollisions.push_back(obstacles[i]->GetPhysicsBodies()[0]);
			std::cout << "collision with obstacle " << i << std::endl;
		}
	}

	// find all enemies the player is colliding with
	for (int i = 0; i < enemyList.size(); i++) {
		if (showPBs) { // shows enemy pbs if showPBs
			enemyList[i]->GetPhysicsBodies()[0]->SetVisible(true);
		}
		bool collision = cherry::PhysicsBody::Collision(playerObj->GetPhysicsBodies()[0], enemyList[i]->GetPhysicsBodies()[0]);
		if (collision) {
			//Player takes damage
		}
	}

	cs = true;
	cw = true;
	ca = true;
	cd = true;


	// TODO: actually fix collisions... and allow player to move out if colliding in all directions. (AKA when all of the above booleans are false)
	// check what directions the player can move in based on its collisions with obstacles in the scene.
	if (playerObstacleCollisions.size() != 0) { // allow movement only in directions oposite of the collision (CUBES ONLY)
		// std::cout << "There are " << playerObstacleCollisions.size() << " playerObj collisions this update!" << std::endl;
		for (int i = 0; i < playerObstacleCollisions.size(); i++) {
			cherry::Vec3 dP = playerObstacleCollisions[i]->GetWorldPosition() - playerObj->GetPosition();

			if ((playerObstacleCollisions[i]->GetWorldPosition().GetY() - playerObj->GetPosition().GetY()) >= 0) { // above the object
				cs = false;
			}
			if ((playerObstacleCollisions[i]->GetWorldPosition().GetY() - playerObj->GetPosition().GetY()) <= 0) { // below the object
				cw = false;
			}
			if ((playerObstacleCollisions[i]->GetWorldPosition().GetX() - playerObj->GetPosition().GetX()) >= 0) { // right of the object
				ca = false;
			}
			if ((playerObstacleCollisions[i]->GetWorldPosition().GetX() - playerObj->GetPosition().GetX()) <= 0) { // left of the object
				cd = false;
			}
			/* This is the old method of detecting which way the player can move. It's broken I'm pretty sure.
			if (fabsf(dP.GetX()) < fabsf(dP.GetY())) { // this is why its cubes only
				if ((playerObstacleCollisions[i]->GetWorldPosition().GetY() - playerObj->GetPosition().GetY()) >= 0) { // above the object
					cs = false;
				}
				else if ((playerObstacleCollisions[i]->GetWorldPosition().GetY() - playerObj->GetPosition().GetY()) <= 0) { // below the object
					cw = false;
				}
			}
			else if (fabsf(dP.GetX()) > fabsf(dP.GetY())) { // this is the same thing, also why its cube only.
				if ((playerObstacleCollisions[i]->GetWorldPosition().GetX() - playerObj->GetPosition().GetX()) >= 0) { // right of the object
					ca = false;
				}
				else if ((playerObstacleCollisions[i]->GetWorldPosition().GetX() - playerObj->GetPosition().GetX()) <= 0) { // left of the object
					cd = false;
				}
			}
			*/
		}
	}

	//// side checks
	//if ((playerObstacleCollisions[i]->GetLocalPosition().GetX() - playerObj->GetPosition().GetX()) >= 0) // right of the object
	//if ((playerObstacleCollisions[i]->GetLocalPosition().GetX() - playerObj->GetPosition().GetX()) <= 0) // left of the object
	//if ((playerObstacleCollisions[i]->GetLocalPosition().GetY() - playerObj->GetPosition().GetY()) >= 0) // above the object
	//if ((playerObstacleCollisions[i]->GetLocalPosition().GetY() - playerObj->GetPosition().GetY()) <= 0) // below the object

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

	playerObj->UpdateAngle(myCamera, game->GetCursorPosX(), game->GetCursorPosY(), game->GetWindowWidth(), game->GetWindowHeight());
	playerObj->SetRotation(cherry::Vec3(90.0f, 0.0f, playerObj->GetDegreeAngle() - 90), true);

	// dodge code
	if (ls) {
		cherry::Vec3 temp;
		if (w && cw) {
			temp.SetY(temp.GetY() - 1.0f);
		}
		if (s && cs) {
			temp.SetY(temp.GetY() + 1.0f);
		}
		if (a && ca) {
			temp.SetX(temp.GetX() + 1.0f);
		}
		if (d && cd) {
			temp.SetX(temp.GetX() - 1.0f);
		}

		playerObj->SetPosition(playerObj->GetPosition() + temp * 2);

		ls = false;
	}

	int enemyCount = 0;

	//Enemy AI
	for (int i = 0; i < enemyList.size(); i++) {
		if (enemyList[i]->alive == true) {
			enemyCount++;
			if (f == true && enemyList[i]->stunned == false) {
				enemyList[i]->stunned = true;
				enemyList[i]->stunTimer = 0;
			}
			else if (enemyList[i]->stunned == true) {
				if (enemyList[i]->stunTimer >= 5.0f) {
					enemyList[i]->stunned = false;
				}
				else {
					enemyList[i]->stunTimer += deltaTime;
				}
			}
		}

		if (enemyList[i]->stunned == false) {
			//Look at player
			enemyList[i]->UpdateAngle(enemyList[i]->GetPhysicsBodies()[0]->GetWorldPosition(), playerObj->GetPhysicsBodies()[0]->GetWorldPosition());
			enemyList[i]->SetRotation(cherry::Vec3(90.0f, 0.0f, enemyList[i]->GetDegreeAngle()), true);

			if (enemyList[i]->WhoAmI() == "Sentry") {
				if (GetDistance(playerObj->GetPosition(), enemyList[i]->GetPosition()) < 10.0f && enemyList[i]->attacking == false) {
					////Spawn projectiles
					//enemyList[i]->attacking = true;
					//projList.push_back(new Projectile(*arrowBase));
					//projTimeList.push_back(0);
					//projList[projList.size() - 1]->AddPhysicsBody(new cherry::PhysicsBodyBox(enemyList[i]->GetPhysicsBodies()[0]->GetWorldPosition(), projList[projList.size() - 1]->GetPBodySize()));
					//projList[projList.size() - 1]->SetWhichGroup(i);
					////projList[projList.size() - 1]->SetWhichEnemy(j);
					//projList[projList.size() - 1]->active = true;
					//projList[projList.size() - 1]->SetPosition(enemyList[i]->GetPosition());
					//projList[projList.size() - 1]->SetRotationDegrees(enemyList[i]->GetRotationDegrees());
					//projList[projList.size() - 1]->SetDirVec(GetUnitDirVec(projList[projList.size() - 1]->GetPosition(), playerObj->GetPosition()));
					//game->AddObjectToScene(projList[projList.size() - 1]);
				}
				else if (GetDistance(playerObj->GetPosition(), enemyList[i]->GetPosition()) > 10.0f) {
					//Move towards player				
					enemyList[i]->SetPosition(enemyList[i]->GetPosition() + (GetUnitDirVec(enemyList[i]->GetPosition(), playerObj->GetPosition()) * 10.0f * deltaTime));
				}
			}
			else if (enemyList[i]->WhoAmI() == "Marauder" && enemyList[i]->attacking == false) {
				if (GetDistance(playerObj->GetPosition(), enemyList[i]->GetPosition()) < 2.0f) {
					//Attack
				}
				else {
					//Move towards player				
					enemyList[i]->SetPosition(enemyList[i]->GetPosition() + (GetUnitDirVec(enemyList[i]->GetPosition(), playerObj->GetPosition()) * 10.0f * deltaTime));
				}
			}
			else if (enemyList[i]->WhoAmI() == "Oracle" && enemyList[i]->attacking == false) {
				if (GetDistance(playerObj->GetPosition(), enemyList[i]->GetPosition()) < 5.0f) {
					//Attack
				}
				else {
					//Move towards player				
					enemyList[i]->SetPosition(enemyList[i]->GetPosition() + (GetUnitDirVec(enemyList[i]->GetPosition(), playerObj->GetPosition()) * 10.0f * deltaTime));
				}
			}
			else if (enemyList[i]->WhoAmI() == "Bastion" && enemyList[i]->attacking == false) {
				if (GetDistance(playerObj->GetPosition(), enemyList[i]->GetPosition()) < 2.0f) {
					//Attack
				}
				else {
					//Move towards player				
					enemyList[i]->SetPosition(enemyList[i]->GetPosition() + (GetUnitDirVec(enemyList[i]->GetPosition(), playerObj->GetPosition()) * 10.0f * deltaTime));
				}
			}
			else if (enemyList[i]->WhoAmI() == "Mechaspider" && enemyList[i]->attacking == false) {
				if (GetDistance(playerObj->GetPosition(), enemyList[i]->GetPosition()) < 6.0f) {
					//Attack
				}
				else {
					//Move towards player				
					enemyList[i]->SetPosition(enemyList[i]->GetPosition() + (GetUnitDirVec(enemyList[i]->GetPosition(), playerObj->GetPosition()) * 10.0f * deltaTime));
				}
			}
			enemyList[i]->Update(deltaTime);
		}
	}

	// Spawn new wave when all enemies are dead
	if (enemyCount == 0) {
		SpawnEnemyGroup();
	}
	//Update Projectiles
	for (int i = 0; i < projList.size(); i++) {
		if (projList[i]->active == true) {
			projList[i]->SetPosition(projList[i]->GetPosition() + (projList[i]->GetDirectionVec() * (100.0f * deltaTime)));
			projList[i]->GetPhysicsBodies()[0]->SetLocalPosition(cherry::Vec3(0, 0, 0));
			projTimeList[i] += deltaTime;
			if (projTimeList[i] >= 5 || cherry::PhysicsBody::Collision(playerObj->GetPhysicsBodies()[0], projList[i]->GetPhysicsBodies()[0])) {
				projList[i]->RemovePhysicsBody(projList[i]->GetPhysicsBodies()[0]);
				enemyList[projList[i]->GetWhichGroup()]->attacking = false;
				projList[i]->active = false;
				projList[i]->SetPosition(cherry::Vec3(1000, 1000, 1000));
				game->DeleteObjectFromScene(projList[i]);
				projList.erase(projList.begin() + i);
				projTimeList.erase(projTimeList.begin() + i);
			}
		}
	}

	//// DASH CODE

	// Dash indicator
	if (playerObj->GetDashTime() >= 1.0f) {
		//Display indicator
		//indArrowAnim->Play();
		//indArrow->SetPosition(playerObj->GetPosition() + cherry::Vec3(0, 0, -2));
		indicatorObj->SetPosition(playerObj->GetPosition() + cherry::Vec3(0, 0, -4));
		indicatorObj->SetVisible(true);
		indicatorObj->SetRotationZDegrees(playerObj->GetRotationZDegrees() + 180);
	}
	else {
		//Hide indicator
		indicatorObj->SetVisible(false);
	}

	if (playerObj->GetDashTime() >= 1.0f && mbLR == true) // if dash timer is above 1.0 and left mouse has been released, do the dash
	{
		cherry::Vec3 dashVec = playerObj->GetDash(playerObj->GetDashDist());
		float tempDist = dashVec.GetLength();
		playerObj->SetDash(true);
		playerObj->SetDashTime(0.0f);

		cherry::PhysicsBody* closestObstacle = GetClosestObstacle();
		
		if (closestObstacle == nullptr) { // if there is no obstacle intersecting the dash vector, dash the full length
			for (int i = 0; i < enemyList.size(); i++) { 
				if (GetEnemiesInDash(dashVec, enemyList[i], playerObj)) { // kill enemies in the dash vector
					enemyList[i]->alive = false;
				}
			}
			playerObj->SetPosition(playerObj->GetPosition() + dashVec); // move the player
		}

		else {
			cherry::Vec3 dP = closestObstacle->GetLocalPosition() - playerObj->GetPosition();
			cherry::Vec3 dPN;

			cherry::Vec3 plyrMeshBody = (playerObj->GetMeshBodyMaximum() - playerObj->GetMeshBodyMinimum());
			cherry::Vec3 obstMeshBody{};

			if (closestObstacle->GetObject() != nullptr) // checking for nullptr.
				obstMeshBody = (closestObstacle->GetObject()->GetMeshBodyMaximum() - closestObstacle->GetObject()->GetMeshBodyMinimum());

			if (dP.GetLength() < tempDist) {
				if (fabsf(dP.GetX()) > fabsf(dP.GetY())) {
					float tempX = 0;
					if (dP.GetX() < 0) {
						tempX = dP.GetX() + ((plyrMeshBody.GetX() / 4) + (obstMeshBody.GetX() / 4));
					}
					else {
						tempX = dP.GetX() - ((plyrMeshBody.GetX() / 4) + (obstMeshBody.GetX() / 4));
					}
					float angle = GetXYAngle(dP);
					float tempY = tempX / tanf(angle);
					dPN.SetX(tempX);
					dPN.SetY(tempY);
				}
				else {
					float tempY = 0;
					if (dP.GetY() < 0) {
						tempY = dP.GetY() + ((plyrMeshBody.GetY() / 4) + (obstMeshBody.GetY() / 4));
					}
					else {
						tempY = dP.GetY() - ((plyrMeshBody.GetY() / 4) + (obstMeshBody.GetY() / 4));
					}
					float angle = GetXYAngle(dP);
					float tempX = tempY * tanf(angle);
					dPN.SetX(tempX);
					dPN.SetY(tempY);
				}

				for (int i = 0; i < enemyList.size(); i++) {
					if (GetEnemiesInDash(dashVec, enemyList[i], playerObj)) {
						enemyList[i]->alive = false;
					}
				}
				playerObj->SetPosition(playerObj->GetPosition() + dPN);
			}
			else {
				for (int i = 0; i < enemyList.size(); i++) {
					if (GetEnemiesInDash(dashVec, enemyList[i], playerObj)) {
						enemyList[i]->alive = false;
					}
				}
				playerObj->SetPosition(playerObj->GetPosition() + dashVec);
			}
		}
		for (int i = 0; i < enemyList.size(); i++) {
			if (enemyList[i]->alive == false) {
				enemyList[i]->RemovePhysicsBody(enemyList[i]->GetPhysicsBodies()[0]);
				cherry::Object* obj = enemyList[i];
				util::removeFromVector(enemyList, enemyList[i]);
				objectList->RemoveObjectByPointer(obj);
				delete obj;
				kills++;
				cout << kills << endl;
				i--;
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
	// testObj->Update(deltaTime);

	//// update physics bodies
	// player PB
	playerObj->GetPhysicsBodies()[0]->SetLocalPosition(cherry::Vec3(0, 0, 1));
	// enemy PBs
	for (int i = 0; i < enemyList.size(); i++) {
		//enemyList[i]->GetPhysicsBodies()[0]->SetLocalPosition(cherry::Vec3(0,0,1));
	}
	// test PB
	//testObj->GetPhysicsBodies()[0]->SetLocalPosition(testObj->GetPosition());


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
	cherry::GameplayScene::Update(deltaTime);
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

