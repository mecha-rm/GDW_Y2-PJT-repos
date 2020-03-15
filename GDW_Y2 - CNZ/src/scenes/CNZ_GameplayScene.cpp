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
	glm::ivec2 myWindowSize = game->GetWindowSize(); // the current window size.

	pauseMenu = new Image("res/images/codename_zero_logo.png", game->GetCurrentSceneName());
	pauseMenu->SetWindowChild(true);
	pauseMenu->SetScale(cherry::Vec3(0.25f, 0.25f, 1.0f));
	pauseMenu->SetPositionByWindowSize(cherry::Vec2(0.5f, 0.25f));

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

	// if (!levelLoading)
	{
		lightList->AddLight(new Light(GetName(), Vec3(-7.0F, 0.0F, 0.0F), Vec3(1.0F, 0.1F, 0.1F),
			Vec3(0.1F, 1.0F, 0.4F), 0.4F, 0.2F, 250.0F, 0.15F));

		lightList->AddLight(new Light(GetName(), Vec3(7.0F, 0.0F, 0.0F), Vec3(0.1, 0.1F, 1.0F),
			Vec3(0.2F, 0.7F, 0.9F), 0.3F, 0.5F, 256.0F, 0.15F));

		lightList->AddLight(new Light(GetName(), Vec3(0.0F, 7.0F, 0.0F), Vec3(0.3, 0.9F, 0.1F),
			Vec3(0.8F, 0.2F, 0.95F), 0.9F, 0.7F, 100.0F, 0.85F));
	}

	matStatic = lightList->GenerateMaterial(STATIC_VS, STATIC_FS, sampler);
	matDynamic = lightList->GenerateMaterial(DYNAMIC_VS, DYNAMIC_FS, sampler);

	
	

	//this->playerCharging->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Charge/One_Charge_000000.obj", 0.041538f));
	//this->playerCharging->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Charge/One_Charge_000001.obj", 0.041538f));
	//this->playerCharging->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Charge/One_Charge_000002.obj", 0.041538f));
	//this->playerCharging->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Charge/One_Charge_000003.obj", 0.041538f));
	//this->playerCharging->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Charge/One_Charge_000004.obj", 0.041538f));
	//this->playerCharging->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Charge/One_Charge_000005.obj", 0.041538f));
	//this->playerCharging->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Charge/One_Charge_000006.obj", 0.041538f));
	//this->playerCharging->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Charge/One_Charge_000007.obj", 0.041538f));
	//this->playerCharging->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Charge/One_Charge_000008.obj", 0.041538f));
	//this->playerCharging->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Charge/One_Charge_000009.obj", 0.041538f));
	//this->playerCharging->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Charge/One_Charge_000010.obj", 0.041538f));
	//this->playerCharging->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Charge/One_Charge_000011.obj", 0.041538f));
	//this->playerCharging->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Charge/One_Charge_000012.obj", 0.041538f));
	//this->playerCharging->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Charge/One_Charge_000013.obj", 0.041538f));
	//this->playerCharging->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Charge/One_Charge_000014.obj", 0.041538f));
	//this->playerCharging->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Charge/One_Charge_000015.obj", 0.041538f));
	//this->playerCharging->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Charge/One_Charge_000016.obj", 0.041538f));
	//this->playerCharging->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Charge/One_Charge_000017.obj", 0.041538f));
	//this->playerCharging->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Charge/One_Charge_000018.obj", 0.041538f));
	//this->playerCharging->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Charge/One_Charge_000019.obj", 0.041538f));
	//this->playerCharging->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Charge/One_Charge_000020.obj", 0.041538f));
	//this->playerCharging->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Charge/One_Charge_000021.obj", 0.041538f));
	//this->playerCharging->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Charge/One_Charge_000022.obj", 0.041538f));
	//this->playerCharging->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Charge/One_Charge_000023.obj", 0.041538f));
	//this->playerCharging->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Charge/One_Charge_000024.obj", 0.041538f));
	//this->playerCharging->AddFrame(new cherry::MorphAnimationFrame("res/objects/anims/player/Attack_Charge/One_Charge_000025.obj", 0.041538f));

	// load all levels here, set main menu scene here. Change scenes in Update based on certain conditions where the level should change.
	if (levelLoading) {
		//// LOAD LEVELS
		// loads level
		MapSceneObjectsToGame();

		ObjectList * objList = objectList;
		LightList * tempList = lightList;
		playerObj = map.GetPlayerObject();
		
		{
			// adding a physics body.
			Vec3 dmns = playerObj->GetMeshBodyMaximum() - playerObj->GetMeshBodyMinimum();
			playerObj->AddPhysicsBody(new PhysicsBodyBox(dmns / 2.0F, Vec3(2, 2, 3)));
		}

		//// ANIMATIONS
		//// Setting animation data
		// Player
		animList.push_back(&playerCharging);
		animList.push_back(&playerCharged);
		animList.push_back(&playerDashing);
		animList.push_back(&playerRun_F);
		animList.push_back(&playerRun_B);
		animList.push_back(&playerRun_L);
		animList.push_back(&playerRun_R);
		// 
		// // Marauder
		// 
		// //// Auto creation of animations based on data (DO NOT CHANGE THIS)
		// // this takes a lot of time.
		std::stringstream curPath;
		float curFrameTime;
		for (int i = 0; i < animList.size(); i++) {
			curFrameTime = animList[i]->animTime / animList[i]->numFrames;
			for (int frame = 0; frame < animList[i]->numFrames; frame++) {
				curPath = std::stringstream();
				if (frame < 10) {
					curPath << animList[i]->basePath << 0 << frame << ".obj";
				}
				else {
					curPath << animList[i]->basePath << frame << ".obj";
				}
				animList[i]->anim->AddFrame(new cherry::MorphAnimationFrame(curPath.str(), curFrameTime));
			}
		}

		//// Add finished animations to objects
		// Player
		playerObj->AddAnimation(playerCharging.anim);
		playerObj->AddAnimation(playerCharged.anim);
		playerObj->AddAnimation(playerDashing.anim);
		playerObj->AddAnimation(playerRun_F.anim);
		playerObj->AddAnimation(playerRun_B.anim);
		playerObj->AddAnimation(playerRun_L.anim);
		playerObj->AddAnimation(playerRun_R.anim);


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
		sentry = Level::sourceSentry;
		// sentry = new Sentry(GetName());
		// sentry->SetVisible(false);

		oracle = Level::sourceOracle;
		// oracle = new Oracle(GetName());
		// oracle->SetVisible(false);

		marauder = Level::sourceMarauder;
		// marauder = new Marauder(GetName());
		// marauder->SetVisible(false);

		bastion = Level::sourceBastion;
		// bastion = new Bastion(GetName());
		// bastion->SetVisible(false);

		mechaspider = Level::sourceSpider;
		// mechaspider = new Mechaspider(GetName());
		// mechaspider->SetVisible(false);

		arrowBase = new Projectile("res/objects/weapons/arrow.obj", GetName());
		arrowBase->SetVisible(false);

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
		SpawnEnemyGroup(4);

		//indArrowAnim = new MorphAnimation();
		//indArrowAnim->AddFrame(new MorphAnimationFrame("res/objects/Arrow_Start.obj", 2.0F));
		//indArrowAnim->AddFrame(new MorphAnimationFrame("res/objects/Arrow_End.obj", 2.0F));
		
		// indArrow = new Object("res/objects/Arrow_Start.obj", GetName(), matDynamic, false, true);
		// indArrow->SetRotationXDegrees(90);
		//indArrow->AddAnimation(indArrowAnim);
		//AddObjectToScene(indArrow);

		indicatorObj = new Object("res/objects/Arrow_End.obj", GetName(), matStatic, false, false); // creates indicator for dash being ready
		indicatorObj->SetRotationXDegrees(90);

		// indicatorObj = new Image("res/images/indicator_arrow.png", GetName(), true, false);
		
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
	else // for testing, loads a level for testing collision, showing all objects and test paths and such
	{
		MapSceneObjectsToGame(levelLoading);
	}

	/*for (int i = 0; i < objList->GetObjectCount(); i++)
	{
		objList->GetObjectByIndex(i)->SetVisible(true);
	}*/

	// sets if the physics bodies should be visible.
	SetVisiblePhysicsBodies(showPBs);

	// Post Processing
	// if(levelLoading)
	if (false)
	{
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
		// layers.push_back(new PostLayer(POST_VS, "res/shaders/post/vibrance.fs.glsl"));
		layers.push_back(lightList->GetPostLayer());

		useFrameBuffers = true;
	}

	// resizing the window so that it fixes the aspect ratio when switching scenes
	// game->Resize(myWindowSize.x, myWindowSize.y);
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
	case GLFW_KEY_ESCAPE:
		paused = !paused;
		break;
	case GLFW_KEY_R:
		if (paused) {
			restart = true;
		}
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
	enemyList.clear();
	curWave++;

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

	for (int j = 0; j < n; j++) {
		if (enemyGroups[i][j] == "sentry") {
			enemyList.push_back(new Sentry(sentry, sceneName));
		}
		else if (enemyGroups[i][j] == "bastion") {
			enemyList.push_back(new Bastion(bastion, sceneName));
		}
		else if (enemyGroups[i][j] == "oracle") {
			enemyList.push_back(new Oracle(oracle, sceneName));
		}
		else if (enemyGroups[i][j] == "marauder") {
			enemyList.push_back(new Marauder(marauder, sceneName));
		}
		else if (enemyGroups[i][j] == "mechaspider") {
			enemyList.push_back(new Mechaspider(mechaspider, sceneName));
		}

		// index
		int index = enemyList.size() - 1;

		enemyList[index]->SetRotation(cherry::Vec3(0, 0, 0), true);
		enemyList[index]->SetPosition(cherry::Vec3(25 + count * 5, 25 - 10 + abs(count) * -5, 0));
		enemyList[index]->alive = true;
		enemyList[index]->SetRotationXDegrees(90);

		// phyiscs body
		PhysicsBody* pb = new cherry::PhysicsBodyBox(enemyList[index]->GetPosition(), enemyList[index]->GetPBodySize());
		pb->SetLocalPosition(cherry::Vec3(0, 0, 1));
		pb->SetVisible(showPBs);

		enemyList[index]->AddPhysicsBody(pb);

		objectList->AddObject(enemyList[index]);

		if (j % 2 == 0) {
			count++;
		}
		count *= -1;
	}
}

