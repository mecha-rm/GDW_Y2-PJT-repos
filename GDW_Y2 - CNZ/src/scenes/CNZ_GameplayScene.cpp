// the game breaks when certain enemies are killed sometimes... But it happens less frequently now. I don't know how to fix it.
// TODO: the enemy that shoots an arrow seems to be the problem. The hitbox is too small to hit anything.
// TODO: the player should be stopped from straying too far out of the arena.
// TODO: music cuts out if exsting the arena

#include "CNZ_GameplayScene.h"

#include "..\CNZ_Game.h"
#include "..\cherry/Instrumentation.h"
#include "CNZ_GameOverScene.h"
#include "..\cherry\utils\math\Interpolation.h"
#include "..\cherry\utils\math\Rotation.h"

#include <stack>

// static variables
std::vector<std::vector<string>> cnz::CNZ_GameplayScene::enemyGroups;
bool cnz::CNZ_GameplayScene::groupsLoaded = false;

// amount of time the player is invincible for.
const float cnz::CNZ_GameplayScene::INVINCIBLE_TIME_MAX = 5.0F;

// time stop maximum value, and amount of digits for text.
const float cnz::CNZ_GameplayScene::TIME_STOP_MAX = 30.0F;
const int cnz::CNZ_GameplayScene::TIME_STOP_DISPLAY_DIGITS_MAX = 8;
// const int cnz::CNZ_GameplayScene::DIGITS_MAX = 8; // maximum integer value is 2147483647.

// Forward Declares

// get Distance Between two Vectors in xy axis. Z-axis information is ignored.
float GetDistanceXY(cherry::Vec2, cherry::Vec2);
float GetDistanceXY(cherry::Vec3, cherry::Vec3);

// gets unit direction vector between two vectors. Z-axis information is ignored.
cherry::Vec3 GetUnitDirVecXY(cherry::Vec2, cherry::Vec2);
cherry::Vec3 GetUnitDirVecXY(cherry::Vec3, cherry::Vec3);

// lerps between two vectors on the xy axis. Z-axis information is ignored.
cherry::Vec3 CameraLerpXY(cherry::Vec2, cherry::Vec2, float);
cherry::Vec3 CameraLerpXY(cherry::Vec3, cherry::Vec3, float);

// constructor
cnz::CNZ_GameplayScene::CNZ_GameplayScene(std::string legendPath, std::string levelPath, std::string sceneName, int mapNumber)
	: cherry::GameplayScene(sceneName)
{
	// creating the map
	map = Level(legendPath, levelPath, sceneName, mapNumber);
}

// constructor - takes information from info provided.
cnz::CNZ_GameplayScene::CNZ_GameplayScene(const LevelLoadInfo& info)
	: cherry::GameplayScene(info.sceneName)
{
	// creating the map.
	map = Level(info.legendPath, info.levelPath, info.sceneName, info.mapNumber);
}

// LoadContent for the scene
void cnz::CNZ_GameplayScene::OnOpen()
{
	using namespace cherry;
	
	// if (PROFILE)
	// 	cherry::ProfilingSession::Start("profiling-cnz_gameplay_scene-open.json");

	// starts timing
	// ProfileTimer loadTimer = ProfileTimer("gameplay-on_open");

	cherry::GameplayScene::OnOpen();

	SetAllowingNewInstances(true);

	// TODO: add in sound effects, then redo the whole system.
	CNZ_Game* game = (CNZ_Game*)CNZ_Game::GetRunningGame();
	Camera::Sptr myCamera = game->myCamera;
	glm::ivec2 myWindowSize = game->GetWindowSize(); // the current window size.
	const std::string SCENE_NAME = GetName();

	// pause menu
	{
		pauseMenu = new Image("res/images/pause_menu_image.png", game->GetCurrentSceneName());
		pauseMenu->SetWindowChild(true);
		// pauseMenu->SetScale(cherry::Vec3(0.25f, 0.25f, 1.0f));
		pauseMenu->SetPositionByWindowSize(cherry::Vec2(0.5f, 0.5f));
		pauseMenu->SetPostProcess(false);
		objectList->AddObject(pauseMenu);
	}

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

	// Audio
	{
		// ProfileTimer audioLoad = ProfileTimer("gameplay-on_open-audio_loading");
		// get instance of audio
		AudioEngine& audio = AudioEngine::GetInstance();

		// switching bgms
		audio.StopEvent("bgm_01");
		audio.PlayEvent("bgm_02");
	}

	// if 'true', then the level loading will be completed.
	bool levelLoading = true;

	// default lights if no level has been loaded. These defaults are not used due to the movable light.
	if (!levelLoading)
	{
		lightList->AddLight(new Light(GetName(), Vec3(-7.0F, 0.0F, 0.0F), Vec3(1.0F, 0.1F, 0.1F),
			Vec3(0.1F, 1.0F, 0.4F), 0.4F, 0.2F, 250.0F, 0.15F));

		lightList->AddLight(new Light(GetName(), Vec3(7.0F, 0.0F, 0.0F), Vec3(0.1, 0.1F, 1.0F),
			Vec3(0.2F, 0.7F, 0.9F), 0.3F, 0.5F, 256.0F, 0.15F));

		lightList->AddLight(new Light(GetName(), Vec3(0.0F, 7.0F, 0.0F), Vec3(0.3, 0.9F, 0.1F),
			Vec3(0.8F, 0.2F, 0.95F), 0.9F, 0.7F, 100.0F, 0.85F));
	}
	else
	{
		// lightList->AddLight(new Light(GetName(), Vec3(0.0F, 0.0F, 45.0F), Vec3(1.0F, 0.0F, 0.73333333333F),
		// 	Vec3(0.002F, 0.001F, 0.2153F), 1.5F, 0.8F, 80.0F, 1.0F / 9000.0F));
	}

	// Player Light
	{
		// TODO: maybe make the light be out in front of the player instead, and be where they're facing.
		// the light is okay for now, but it could be better.
		playerLight = new Light(SCENE_NAME, Vec3(0, 0, 1.5F), Vec3(1, 1, 1), Vec3(0.015F, 0.012F, 0.912F), 0.15f, 0.12f, 10.0f, 1/100.0f);
		playerLightOffset = PLAYER_LIGHT_OFFSET_BASE;
		lightList->AddLight(playerLight);
	}

	// Player Score
	{
		std::string tempStr = "";
		tempStr.resize(SCORE_DIGIT_LIMIT, '0');

		scoreText = new cherry::Text(tempStr, GetName(), FONT_ARIAL, cherry::Vec4(1.0F, 1.0F, 1.0F, 1.0F), 10.0F);
		scoreText->SetWindowChild(true);
		scoreText->SetPostProcess(false);
		scoreText->SetPositionByWindowSize(Vec2(0.20F, 0.05F));
		scoreText->SetVisible(showScore);
		scoreText->SetScale(0.8F);

		objectList->AddObject(scoreText);
	}

	// Time Stop
	{
		std::string tempStr = "";
		tempStr.resize(TIME_STOP_DISPLAY_DIGITS_MAX, '0');

		timeStopText = new cherry::Text(tempStr, GetName(), FONT_ARIAL, cherry::Vec4(1.0F, 1.0F, 1.0F, 1.0F), 10.0F);
		timeStopText->SetWindowChild(true);
		timeStopText->SetPostProcess(false);
		timeStopText->SetPositionByWindowSize(Vec2(0.958F, 0.945F));
		timeStopText->SetVisible(showTimeStopText);
		timeStopText->SetScale(0.8F);

		objectList->AddObject(timeStopText);
	}
	
	// Default Materials
	// This is because a reference to these materails still exist in memory. When these scenes are opened again, they try to...
	// rebuild these materials for a second time, getting rid of these references. This may be the problem.
	matStatic = lightList->GenerateMaterial(STATIC_VS, STATIC_FS, sampler); // error caused here.
	matDynamic = lightList->GenerateMaterial(DYNAMIC_VS, DYNAMIC_FS, sampler);


	// load all levels here, and set main menu scene here. Change scenes in Update based on certain conditions where the level should change.
	if (levelLoading) 
	{
		// ProfileTimer levelLoad = ProfileTimer("gameplay-on_open-level_load");

		//// LOAD LEVELS
		// loads level
		// ProfileTimer mapLoad = ProfileTimer("gameplay-on_open-map_load");
		
		MapSceneObjectsToGame();

		// stops the map loading.
		// mapLoad.Stop();

		// Moved to MapSceneObjectsToGame()
		// playerObj = map.GetPlayerObject(); // gets player object
		// playerSpawn = map.GetPlayerSpawnPosition(); // gets player spawn point 

		//Skybox stuff
		skyboxObj = cherry::Skybox(
			"res/images/cubemaps/oSky.jpg", "res/images/cubemaps/oSky.jpg", 
			"res/images/cubemaps/oFloor.jpg", "res/images/cubemaps/oSky.jpg", 
			"res/images/cubemaps/oSky.jpg", "res/images/cubemaps/oSky.jpg"
		);

		// called in game->SetSkybox
		// skyboxObj.AddSkyboxToScene(game->GetCurrentScene());
		game->SetSkybox(skyboxObj, GetName());

		// Jonah Load Enemy Stuff
		// Gets the soruce objects.
		sentry = Level::sourceSentry;
		oracle = Level::sourceOracle;
		marauder = Level::sourceMarauder;
		bastion = Level::sourceBastion;
		mechaspider = Level::sourceSpider;
		arrowBase = Level::sourceArrow;

		// if the enemy groups have not been loaded yet.
		if (!groupsLoaded || enemyGroups.empty())
			LoadEnemyGroups();
		

		// times enemy spawning
		// ProfileTimer enemySpawnTimer = ProfileTimer("gameplay-on_open-enemy_init_load");

		// Number corresponds with enemygroups first index
		// SpawnEnemyGroup(4);
		SpawnEnemyGroup(rand() % 4 + 1);

		// stops eneemy spawning.
		// enemySpawnTimer.Stop();

		//indArrowAnim = new MorphAnimation();
		//indArrowAnim->AddFrame(new MorphAnimationFrame("res/objects/Arrow_Start.obj", 2.0F));
		//indArrowAnim->AddFrame(new MorphAnimationFrame("res/objects/Arrow_End.obj", 2.0F));
		
		// indArrow = new Object("res/objects/Arrow_Start.obj", GetName(), matDynamic, false, true);
		// indArrow->SetRotationXDegrees(90);
		//indArrow->AddAnimation(indArrowAnim);
		//AddObjectToScene(indArrow);

		// replacing model with image.
		// indicatorObj = new Object("res/objects/Arrow_End.obj", GetName(), matStatic, false, false); // creates indicator for dash being ready

		// new file with merged objects
		// indicatorObj = new Object("res/objects/indicator_arrow.obj", GetName(), matStatic, false, false); // creates indicator for dash being ready
		
		// the arrow, except it's flat.
		indicatorObj = new Object("res/objects/indicator_arrow_flat.obj", GetName(), matStatic, false, false); // creates indicator for dash being ready

		// the indicator shouldn't be visible at first.
		indicatorObj->SetVisible(false);
		indicatorObj->SetRotationXDegrees(90); // uncomment if using 3D model

		// indicatorObj = new Image("res/images/indicator_arrow.png", GetName(), false, false);
		// indicatorObj->SetScale(0.1F);
		

		// indicatorObj = new Image("res/images/indicator_arrow.png", GetName(), true, false);
		
		//indicatorObj->AddPhysicsBody(new cherry::PhysicsBodyBox(indicatorObj->GetPosition(), indicatorObj->GetPBodySize()));
		game->AddObjectToScene(indicatorObj);

		//// setting up the camera 
		myCamera->SetPosition(glm::vec3(playerObj->GetPosition().GetX(), playerObj->GetPosition().GetY() + CAMERA_POS_OFFSET.y, playerObj->GetPosition().GetZ() + CAMERA_POS_OFFSET.z));
		
		// sets the light to be the player position.
		playerLight->SetLightPositionX(playerObj->GetPositionX());
		playerLight->SetLightPositionY(playerObj->GetPositionY());

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


		// level loading finished.
		// levelLoad.Stop();
	}
	else // for testing, loads a level for testing collision, showing all objects and test paths and such
	{
		MapSceneObjectsToGame(levelLoading);
	}

	/*for (int i = 0; i < objList->GetObjectCount(); i++)
	{
		objList->GetObjectByIndex(i)->SetVisible(true);
	}*/

	// TODO: include life icon
	{
		// TODO: these will be toggled on and off based on how many lives the player has.
		// offset
		float offset = 0.06F;

		// icon 1
		lifeIcon_1 = new Image("res/images/life_icon.png", SCENE_NAME, false, false);
		lifeIcon_1->SetWindowChild(true);
		lifeIcon_1->SetPostProcess(false);
		lifeIcon_1->SetPositionByWindowSize(Vec2(0.95F, 0.05F));
		lifeIcon_1->SetScale(0.35F);

		objectList->AddObject(lifeIcon_1);

		// icon 2
		lifeIcon_2 = new Image("res/images/life_icon.png", SCENE_NAME, false, false);
		lifeIcon_2->SetWindowChild(true);
		lifeIcon_2->SetPostProcess(false);
		lifeIcon_2->SetPositionByWindowSize(Vec2(0.95F - offset, 0.05F));
		lifeIcon_2->SetScale(0.35F);

		objectList->AddObject(lifeIcon_2);

		// icon 3
		lifeIcon_3 = new Image("res/images/life_icon.png", SCENE_NAME, false, false);
		lifeIcon_3->SetWindowChild(true);
		lifeIcon_3->SetPostProcess(false);
		lifeIcon_3->SetPositionByWindowSize(Vec2(0.95F - offset * 2, 0.05F));
		lifeIcon_3->SetScale(0.35F);

		objectList->AddObject(lifeIcon_3);
	}

	// sets if the physics bodies should be visible.
	SetVisiblePhysicsBodies(showPBs);

	// Post Processing
	// if(levelLoading)
	if (postProcess)
	{
		// ProfileTimer postProcessLoad = ProfileTimer("gameplay-on_open-post_process_load");

		// frame buffer
		FrameBuffer::Sptr fb = std::make_shared<FrameBuffer>(myWindowSize.x, myWindowSize.y);

		// scene colour
		RenderBufferDesc sceneColor = RenderBufferDesc();
		sceneColor.ShaderReadable = true;
		sceneColor.Attachment = RenderTargetAttachment::Color0;
		sceneColor.Format = RenderTargetType::Color24; // loads with RGB

		// scene depth
		RenderBufferDesc sceneDepth = RenderBufferDesc();
		sceneDepth.ShaderReadable = true;
		sceneDepth.Attachment = RenderTargetAttachment::Depth;
		sceneDepth.Format = RenderTargetType::Depth24;

		// colour and depth attachments
		fb->AddAttachment(sceneColor);
		fb->AddAttachment(sceneDepth);

		// fb->AddAttachment()
		Registry().ctx_or_set<FrameBuffer::Sptr>(fb);

		// adds a post-processing 
		
		// lighting layer
		// layers.push_back(new PostLayer(POST_VS, "res/shaders/post/vibrance.fs.glsl"));
		lightList->SetIgnoreBackground(true);
		layers.push_back(lightList->GetPostLayer());



		glm::mat3 postMat3 = KERNEL_EDGE_3;
		// glm::mat3 postMat3 = KERNEL_BOX_BLUR;
		// edge detection layer
		edgeDetect.SetMatrix(postMat3);

		useFrameBuffers = true;

		// postProcessLoad.Stop();
	}

	// resizing the window so that it fixes the aspect ratio when switching scenes
	game->Resize(myWindowSize.x, myWindowSize.y);

	// stops the load timer
	// loadTimer.Stop();

	if (PROFILE)
		cherry::ProfilingSession::End();
}

