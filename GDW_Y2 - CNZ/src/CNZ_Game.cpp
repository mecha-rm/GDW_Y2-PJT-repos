#include "CNZ_Game.h"
#include <GLM/gtc/matrix_transform.hpp> // camera modes

// constructor
cnz::CNZ_Game::CNZ_Game() : Game() {}

// constructor with window size
// TODO: change 'true' to 'false' so default values aren't used;
// TODO: don't use full screen.
cnz::CNZ_Game::CNZ_Game(float windowWidth, float windowHeight, bool fullScreen) : Game(windowWidth, windowHeight, fullScreen, true, true) {}

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

// loads content
void cnz::CNZ_Game::LoadContent()
{
	Game::LoadContent(); // calls the load content

	playerObj = new Player("res/objects/monkey.obj", getCurrentScene(), material); // creates the player.
	testObj = new Player("res/objects/monkey.obj", getCurrentScene(), material); // creates the player.
	// playerObj->CreateEntity(getCurrentScene(), material);
	playerObj->SetRotation(cherry::Vec3(0, 0, 0), true);
	testObj->SetRotation(cherry::Vec3(0, 0, 0), true);
	addObject(playerObj);
	addObject(testObj);

	//// setting up the camera
	//myCamera->SetPosition(glm::vec3(0, 5, 10));
	//myCamera->LookAt(glm::vec3(0));

	//// sets the camera to perspective mode for the scene.
	//// myCamera->SetPerspectiveMode(glm::perspective(glm::radians(60.0f), 1.0f, 0.01f, 1000.0f));
	////myCamera->SetPerspectiveMode(glm::perspective(glm::radians(60.0f), 1.0f, 0.01f, 1000.0f));
	myCamera->SetPerspectiveMode(glm::perspective(glm::radians(60.0f), 1.0f, 0.01f, 1000.0f));
	//// myCamera->SetPerspectiveMode(glm::perspective(glm::radians(10.0f), 1.0f, 0.01f, 1000.0f));

	//// sets the orthographic mode values. False is passed so that the camera starts in perspective mode.
	//myCamera->SetOrthographicMode(glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.0f, 100.0f), false);

	
}

// Update function
void cnz::CNZ_Game::Update(float deltaTime)
{
	float moveInc = -10.0F; // the movement incrementer.

	// moving the player
	if (w) { // up
		playerObj->SetPosition(playerObj->GetPosition() + cherry::Vec3(0.0F, moveInc * deltaTime, 0.0F));
	}
	if (s) { // down
		playerObj->SetPosition(playerObj->GetPosition() + cherry::Vec3(0.0F, -moveInc * deltaTime, 0.0F));
	}
	if (a) { // left
		playerObj->SetPosition(playerObj->GetPosition() + cherry::Vec3(-moveInc * deltaTime, 0.0F, 0.0F));
	}
	if (d) { // right
		playerObj->SetPosition(playerObj->GetPosition() + cherry::Vec3(moveInc * deltaTime, 0.0F, 0.0F));
	}

	playerObj->UpdateAngle(myCamera, GetCursorPosX(), GetCursorPosY(), GetWindowWidth(), GetWindowHeight());
	playerObj->SetRotation(cherry::Vec3(0.0f, 0.0f, playerObj->GetDegreeAngle()), true);
	// check if mouse left button is being held down
	if (playerObj->GetDashTime() >= 1.0f) 
	{
		playerObj->SetDash(true);
		playerObj->SetDashTime(0.0f);
		playerObj->SetPosition(playerObj->GetPosition() + playerObj->GetDash(0.1f));
	}
	else if (mbLP == true && mbLR == false) 
	{
		playerObj->SetDashTime(playerObj->GetDashTime() + 1.25f * deltaTime);
		std::cout << playerObj->GetDashTime() << std::endl;
	}
	else if (mbLP == true && mbLR == true) {
		playerObj->SetDashTime(0.0f);
		//Logger::GetLogger()->info(this->dashTime);
		this->mbLP = false;
		this->mbLR = false;
	}

	// std::cout << playerObj->GetPosition().toString() << std::endl;

	//myCamera->SetPosition(playerObj->GetPosition() + cherry::Vec3(-10.0F, 10.0F, 15.0F));
	myCamera->SetPosition(playerObj->GetPosition() + cherry::Vec3(0.0F, 5.0F, 20.0F));
	myCamera->LookAt(playerObj->GetPositionGLM());

	// calls the main game Update function to go through every object.
	Game::Update(deltaTime);
}
