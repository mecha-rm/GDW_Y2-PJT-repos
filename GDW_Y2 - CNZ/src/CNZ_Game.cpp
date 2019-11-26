#include "CNZ_Game.h"
#include <GLM/gtc/matrix_transform.hpp> // camera modes


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
	}
}

//////////////////TODO: Why is this defined in here??????????????????????? Should it not be in some sort of PhysicsBody file or something?////////////////////////
// returns the physics body of the closest obstacle to the player, in the direction that the player is facing.
// Used later to verify if the player is allowed to dash the full distance in their intended direction
cherry::PhysicsBody* cnz::CNZ_Game::getClosestObstacle()
{
	cherry::PhysicsBody* closestBody = nullptr;
	float cbDist = 0.0f;
	for (int i = 0; i < obstaclePBs.size(); i++) {
		cherry::Vec3 delta;
		delta.SetX(obstaclePBs[i]->GetModelPosition().GetX() - playerObj->GetPosition().GetX());
		delta.SetY(obstaclePBs[i]->GetModelPosition().GetY() - playerObj->GetPosition().GetY());

		float angleFromPlayer = getXYAngle(delta);
		float dAngle = angleFromPlayer - getXYAngle(playerObj->GetDash(playerObj->GetDashDist()));
		
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

// test func to get angle in xy axes of a vec. use only when you stored delta x, delta y and delta z in a vec3.
// this doesnt make sense to use on a position, but rather only on a difference in position
float cnz::CNZ_Game::getXYAngle(cherry::Vec3 vec)
{
	return atanf(vec.GetX() / vec.GetY());
}

void cnz::CNZ_Game::spawnEnemyGroup(int i)
{
	int n = enemyGroups[i].size();

	for (int j = 0; j < n; j++) {
		enemyGroups[i][j]->SetRotation(cherry::Vec3(0, 0, 0), true);
		enemyGroups[i][j]->SetPosition(cherry::Vec3(0 + j * 5, 0 + j * 5, 0));
		enemyGroups[i][j]->AddPhysicsBody(new cherry::PhysicsBodyBox(enemyGroups[i][j]->GetPosition(), enemyGroups[i][j]->getPBodySize()));

		AddObject(enemyGroups[i][j]);
	}
}

// loads content
void cnz::CNZ_Game::LoadContent()
{
	Game::LoadContent(); // calls the load content

	playerObj = new Player("res/objects/monkey.obj", getCurrentScene(), material); // creates the player.
	testObj = new Player("res/objects/monkey.obj", getCurrentScene(), material); // creates the not player.
	testEnemy = new Enemies("res/objects/cube.obj", getCurrentScene(), material);

	sentry = new Enemies("res/objects/cube.obj", getCurrentScene(), material);
	oracle = new Enemies("res/objects/cube.obj", getCurrentScene(), material);
	marauder = new Enemies("res/objects/sphere.obj", getCurrentScene(), material);
	bastion = new Enemies("res/objects/sphere.obj", getCurrentScene(), material);
	mechaspider = new Enemies("res/objects/sphere.obj", getCurrentScene(), material);

	enemyGroups.push_back(std::vector<Enemies*>());
	enemyGroups[0].push_back(new Marauder(marauder, getCurrentScene()));
	enemyGroups[0].push_back(new Sentry(sentry, getCurrentScene()));
	enemyGroups[0].push_back(new Sentry(sentry, getCurrentScene()));
	
	enemyGroups.push_back(std::vector<Enemies*>());
	enemyGroups[1].push_back(new Bastion(bastion, getCurrentScene()));
	enemyGroups[1].push_back(new Oracle(oracle, getCurrentScene()));
	enemyGroups[1].push_back(new Oracle(oracle, getCurrentScene()));
	
	enemyGroups.push_back(std::vector<Enemies*>());
	enemyGroups[2].push_back(new Mechaspider(mechaspider, getCurrentScene()));
	enemyGroups[2].push_back(new Mechaspider(mechaspider, getCurrentScene()));
	enemyGroups[2].push_back(new Mechaspider(mechaspider, getCurrentScene()));

	testSentry = new Sentry(testEnemy, getCurrentScene());
	testSentry2 = new Sentry(testEnemy, getCurrentScene());
	testSentry3 = new Sentry(testEnemy, getCurrentScene());

	// playerObj->CreateEntity(getCurrentScene(), material);
	playerObj->SetRotation(cherry::Vec3(0, 0, 0), true);
	testSentry2->SetRotation(cherry::Vec3(0, 0, 0), true);
	testSentry3->SetRotation(cherry::Vec3(0, 0, 0), true);
	testObj->SetRotation(cherry::Vec3(0, 0, 0), true);
	testObj->SetPosition(cherry::Vec3(0, -5, 0));
	testSentry2->SetPosition(cherry::Vec3(-5, 2.5, 0));
	testSentry3->SetPosition(cherry::Vec3(5, 2.5, 0));

	playerObj->AddPhysicsBody(new cherry::PhysicsBodyBox(playerObj->GetPosition(), playerObj->getPBodySize()));
	testObj->AddPhysicsBody(new cherry::PhysicsBodyBox(testObj->GetPosition(), testObj->getPBodySize()));

	testObj->GetPhysicsBodies()[0]->SetModelPosition(testObj->GetPosition());

	//Number corresponds with enemygroups first index
	spawnEnemyGroup(2);
	AddObject(playerObj);
	AddObject(testObj);
	//AddObject(testSentry);
	//AddObject(testSentry2);
	//AddObject(testSentry3);

	if (!playerObj->setDrawPBody(true)) {
		std::cout << "Ruhroh... Couldn't set drawPBody on playerObj!" << std::endl;
	}

	obstaclePBs.push_back(testObj->GetPhysicsBodies()[0]);

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

// Update function
void cnz::CNZ_Game::Update(float deltaTime)
{
	this->playerPrevPos = playerObj->GetPosition();

	float moveInc = -10.0F; // the movement incrementer.

	vector<cherry::PhysicsBody*> playerCollisions; // TODO: figure out a system so that not all collisions are player - obstacle only. Maybe tags?

	for (int i = 0; i < obstaclePBs.size(); i++) {
		bool collision = cherry::PhysicsBody::Collision(playerObj->GetPhysicsBodies()[0], obstaclePBs[i]);
		if (collision) {
			playerCollisions.push_back(obstaclePBs[i]);
		}
		else {
			std::cout << i << " is not a collision." << std::endl;
		}
	}

	cs = true;
	cw = true;
	ca = true;
	cd = true;

	if (playerCollisions.size() != 0) { // allow movement only in directions oposite of the collision (CUBES ONLY)
		std::cout << "There are " << playerCollisions.size() << " playerObj collisions this update!" << std::endl;
		for (int i = 0; i < playerCollisions.size(); i++) {
			cherry::Vec3 dP = playerCollisions[i]->GetModelPosition() - playerObj->GetPosition();
			if (fabsf(dP.GetX()) < fabsf(dP.GetY())) { // this is why its cubes only
				if ((playerCollisions[i]->GetModelPosition().GetY() - playerObj->GetPosition().GetY()) >= 0) { // above the object
					cs = false;
				}
				else if ((playerCollisions[i]->GetModelPosition().GetY() - playerObj->GetPosition().GetY()) <= 0) { // below the object
					cw = false;
				}
			}
			else if (fabsf(dP.GetX()) > fabsf(dP.GetY())) { // this is the same thing, also why its cube only.
				if ((playerCollisions[i]->GetModelPosition().GetX() - playerObj->GetPosition().GetX()) >= 0) { // right of the object
					ca = false;
				}
				else if ((playerCollisions[i]->GetModelPosition().GetX() - playerObj->GetPosition().GetX()) <= 0) { // left of the object
					cd = false;
				}
			}
		}
	}
	
	// side checks
	//if ((playerCollisions[i]->GetModelPosition().GetX() - playerObj->GetPosition().GetX()) >= 0) // right of the object
	//if ((playerCollisions[i]->GetModelPosition().GetX() - playerObj->GetPosition().GetX()) <= 0) // left of the object
	//if ((playerCollisions[i]->GetModelPosition().GetY() - playerObj->GetPosition().GetY()) >= 0) // above the object
	//if ((playerCollisions[i]->GetModelPosition().GetY() - playerObj->GetPosition().GetY()) <= 0) // below the object

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
	playerObj->SetRotation(cherry::Vec3(0.0f, 0.0f, playerObj->GetDegreeAngle()), true);
	
	
	// check if mouse left button is being held down
	if (playerObj->GetDashTime() >= 1.0f && mbLR == true) 
	{
		cherry::Vec3 dashVec = playerObj->GetDash(playerObj->GetDashDist());
		float tempDist = dashVec.GetLength();
		playerObj->SetDash(true);
		playerObj->SetDashTime(0.0f);
		cherry::PhysicsBody* closestObstacle = getClosestObstacle();
		if (closestObstacle == nullptr) {
			playerObj->SetPosition(playerObj->GetPosition() + dashVec);
		}
		else {
			cherry::Vec3 dP = closestObstacle->GetModelPosition() - playerObj->GetPosition();
			cherry::Vec3 dPN;
			if (fabsf(dP.GetX()) > fabsf(dP.GetY())) {
				float tempX = 0;
				if (dP.GetX() < 0) {
					tempX = dP.GetX() + ((playerObj->getPBodySize().GetX() / 4) + (closestObstacle->GetObject()->getPBodySize().GetX() / 4));
				}
				else {
					tempX = dP.GetX() - ((playerObj->getPBodySize().GetX() / 4) + (closestObstacle->GetObject()->getPBodySize().GetX() / 4));
				}
				float angle = getXYAngle(dP);
				float tempY = tempX / tanf(angle);
				dPN.SetX(tempX);
				dPN.SetY(tempY);
			}
			else {
				float tempY = 0;
				if (dP.GetY() < 0) {
					tempY = dP.GetY() + ((playerObj->getPBodySize().GetY() / 4) + (closestObstacle->GetObject()->getPBodySize().GetY() / 4));
				}
				else {
					tempY = dP.GetY() - ((playerObj->getPBodySize().GetY() / 4) + (closestObstacle->GetObject()->getPBodySize().GetY() / 4));
				}
				float angle = getXYAngle(dP);
				float tempX = tempY * tanf(angle);
				dPN.SetX(tempX);
				dPN.SetY(tempY);
			}
			
			if (dP.GetLength() < tempDist) {
				playerObj->SetPosition(playerObj->GetPosition() + dPN);
			}
			else {
				playerObj->SetPosition(playerObj->GetPosition() + dashVec);
			}
		}
		
	}
	else if (mbLP == true && mbLR == false) 
	{
		playerObj->SetDashTime(playerObj->GetDashTime() + 1.25f * deltaTime);
		//std::cout << playerObj->GetDashTime() << std::endl;
	}
	else if (mbLP == true && mbLR == true) {
		playerObj->SetDashTime(0.0f);
		//Logger::GetLogger()->info(this->dashTime);
		this->mbLP = false;
		this->mbLR = false;
	}

	// update physics bodies
	playerObj->GetPhysicsBodies()[0]->SetModelPosition(playerObj->GetPosition());

	//cherry::Vec3 dPpb = playerObj->GetPhysicsBodies()[0]->getModelPosition() - playerObj->GetPosition();
	//std::cout << "PBPos: " << playerObj->GetPhysicsBodies()[0]->GetModelPosition().ToString() << std::endl;
	//std::cout << "P2Pos: " << testObj->GetPhysicsBodies()[0]->GetModelPosition().ToString() << std::endl;
	//std::cout << "PWPos: " << playerObj->GetPosition().ToString() << std::endl;
	//std::cout << "x: " << playerObj->GetPosition().GetX() << "	y: " << playerObj->GetPosition().GetY() << std::endl;
	//std::cout << "x: " << playerObj->GetPhysicsBodies()[0]->GetModelPosition().GetX() << "	y: " << playerObj->GetPhysicsBodies()[0]->GetModelPosition().GetY() << std::endl;


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