// called when the scene is being closed.
void cnz::CNZ_GameplayScene::OnClose()
{
	// setting variables back to original values
	curWave = 0; //Current enemy wave

	mouseLeft = false;
	mouseRight = false;

	camLerpPercent = 0.0f;

	// testPlayPos = glm::vec3(0, 0, 0);
	playerPrevPos = glm::vec3(0, 0, 0);;
	playerSpawn = glm::vec3(0, 0, 0);;

	// player movement
	w = false;
	a = false;
	s = false;
	d = false;
	f = false;
	leftShift = false;
	spaceBar = false;

	// stops the player from moving through solid objects.
	cw = true;
	ca = true;
	cs = true;
	cd = true;

	paused = false;
	restart = false;

	isInvincible = false;
	invincibleCountdown = 0.0F;

	kills = 0;
	lives = 3;
	curGroup = -1;
	score = 0;

	// switching bgms for the menu.
	cherry::AudioEngine& audio = cherry::AudioEngine::GetInstance();
	audio.StopEvent("bgm_02");
	audio.PlayEvent("bgm_01"); // TODO: change audio position so that it's quieter.
	
	// TODO: delete other pointers
	cherry::GameplayScene::OnClose();
}

// creates a new instance of this scene.
cherry::Scene* cnz::CNZ_GameplayScene::GenerateNewInstance() const
{
	// creates a new instance of the gameplay scene
	CNZ_GameplayScene* cgs;
	cgs = new CNZ_GameplayScene(map.legendPath, map.levelPath, GetName(), map.mapNumber);

	return cgs;
}

// mouse button has been pressed. 
void cnz::CNZ_GameplayScene::MouseButtonPressed(GLFWwindow* window, int button)
{
	// checks each button
	switch (button) {
	case GLFW_MOUSE_BUTTON_LEFT:
		mouseLeft = true;
		break;

	case GLFW_MOUSE_BUTTON_MIDDLE:
		break;

	case GLFW_MOUSE_BUTTON_RIGHT:
		mouseRight = true;
		break;
	}
}

// mouse button has been released.
void cnz::CNZ_GameplayScene::MouseButtonReleased(GLFWwindow* window, int button)
{
	// checks each button
	switch (button) {
	case GLFW_MOUSE_BUTTON_LEFT:
		mouseLeft = false;
		break;

	case GLFW_MOUSE_BUTTON_MIDDLE:
		break;

	case GLFW_MOUSE_BUTTON_RIGHT:
		mouseRight = false;
		break;
	}
}