// 
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
	obstacles = map.GetObstacles();
	playerObj = map.GetPlayerObject();

	// updates the light list to apply lights to all materials
	lgtList->Update(1.0F);

	// std::vector<cherry::Object*> allSceneObjects = objList->GetObjects();
	// std::string curObjStr;
	// 
	// this->obstacles.clear();
	// 
	// 
	// for (int i = 0; i < allSceneObjects.size(); i++) {
	// 	curObjStr = allSceneObjects[i]->ToString();
	// 
	// 	if (curObjStr.find("GDW_1_Y2 - Tile Sets (MAS_1 - ASN03 - Texturing).blend") != std::string::npos) { // wall
	// 		//std::cout << "its a wall" << std::endl;
	// 		this->obstacles.push_back(allSceneObjects[i]);
	// 		//cherry::Material::Sptr mat = allSceneObjects[i]->GetMaterial();
	// 		//tempList->ApplyLights(mat, tempList->GetLightCount());
	// 	}
	// 	else if (curObjStr.find("charactoereee.blend") != std::string::npos) { // player
	// 		//std::cout << "its a player" << std::endl;
	// 		this->playerObj = (cnz::Player*)allSceneObjects[i];
	// 		this->playerObj->SetPosition(allSceneObjects[i]->GetPosition());
	// 		this->playerSpawn = allSceneObjects[i]->GetPosition();
	// 		this->playerObj->SetRotation(cherry::Vec3(0, 0, 0), true);
	// 		this->playerObj->SetRotationXDegrees(90);
	// 		this->playerObj->SetRotationZDegrees(180);
	// 		this->playerObj->AddPhysicsBody(new cherry::PhysicsBodyBox(playerObj->GetPosition(), playerObj->GetPBodySize()));
	// 	}
	// 	else if (curObjStr.find("Dumpster.blend") != std::string::npos) { // dumpster
	// 		//std::cout << "its a dumpster" << std::endl;
	// 		this->obstacles.push_back(allSceneObjects[i]);
	// 		//cherry::Material::Sptr mat = allSceneObjects[i]->GetMaterial();
	// 		//tempList->ApplyLights(mat, tempList->GetLightCount());
	// 	}
	// 	else if (curObjStr.find("Lamp_Side.blend") != std::string::npos) { // lamp post
	// 		//std::cout << "its a lamp post" << std::endl;
	// 		this->obstacles.push_back(allSceneObjects[i]);
	// 		tempList->AddLight(new cherry::Light(GetName(), allSceneObjects[i]->GetPosition(), cherry::Vec3(1.0f, 1.0f, 1.0f), cherry::Vec3(0.5f, 0.5f, 0.5f), 0.1f, 0.7f, 0.6f, 1.0f / 100.0f));
	// 
	// 		//cherry::Material::Sptr mat = allSceneObjects[i]->GetMaterial();
	// 		//tempList->ApplyLights(mat, tempList->GetLightCount());
	// 	}
	// 	else if (curObjStr.find("Lamp_Corner.blend") != std::string::npos) { // lamp post corner
	// 	//	std::cout << "its a lamp post corner" << std::endl;
	// 		this->obstacles.push_back(allSceneObjects[i]);
	// 		tempList->AddLight(new cherry::Light(GetName(), allSceneObjects[i]->GetPosition(), cherry::Vec3(1.0f, 1.0f, 1.0f), cherry::Vec3(0.5f, 0.5f, 0.5f), 0.1f, 0.7f, 0.6f, 1.0f / 100.0f));
	// 
	// 		//cherry::Material::Sptr mat = allSceneObjects[i]->GetMaterial();
	// 		//tempList->ApplyLights(mat, tempList->GetLightCount());
	// 	}
	// 	else if (curObjStr.find("Lamp_Center.blend") != std::string::npos) { // lamp post middle
	// 		//std::cout << "its a lamp post middle" << std::endl;
	// 		this->obstacles.push_back(allSceneObjects[i]);
	// 		tempList->AddLight(new cherry::Light(GetName(), allSceneObjects[i]->GetPosition(), cherry::Vec3(1.0f, 1.0f, 1.0f), cherry::Vec3(0.5f, 0.5f, 0.5f), 0.1f, 0.7f, 0.6f, 1.0f / 100.0f));
	// 
	// 		//cherry::Material::Sptr mat = allSceneObjects[i]->GetMaterial();
	// 		//tempList->ApplyLights(mat, tempList->GetLightCount());
	// 	}
	// 	else if (curObjStr.find("drum.blend") != std::string::npos) { // barrel
	// 		//std::cout << "its a barrel" << std::endl;
	// 		this->obstacles.push_back(allSceneObjects[i]);
	// 		//cherry::Material::Sptr mat = allSceneObjects[i]->GetMaterial();
	// 		//tempList->ApplyLights(mat, tempList->GetLightCount());
	// 	}
	// 	else if (curObjStr.find("katana.blend") != std::string::npos) { // katana
	// 		//std::cout << "its a katana" << std::endl;
	// 		this->obstacles.push_back(allSceneObjects[i]);
	// 		//cherry::Material::Sptr mat = allSceneObjects[i]->GetMaterial();
	// 		//tempList->ApplyLights(mat, tempList->GetLightCount());
	// 	}
	// 	else if (curObjStr.find("GDW_1_Y2 - Pillar.blend") != std::string::npos) { // pillar
	// 		//std::cout << "its a pillar" << std::endl;
	// 		this->obstacles.push_back(allSceneObjects[i]);
	// 		//cherry::Material::Sptr mat = allSceneObjects[i]->GetMaterial();
	// 		//tempList->ApplyLights(mat, tempList->GetLightCount());
	// 	}
	// 	else if (curObjStr.find("manhole.blend") != std::string::npos) { // manhole
	// 		//std::cout << "its a manhole" << std::endl;
	// 		this->obstacles.push_back(allSceneObjects[i]);
	// 		//cherry::Material::Sptr mat = allSceneObjects[i]->GetMaterial();
	// 		//tempList->ApplyLights(mat, tempList->GetLightCount());
	// 	}
	// 	else if (curObjStr.find("Road.blend") != std::string::npos) { // road
	// 		//std::cout << "its a road" << std::endl;
	// 		this->obstacles.push_back(allSceneObjects[i]);
	// 		//cherry::Material::Sptr mat = allSceneObjects[i]->GetMaterial();
	// 		//tempList->ApplyLights(mat, tempList->GetLightCount());
	// 	}
	// 	else if (curObjStr.find("sidewalk.blend") != std::string::npos) { // sidewalk
	// 		//std::cout << "its a sidewalk" << std::endl;
	// 		this->obstacles.push_back(allSceneObjects[i]);
	// 		//cherry::Material::Sptr mat = allSceneObjects[i]->GetMaterial();
	// 		//tempList->ApplyLights(mat, tempList->GetLightCount());
	// 	}
	// }
	// tempList->Update(0.0f);
	// allSceneObjects.clear(); // clear up some memory since all of these pointers are added to other lists
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