// TODO: add button to go back to main menu, and button to exit game.
// key pressed
void cnz::CNZ_GameplayScene::KeyPressed(GLFWwindow* window, int key)
{
	switch (key) // checks the keys
	{
	case GLFW_KEY_SPACE:
		spaceBar = true;
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

		// if (postProcess) // add post processing layer
		// {
		// 	// layers.clear();
		// 
		// 	// plays the sound effect if the layer has just been added.
		// 	if(util::addToVector(layers, edgeDetect.GetPostLayer()))
		// 		cherry::AudioEngine::GetInstance().PlayEvent("timestop");
		// 	
		// }
		break;
	case GLFW_KEY_LEFT_SHIFT:
		leftShift = true;
		break;
	case GLFW_KEY_P: // PAUSE
		paused = !paused;
		cherry::AudioEngine::GetInstance().PlayEvent("menu_click");
		break;
	case GLFW_KEY_B: // RETURN TO MENU
		if (paused)  // return to main menu
		{
			exitMap = true;
			cherry::AudioEngine::GetInstance().PlayEvent("menu_accept");
		}

		break;

	case GLFW_KEY_R: // RESTART
		if (paused) // if pasued, restart the game
		{
			restart = true;
			cherry::AudioEngine::GetInstance().PlayEvent("menu_accept");
		}
		break;
	case GLFW_KEY_ESCAPE: // close the game
		if (paused) // if paused, close the game.
		{
			cherry::Game::GetRunningGame()->CloseWindow();
		}
		else // if not paused, pause the game.
		{
			paused = true;
			cherry::AudioEngine::GetInstance().PlayEvent("menu_click");
		}
		break;
	
	// PLAYER LIGHT CONTROLS
	case GLFW_KEY_UP: // y+
		playerLightDir.y = 1;
		break;
	case GLFW_KEY_DOWN: // y-
		playerLightDir.y = -1;
		break;
	case GLFW_KEY_LEFT: // x-
		playerLightDir.x = -1;
		break;
	case GLFW_KEY_RIGHT: // x+
		playerLightDir.x = 1;
		break;
	case GLFW_KEY_PAGE_UP: // z+
		playerLightDir.z = 1;
		break;
	case GLFW_KEY_PAGE_DOWN: // z-
		playerLightDir.z = -1;
		break;
	case GLFW_KEY_PERIOD: // reset position
		playerLightOffset = PLAYER_LIGHT_OFFSET_BASE;
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
		leftShift = false;
		break;

	case GLFW_KEY_SPACE:
		spaceBar = false;
		break;

	// PLAYER LIGHT CONTROLS
	case GLFW_KEY_UP: // Y
	case GLFW_KEY_DOWN:
		playerLightDir.y = 0;
		break;

	case GLFW_KEY_LEFT: // X
	case GLFW_KEY_RIGHT:
		playerLightDir.x = 0;
		break;

	case GLFW_KEY_PAGE_UP: // Z
	case GLFW_KEY_PAGE_DOWN:
		playerLightDir.z = 0;
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
	using namespace cherry;
	
	// player position
	glm::vec3 plyrPos = playerObj->GetPositionGLM();
	// player dash distance
	float plyr_xyAngle = GetXYAngle(playerObj->GetDash(playerObj->GetDashDist()));

	// closest body
	cherry::PhysicsBody* closestBody = nullptr;
	
	float cbDist = 0.0f;
	cherry::Vec3 delta;

	float angleFromPlayer = 0.0F;
	float dAngle = 0.0F;

	// player physics bodies
	std::vector<PhysicsBody*> plyrPbs;

	for (int i = 0; i < obstacles.size(); i++) {
		// obstacle physics bodies
		std::vector<PhysicsBody*> obstPbs = obstacles[i]->GetPhysicsBodies();

		for (int j = 0; j < obstPbs.size(); j++)
		{
			delta.SetX(obstPbs[j]->GetLocalPosition().v.x - plyrPos.x);
			delta.SetY(obstPbs[j]->GetLocalPosition().v.y - plyrPos.y);

			angleFromPlayer = GetXYAngle(delta);

			// gets the dAngle
			dAngle = angleFromPlayer - plyr_xyAngle;

			if (dAngle <= 0.25 && dAngle >= -0.25) { // if angle difference is less than ~15 degrees. 
				float deltaLength = delta.GetLength();

				if (cbDist == 0.0f) { // if this is the first loop. (we should never get a dist of 0.0f anyway)
					closestBody = obstPbs[j];
					cbDist = deltaLength;
				}
				else if (deltaLength < cbDist) { // if the current pbody is closer than the last.
					closestBody = obstPbs[j];
					cbDist = deltaLength;
				}
			}
		}
	}
	return closestBody;
}

// gets the enemies in the dash.
bool cnz::CNZ_GameplayScene::GetEnemiesInDash(cherry::Vec3 dashVec, cnz::Enemy* enemy, cnz::Player* player)
{
	using namespace cherry;

	glm::vec3 plyrPos = player->GetPositionGLM();
	cherry::Vec3 delta;
	float angleFromPlayer;

	float dAngle;
	float dLen = dashVec.GetLength();

	// gets the enemies physics bodies
	std::vector<PhysicsBody*> pbs = enemy->GetPhysicsBodies();

	for (int i = 0; i < pbs.size(); i++)
	{
		glm::vec3 pbsPos = pbs[i]->GetWorldPositionGLM();

		// getting the values
		delta.v.x = pbsPos.x - plyrPos.x;
		delta.v.y = pbsPos.y - plyrPos.y;

		angleFromPlayer = GetXYAngle(delta);
		dAngle = angleFromPlayer - GetXYAngle(dashVec);

		if (dAngle <= 0.25 && dAngle >= -0.25) { // if the angle difference is less than ~15 degrees. 
			if (delta.GetLength() < dLen) { // if the current pbody is closer than the last.
				return true;
			}
		}
	}

	// not in dash
	return false;
}

// test func to get angle in xy axes of a vec. use only when you stored delta x, delta y and delta z in a vec3.
// this doesnt make sense to use on a position, but rather only on a difference in position
float cnz::CNZ_GameplayScene::GetXYAngle(cherry::Vec3 vec)
{
	return atanf(vec.v.x / vec.v.y);
}

// spawn enemy
void cnz::CNZ_GameplayScene::SpawnEnemyGroup(int i)
{
	using namespace cherry;

	std::string sceneName = GetName();
	// TODO: check for deletion
	enemyList.clear();
	curWave++;

	// gets the player position
	cherry::Vec3 plyrPos = (playerObj != nullptr)
		? playerObj->GetPosition() : cherry::Vec3(0.0F, 0.0F, 0.0F);

	// gets the player spawn index
	const glm::ivec2 PLYR_SPAWN_INDEX = map.GetPlayerSpawnIndex();

	// tells the game to use the player as a position base
	bool plyrBase = (playerObj != nullptr) ? true : false;
	// plyrBase = false; // revert to old

	// rotation factor and radius
	// this helps form a vector based on the player's current position and how far away they should be.
	// const int OPS = 5;
	// float dists[OPS] = { 25.0F, 30.0F, 35.0F, 40.0F, 45.0F };

	// gets the map limits, and the map dimensions
	const glm::vec4 MAP_LIMITS = map.GetMapLimits();
	glm::ivec2 MAP_DIMENS{ map.GetMapRowCount(), map.GetMapColumnCount() };

	// subtracts from the map dimensions if there are enough rows and columns
	glm::ivec2 mapDimensSub{};
	if (MAP_DIMENS.x >= 5 && MAP_DIMENS.y >= 5)
	{
		mapDimensSub.x = 3;
		mapDimensSub.y = 3;
	}

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

	// cout << i << endl;
	curGroup = i;

	int n = enemyGroups[i].size();
	
	// for (int j = 0; j < n; j++) {
	// 	if (enemyGroups[i][j] == "sentry") {
	// 		enemyList.push_back(new Sentry(sentry, sceneName));
	// 	}
	// 	else if (enemyGroups[i][j] == "bastion") {
	// 		enemyList.push_back(new Bastion(bastion, sceneName));
	// 	}
	// 	else if (enemyGroups[i][j] == "oracle") {
	// 		enemyList.push_back(new Oracle(oracle, sceneName));
	// 	}
	// 	else if (enemyGroups[i][j] == "marauder") {
	// 		enemyList.push_back(new Marauder(marauder, sceneName));
	// 	}
	// 	else if (enemyGroups[i][j] == "mechaspider") {
	// 		enemyList.push_back(new Mechaspider(mechaspider, sceneName));
	// 	}
	// 
	// 	// this is index
	// 	int index = enemyList.size() - 1;
	// 
	// 	enemyList[index]->SetRotation(cherry::Vec3(0, 0, 0), true);
	// 	enemyList[index]->SetPosition(cherry::Vec3(25 + count * 5, 25 - 10 + abs(count) * -5, 0));
	// 	enemyList[index]->alive = true;
	// 	enemyList[index]->SetRotationXDegrees(90);
	// 
	// 	// TODO: these physics bodies could be added to the source objects instead, which would be more efficient.
	// 	// phyiscs body
	// 	PhysicsBody* pb = new cherry::PhysicsBodyBox(enemyList[index]->GetPosition(), enemyList[index]->GetPBodySize());
	// 	pb->SetLocalPosition(cherry::Vec3(0, 0, 1));
	// 	pb->SetVisible(showPBs);
	// 
	// 	enemyList[index]->AddPhysicsBody(pb);
	// 
	// 	objectList->AddObject(enemyList[index]);
	// 
	// 	if (j % 2 == 0) {
	// 		count++;
	// 	}
	// 	count *= -1;
	// }

	// NEW
	// if (false)
	{
		std::queue<int> spawns; // amount of spawns
		Enemy* enemy;
		int eNum = 0;

		// generates a series of random numbers.
		for (int i = 1; i <= waveEnemyCount; i++)
		{
			// 0 = null, 1 = bastion, 2 = marauder, 3 = mechaspider, 4 = oracle, 5 = sentry
			spawns.push(rand() % 5 + 1);
		}

		// while there are still enemies to spawn
		while (!spawns.empty())
		{
			switch (spawns.front())
			{
			case cnz::marauder: // 1
			default: // 0
				enemy = new Marauder(marauder, sceneName);
				break;
			case cnz::oracle: // 2
				enemy = new Oracle(oracle, sceneName);
				break;
			case cnz::sentry: // 3
				enemy = new Sentry(sentry, sceneName);
				break;
			case cnz::bastion: // 4
				enemy = new Bastion(bastion, sceneName);
				break;
			case cnz::mechaspider: // 5
				enemy = new Mechaspider(mechaspider, sceneName);
				break;
			}

			// recommended to have the player near the middle of the arena
			// if the player is being used as a base
			if(plyrBase)
			{
				// // gets a random rotation factor for placing the enemy.
				// float rotDeg = rand() % 360;
				// 
				// // gets the distance from the player (radius)
				// float d = dists[rand() % OPS];
				// 
				// // rotation vector
				// util::math::Vec2 rotVec{ rotDeg, 0.0F };
				// rotVec = util::math::rotate(rotVec, rotDeg, true);
				// 
				// // sets the position of the enemy
				// enemy->SetPosition(
				// 	plyrPos.v.x + rotVec.x, 
				// 	plyrPos.v.y + rotVec.y,
				// 	0.0F);

				// gets the row and column
				int x = 0, y = 0;

				// if the map is big enough, the enemies are placed one cell in.
				x = (rand() % (MAP_DIMENS.x - mapDimensSub.x)) + mapDimensSub.x / 2;
				y = (rand() % (MAP_DIMENS.y - mapDimensSub.y)) + mapDimensSub.y / 2;

				// if the enemy would be placed right on top of the player
				if (x == PLYR_SPAWN_INDEX.x && y == PLYR_SPAWN_INDEX.y)
				{
					// forces the enemy two spaces away.
					int x2 = rand() % 3 + 2;
					int y2 = rand() % 3 + 2;

					// positive or negative
					x2 *= ((rand() % 2) > 0) ? 1 : -1;
					y2 *= ((rand() % 2) > 0) ? 1 : -1;

					x += x2;
					y += y2;
				}

				// sets the enemy position based on the randomized map dimensions.
				enemy->SetPosition(x * Level::CELL_OFFSET, y * Level::CELL_OFFSET, 0.0F);
			}
			else // original
			{
				enemy->SetPosition(cherry::Vec3(25 + eNum * 5, 25 - 10 + abs(eNum) * -5, 0));
			}

			enemy->SetRotation(cherry::Vec3(0, 0, 0), true);
			enemy->alive = true;
			enemy->SetRotationXDegrees(90);

			// gets the physics bodies, and sets whether they should be visible or not.
			if (showPBs == true)
			{
				std::vector<PhysicsBody*> pbodies = enemy->GetPhysicsBodies();
				for (PhysicsBody* body : pbodies)
				{
					body->SetVisible(showPBs);
				}
			}


			// // TODO: these physics bodies could be added to the source objects instead, which would be more efficient.
			// // phyiscs body
			// PhysicsBody* pb = new cherry::PhysicsBodyBox(enemy->GetPosition(), enemy->GetPBodySize());
			// pb->SetLocalPosition(cherry::Vec3(0, 0, 1)); // enemy->GetPosition() is useless, since this overrides it.
			// pb->SetVisible(showPBs);
			// 
			// enemy->AddPhysicsBody(pb);

			// add them to the lists
			objectList->AddObject(enemy);
			enemyList.push_back(enemy);

			// remove front
			spawns.pop();
			// enemy number 
			eNum++;
		}

		// increasing the amount of enemies per the wave.
		if (wave % 3 == 0 && waveEnemyCount < WAVE_ENEMY_COUNT_MAX)
		{
			waveEnemyCount++;
		}

		wave++;
	}

	// new wave chime
	// TODO: add a bool to check and see if it's the first time the game is starting. The voice clip doesn't make sense if the game just started.
	cherry::AudioEngine::GetInstance().PlayEvent("new_wave");
}

// generates the objects.
void cnz::CNZ_GameplayScene::MapSceneObjectsToGame(bool loadFromFile) {
	using namespace cherry;

	// generates the objects
	if (loadFromFile)
		map.GenerateObjects();
	else
		map.GenerateDefaults();
	
	// saves the object lsit
	ObjectList* objList = objectList;
	LightList* lgtList = lightList;
	
	// grabs the objects
	playerObj = map.GetPlayerObject(); // gets player object.
	playerSpawn = map.GetPlayerSpawnPosition(); // gets player spawn point.

	obstacles = map.GetObstacles(); // gets the map obstacles.

	// updates the light list to apply lights to all materials
	lgtList->Update(1.0F);
}

// loads the enemy strings
void cnz::CNZ_GameplayScene::LoadEnemyGroups()
{
	enemyGroups.clear();

	// Mode 1 vs. Mode 2
	const int VER = 0;

	if(VER <= 1)
	{
		// filling the vector
		for (int i = 0; i < 20; i++) {
			enemyGroups.push_back(std::vector<string>());
		}

		// Original
		{
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

			groupsLoaded = true;
		}
	}
	// Enemy ~ 20 lists
	else if (VER > 1)
	{
		// Group 0 - Easy
		{
			// vector to be pushed onto enemy groups
			std::vector<std::string> vec;

			vec.push_back("marauder");
			vec.push_back("marauder");
			vec.push_back("sentry");

			enemyGroups.push_back(vec);
		}

		// Group 1 - Easy
		{
			std::vector<std::string> vec;

			vec.push_back("mechaspider");
			vec.push_back("mechaspider");
			vec.push_back("mechaspider");
			vec.push_back("mechaspider");
			vec.push_back("mechaspider");

			enemyGroups.push_back(vec);
		}

		// Group 2 - Easy
		{
			std::vector<std::string> vec;

			vec.push_back("bastion");
			vec.push_back("mechaspider");
			vec.push_back("mechaspider");
			vec.push_back("mechaspider");

			enemyGroups.push_back(vec);
		}

		// Group 3 - Easy
		{
			std::vector<std::string> vec;

			vec.push_back("marauder");
			vec.push_back("marauder");
			vec.push_back("oracle");
			vec.push_back("oracle");

			enemyGroups.push_back(vec);
		}

		// Group 4 - Easy
		{
			std::vector<std::string> vec;

			vec.push_back("marauder");
			vec.push_back("marauder");
			vec.push_back("marauder");
			vec.push_back("marauder");
			vec.push_back("marauder");

			enemyGroups.push_back(vec);
		}

		// Group 5 - Easy
		{
			std::vector<std::string> vec;

			vec.push_back("bastion");
			vec.push_back("bastion");

			enemyGroups.push_back(vec);
		}

		// Group 6 - Medium
		{
			std::vector<std::string> vec;

			vec.push_back("sentry");
			vec.push_back("sentry");
			vec.push_back("oracle");

			enemyGroups.push_back(vec);
		}

		// Group 7 - Medium
		{
			std::vector<std::string> vec;

			vec.push_back("marauder");
			vec.push_back("sentry");
			vec.push_back("oracle");
			vec.push_back("bastion");

			enemyGroups.push_back(vec);
		}

		// Group 8 - Medium
		{
			std::vector<std::string> vec;

			vec.push_back("oracle");
			vec.push_back("oracle");
			vec.push_back("bastion");
			vec.push_back("mechaspider");

			enemyGroups.push_back(vec);
		}

		// Group 9 - Medium
		{
			std::vector<std::string> vec;

			vec.push_back("sentry");
			vec.push_back("bastion");
			vec.push_back("bastion");

			enemyGroups.push_back(vec);
		}

		// Group 10 - Medium
		{
			std::vector<std::string> vec;

			vec.push_back("oracle");
			vec.push_back("oracle");
			vec.push_back("oracle");
			vec.push_back("oracle");

			enemyGroups.push_back(vec);
		}

		// Group 11 - Medium
		{
			std::vector<std::string> vec;

			vec.push_back("marauder");
			vec.push_back("marauder");
			vec.push_back("marauder");
			vec.push_back("marauder");
			vec.push_back("marauder");
			vec.push_back("marauder");
			vec.push_back("marauder");
			vec.push_back("mechaspider");

			enemyGroups.push_back(vec);
		}

		// Group 12 - Medium
		{
			std::vector<std::string> vec;

			vec.push_back("sentry");
			vec.push_back("sentry");
			vec.push_back("bastion");
			vec.push_back("bastion");
			vec.push_back("bastion");

			enemyGroups.push_back(vec);
		}

		// Group 13 - Hard
		{
			std::vector<std::string> vec;

			vec.push_back("sentry");
			vec.push_back("sentry");
			vec.push_back("sentry");
			vec.push_back("sentry");
			vec.push_back("sentry");

			enemyGroups.push_back(vec);
		}

		// Group 14 - Hard
		{
			std::vector<std::string> vec;

			vec.push_back("sentry");
			vec.push_back("oracle");
			vec.push_back("oracle");
			vec.push_back("oracle");
			vec.push_back("bastion");

			enemyGroups.push_back(vec);
		}

		// Group 15 - Hard
		{
			std::vector<std::string> vec;

			vec.push_back("marauder");
			vec.push_back("marauder");
			vec.push_back("oracle");
			vec.push_back("oracle");
			vec.push_back("oracle");
			vec.push_back("mechaspider");
			vec.push_back("mechaspider");
			vec.push_back("mechaspider");

			enemyGroups.push_back(vec);
		}

		// Group 16 - Hard
		{
			std::vector<std::string> vec;

			vec.push_back("bastion");
			vec.push_back("bastion");
			vec.push_back("mechaspider");
			vec.push_back("mechaspider");
			vec.push_back("mechaspider");
			vec.push_back("mechaspider");
			vec.push_back("mechaspider");
			vec.push_back("mechaspider");
			vec.push_back("mechaspider");
			vec.push_back("mechaspider");
			vec.push_back("mechaspider");
			vec.push_back("mechaspider");

			enemyGroups.push_back(vec);
		}

		// Group 17 - Hard
		{
			std::vector<std::string> vec;

			vec.push_back("marauder");
			vec.push_back("marauder");
			vec.push_back("marauder");
			vec.push_back("marauder");
			vec.push_back("sentry");
			vec.push_back("sentry");
			vec.push_back("bastion"); 
			vec.push_back("bastion");
			vec.push_back("bastion");

			enemyGroups.push_back(vec);
		}

		// Group 18 - Insane
		{
			std::vector<std::string> vec;

			vec.push_back("sentry");
			vec.push_back("sentry");
			vec.push_back("sentry");
			vec.push_back("oracle");
			vec.push_back("oracle");
			vec.push_back("oracle");
			vec.push_back("bastion");
			vec.push_back("bastion");
			vec.push_back("bastion");

			enemyGroups.push_back(vec);
		}

		// Group 19 - Insane
		{
			std::vector<std::string> vec;

			vec.push_back("marauder");
			vec.push_back("marauder");
			vec.push_back("sentry");
			vec.push_back("sentry");
			vec.push_back("sentry");
			vec.push_back("oracle");
			vec.push_back("oracle");
			vec.push_back("bastion");
			vec.push_back("mechaspider");
			vec.push_back("mechaspider");
			vec.push_back("mechaspider");

			enemyGroups.push_back(vec);
		}
		
	}
}

// returns the player object.
cnz::Player* const cnz::CNZ_GameplayScene::GetPlayer() const
{
	return playerObj;
}

// gets the visible physics bodies.
bool cnz::CNZ_GameplayScene::GetVisiblePhysicsBodies() const { return showPBs; }

// sets if the physics bodies should be visible or not.
void cnz::CNZ_GameplayScene::SetVisiblePhysicsBodies(bool visible)
{
	using namespace cherry;

	// goes through each object
	for (Object* obj : objectList->objects)
	{
		// gets the physics bodies
		std::vector<PhysicsBody*> bodies = obj->GetPhysicsBodies();

		// goes through all the bodies
		for (PhysicsBody* body : bodies)
			body->SetVisible(visible);
	}
	
	showPBs = visible;
}

// updates the score string
void cnz::CNZ_GameplayScene::UpdateScore()
{
	// maximum score
	std::string maxScoreStr = std::string(SCORE_DIGIT_LIMIT, '9');
	int maxScore = util::convertString<int>(maxScoreStr);

	// fills the score with zeroes.
	std::string zeroFilled = util::zeroFill(score, SCORE_DIGIT_LIMIT);

	// clamping the score.
	score = glm::clamp(score, 0, maxScore);

	// setting the text.
	scoreText->SetText(zeroFilled);
	// scoreText->SetPositionByWindowSize(cherry::Vec2(0.95F, 0.05F), cherry::Game::GetRunningGame()->GetWindowSize());
}

// updates the time stop
void cnz::CNZ_GameplayScene::UpdateTimeStop()
{
	// gets the value, showing it as being 1 out of 100.
	float val = timeStopTimer / TIME_STOP_MAX * 100.0F;

	// gets the value as a string.
	std::string valStr = std::to_string(val);

	// truncate the values
	valStr.resize(TIME_STOP_DISPLAY_DIGITS_MAX);

	// fixes the time stop.
	timeStopText->SetText(valStr);

}

// TODO: maybe move the gameplay loop here.

// gameplay update
//void cnz::CNZ_GameplayScene::GameplayUpdate()
//{
//
//}
//
//// pause update
//void cnz::CNZ_GameplayScene::PauseUpdate()
//{
//	// if the game should be restated.
//	if (restart) {
//		//Reset Everything
//		kills = 0;
//		curGroup = -1;
//		curWave = 0;
//		score = 0;
//
//		// score needs to be updated.
//		updateScore = true;
//
//		//Enemies
//		for (int i = 0; i < enemyList.size(); i++) {
//			enemyList[i]->RemovePhysicsBody(enemyList[i]->GetPhysicsBodies()[0]);
//			cherry::Object* obj = enemyList[i];
//			util::removeFromVector(enemyList, enemyList[i]);
//			objectList->RemoveObjectByPointer(obj);
//			delete obj;
//			kills++;
//			// cout << kills << endl;
//			i--;
//		}
//		enemyList.clear();
//
//		//Player
//		// not needed?
//		// playerObj->AddPhysicsBody(new cherry::PhysicsBodyBox(playerObj->GetPosition(), playerObj->GetPBodySize()));
//		playerObj->SetPosition(playerSpawn);
//
//		paused = false;
//		restart = false;
//	}
//
//	pauseMenu->SetVisible(true);
//}

// update loop
void cnz::CNZ_GameplayScene::Update(float deltaTime)
{
	if(PROFILE)
		cherry::ProfilingSession::Start("profiling-cnz_gameplay_scene-update.json");

	// TODO: make it so that the dash can't be triggered if the camera hasn't caught up?
	// boolean for triggering the dash.
	// if the dash trigger is false, either the dash hasn't been triggrered, or was just released.
	bool dashTrigger = (mouseLeft || spaceBar); // left mouse button and space bar trigger the dash.

	// if 'true', the score text gets updated.
	bool updateScore = false;

	// tells the game to active the time stop effect if possible.
	bool stopTime = f;

	if (paused == false) {
		pauseMenu->SetVisible(false);
		CNZ_Game* game = (CNZ_Game*)cherry::Game::GetRunningGame();
		cherry::Camera::Sptr myCamera = game->myCamera;

		//GLenum test = glGetError();
		playerPrevPos = playerObj->GetPosition(); // store previous position

		// if (showPBs) { // show player and projectile physics bodies if showPBs
		// 	playerObj->GetPhysicsBodies()[0]->SetVisible(true);
		// 	for (int i = 0; i < projList.size(); i++) {
		// 		projList[i]->GetPhysicsBodies()[0]->SetVisible(true);
		// 	}
		// }

		float moveInc = -10.0F; // the movement incrementer.

		vector<cherry::PhysicsBody*> playerObstacleCollisions; // obstacle PBs with which the player's PB is colliding with
		vector<cherry::PhysicsBody*> playerEnemyCollisions; // enemy PBs with which the player's PB is colliding with

		// gets the player's physics body
		vector<cherry::PhysicsBody*> playerBodies = playerObj->GetPhysicsBodies();

		// cherry::ProfileTimer colTimer = cherry::ProfileTimer("profiling-player_collisions");

		// goes through each physics body for the player
		for (cherry::PhysicsBody* pBody : playerBodies)
		{
			// find all obstacles the player is colliding with.
			for (int i = 0; i < obstacles.size(); i++) {
				
				bool collision = false;
				
				// gets the enemy's phyiscs bodies
				vector<cherry::PhysicsBody*> oBodies = obstacles[i]->GetPhysicsBodies();

				// if (showPBs) { // shows obstacle physics bodies if showPBs // TODO: this shouldn't be checked every frame.
				// 	obstacles[i]->GetPhysicsBodies()[0]->SetVisible(true);
				// }
				
				for (cherry::PhysicsBody* oBody : oBodies)
				{
					collision = cherry::PhysicsBody::Collision(pBody, oBody);

					if (collision) {
						playerObstacleCollisions.push_back(oBody);
						// std::cout << "collision with obstacle " << i << std::endl;
					}
				}
			}

			// find all enemies the player is colliding with
			// if the player is invincible, no collision check with enemiesh appens
			if (!isInvincible) // is not invincible
			{
				for (int i = 0; i < enemyList.size(); i++)
				{
					// gets the enemy bodies
					vector<cherry::PhysicsBody*> eBodies = enemyList[i]->GetPhysicsBodies();

					for (cherry::PhysicsBody* eBody : eBodies)
					{
						// collision check
						bool collision = cherry::PhysicsBody::Collision(pBody, eBody);

						if (collision) {
							// player takes damage
							lives--;
							
							// life count has changed.
							switch (lives)
							{
							case 2:
								lifeIcon_3->SetVisible(false);
								break;
							case 1:
								lifeIcon_2->SetVisible(false);
								break;
							case 0:
								lifeIcon_1->SetVisible(false);
								break;
							}

							playerObj->SetPosition(playerSpawn);

							// resetting variables
							// this is to fix a glitch where the player's attakc would stop working on respawn.
							w = false;
							a = false;
							s = false;
							d = false;
							f = false;
							leftShift = false;

							mouseLeft = false;
							mouseRight = false;
							spaceBar = false;

							// stops the player from moving through solid objects.
							// taken out because the player kept getting stuck.
							// cw = false;
							// ca = false;
							// cs = false;
							// cd = false;

							// invincibility period activated
							isInvincible = true;
							invincibleCountdown = INVINCIBLE_TIME_MAX;
							playerObj->SetAlpha(0.5F);
						}
					}

					// there shouldn't be nullptr bodies anyway, since they should just be removed along with the enemy.
					// if (enemyList[i]->GetPhysicsBodies()[0] == nullptr || playerObj->GetPhysicsBodies()[0] == nullptr) {
					// 	// if (showPBs) { // shows enemy pbs if showPBs
					// 	// 	enemyList[i]->GetPhysicsBodies()[0]->SetVisible(true);
					// 	// }
					// 	bool collision = cherry::PhysicsBody::Collision(playerObj->GetPhysicsBodies()[0], enemyList[i]->GetPhysicsBodies()[0]);
					// 	if (collision) {
					// 		//Player takes damage
					// 		lives--;
					// 		playerObj->SetPosition(playerSpawn);
					// 	}
					// }
					// else {
					// 	// cout << "Enemy at " << i << " or player physics body could not be found!" << endl;
					// }
				}
			}
			else // is invincible
			{
				invincibleCountdown -= deltaTime;

				if (invincibleCountdown <= 0.0F) // invincibiltiy over
				{
					invincibleCountdown = 0.0F;
					isInvincible = false;
					playerObj->SetAlpha(1.0F);
				}
			}
		}

		// colTimer.Stop();

		cs = true;
		cw = true;
		ca = true;
		cd = true;


		// cherry::ProfileTimer colBehaviourTimer("profiling-cnz_player_col_behaviour");

		// TODO: actually fix collisions... and allow player to move out if colliding in all directions. (AKA when all of the above booleans are false)
		// check what directions the player can move in based on its collisions with obstacles in the scene.
		if (playerObstacleCollisions.size() != 0) { // allow movement only in directions oposite of the collision (CUBES ONLY)
			// std::cout << "There are " << playerObstacleCollisions.size() << " playerObj collisions this update!" << std::endl;
			
			// player world position
			cherry::Vec3 plyrPos = playerObj->GetPosition();

			for (int i = 0; i < playerObstacleCollisions.size(); i++) {
				// gets the world position of the obstacle
				cherry::Vec3 obstPos = playerObstacleCollisions[i]->GetWorldPosition();
				//cherry::Vec3 dP = obstPos - plyrPos;

				if ((obstPos.v.y - plyrPos.v.y) >= 0) { // above the object
					cs = false;
				}
				if ((obstPos.v.y - plyrPos.v.y) <= 0) { // below the object
					cw = false;
				}
				if ((obstPos.v.x - plyrPos.v.x) >= 0) { // right of the object
					ca = false;
				}
				if ((obstPos.v.x - plyrPos.v.x) <= 0) { // left of the object
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

		cherry::Vec3 curMoveDir;

		// moving the player.
		if (w && cw) // up
		{ 
			curMoveDir += cherry::Vec3(0, 1, 0);
			playerObj->SetPosition(playerObj->GetPosition() + cherry::Vec3(0.0F, moveInc * deltaTime, 0.0F));
		}
		if (s && cs) // down
		{ 
			curMoveDir += cherry::Vec3(0, -1, 0);
			playerObj->SetPosition(playerObj->GetPosition() + cherry::Vec3(0.0F, -moveInc * deltaTime, 0.0F));
		}
		if (a && ca) // left
		{ 
			curMoveDir += cherry::Vec3(-1, 0, 0);
			playerObj->SetPosition(playerObj->GetPosition() + cherry::Vec3(-moveInc * deltaTime, 0.0F, 0.0F));
		}
		if (d && cd) // right
		{ 
			curMoveDir += cherry::Vec3(1, 0, 0);
			playerObj->SetPosition(playerObj->GetPosition() + cherry::Vec3(moveInc * deltaTime, 0.0F, 0.0F));
		}

		// TODO: the footsteps won't play since the sound effect keeps starting over.
		if (!((w && cw) || (s && cs) || (a && ca) || (d && cd))) // if the player is not moving 
		{ 
			playerObj->SetState(0);
			// if (cherry::AudioEngine::GetInstance().isEventPlaying("footstep")) { // if footstep noise is playing, stop it since we are idle
			// 	cherry::AudioEngine::GetInstance().StopEvent("footstep");
			// }
		}
		else { // if the player is walking
			playerObj->SetState(1);
			// if (!cherry::AudioEngine::GetInstance().isEventPlaying("footstep")) { // if footstep noise is NOT being played
			// 	cherry::AudioEngine::GetInstance().SetEventPosition("footstep", playerObj->GetPositionGLM());
			// 	cherry::AudioEngine::GetInstance().PlayEvent("footstep");
			// }
		}

		playerObj->UpdateAngle(myCamera, game->GetCursorViewPositionX(), game->GetCursorViewPositionY(), game->GetWindowWidth(), game->GetWindowHeight());
		playerObj->SetRotation(cherry::Vec3(90.0f, 0.0f, playerObj->GetDegreeAngle() - 90), true);

		// dodge code
		if (leftShift) 
		{
			cherry::Vec3 temp;
			if (w && cw) 
			{
				temp.v.y = (temp.v.y - 1.0f);
			}
			if (s && cs) 
			{
				temp.v.y = (temp.v.y + 1.0f);
			}
			if (a && ca) 
			{
				temp.v.x = (temp.v.x + 1.0f);
			}
			if (d && cd) 
			{
				temp.v.x = (temp.v.x - 1.0f);
			}

			playerObj->SetPosition(playerObj->GetPosition() + temp * 2);

			leftShift = false;
		}

		// colBehaviourTimer.Stop();

		// since we don't check f outside of the AI loop on a per enemy basis and only if that enemy is stunned, I will check it here as well.
		// moved to key pressed section.
		// if (f && (!cherry::AudioEngine::GetInstance().isEventPlaying("timestop"))) {
		// 	cherry::AudioEngine::GetInstance().PlayEvent("timestop");
		// }

		// TIME EFFECT //
		if (timeStopActive && timeStopTimer > 0.0F) // effect is active, and has not run out.
		{
			timeStopTimer -= timeDecRate * deltaTime;
		
			if (timeStopTimer < 0.0F) // capping
				timeStopTimer = 0.0F;
		
			// effect has run out.
			if (timeStopTimer == 0.0F)
			{
				if (postProcess) // remove post processing layer
				{
					util::removeFromVector(layers, edgeDetect.GetPostLayer());
				}

				timeStopText->SetColor(1.0F, 0.075F, 0.101F, 1.0F); // text is made light red
				timeStopActive = false; // no longer in effect
			}

			// if the time stop text should be shown.
			if(showTimeStopText)
				UpdateTimeStop();
			
		}
		else if (!timeStopActive && timeStopTimer < TIME_STOP_MAX) // effect is not active, and has not charged yet.
		{
			timeStopTimer += timeIncRate * deltaTime;
		
			// stop stop now available
			if (timeStopTimer > TIME_STOP_MAX) // capping
				timeStopTimer = TIME_STOP_MAX;

			// time stop is now available.
			if (timeStopTimer == TIME_STOP_MAX)
				timeStopText->SetColor(1.0F, 1.0F, 1.0F, 1.0F); // text is made white.

			// if the time stop text should be shown.
			if (showTimeStopText)
			{
				UpdateTimeStop();
			}
		}
		else if(!timeStopActive && timeStopTimer == TIME_STOP_MAX && stopTime) // user can and has activated time slow.
		{
			if (postProcess) // add post processing layer
			{
				// plays the sound effect if the layer has just been added.
				if (util::addToVector(layers, edgeDetect.GetPostLayer()))
					cherry::AudioEngine::GetInstance().PlayEvent("timestop");
			}
		
			// activate time effect.
			timeStopActive = true;
		}
		// //

		// int enemyCount = 0;

		//Enemy AI
		// if the time stop is not active, then update the enemies in the list.
		if (timeStopActive == false)
		{
			// gets the player position, primary phyiscs body, and player boy's world position.
			const cherry::Vec3 plyrPos = playerObj->GetPosition();
			const cherry::PhysicsBody* plyrBody = playerObj->GetPrimaryPhysicsBody();
			const cherry::Vec3 plyrBodyWorldPos = (plyrBody != nullptr) ? plyrBody->GetWorldPosition() : plyrPos;

			for (int i = 0; i < enemyList.size(); i++) 
			{
				// gets the primary physics body of the enemy. This one is used for changing the angle.
				const cherry::PhysicsBody* enemyBody = enemyList[i]->GetPrimaryPhysicsBody();
				const cherry::Vec3 enemyBodyWorldPos = (enemyBody != nullptr) ? enemyBody->GetWorldPosition() : enemyList[i]->GetPosition();

				// the enemy is alive.
				// if (enemyList[i]->alive == true)
				// {
				// 	enemyCount++;
				// 	// if (f == true && enemyList[i]->stunned == false) {
				// 	// 	enemyList[i]->stunned = true;
				// 	// 	enemyList[i]->stunTimer = 0;
				// 	// }
				// 	// else if (enemyList[i]->stunned == true) {
				// 	// 	// TODO: maybe make this static so that all enemies stop getting stunned at the same time
				// 	// 	if (enemyList[i]->stunTimer >= 5.0f) {
				// 	// 		enemyList[i]->stunned = false;
				// 	// 
				// 	// 		if (postProcess) // removes edge detection layer
				// 	// 		{
				// 	// 			util::removeFromVector(layers, edgeDetect.GetPostLayer());
				// 	// 			// util::addToVector(layers, lightList->GetPostLayer());
				// 	// 		}
				// 	// 	}
				// 	// 	else {
				// 	// 		enemyList[i]->stunTimer += deltaTime;
				// 	// 	}
				// 	// }
				// }
				// 
				// TODO: have variable to keep track of distance needed for attack to happen.
				if (enemyList[i]->stunned == false) {
					//Look at player
					
					// for some reason it only works if done like this.
					// enemyList[i]->UpdateAngle(enemyList[i]->GetPhysicsBodies()[0]->GetWorldPosition(), playerObj->GetPhysicsBodies()[0]->GetWorldPosition());
					// enemyList[i]->UpdateAngle(enemyBody->GetWorldPosition(), plyrBodyWorldPos);
					// enemyList[i]->UpdateAngle(enemyList[i]->GetPrimaryPhysicsBody()->GetWorldPosition(), plyrBodyWorldPos);
					enemyList[i]->UpdateAngle(enemyBodyWorldPos, plyrBodyWorldPos);

					// enemyList[i]->UpdateAngle(
					// 	enemyList[i]->GetPrimaryPhysicsBody()->GetWorldPosition(), 
					// 	playerObj->GetPrimaryPhysicsBody()->GetWorldPosition()
					// );
					
					enemyList[i]->SetRotation(cherry::Vec3(90.0f, 0.0f, enemyList[i]->GetDegreeAngle()), true);


					// TODO: replace the function calls so that you have the position.
					switch (enemyList[i]->GetType())
					{
					case cnz::sentry:
						// if (GetDistanceXY(plyrPos, enemyList[i]->GetPosition()) < 10.0f && enemyList[i]->attacking == false) {
						if (GetDistanceXY(plyrPos, enemyList[i]->GetPosition()) < 10.0f && enemyList[i]->GetProjectileAvailable()) {
							// Spawn projectiles
							// enemyList[i]->attacking = true;

							// creates a new projectile.
							Projectile* proj = new Projectile(arrowBase, GetName());

							// adding to lists
							projList.push_back(proj);
							// projTimeList.push_back(0);

							// proj->SetWhichGroup(i);
							// proj->SetWhichEnemy(j);
							proj->active = true;
							proj->SetPosition(enemyList[i]->GetPosition());
							proj->SetRotationDegrees(enemyList[i]->GetRotationDegrees());
							proj->SetDirVec(GetUnitDirVecXY(projList[projList.size() - 1]->GetPosition(), plyrPos));
							objectList->AddObject(proj);

							// TODO: change arrow volume
							// cherry::AudioEngine::GetInstance().SetEventPosition("arrow", proj->GetPositionGLM());
							cherry::AudioEngine::GetInstance().PlayEvent("arrow");

							// projectile has been fired.
							enemyList[i]->ProjectileFired();

							// projList.push_back(new Projectile(*arrowBase));
							// projTimeList.push_back(0);
							// projList[projList.size() - 1]->AddPhysicsBody(new cherry::PhysicsBodyBox(enemyList[i]->GetPhysicsBodies()[0]->GetWorldPosition(), projList[projList.size() - 1]->GetPBodySize()));
							// projList[projList.size() - 1]->SetWhichGroup(i);
							// //projList[projList.size() - 1]->SetWhichEnemy(j);
							// projList[projList.size() - 1]->active = true;
							// projList[projList.size() - 1]->SetPosition(enemyList[i]->GetPosition());
							// projList[projList.size() - 1]->SetRotationDegrees(enemyList[i]->GetRotationDegrees());
							// projList[projList.size() - 1]->SetDirVec(GetUnitDirVec(projList[projList.size() - 1]->GetPosition(), playerObj->GetPosition()));
							// game->AddObjectToScene(projList[projList.size() - 1]);

							if (enemyList[i]->GetCurrentAnimation() != enemyList[i]->GetAttackAnimation()) {
								if (enemyList[i]->GetCurrentAnimation() != nullptr)
									enemyList[i]->GetCurrentAnimation()->Stop(); // stop current animation 

								enemyList[i]->SetCurrentAnimationByIndex(enemyList[i]->GetAttackAnimationIndex()); // change animation
								enemyList[i]->GetCurrentAnimation()->Play(); // play animation 

							}
						}
						else if (GetDistanceXY(plyrPos, enemyList[i]->GetPosition()) > 0.001f) { // changed due to glitch
							//Move towards player				
							enemyList[i]->SetPosition(enemyList[i]->GetPosition() + (GetUnitDirVecXY(enemyList[i]->GetPosition(), plyrPos) * enemyList[i]->GetSpeedMultiplier() * deltaTime));

							// walk animation is not active.
							if (enemyList[i]->GetCurrentAnimation() != enemyList[i]->GetWalkAnimation()) {
								if (enemyList[i]->GetCurrentAnimation() != nullptr)
									enemyList[i]->GetCurrentAnimation()->Stop(); // stop current animation

								enemyList[i]->SetCurrentAnimationByIndex(enemyList[i]->GetWalkAnimationIndex()); // change animation
								enemyList[i]->GetCurrentAnimation()->Play(); // play animation

							}
						}
						break;

					case cnz::marauder:
						// if (enemyList[i]->attacking == false)
						{
							if (GetDistanceXY(plyrPos, enemyList[i]->GetPosition()) < 2.0f) {
								//Attack
								if (enemyList[i]->GetCurrentAnimation() != enemyList[i]->GetAttackAnimation()) {
									if (enemyList[i]->GetCurrentAnimation() != nullptr)
										enemyList[i]->GetCurrentAnimation()->Stop(); // stop current animation 

									enemyList[i]->SetCurrentAnimationByIndex(enemyList[i]->GetAttackAnimationIndex()); // change animation
									enemyList[i]->GetCurrentAnimation()->Play(); // play animation 

								}
							}
							else {
								//Move towards player				
								enemyList[i]->SetPosition(enemyList[i]->GetPosition() + (GetUnitDirVecXY(enemyList[i]->GetPosition(), plyrPos) * enemyList[i]->GetSpeedMultiplier() * deltaTime));

								// walk animation is not active.
								if (enemyList[i]->GetCurrentAnimation() != enemyList[i]->GetWalkAnimation()) {
									if (enemyList[i]->GetCurrentAnimation() != nullptr)
										enemyList[i]->GetCurrentAnimation()->Stop(); // stop current animation

									enemyList[i]->SetCurrentAnimationByIndex(enemyList[i]->GetWalkAnimationIndex()); // change animation
									enemyList[i]->GetCurrentAnimation()->Play(); // play animation

								}
							}
						}
						break;

					case cnz::oracle:
						// if (enemyList[i]->attacking == false)
						{
							if (GetDistanceXY(plyrPos, enemyList[i]->GetPosition()) < 5.0f) {
								//Attack
								if (enemyList[i]->GetCurrentAnimation() != enemyList[i]->GetAttackAnimation()) {
									if (enemyList[i]->GetCurrentAnimation() != nullptr)
										enemyList[i]->GetCurrentAnimation()->Stop(); // stop current animation 

									enemyList[i]->SetCurrentAnimationByIndex(enemyList[i]->GetAttackAnimationIndex()); // change animation
									enemyList[i]->GetCurrentAnimation()->Play(); // play animation 

								}
							}
							else {
								//Move towards player				
								enemyList[i]->SetPosition(enemyList[i]->GetPosition() + (GetUnitDirVecXY(enemyList[i]->GetPosition(), plyrPos) * enemyList[i]->GetSpeedMultiplier() * deltaTime));

								// walk animation is not active.
								if (enemyList[i]->GetCurrentAnimation() != enemyList[i]->GetWalkAnimation()) {
									if (enemyList[i]->GetCurrentAnimation() != nullptr)
										enemyList[i]->GetCurrentAnimation()->Stop(); // stop current animation

									enemyList[i]->SetCurrentAnimationByIndex(enemyList[i]->GetWalkAnimationIndex()); // change animation
									enemyList[i]->GetCurrentAnimation()->Play(); // play animation

								}
							}
						}
						break;

					case cnz::bastion:
						// if (enemyList[i]->attacking == false)
						{
							if (GetDistanceXY(plyrPos, enemyList[i]->GetPosition()) < 2.0f) {
								//Attack
								if (enemyList[i]->GetCurrentAnimation() != enemyList[i]->GetAttackAnimation()) {
									if (enemyList[i]->GetCurrentAnimation() != nullptr)
										enemyList[i]->GetCurrentAnimation()->Stop(); // stop current animation 

									enemyList[i]->SetCurrentAnimationByIndex(enemyList[i]->GetAttackAnimationIndex()); // change animation
									enemyList[i]->GetCurrentAnimation()->Play(); // play animation 

								}
							}
							else {
								//Move towards player				
								enemyList[i]->SetPosition(enemyList[i]->GetPosition() + (GetUnitDirVecXY(enemyList[i]->GetPosition(), plyrPos) * enemyList[i]->GetSpeedMultiplier() * deltaTime));

								// walk animation is not active.
								if (enemyList[i]->GetCurrentAnimation() != enemyList[i]->GetWalkAnimation()) {
									if (enemyList[i]->GetCurrentAnimation() != nullptr)
										enemyList[i]->GetCurrentAnimation()->Stop(); // stop current animation

									enemyList[i]->SetCurrentAnimationByIndex(enemyList[i]->GetWalkAnimationIndex()); // change animation
									enemyList[i]->GetCurrentAnimation()->Play(); // play animation

								}
							}
						}
						break;

					case cnz::mechaspider:
						// starts the attack animation
						if (GetDistanceXY(plyrPos, enemyList[i]->GetPosition()) < 6.0f) {
							// if the attack animation hasn't started playing yet, play it.
							// if (enemyList[i]->attacking == false)
							// {
							// 	if (enemyList[i]->GetCurrentAnimation() != nullptr) {
							// 		enemyList[i]->GetCurrentAnimation()->Stop();
							// 	}
							// 
							// 	enemyList[i]->SetCurrentAnimationByIndex(enemyList[i]->GetAttackAnimationIndex());
							// 	enemyList[i]->GetCurrentAnimation()->Play();
							// 
							// 	enemyList[i]->attacking = true;
							// }

							// attack animation is not active.
							if (enemyList[i]->GetCurrentAnimation() != enemyList[i]->GetAttackAnimation()) {
								if (enemyList[i]->GetCurrentAnimation() != nullptr)
									enemyList[i]->GetCurrentAnimation()->Stop(); // stop current animation 
								
								enemyList[i]->SetCurrentAnimationByIndex(enemyList[i]->GetAttackAnimationIndex()); // change animation
								enemyList[i]->GetCurrentAnimation()->Play(); // play animation 

							}

						}
						else // walk animation
						{
							//Move towards player				
							enemyList[i]->SetPosition(enemyList[i]->GetPosition() + (GetUnitDirVecXY(enemyList[i]->GetPosition(), plyrPos) * enemyList[i]->GetSpeedMultiplier() * deltaTime));

							// // changes the animation if the enemy is in its attack animation, or if no animation is playing
							// if (
							// 	(enemyList[i]->attacking == true && enemyList[i]->GetCurrentAnimation() != enemyList[i]->GetWalkAnimation()) ||
							// 	(enemyList[i]->GetCurrentAnimation() == nullptr)
							// 	)
							// {
							// 	enemyList[i]->SetCurrentAnimationByIndex(enemyList[i]->GetWalkAnimationIndex());
							// 	enemyList[i]->GetCurrentAnimation()->Play();
							// 
							// 	// if (enemyList[i]->GetCurrentAnimation() == nullptr || enemyList[i]->GetCurrentAnimation() != enemyList[i]->GetAnimation(0)) {
							// 	// 
							// 	// 	enemyList[i]->SetCurrentAnimationByIndex(enemyList[i]->GetWalkAnimationIndex());
							// 	// 	enemyList[i]->GetCurrentAnimation()->Play();
							// 	// 	// enemyList[i]->SetCurrentAnimationByIndex(0); // walk anim
							// 	// 	// enemyList[i]->GetCurrentAnimation()->Play();
							// 	// }
							// 
							// 	// not attacking
							// 	enemyList[i]->attacking = false;
							// }

							// walk animation is not active.
							if (enemyList[i]->GetCurrentAnimation() != enemyList[i]->GetWalkAnimation()) {
								if(enemyList[i]->GetCurrentAnimation() != nullptr)
									enemyList[i]->GetCurrentAnimation()->Stop(); // stop current animation
								
								enemyList[i]->SetCurrentAnimationByIndex(enemyList[i]->GetWalkAnimationIndex()); // change animation
								enemyList[i]->GetCurrentAnimation()->Play(); // play animation

							}

						}


						//// TODO: the animation change is being checked every frame when it shouldn't be.
						//if (enemyList[i]->attacking == false)
						//{
						//	if (GetDistanceXY(plyrPos, enemyList[i]->GetPosition()) < 6.0f) {
						//		//Attack
						//		if (enemyList[i]->GetCurrentAnimation() != nullptr) {
						//			enemyList[i]->GetCurrentAnimation()->Stop();
						//		}

 					//			enemyList[i]->SetCurrentAnimationByIndex(enemyList[i]->GetAttackAnimationIndex());
						//		enemyList[i]->GetCurrentAnimation()->Play();
						//	}
						//	else {
						//		//Move towards player				
						//		enemyList[i]->SetPosition(enemyList[i]->GetPosition() + (GetUnitDirVecXY(enemyList[i]->GetPosition(), plyrPos) * enemyList[i]->GetSpeedMultiplier() * deltaTime));

						//		if (enemyList[i]->GetCurrentAnimation() == nullptr || enemyList[i]->GetCurrentAnimation() != enemyList[i]->GetAnimation(0)) {
						//			
						//			enemyList[i]->SetCurrentAnimationByIndex(enemyList[i]->GetWalkAnimationIndex());
						//			enemyList[i]->GetCurrentAnimation()->Play();
						//			// enemyList[i]->SetCurrentAnimationByIndex(0); // walk anim
						//			// enemyList[i]->GetCurrentAnimation()->Play();
						//		}
						//	}
						//}
						break;

					}
					// TODO: why is this being called here? It errors out.
					// enemyList[i]->Update(deltaTime);
				}
			}
		}

		// Spawn new wave when all enemies are dead
		// if (enemyCount == 0) {
		if (enemyList.empty()) {
			SpawnEnemyGroup();
			// if (!cherry::AudioEngine::GetInstance().isEventPlaying("new_wave")) {
			// 	cherry::AudioEngine::GetInstance().SetEventPosition("new_wave", glm::vec3(25, 25, 0));
			// 	cherry::AudioEngine::GetInstance().PlayEvent("new_wave");
			// }
		}


		// cherry::ProfileTimer projTimer("profiling-projectile-update");

		// temporary stack of projectiles to be deleted
		std::stack<Projectile*> projKillList;
		// std::stack<int> indexKillList;

		//Update Projectiles
		for (int i = 0; i < projList.size(); i++) 
		{
			if (projList[i]->active == true) {
				projList[i]->SetPosition(projList[i]->GetPosition() + (projList[i]->GetDirectionVec() * (100.0f * deltaTime)));

				// collision check
				std::vector<cherry::PhysicsBody*>projBodies = projList[i]->GetPhysicsBodies();
				std::vector<cherry::PhysicsBody*>playerBodies = playerObj->GetPhysicsBodies();

				// collision check
				bool deleteProj = false;

				// projectile bodies
				for (cherry::PhysicsBody* projBody : projBodies)
				{
					for (cherry::PhysicsBody* plyrBody : playerBodies)
					{
						// checks for collision
						deleteProj = cherry::PhysicsBody::Collision(projBody, plyrBody);
						if (deleteProj == true) // colision has occurred.
						{
							break;
						}
					}

					if (deleteProj) // collision has occurred
						break;
				}

				// projTimeList[i] += deltaTime; // in projectile update loop.

				// no collision, but alloted time has passed.
				// if (!deleteProj && projTimeList[i] >= 5.0F)
				// 	deleteProj = true;

				// projectile has surpassed lifetime.
				if (!deleteProj && projList[i]->ReachedMaximumLifeTime())
					deleteProj = true;

				if (deleteProj)
				{
					projKillList.push(projList[i]); // saves object to delete.
					// indexKillList.push(i); // saves index of value to delete.
				}

				// is this needed?
				// projList[i]->GetPhysicsBodies()[0]->SetLocalPosition(cherry::Vec3(0, 0, 0));
				
				// if (projTimeList[i] >= 5 || cherry::PhysicsBody::Collision(playerObj->GetPhysicsBodies()[0], projList[i]->GetPhysicsBodies()[0])) {
				// 	projList[i]->RemovePhysicsBody(projList[i]->GetPhysicsBodies()[0]);
				// 	enemyList[projList[i]->GetWhichGroup()]->attacking = false;
				// 	projList[i]->active = false;
				// 	projList[i]->SetPosition(cherry::Vec3(1000, 1000, 1000));
				// 	game->DeleteObjectFromScene(projList[i]);
				// 	projList.erase(projList.begin() + i);
				// 	projTimeList.erase(projTimeList.begin() + i);
				// }
			}
		}

		// projTimer.Stop(); 

		// removing and kills projectiles  
		while (!projKillList.empty())
		{
			Projectile* temp = projKillList.top();

			// removing from lists
			projKillList.pop();
			util::removeFromVector(projList, temp);
			objectList->DeleteObjectByPointer(temp);
		}

		// removing time values from the proj time list
		// while (!indexKillList.empty()) 
		// {
		// 	// removing from the time list
		// 	// projTimeList.erase(projTimeList.begin() + indexKillList.top());
		// 	indexKillList.pop(); // removing value
		// }

		//// DASH CODE

		// cherry::ProfileTimer dashProfiler("profiling-dash_timer");

		// NEW
		// TODO: reduce the amount of function calls by pulling these values once.

		// the dash charge was just triggered, so the player update will start charging things up.
		if (dashTrigger == true && playerObj->IsChargingDash() == false)
		{
			playerObj->SetChargingDash(true);
			playerObj->SetState(2); // charge animation

		}
		// the das button is still being held, but the dash has been fully charged, so the arrow should be visible.
		// the isChargingDash() function should not need to be checked.
		else if (dashTrigger == true && playerObj->IsDashFullyCharged() == true)
		{
			// repositioning indicator.
			indicatorObj->SetPosition(playerObj->GetPosition() + cherry::Vec3(0, 0, 0.001f));
			indicatorObj->SetVisible(true);
			indicatorObj->SetRotationZDegrees(playerObj->GetRotationZDegrees() + 180);

			// charged animation.
			playerObj->SetState(3); 
		}
		// the dash button has been let go, but the dash was not fully charged, so nothing happens.
		else if(dashTrigger == false && playerObj->IsChargingDash() == true && playerObj->IsDashFullyCharged() == false)
		{
			playerObj->SetChargingDash(false);
			playerObj->SetState(0);
		}
		// the dash button has been let go, and the dash has finished charging, so a successful dash is pulled off.
		else if (dashTrigger == false && playerObj->IsChargingDash() == true && playerObj->IsDashFullyCharged() == true)
		{
			// TODO: maybe try adding a blur effect.
			playerObj->SetChargingDash(false); // the dash is no longer being charged.
			camLerping = true; // the camera should lerp to the player.
			camLerpStartPos = myCamera->GetPosition(); // gets the starting position of the camera.

			indicatorObj->SetVisible(false); // hide indicator object.
			playerObj->SetState(4); // dash animation

			// the position sound shouldn't need to be changed since the player is always has the same screen position.
			// cherry::AudioEngine::GetInstance().SetEventPosition("dash", playerObj->GetPositionGLM());

			// play dash sound
			cherry::AudioEngine::GetInstance().PlayEvent("dash");

			// Attack Calculations
			// dash vector
			cherry::Vec3 dashVec = playerObj->GetDash(playerObj->GetDashDist());
			float tempDist = dashVec.GetLength();

			// gets closest obstacle
			cherry::PhysicsBody* closestObstacle = GetClosestObstacle();

			// if there is no obstacle intersecting the dash vector, dash the full length
			if (closestObstacle == nullptr) 
			{ 
				// kills enemies that are in the dash path.
				for (int i = 0; i < enemyList.size(); i++) 
				{
					if (GetEnemiesInDash(dashVec, enemyList[i], playerObj)) // kill enemies in the dash vector
					{ 
						enemyList[i]->alive = false;
						score += enemyList[i]->GetPoints(); // adds the points from the given enemy.
						updateScore = true;
					}
				}

				playerObj->SetPosition(playerObj->GetPosition() + dashVec); // move the player
			}
			else // not sure what this is...
			{
				cherry::Vec3 dP = closestObstacle->GetLocalPosition() - playerObj->GetPosition();
				cherry::Vec3 dPN;

				cherry::Vec3 plyrMeshBody = (playerObj->GetMeshBodyMaximum() - playerObj->GetMeshBodyMinimum());
				cherry::Vec3 obstMeshBody{};

				if (closestObstacle->GetObject() != nullptr) // checking for nullptr.
					obstMeshBody = (closestObstacle->GetObject()->GetMeshBodyMaximum() - closestObstacle->GetObject()->GetMeshBodyMinimum());

				if (dP.GetLength() < tempDist) {
					if (fabsf(dP.GetX()) > fabsf(dP.GetY())) 
					{
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
					else 
					{
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

					for (int i = 0; i < enemyList.size(); i++) 
					{
						if (GetEnemiesInDash(dashVec, enemyList[i], playerObj)) {
							enemyList[i]->alive = false;
						}
					}

					playerObj->SetPosition(playerObj->GetPosition() + dPN);
				}
				else {
					for (int i = 0; i < enemyList.size(); i++) 
					{
						if (GetEnemiesInDash(dashVec, enemyList[i], playerObj)) {
							enemyList[i]->alive = false;
						}
					}
					playerObj->SetPosition(playerObj->GetPosition() + dashVec);
				}
			}

			// the enemy deletion works, but it's coded a bit weirdly.
			// TODO: maybe see if you can optimize this?

			// stacks of indexes to be removed.
			std::stack<int> indexes;

			for (int i = 0; i < enemyList.size(); i++) {
				if (enemyList[i]->alive == false)
				{
					// deletes the object.
					bool destroyed = objectList->DeleteObjectByPointer(enemyList[i]);

					// kill count
					kills++;

					// if the object was sucessfully destroyed.
					if (destroyed)
						indexes.push(i); // the index to be removed.
					else
						std::cout << "Enemy Deletion Error." << std::endl;
				}
			}

			// dashProfiler.Stop();

			{
				// tracks amount of deaths to use for triggering death sounds
				int deaths = 0;

				// while the stack is not empty.
				while (!indexes.empty())
				{
					int index = indexes.top(); // gets the top value

					enemyList.erase(enemyList.begin() + index);
					indexes.pop(); // pops off value.

					// there are four different deaths sounds. Playing the same death sound will cause the effect to start over.
					// this is why the sound effect will only SOMETIMES play.
					// sounds: enemy_death_01, enemy_death_02, enemy_death_03, enemy_death_04
					//	* enemy_death is the same as enemy_death_01

					// moved from 'ClosestObstacle' section above.
					// TODO: the death sound keeps starting over, hence why it's not playing. Fix that.
					// cherry::AudioEngine::GetInstance().SetEventPosition("enemy_death", enemyList[i]->GetPositionGLM());
					// cherry::AudioEngine::GetInstance().PlayEvent("enemy_death"); // play death noise


					// TODO: implement other enemy death sounds.
					deaths++;

					switch (deaths)
					{
					case 0: // death default (should never be played
						cherry::AudioEngine::GetInstance().PlayEvent("enemy_death");
						break;
					case 1: // death sound 1 (same as enemy_death)
						cherry::AudioEngine::GetInstance().PlayEvent("enemy_death_01");
						break;
					case 2: // death sound 2
						cherry::AudioEngine::GetInstance().PlayEvent("enemy_death_02");
						break;
					case 3: // death sound 3
						cherry::AudioEngine::GetInstance().PlayEvent("enemy_death_03");
						break;
					case 4: // death sound 4
						cherry::AudioEngine::GetInstance().PlayEvent("enemy_death_04");
						break;
					}
					
				}
			}

			// for (int i = 0; i < enemyList.size(); i++) {
			// 	if (enemyList[i]->alive == false) {
			// 		// TODO: the destructor already removes the phyiscs body. So this is probably unneeded.
			// 		enemyList[i]->RemovePhysicsBody(enemyList[i]->GetPhysicsBodies()[0]);
			// 		cherry::Object* obj = enemyList[i];
			// 		util::removeFromVector(enemyList, enemyList[i]);
			// 		objectList->RemoveObjectByPointer(obj);
			// 		delete obj;
			// 		kills++;
			// 		cout << kills << endl;
			// 		i--;
			// 	}
			// }
		}
		

		/////////////////////////////////////////////////////
		/// OLD

		// this has been changed from before.
		// Dash indicator - ready to dash but hasn't released chargey button yet
		// if (playerObj->GetCurrentDashTime() >= 1.0f) 
		// { 
		// 	//Display indicator
		// 	//indArrowAnim->Play();
		// 	indicatorObj->SetPosition(playerObj->GetPosition() + cherry::Vec3(0, 0, 0.001f));
		// 	indicatorObj->SetVisible(true);
		// 	indicatorObj->SetRotationZDegrees(playerObj->GetRotationZDegrees() + 180);
		// }
		// else // dash timer is below 1.0f 
		// { 
		// 	//Hide indicator
		// 	indicatorObj->SetVisible(false);
		// }
		// 
		// // if (playerObj->GetDashTime() >= 1.0f && mouseRight == true)
		// if (playerObj->GetCurrentDashTime() >= 1.0f && dashTrigger) // if dash timer is above 1.0 and left mouse has been released, do the dash
		// {
		// 	if (!cherry::AudioEngine::GetInstance().isEventPlaying("dash")) { // if dash sound is NOT playing, play it
		// 		cherry::AudioEngine::GetInstance().SetEventPosition("dash", playerObj->GetPositionGLM());
		// 		cherry::AudioEngine::GetInstance().PlayEvent("dash");
		// 	}
		// 
		// 	cherry::Vec3 dashVec = playerObj->GetDash(playerObj->GetDashDist());
		// 	float tempDist = dashVec.GetLength();
		// 	playerObj->SetChargingDash(true);
		// 	playerObj->SetState(4); // set state to dashing
		// 	playerObj->SetCurrentDashTime(0.0f);
		// 
		// 	cherry::PhysicsBody* closestObstacle = GetClosestObstacle();
		// 
		// 	if (closestObstacle == nullptr) { // if there is no obstacle intersecting the dash vector, dash the full length
		// 		for (int i = 0; i < enemyList.size(); i++) {
		// 			if (GetEnemiesInDash(dashVec, enemyList[i], playerObj)) { // kill enemies in the dash vector
		// 				enemyList[i]->alive = false;
		// 				score += enemyList[i]->GetPoints();
		// 				updateScore = true;
		// 				cherry::AudioEngine::GetInstance().SetEventPosition("enemy_death", enemyList[i]->GetPositionGLM());
		// 				cherry::AudioEngine::GetInstance().PlayEvent("enemy_death"); // play death noise
		// 			}
		// 		}
		// 		playerObj->SetPosition(playerObj->GetPosition() + dashVec); // move the player
		// 	}
		// 	else 
		// 	{
		// 		cherry::Vec3 dP = closestObstacle->GetLocalPosition() - playerObj->GetPosition();
		// 		cherry::Vec3 dPN;
		// 
		// 		cherry::Vec3 plyrMeshBody = (playerObj->GetMeshBodyMaximum() - playerObj->GetMeshBodyMinimum());
		// 		cherry::Vec3 obstMeshBody{};
		// 
		// 		if (closestObstacle->GetObject() != nullptr) // checking for nullptr.
		// 			obstMeshBody = (closestObstacle->GetObject()->GetMeshBodyMaximum() - closestObstacle->GetObject()->GetMeshBodyMinimum());
		// 
		// 		if (dP.GetLength() < tempDist) {
		// 			if (fabsf(dP.GetX()) > fabsf(dP.GetY())) {
		// 				float tempX = 0;
		// 				if (dP.GetX() < 0) {
		// 					tempX = dP.GetX() + ((plyrMeshBody.GetX() / 4) + (obstMeshBody.GetX() / 4));
		// 				}
		// 				else {
		// 					tempX = dP.GetX() - ((plyrMeshBody.GetX() / 4) + (obstMeshBody.GetX() / 4));
		// 				}
		// 				float angle = GetXYAngle(dP);
		// 				float tempY = tempX / tanf(angle);
		// 				dPN.SetX(tempX);
		// 				dPN.SetY(tempY);
		// 			}
		// 			else {
		// 				float tempY = 0;
		// 				if (dP.GetY() < 0) {
		// 					tempY = dP.GetY() + ((plyrMeshBody.GetY() / 4) + (obstMeshBody.GetY() / 4));
		// 				}
		// 				else {
		// 					tempY = dP.GetY() - ((plyrMeshBody.GetY() / 4) + (obstMeshBody.GetY() / 4));
		// 				}
		// 				float angle = GetXYAngle(dP);
		// 				float tempX = tempY * tanf(angle);
		// 				dPN.SetX(tempX);
		// 				dPN.SetY(tempY);
		// 			}
		// 
		// 			for (int i = 0; i < enemyList.size(); i++) {
		// 				if (GetEnemiesInDash(dashVec, enemyList[i], playerObj)) {
		// 					enemyList[i]->alive = false;
		// 				}
		// 			}
		// 			playerObj->SetPosition(playerObj->GetPosition() + dPN);
		// 		}
		// 		else {
		// 			for (int i = 0; i < enemyList.size(); i++) {
		// 				if (GetEnemiesInDash(dashVec, enemyList[i], playerObj)) {
		// 					enemyList[i]->alive = false;
		// 				}
		// 			}
		// 			playerObj->SetPosition(playerObj->GetPosition() + dashVec);
		// 		}
		// 	}
		// 
		// 	// the enemy deletion works, but it's coded a bit weirdly.
		// 	// TODO: maybe see if you can optimize this?
		// 
		// 	// stacks of indexes to be removed.
		// 	std::stack<int> indexes;
		// 	
		// 	for (int i = 0; i < enemyList.size(); i++) {
		// 		if (enemyList[i]->alive == false) 
		// 		{
		// 			// deletes the object.
		// 			bool destroyed = objectList->DeleteObjectByPointer(enemyList[i]);
		// 
		// 			// kill count
		// 			kills++;
		// 
		// 			// if the object was sucessfully destroyed.
		// 			if (destroyed)
		// 				indexes.push(i); // the index to be removed.
		// 			else
		// 				std::cout << "Enemy Deletion Error." << std::endl;
		// 		}
		// 	}
		// 
		// 	// dashProfiler.Stop();
		// 
		// 	// while the stack is not empty.
		// 	while (!indexes.empty())
		// 	{
		// 		int index = indexes.top(); // gets the top value
		// 
		// 		enemyList.erase(enemyList.begin() + index);
		// 		indexes.pop(); // pops off value.
		// 
		// 		// there are four different deaths sounds. Playing the same death sound will cause the effect to start over.
		// 		// this is why the sound effect will only SOMETIMES play.
		// 		// sounds: enemy_death_01, enemy_death_02, enemy_death_03, enemy_death_04
		// 		//	* enemy_death is the same as enemy_death_01
		// 
		// 		// TODO: implement other enemy death sounds.
		// 		cherry::AudioEngine::GetInstance().PlayEvent("enemy_death");
		// 	}
		// 
		// 	// for (int i = 0; i < enemyList.size(); i++) {
		// 	// 	if (enemyList[i]->alive == false) {
		// 	// 		// TODO: the destructor already removes the phyiscs body. So this is probably unneeded.
		// 	// 		enemyList[i]->RemovePhysicsBody(enemyList[i]->GetPhysicsBodies()[0]);
		// 	// 		cherry::Object* obj = enemyList[i];
		// 	// 		util::removeFromVector(enemyList, enemyList[i]);
		// 	// 		objectList->RemoveObjectByPointer(obj);
		// 	// 		delete obj;
		// 	// 		kills++;
		// 	// 		cout << kills << endl;
		// 	// 		i--;
		// 	// 	}
		// 	// }
		// 
		// }
		// else if (dashTrigger == true) // before dash, while left mouse is being held
		// {
		// 	// else if (mouseLeft == true && mouseRight == false)
		// 	if (playerObj->GetCurrentDashTime() >= 1.0f) {
		// 		playerObj->SetState(3); // charged 
		// 	}
		// 	else {
		// 		playerObj->SetState(2); // set state to dash charging 
		// 	}
		// 	playerObj->SetCurrentDashTime(playerObj->GetCurrentDashTime() + 1.25f * deltaTime);
		// 	//std::cout << playerObj->GetDashTime() << std::endl;
		// }
		// // else if (mouseLeft == true && mouseRight == true) { // left mouse has been released, reset dash timer
		// else if (dashTrigger == false) { 
		// 	playerObj->SetCurrentDashTime(0.0f);
		// 	//Logger::GetLogger()->info(this->dashTime);
		// 	mouseLeft = false;
		// 	mouseRight = false;
		// 	spaceBar = false;
		// }

		// Path update
		// testObj->Update(deltaTime);


		// cherry::ProfileTimer animationTimer("profiling-animation-change");

		//// ANIMATION UPDATES
		// Player
		{
			// TODO: move this to state section
			// these items cannot be declared in a case statement.
			float angle = 0;
			float playerAngle = playerObj->GetDegreeAngle();
			float angleDiff = 0;

			// changes the playe based on the state.
			// TODO: this should really be in the player class, but it's being left in the GameplayScene for now.
			switch (playerObj->GetState())
			{
			case 1: // walking
				if (curMoveDir.GetY() == 1 && curMoveDir.GetX() == 1) { // moving up right 
					angle = 315.0f;
				}
				else if (curMoveDir.GetY() == 1 && curMoveDir.GetX() == -1) { // moving up left 
					angle = 45.0f;
				}
				else if (curMoveDir.GetY() == -1 && curMoveDir.GetX() == 1) { // moving down right 
					angle = 225.0f;
				}
				else if (curMoveDir.GetY() == -1 && curMoveDir.GetX() == -1) { // moving down left 
					angle = 135.0f;
				}
				else if (curMoveDir.GetY() == 1) { // moving up 
					angle = 0.0f;
				}
				else if (curMoveDir.GetY() == -1) { // moving down 
					angle = 180.0f;
				}
				else if (curMoveDir.GetX() == 1) { // moving right 
					angle = 270.0f;
				}
				else if (curMoveDir.GetX() == -1) { // moving left 
					angle = 90.0f;
				}

				// playerAngle = playerObj->GetDegreeAngle(); // moved to before switch statement.

				// always get a positive difference 
				if (playerAngle > angle) 
				{
					angleDiff = playerAngle - angle;
				}
				else if (angle > playerAngle) 
				{
					angleDiff = angle - playerAngle;
				}
				else {
					angleDiff = 0.0f;
				}

				angleDiff = angleDiff - 90.0f; // subrtact 90 to fix werid rotation...

				// make sure we are between 0 and 360. this should never be an issue but why NOT add redundant computations? /s 
				while (angleDiff < 0.0f) 
				{
					angleDiff = angleDiff + 360.0f;
				}
				while (angleDiff > 360.0f) 
				{
					angleDiff = angleDiff - 360.0f;
				}

				if ((angle <= 45.0f && angle >= 0.0f) || (angle <= 360.0f && angle >= 315.0f)) // forward walking animation 
				{
					// play the animation
					if (playerObj->GetCurrentAnimation() != playerObj->GetRunFAnimation())
					{
						// if an animation is currently playing
						if (playerObj->GetCurrentAnimation() != nullptr)
							playerObj->GetCurrentAnimation()->Stop();

						playerObj->SetCurrentAnimationByIndex(playerObj->GetRunFAnimationIndex());
						playerObj->GetCurrentAnimation()->Play();
					}
				}
				else if (angle > 45.0f && angle <= 135.0f) // right walking animation 
				{
					// play the animation
					if (playerObj->GetCurrentAnimation() != playerObj->GetRunRAnimation())
					{
						// if an animation is currently playing
						if (playerObj->GetCurrentAnimation() != nullptr)
							playerObj->GetCurrentAnimation()->Stop();

						playerObj->SetCurrentAnimationByIndex(playerObj->GetRunRAnimationIndex());
						playerObj->GetCurrentAnimation()->Play();
					}
				}
				else if (angle > 135.0f && angle <= 225.0f) { // backwards walking animation 
					// play the animation
					if (playerObj->GetCurrentAnimation() != playerObj->GetRunBAnimation())
					{
						// if an animation is currently playing
						if (playerObj->GetCurrentAnimation() != nullptr)
							playerObj->GetCurrentAnimation()->Stop();

						playerObj->SetCurrentAnimationByIndex(playerObj->GetRunBAnimationIndex());
						playerObj->GetCurrentAnimation()->Play();
					}
				}
				else if (angle > 225.0f && angle < 315.0f) { // left walking animation 
					// play the animation
					if (playerObj->GetCurrentAnimation() != playerObj->GetRunLAnimation())
					{
						// if an animation is currently playing
						if (playerObj->GetCurrentAnimation() != nullptr)
							playerObj->GetCurrentAnimation()->Stop();

						playerObj->SetCurrentAnimationByIndex(playerObj->GetRunLAnimationIndex());
						playerObj->GetCurrentAnimation()->Play();
					}
				}

				break;

			case 2:  // charging dash attack
				// play the animation
				if (playerObj->GetCurrentAnimation() != playerObj->GetChargingAnimation())
				{
					// if an animation is currently playing
					if (playerObj->GetCurrentAnimation() != nullptr)
						playerObj->GetCurrentAnimation()->Stop();

					playerObj->SetCurrentAnimationByIndex(playerObj->GetChargingAnimationIndex());
					playerObj->GetCurrentAnimation()->Play();
				}
				break;

			case 3: // dash charged 
				// play the animation
				if (playerObj->GetCurrentAnimation() != playerObj->GetChargedAnimation())
				{
					// if an animation is currently playing
					if (playerObj->GetCurrentAnimation() != nullptr)
						playerObj->GetCurrentAnimation()->Stop();

					playerObj->SetCurrentAnimationByIndex(playerObj->GetChargedAnimationIndex());
					playerObj->GetCurrentAnimation()->Play();
				}
				break;

			case 4: // dashing 
				// play the animation
				if (playerObj->GetCurrentAnimation() != playerObj->GetDashingAnimation())
				{
					// if an animation is currently playing
					if (playerObj->GetCurrentAnimation() != nullptr)
						playerObj->GetCurrentAnimation()->Stop();

					playerObj->SetCurrentAnimationByIndex(playerObj->GetDashingAnimationIndex());
					playerObj->GetCurrentAnimation()->Play();
				}
				break;
			}

		}
		// animationTimer.Stop();

		// cherry::ProfileTimer cameraTimer("profiling-camera_move");

		// camera position update code
		if (myCamera->GetPosition().x != playerObj->GetPosition().GetX() || myCamera->GetPosition().y != playerObj->GetPosition().GetY() + CAMERA_POS_OFFSET.y) {
			// the player is not charging their dash
			// if (!playerObj->IsChargingDash()) // TODO: have a tracker to indicate that the dash has happened.
			if (!camLerping)
			{
				goto notDashing;
			}

			/// <summary>
			/// So the way this worked is that it would lerp from the camera's current position to the player's position/
			/// This kind of created the easing effect that the camera had before.
			///  Now it goes based on the camera's position when the lerping started.
			/// </summary>


			// this should be based on delta time.
			// camLerpPercent += 0.01f; 
			camLerpPercent += CAM_LERP_INC * deltaTime;  // originally += 0.01f without delta time. Now set to 0.05F

			// I assume this is where the CameraLerp function would've been called.
			// This has also been moved to be after the camera percentage change.
			if (camLerpPercent >= 1.0f)
			{
				camLerpPercent = 1.0f; // original set back to 0. Now it just sets back to 1.0F
			}

			// variables used for saving values.
			glm::vec3 temp;
			glm::vec2 xyCam;
			glm::vec2 xyPla;
			cherry::Vec2 xyCur;

			// xyCam.x = myCamera->GetPosition().x;
			// xyCam.y = myCamera->GetPosition().y;
			xyCam.x = camLerpStartPos.x;
			xyCam.y = camLerpStartPos.y;

			xyPla.x = playerObj->GetPosition().GetX();
			xyPla.y = playerObj->GetPosition().GetY() + CAMERA_POS_OFFSET.y;

			// lerp (it moves fast enough that easing doesn't seem to be needed)
			xyCur = cherry::Vec2::Lerp(xyCam, xyPla, camLerpPercent);

			temp.x = xyCur.GetX();
			temp.y = xyCur.GetY();
			temp.z = CAMERA_POS_OFFSET.z;

			myCamera->SetPosition(temp);

			// the camera has reached the end of the line.
			if (camLerpPercent == 1.0f)
			{
				// the camera is no longer lerping, and thus the lerp percentage is set back to 0.
				camLerping = false;
				camLerpPercent = 0.0F;
			}
		}
		else 
		{
			// these writes may be unneeded.
			camLerpPercent = 0.0f;
			// playerObj->SetChargingDash(false);
			camLerping = false;

		notDashing:
			myCamera->SetPosition(cherry::Vec3(playerObj->GetPosition().GetX(), playerObj->GetPosition().GetY() + CAMERA_POS_OFFSET.y, CAMERA_POS_OFFSET.z));
		}

		// cameraTimer.Stop();

		// player light offset.
		bool lightPosChange = false;

		if (LIGHT_CONTROL && playerLightDir != glm::ivec3(0, 0, 0))
		{
			// the x-axis and y-axis positions are inverted due to the camera position.
			playerLightOffset += glm::vec3(
				LIGHT_OFFSET_INC.x * -playerLightDir.x * deltaTime,
				LIGHT_OFFSET_INC.y * -playerLightDir.y * deltaTime,
				LIGHT_OFFSET_INC.z * playerLightDir.z * deltaTime
			);

			lightPosChange = true;
		}

		// moving the player light.
		if (playerLight->GetLightPosition() != playerObj->GetPosition() || lightPosChange == true)
		{
			playerLight->SetLightPositionX(playerObj->GetPositionX() + playerLightOffset.x);
			playerLight->SetLightPositionY(playerObj->GetPositionY() + playerLightOffset.y);

			// EX: (this should be removed)
			playerLight->SetLightPositionZ(playerLightOffset.z);

			lightList->UpdatePostLayer(); // updates the post layer.
		}
	}
	else 
	{
		if (exitMap) // if the map should be exited.
		{
			// Codeanme Zero Game
			cnz::CNZ_Game* const game = (CNZ_Game*)cherry::Game::GetRunningGame();

			game->SetCurrentScene(game->titleSceneName, true); // return to main menu
			return;
		}
		else if (restart) // Pause Menu Code (this needs to be fixed)
		{
			// Resets Everything
			// life icons are visible now.
			lives = 3;
			lifeIcon_1->SetVisible(true);
			lifeIcon_2->SetVisible(true);
			lifeIcon_3->SetVisible(true);

			kills = 0;
			curGroup = -1;
			wave = 0;
			curWave = 0;
			score = 0;

			// reset player light to base position
			playerLightOffset = PLAYER_LIGHT_OFFSET_BASE;

			// invincibility
			isInvincible = false;
			invincibleCountdown = 0.0F;
			playerObj->SetAlpha(1.0F); // player has max opacity.

			// time stop
			timeStopTimer = TIME_STOP_MAX;
			timeStopActive = false;

			// remove layer from list if it's in there
			if(postProcess)
				util::removeFromVector(layers, edgeDetect.GetPostLayer());

			// score needs to be updated.
			updateScore = true;


			// while there are still enemies to kill.
			while (enemyList.empty() == false)
			{
				// gets the enemy.
				cnz::Enemy* enemy = enemyList[0];

				// removes from the enemy list.
				util::removeFromVector(enemyList, enemy); 
				
				// deletes object from scene.
				bool deleted = objectList->DeleteObjectByPointer(enemy);

				// enemy was not deleted properly, so it gets deleted from its proper list.
				if (deleted == false)
				{
					// second attempt to delete the enemy 
					bool del2 = cherry::ObjectManager::DeleteObjectFromSceneObjectList(enemy);
					
					if (del2 == false)
						throw std::runtime_error("Object Deletion Failure. Don't put unused enemy objects in this list.");
				}

				// this is a reset, so the kill count should not be increased?
				// kills++;
			}

			// deleting projectiles
			while (!projList.empty())
			{
				// gets the enemy.
				cnz::Projectile* proj = projList[0];

				// removes from the enemy list.
				util::removeFromVector(projList, proj);

				// deletes object from scene.
				bool deleted = objectList->DeleteObjectByPointer(proj);

				// enemy was not deleted properly, so it gets deleted from its proper list.
				if (deleted == false)
				{
					// second attempt to delete the enemy 
					bool del2 = cherry::ObjectManager::DeleteObjectFromSceneObjectList(proj);

					if (del2 == false)
						throw std::runtime_error("Object Deletion Failure. Don't put unused enemy objects in this list.");
				}
			}

			// Player - Reset to Spawn Position
			playerObj->SetPosition(playerSpawn);

			paused = false;
			restart = false;
		}

		pauseMenu->SetVisible(true);
	}

	// GAME OVER - switching the scene.
	if (lives <= 0)
	{
		// game
		cnz::CNZ_Game* const game = (CNZ_Game*)cherry::Game::GetRunningGame();

		// gets the game over scene
		cnz::CNZ_GameOverScene* gos = nullptr;
		gos = (CNZ_GameOverScene*)cherry::SceneManager::Get(game->gameOverSceneName);
		
		// leaving the game.
		if (gos == nullptr) // goes to main title if the game over scene is unavailable.
		{
			game->SetCurrentScene(game->titleSceneName, true); // main menu
		}
		else // goes to game over screen.
		{
			gos->SetMapNumber(map.GetMapNumber());
			gos->SetScore(score); // gives the scene the score.
			game->SetCurrentScene(gos->GetName(), false); // game over
		}

		// this shouldn't ever be reached.
		return;
	}

	// updates the score if it has been changed.
	if (updateScore)
		UpdateScore();

	// calls the main game Update function to go through every object.
	cherry::GameplayScene::Update(deltaTime);

	if (PROFILE)
		cherry::ProfilingSession::End();
}

// returns the distance between two vectors on the xy axis only
float GetDistanceXY(cherry::Vec2 one, cherry::Vec2 two)
{
	float x = two.GetX() - one.GetX();
	float y = two.GetY() - one.GetY();

	return sqrt(pow(x, 2) + pow(y, 2));
}

// returns the distance between two vectors on the xy axis only. Z-axis information is discarded.
float GetDistanceXY(cherry::Vec3 one, cherry::Vec3 two) 
{
	return GetDistanceXY(cherry::Vec2(one.v.x, one.v.y), cherry::Vec2(two.v.x, two.v.y));
}

// gets the unit direction vector on the xy axis.
cherry::Vec3 GetUnitDirVecXY(cherry::Vec2 one, cherry::Vec2 two)
{
	cherry::Vec2 newVec = two - one;

	float temp = (newVec.v.x * newVec.v.x + newVec.v.y * newVec.v.y);

	return cherry::Vec3(newVec.v.x, newVec.v.y, 0.0F) / temp;
}


// gets the unit direction vector between two vectors on the xy axis only.
cherry::Vec3 GetUnitDirVecXY(cherry::Vec3 one, cherry::Vec3 two) 
{
	cherry::Vec3 newVec = two - one;
	newVec.SetZ(0.0f);

	float temp = (newVec.GetX() * newVec.GetX() + newVec.GetY() * newVec.GetY());

	return newVec / temp;
}

// lerp between two points on the xy axis.
cherry::Vec3 CameraLerpXY(cherry::Vec2 start, cherry::Vec2 end, float percent)
{
	// uses glm lerp
	glm::vec2 v = glm::mix(
		glm::vec2(start.v.x, start.v.y), 
		glm::vec2(end.v.x, end.v.y),
		percent
	);

	return cherry::Vec3(v.x, v.y, 0.0F);
}

//Lerp between two vectors in xy axis
// todo: remove this function? Nothign calls it.
cherry::Vec3 CameraLerpXY(cherry::Vec3 start, cherry::Vec3 end, float percent) 
{
	// this isn't called by the camera at all.
	// also, this isn't part of any class.


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