// update loop
void cnz::CNZ_GameplayScene::Update(float deltaTime)
{
	if (paused == false) {
		pauseMenu->SetVisible(false);
		CNZ_Game* game = (CNZ_Game*)cherry::Game::GetRunningGame();
		cherry::Camera::Sptr myCamera = game->myCamera;

		//GLenum test = glGetError();
		this->playerPrevPos = playerObj->GetPosition(); // store previous position

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

		// goes through each physics body for hte player
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
			for (int i = 0; i < enemyList.size(); i++) 
			{
				// gets the enemy bodies
				vector<cherry::PhysicsBody*> eBodies = enemyList[i]->GetPhysicsBodies();

				for (cherry::PhysicsBody* eBody : eBodies)
				{
					// collision check
					bool collision = cherry::PhysicsBody::Collision(pBody, eBody);
					
					if (collision) {
						//Player takes damage
						lives--;
						playerObj->SetPosition(playerSpawn);
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

		cs = true;
		cw = true;
		ca = true;
		cd = true;


		// TODO: actually fix collisions... and allow player to move out if colliding in all directions. (AKA when all of the above booleans are false)
		// check what directions the player can move in based on its collisions with obstacles in the scene.
		if (playerObstacleCollisions.size() != 0) { // allow movement only in directions oposite of the collision (CUBES ONLY)
			// std::cout << "There are " << playerObstacleCollisions.size() << " playerObj collisions this update!" << std::endl;
			
			// player world position
			cherry::Vec3 plyrPos = playerObj->GetPosition();

			for (int i = 0; i < playerObstacleCollisions.size(); i++) {
				cherry::Vec3 dP = playerObstacleCollisions[i]->GetWorldPosition() - playerObj->GetPosition();

				// gets the world position of the obstacle
				cherry::Vec3 obstPos = playerObstacleCollisions[i]->GetWorldPosition();

				if ((obstPos.GetY() - plyrPos.GetY()) >= 0) { // above the object
					cs = false;
				}
				if ((obstPos.GetY() - plyrPos.GetY()) <= 0) { // below the object
					cw = false;
				}
				if ((obstPos.GetX() - plyrPos.GetX()) >= 0) { // right of the object
					ca = false;
				}
				if ((obstPos.GetX() - plyrPos.GetX()) <= 0) { // left of the object
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
		if (w && cw) { // up
			curMoveDir += cherry::Vec3(0, 1, 0);
			playerObj->SetPosition(playerObj->GetPosition() + cherry::Vec3(0.0F, moveInc * deltaTime, 0.0F));
		}
		if (s && cs) { // down
			curMoveDir += cherry::Vec3(0, -1, 0);
			playerObj->SetPosition(playerObj->GetPosition() + cherry::Vec3(0.0F, -moveInc * deltaTime, 0.0F));
		}
		if (a && ca) { // left
			curMoveDir += cherry::Vec3(-1, 0, 0);
			playerObj->SetPosition(playerObj->GetPosition() + cherry::Vec3(-moveInc * deltaTime, 0.0F, 0.0F));
		}
		if (d && cd) { // right
			curMoveDir += cherry::Vec3(1, 0, 0);
			playerObj->SetPosition(playerObj->GetPosition() + cherry::Vec3(moveInc * deltaTime, 0.0F, 0.0F));
		}

		if (!((w && cw) || (s && cs) || (a && ca) || (d && cd))) { // if the player is not moving
			playerObj->SetState(0);
		}
		else { // if the player is walking
			playerObj->SetState(1);
		}

		playerObj->UpdateAngle(myCamera, game->GetCursorViewPositionX(), game->GetCursorViewPositionY(), game->GetWindowWidth(), game->GetWindowHeight());
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
						enemyList[i]->attacking = true;
						projList.push_back(new Projectile(*arrowBase));
						projTimeList.push_back(0);
						projList[projList.size() - 1]->AddPhysicsBody(new cherry::PhysicsBodyBox(enemyList[i]->GetPhysicsBodies()[0]->GetWorldPosition(), projList[projList.size() - 1]->GetPBodySize()));
						projList[projList.size() - 1]->SetWhichGroup(i);
						//projList[projList.size() - 1]->SetWhichEnemy(j);
						projList[projList.size() - 1]->active = true;
						projList[projList.size() - 1]->SetPosition(enemyList[i]->GetPosition());
						projList[projList.size() - 1]->SetRotationDegrees(enemyList[i]->GetRotationDegrees());
						projList[projList.size() - 1]->SetDirVec(GetUnitDirVec(projList[projList.size() - 1]->GetPosition(), playerObj->GetPosition()));
						game->AddObjectToScene(projList[projList.size() - 1]);
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
		if (playerObj->GetDashTime() >= 1.0f) { // ready to dash but hasn't released chargey button yet
			//Display indicator
			//indArrowAnim->Play();
			indicatorObj->SetPosition(playerObj->GetPosition() + cherry::Vec3(0, 0, -1.90f));
			indicatorObj->SetVisible(true);
			indicatorObj->SetRotationZDegrees(playerObj->GetRotationZDegrees() + 180);
		}
		else { // dash timer is below 1.0f
			//Hide indicator
			indicatorObj->SetVisible(false);
		}

		if (playerObj->GetDashTime() >= 1.0f && mbLR == true) // if dash timer is above 1.0 and left mouse has been released, do the dash
		{
			cherry::Vec3 dashVec = playerObj->GetDash(playerObj->GetDashDist());
			float tempDist = dashVec.GetLength();
			playerObj->SetDash(true);
			playerObj->SetState(4); // set state to dashing
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
					// TODO: the destructor already removes the phyiscs body. So this is probably unneeded.
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
			if (playerObj->GetDashTime() >= 1.0f) {
				playerObj->SetState(3); // charged 
			}
			else {
				playerObj->SetState(2); // set state to dash charging 
			}
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
		// // enemy PBs
		// for (int i = 0; i < enemyList.size(); i++) {
		// 	//enemyList[i]->GetPhysicsBodies()[0]->SetLocalPosition(cherry::Vec3(0,0,1));
		// }
		// test PB
		//testObj->GetPhysicsBodies()[0]->SetLocalPosition(testObj->GetPosition());


		//// ANIMATION UPDATES
		// Player
		if (playerObj->GetState() == 1) { // walking
			float angle = 0;
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

			float playerAngle = playerObj->GetDegreeAngle();
			float angleDiff;

			// always get a positive difference 
			if (playerAngle > angle) {
				angleDiff = playerAngle - angle;
			}
			else if (angle > playerAngle) {
				angleDiff = angle - playerAngle;
			}
			else {
				angleDiff = 0.0f;
			}

			angleDiff = angleDiff - 90.0f; // subrtact 90 to fix werid rotation...

			// make sure we are between 0 and 360. this should never be an issue but why NOT add redundant computations? /s 
			while (angleDiff < 0.0f) {
				angleDiff = angleDiff + 360.0f;
			}
			while (angleDiff > 360.0f) {
				angleDiff = angleDiff - 360.0f;
			}

			if ((angle <= 45.0f && angle >= 0.0f) || (angle <= 360.0f && angle >= 315.0f)) { // forward walking animation 
				if ((playerObj->GetCurrentAnimation() == nullptr) || (playerObj->GetAnimation(3) != playerObj->GetCurrentAnimation())) { // check if charge anim is already playing 
					playerObj->SetCurrentAnimation(3);
					playerObj->GetCurrentAnimation()->Play();
				}
			}
			else if (angle > 45.0f && angle <= 135.0f) { // right walking animation 

				if ((playerObj->GetCurrentAnimation() == nullptr) || (playerObj->GetAnimation(6) != playerObj->GetCurrentAnimation())) { // check if charge anim is already playing 
					playerObj->SetCurrentAnimation(6);
					playerObj->GetCurrentAnimation()->Play();
				}
			}
			else if (angle > 135.0f && angle <= 225.0f) { // backwards walking animation 
				if ((playerObj->GetCurrentAnimation() == nullptr) || (playerObj->GetAnimation(4) != playerObj->GetCurrentAnimation())) { // check if charge anim is already playing 
					playerObj->SetCurrentAnimation(4);
					playerObj->GetCurrentAnimation()->Play();
				}
			}
			else if (angle > 225.0f && angle < 315.0f) { // left walking animation 
				if ((playerObj->GetCurrentAnimation() == nullptr) || (playerObj->GetAnimation(5) != playerObj->GetCurrentAnimation())) { // check if charge anim is already playing 
					playerObj->SetCurrentAnimation(5);
					playerObj->GetCurrentAnimation()->Play();
				}
			}
		}
		else if (playerObj->GetState() == 2) { // charging dash attack
			if ((playerObj->GetCurrentAnimation() == nullptr) || (playerObj->GetAnimation(0) != playerObj->GetCurrentAnimation())) { // check if charge anim is already playing
				playerObj->SetCurrentAnimation(0);
				playerObj->GetCurrentAnimation()->Play();
			}
		}
		else if (playerObj->GetState() == 3) { // dash charged 
			if (playerObj->GetCurrentAnimation() != nullptr) {
				playerObj->GetCurrentAnimation()->Stop();
			}
			playerObj->SetCurrentAnimation(1);
			playerObj->GetCurrentAnimation()->Play();
		}
		else if (playerObj->GetState() == 4) { // dashing 
			if ((playerObj->GetCurrentAnimation() == nullptr) || (playerObj->GetAnimation(2) != playerObj->GetCurrentAnimation())) {
				playerObj->SetCurrentAnimation(2);
				playerObj->GetCurrentAnimation()->Play();
			}
		}


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
	}
	else {
		//Pause Menu Code
		if (restart) {
			//Reset Everything
			kills = 0;
			curGroup = -1;
			curWave = 0;

			//Enemies
			for (int i = 0; i < enemyList.size(); i++) {
				enemyList[i]->RemovePhysicsBody(enemyList[i]->GetPhysicsBodies()[0]);
				cherry::Object* obj = enemyList[i];
				util::removeFromVector(enemyList, enemyList[i]);
				objectList->RemoveObjectByPointer(obj);
				delete obj;
				kills++;
				// cout << kills << endl;
				i--;
			}
			enemyList.clear();

			//Player
			// not needed?
			// playerObj->AddPhysicsBody(new cherry::PhysicsBodyBox(playerObj->GetPosition(), playerObj->GetPBodySize()));
			playerObj->SetPosition(playerSpawn);

			paused = false;
			restart = false;
		}

		pauseMenu->SetVisible(true);
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
// todo: remove this function? Nothign calls it.
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

