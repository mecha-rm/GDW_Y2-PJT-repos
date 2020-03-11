// GAME CLASS (SOURCE)

#include "Game.h"

#include <stdexcept>
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>
#include <GLM/gtc/matrix_transform.hpp>
#include <toolkit/Logging.h>

#include "MeshRenderer.h"
#include "textures/Texture2D.h"

#include "physics/PhysicsBody.h"
#include "utils/Utils.h"
#include "WorldTransform.h"
#include "scenes/EngineScene.h"

#include<functional>
#include<time.h>
#include<random>

/*
	Handles debug messages from OpenGL
	https://www.khronos.org/opengl/wiki/Debug_Output#Message_Components
	@param source    Which part of OpenGL dispatched the message
	@param type      The type of message (ex: error, performance issues, deprecated behavior)
	@param id        The ID of the error or message (to distinguish between different types of errors, like nullref or index out of range)
	@param severity  The severity of the message (from High to Notification)
	@param length    The length of the message
	@param message   The human readable message from OpenGL
	@param userParam The pointer we set with glDebugMessageCallback (should be the game pointer)
*/
void GlDebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	switch (severity) {
	case GL_DEBUG_SEVERITY_LOW:          LOG_INFO(message); break;
	case GL_DEBUG_SEVERITY_MEDIUM:       LOG_WARN(message); break;
	case GL_DEBUG_SEVERITY_HIGH:         LOG_ERROR(message); break;
#ifdef LOG_GL_NOTIFICATIONS
	case GL_DEBUG_SEVERITY_NOTIFICATION: LOG_INFO(message); break;
#endif
	default: break;
	}
}

// TODO: have the callbakcs call the scene directly?
// call this function to resize the window.
void GlfwWindowResizedCallback(GLFWwindow* window, int width, int height) {

	// the width and height must be greater than 0
	if (width > 0 && height > 0)
	{
		glViewport(0, 0, width, height);

		cherry::Game* game = (cherry::Game*)glfwGetWindowUserPointer(window);

		if (game != nullptr && width)
		{
			if (game->changeImageAspectOnWindowResize) // if the aspect ratio should change with the window
				game->Resize(width, height);
		}
	}
}

// called when a mouse button event is recorded
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	cherry::Game* game = (cherry::Game*)glfwGetWindowUserPointer(window);

	// returns the function early if this isn't a game class
	if (game == nullptr) {
		return;
	}

	switch (action) {
	case GLFW_PRESS:
		game->MouseButtonPressed(window, button);
		break;

	case GLFW_REPEAT:
		game->MouseButtonHeld(window, button);
		break;

	case GLFW_RELEASE:
		game->MouseButtonReleased(window, button);
		break;
	}
}



// called when a cursor enters the content area window.
void CursorEnterCallback(GLFWwindow* window, int enter)
{
	cherry::Game* game = (cherry::Game*)glfwGetWindowUserPointer(window);

	if (game == nullptr) {
		return;
	}

	// sets whether the mouse cursor is in the window
	game->SetCursorInWindow(enter);
}

// called when the mouse moves over the screen, getting the position.
void CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
	cherry::Game* game = (cherry::Game*)glfwGetWindowUserPointer(window);

	if (game == nullptr) {
		return;
	}
	else {
		game->UpdateCursorPos(xpos, ypos);
	}
}


// called when a key has been pressed, held down, or released. This function figures out which, and calls the appropriate function to handle it.
// KeyCallback(Window, Keyboard Key, Platform-Specific Scancode, Key Action, and Modifier Bits)
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	cherry::Game* game = (cherry::Game*)glfwGetWindowUserPointer(window);

	if (game != nullptr)
	{
		// checks for what type of button press happened.
		switch (action)
		{
		case GLFW_PRESS: // key has been pressed
			game->KeyPressed(window, key);
			break;

		case GLFW_REPEAT: // key is held down
			game->KeyHeld(window, key);
			break;

		case GLFW_RELEASE: // key has been released
			game->KeyReleased(window, key);
			break;
		}
	}
}

// GAME FUNCTIONS
short int cherry::Game::FPS = 0;
cherry::Game* cherry::Game::runningGame = nullptr;

// Game
// constructor
cherry::Game::Game() :
	myWindow(nullptr),
	myWindowTitle("GDW_Y2 - BnsFt - Cherry Engine"),
	myClearColor(glm::vec4(0.1f, 0.7f, 0.5f, 1.0f)), // default clear colour
	myModelTransform(glm::mat4(1)), // my model transform
	myWindowSize(600, 600) // window size (default)
{
}

// creates window with a width, height, and whether or not it's in full screen.
cherry::Game::Game(const char windowTitle[WINDOW_TITLE_CHAR_MAX], float _width, float _height, bool _fullScreen, cherry::Scene * _openingScene, bool _imgui)
	: Game()
{
	// setting the values
	memcpy(myWindowTitle, windowTitle, strlen(windowTitle) + 1);
	myWindowSize = glm::ivec2(_width, _height);
	fullScreen = _fullScreen;
	openingScene = _openingScene;
	imguiMode = _imgui;
}

// destructor
cherry::Game::~Game() { }

// returns the window title
const std::string cherry::Game::GetWindowTitle() const
{
	// the window title
	std::string windowTitle = "";

	// putting the characters into the string
	for (int i = 0; i < WINDOW_TITLE_CHAR_MAX; i++)
	{
		// if a termination character is encountered, then the string is finished.
		// all unused indexes in the myWindowTitle array default to '\0' (null termination character)
		if (myWindowTitle[i] == '\0')
			break;
		
		// puts the character onto the string.
		windowTitle.push_back(myWindowTitle[i]);
	}

	// returns the string.
	return windowTitle;
}

// gets the length of the window title.
int cherry::Game::GetWindowTitleLength() const { return GetWindowTitle().size(); }

// gets the GLFW window.
GLFWwindow* cherry::Game::GetWindow() const { return myWindow; }

// gets the window width
int cherry::Game::GetWindowWidth() const { return myWindowSize.x; }

// gets the window height
int cherry::Game::GetWindowHeight() const { return myWindowSize.y; }

// gets the window size
glm::ivec2 cherry::Game::GetWindowSize() const { return myWindowSize; }

// shows whether the window is in full screen or not
bool cherry::Game::IsFullScreen() const { return fullScreen; }

// gets whether the cursor is in the window content or not.
bool cherry::Game::GetCursorInWindow() const { return mouseEnter; }

// sets whether the cursor is in the window.
void cherry::Game::SetCursorInWindow(bool inWindow) { mouseEnter = inWindow; }

// updates cursor position variables
void cherry::Game::UpdateCursorPos(double xpos, double ypos)
{
	// Game* game = (Game*)glfwGetWindowUserPointer(myWindow);

	mousePos = glm::dvec2(xpos - this->myWindowSize.x / 2.0F, ypos - this->myWindowSize.y / 2.0F);

	//this->XcursorPos = xpos;
	//this->YcursorPos = ypos;
	// xpos = xpos - this->windowWidth / 2.0F;
	// ypos = ypos - this->windowHeight / 2.0F;

	//Logger::GetLogger()->info(xpos);
	//Logger::GetLogger()->info(ypos);

}

// gets the cursor position
cherry::Vec2 cherry::Game::GetCursorPos() const { return Vec2(mousePos); }

// gets the cursor position as a glm vector
glm::dvec2 cherry::Game::GetCursorPosGLM() const { return mousePos; }

float cherry::Game::GetCursorPosX() const { return mousePos.x; }

// returns the cursor position on the y-axis
float cherry::Game::GetCursorPosY() const { return mousePos.y; }

// called when a mouse button has been pressed
void cherry::Game::MouseButtonPressed(GLFWwindow* window, int button) {
	Game* game = (Game*)glfwGetWindowUserPointer(window);
	Scene* scene = CurrentScene();

	if (game == nullptr || scene == nullptr) // if game is 'null', then it is returned
		return;

	scene->MouseButtonPressed(window, button);
}

// called when a mouse button is being held
void cherry::Game::MouseButtonHeld(GLFWwindow* window, int button) {
	Game* game = (Game*)glfwGetWindowUserPointer(window);
	Scene* scene = CurrentScene();

	if (game == nullptr || scene == nullptr) // if game is 'null', then it is returned
		return;

	scene->MouseButtonHeld(window, button);
}

// called when a mouse button has been released
void cherry::Game::MouseButtonReleased(GLFWwindow* window, int button) {
	Game* game = (Game*)glfwGetWindowUserPointer(window);
	Scene* scene = CurrentScene();

	if (game == nullptr || scene == nullptr) // if game is 'null', then it is returned
		return;

	scene->MouseButtonReleased(window, button);
}

// key has been pressed
void cherry::Game::KeyPressed(GLFWwindow* window, int key)
{
	Game* game = (Game*)glfwGetWindowUserPointer(window);
	Scene* scene = CurrentScene();

	if (game == nullptr || scene == nullptr) // if game is 'null', then it is returned
		return;

	scene->KeyPressed(window, key);
}

// key is being held
void cherry::Game::KeyHeld(GLFWwindow* window, int key)
{
	Game* game = (Game*)glfwGetWindowUserPointer(window);
	Scene* scene = CurrentScene();

	if (game == nullptr || scene == nullptr) // if game is 'null', then it is returned
		return;

	scene->KeyHeld(window, key);
}

// key hs been released
void cherry::Game::KeyReleased(GLFWwindow* window, int key)
{
	Game* game = (Game*)glfwGetWindowUserPointer(window);
	Scene* scene = CurrentScene();

	if (game == nullptr || scene == nullptr) // if game is 'null', then it is returned
		return;

	scene->KeyReleased(window, key);
}

// static screne creation
bool cherry::Game::CreateScene(const std::string sceneName)
{
	// creating a default skybox for the scene, since none was provided
	Skybox skybox(
		"res/images/cubemaps/checkerboard_black-grey_d.jpg",
		"res/images/cubemaps/checkerboard_black-grey_d.jpg",
		"res/images/cubemaps/checkerboard_black-grey_d.jpg",
		"res/images/cubemaps/checkerboard_black-grey_d.jpg",
		"res/images/cubemaps/checkerboard_black-grey_d.jpg",
		"res/images/cubemaps/checkerboard_black-grey_d.jpg"
	);

	// calling other static function
	return CreateScene(sceneName, skybox);
}

// static scene creation (with skybox)
bool cherry::Game::CreateScene(const std::string sceneName, const cherry::Skybox skybox)
{
	if (SceneManager::HasScene(sceneName)) // if the scene already exists
	{
		return false;
	}
	else
	{
		SceneManager::RegisterScene(sceneName); // registering the scene
		cherry::Scene* scene = SceneManager::Get(sceneName); // getting the scene
		skybox.AddSkyboxToScene(scene); // adds the skybox to the scene.

		ObjectManager::CreateSceneObjectList(sceneName); // creating an object list.
		LightManager::CreateSceneLightList(sceneName); // creating a light list.

		return true;
	}
}

// makes a scene
bool cherry::Game::CreateScene(const std::string sceneName, const bool makeCurrent)
{
	// creating a default skybox for the scene, since none was provided
	Skybox skybox(
		"res/images/cubemaps/checkerboard_black-grey_d.jpg",
		"res/images/cubemaps/checkerboard_black-grey_d.jpg",
		"res/images/cubemaps/checkerboard_black-grey_d.jpg",
		"res/images/cubemaps/checkerboard_black-grey_d.jpg",
		"res/images/cubemaps/checkerboard_black-grey_d.jpg",
		"res/images/cubemaps/checkerboard_black-grey_d.jpg"
	);

	return CreateScene(sceneName, skybox, makeCurrent);
}

// creates a scene, and returns 'true' if it was successful.
bool cherry::Game::CreateScene(const std::string sceneName, const cherry::Skybox skybox, const bool makeCurrent)
{
	if (SceneManager::HasScene(sceneName)) // if the scene already exists
	{
		return false;
	}
	else
	{
		SceneManager::RegisterScene(sceneName); // registering the scene
		cherry::Scene* scene = SceneManager::Get(sceneName); // getting the scene

		skybox.AddSkyboxToScene(scene); // adds the skybox to the scene.

		// creating an object list.
		ObjectManager::CreateSceneObjectList(sceneName);

		if (makeCurrent) // if it should be the current object list.
			objectList = ObjectManager::GetSceneObjectListByName(sceneName);

		// creating a light list.
		LightManager::CreateSceneLightList(sceneName);

		if (makeCurrent) // if it should be the current light list.
			lightList = LightManager::GetSceneLightListByName(sceneName);

		if (makeCurrent) // if the new scene should be the current scene.
		{
			SceneManager::SetCurrentScene(sceneName);
		}


		return true;
	}
}

// creates a scene
bool cherry::Game::RegisterScene(cherry::Scene* scene)
{
	if (scene == nullptr)
		return false;

	// if the scene already exists
	if (SceneManager::HasScene(scene->GetName()))
	{
		return false;
	}
	else
	{
		SceneManager::RegisterScene(scene); // registers the scene

		// adds a default skybox if one doesn't already exist.
		if (scene->SkyboxMesh == nullptr)
		{
			Skybox skybox(
				"res/images/cubemaps/checkerboard_black-grey_d.jpg",
				"res/images/cubemaps/checkerboard_black-grey_d.jpg",
				"res/images/cubemaps/checkerboard_black-grey_d.jpg",
				"res/images/cubemaps/checkerboard_black-grey_d.jpg",
				"res/images/cubemaps/checkerboard_black-grey_d.jpg",
				"res/images/cubemaps/checkerboard_black-grey_d.jpg"
			);

			skybox.AddSkyboxToScene(scene);
		}

		ObjectManager::CreateSceneObjectList(scene->GetName()); // creating an object list.
		LightManager::CreateSceneLightList(scene->GetName()); // creating a light list.

		return true;
	}
}

// creates the scene
bool cherry::Game::RegisterScene(cherry::Scene* scene, const cherry::Skybox skybox)
{
	// checking if the scene exists.
	if (scene == nullptr)
		return false;

	// if the scene already exists
	if (SceneManager::HasScene(scene->GetName()))
	{
		return false;
	}
	else
	{
		SceneManager::RegisterScene(scene); // registers the scene
		skybox.AddSkyboxToScene(scene);

		ObjectManager::CreateSceneObjectList(scene->GetName()); // creating an object list.
		LightManager::CreateSceneLightList(scene->GetName()); // creating a light list.

		return true;
	}
}

// creates the scene, and sees if it should be the current scene.
bool cherry::Game::RegisterScene(cherry::Scene* scene, const bool makeCurrent)
{
	if (scene == nullptr)
		return false;

	// if there is no skybox.
	if (scene->SkyboxMesh == nullptr)
	{
		Skybox skybox(
			"res/images/cubemaps/checkerboard_black-grey_d.jpg",
			"res/images/cubemaps/checkerboard_black-grey_d.jpg",
			"res/images/cubemaps/checkerboard_black-grey_d.jpg",
			"res/images/cubemaps/checkerboard_black-grey_d.jpg",
			"res/images/cubemaps/checkerboard_black-grey_d.jpg",
			"res/images/cubemaps/checkerboard_black-grey_d.jpg"
		);

		skybox.AddSkyboxToScene(scene);
	}

	// registers the scene
	SceneManager::RegisterScene(scene); // makes the scene

	ObjectManager::CreateSceneObjectList(scene->GetName());
	LightManager::CreateSceneLightList(scene->GetName());

	if (makeCurrent) // if this should be the current scene.
	{
		SetCurrentScene(scene->GetName(), false);
	}

	return true;
}

// creates the scene
bool cherry::Game::RegisterScene(cherry::Scene* scene, const cherry::Skybox skybox, const bool makeCurrent)
{
	if (scene == nullptr)
		return false;

	// the scene already exists
	if (SceneManager::HasScene(scene->GetName()))
	{
		return false;
	}
	else // making the new scene
	{
		skybox.AddSkyboxToScene(scene);

		SceneManager::RegisterScene(scene); // makes the scene

		ObjectManager::CreateSceneObjectList(scene->GetName());
		LightManager::CreateSceneLightList(scene->GetName());

		if (makeCurrent) // if this should be the current scene.
		{
			SetCurrentScene(scene->GetName(), false);
		}

		return true;
	}
}

// gets the requested scene via its name
cherry::Scene* cherry::Game::GetScene(std::string sceneName) const { return SceneManager::Get(sceneName); }

// gets the current scene.
cherry::Scene* cherry::Game::GetCurrentScene() const { return CurrentScene(); }

// sets the current scene
bool cherry::Game::SetCurrentScene(std::string sceneName, bool createScene)
{
	// saves the lists in case the scene switch fails.
	ObjectList* tempObjList = objectList;
	LightList* tempLgtList = lightList;

	if (SceneManager::HasScene(sceneName)) // sets the current scene.
	{
		// object manager
		if (ObjectManager::SceneObjectListExists(sceneName)) // if the scene object list exists
		{
			objectList = ObjectManager::GetSceneObjectListByName(sceneName);
		}
		else // it doesn't exist, so it should be made.
		{
			ObjectManager::CreateSceneObjectList(sceneName);
			objectList = ObjectManager::GetSceneObjectListByName(sceneName);
		}

		// light manager
		if (LightManager::SceneLightListExists(sceneName)) // if the light list exists
		{
			lightList = LightManager::GetSceneLightListByName(sceneName);
		}
		else // it doesn't exist, so it should be made.
		{
			LightManager::CreateSceneLightList(sceneName);
			lightList = LightManager::GetSceneLightListByName(sceneName);
		}

		if (SceneManager::SetCurrentScene(sceneName)) // if the scene switch was successful.
		{
			return true;
		}
		else // scene switch failed.
		{
			// swtiching the lists back
			objectList = tempObjList;
			lightList = tempLgtList;

			return false;
		}

	}
	else // scene doesn't exist
	{
		if (createScene) // new scene should be made.
		{
			return CreateScene(sceneName, true); // creates the scene, and makes it the current scene.
		}
		else // scene doesn't exist, and shouldn't be made.
		{
			return false;
		}

	}
}

// returns the name of the current scene.
const std::string& cherry::Game::GetCurrentSceneName() const
{
	Scene* scene = CurrentScene();

	if (scene != nullptr) // scene exists
		scene->GetName();
	else // scene doesn't exist
		return std::string("");
}

// destroys all scenes
void cherry::Game::DestroyScenes() { SceneManager::DestroyScenes(); }

// sets the skybox for the current scene.
void cherry::Game::SetSkybox(cherry::Skybox& skybox, const bool visible)
{
	auto scene = CurrentScene();

	if (scene != nullptr) // if the scene exists.
	{
		skybox.AddSkyboxToScene(scene);
		scene->SkyboxMesh->SetVisible(visible);
	}
}

// sets the skybox for a scene, provides a scene name, and sets if it should be visible.
void cherry::Game::SetSkybox(cherry::Skybox& skybox, const std::string sceneName, const bool visible)
{
	auto scene = SceneManager::Get(sceneName);

	if (scene != nullptr) // if the scene exists.
	{
		skybox.AddSkyboxToScene(scene);
		scene->SkyboxMesh->SetVisible(visible);
	}
}

// gets whether the skybox is visible for the current scene or not.
bool cherry::Game::GetSkyboxVisible() const
{
	auto scene = CurrentScene();

	if (scene != nullptr) // if the scene exists.
		return scene->SkyboxMesh->IsVisible();
}

// gets the visibility of the skybox in the scene.
bool cherry::Game::GetSkyboxVisible(std::string sceneName) const
{
	auto scene = SceneManager::Get(sceneName);

	if (scene != nullptr) // if the scene exists.
		return scene->SkyboxMesh->IsVisible();
}

// changes whether the skybox should be visible or not.
void cherry::Game::SetSkyboxVisible(bool skybox)
{
	auto scene = CurrentScene();

	if (scene != nullptr) // if the scene exists.
		return scene->SkyboxMesh->SetVisible(skybox);
}

// sets if the skybox is visible for the provided scene or not.
void cherry::Game::SetSkyboxVisible(bool skybox, std::string sceneName)
{
	auto scene = SceneManager::Get(sceneName);

	if (scene != nullptr) // if the scene exists.
		scene->SkyboxMesh->SetVisible(skybox);
}

// gets the total amount of sceneLists
unsigned int cherry::Game::GetObjectCount() const { return objectList->objects.size(); }

// gets the object list for the scene
cherry::ObjectList* cherry::Game::GetSceneObjectList() const { return objectList; }

// gets the object list for the provided scene.
cherry::ObjectList* cherry::Game::GetSceneObjectList(std::string scene) { return ObjectManager::GetSceneObjectListByName(scene); }

// gets an object from the current scene
cherry::Object* cherry::Game::GetCurrentSceneObjectByIndex(unsigned int index) const
{
	if (objectList != nullptr) // if there is an object list for this scene
	{
		return objectList->GetObjectByIndex(index);
	}
	else // if there isn't an object list for this scene
	{
		return GetSceneObjectByIndex(GetCurrentSceneName(), index);
	}
}

// gets an object from the provided scene
cherry::Object* cherry::Game::GetSceneObjectByIndex(std::string scene, unsigned int index) const
{
	// TODO: check for proper scene
	if (index > objectList->objects.size())
		return nullptr;
	else
		return ObjectManager::GetSceneObjectListByName(scene)->GetObjectByIndex(index);
}

// gets a scene object by name
cherry::Object* cherry::Game::GetCurrentSceneObjectByName(std::string name) const
{
	for (Object* obj : objectList->objects)
	{
		if (obj->GetName() == name && obj->GetSceneName() == GetCurrentSceneName())
			return obj;
	}
	return nullptr;
}

// gets a scene object from the provided scene, using its name
cherry::Object* cherry::Game::GetSceneObjectByName(std::string scene, std::string name) const
{
	return ObjectManager::GetSceneObjectListByName(scene)->GetObjectByName(name);
}

// adds an object to the m_Scene
bool cherry::Game::AddObjectToScene(cherry::Object* obj)
{
	return ObjectManager::AddObjectToSceneObjectList(obj);
}

// removes an object from the sceneLists vector.
bool cherry::Game::DeleteObjectFromScene(cherry::Object* obj)
{
	if (objectList != nullptr) // if the list hasn't been set, then it must be received.
	{
		return objectList->DeleteObjectByPointer(obj);
	}
	else
	{
		objectList = ObjectManager::GetSceneObjectListByName(GetCurrentSceneName());
		return ObjectManager::GetSceneObjectListByName(GetCurrentSceneName())->DeleteObjectByPointer(obj);
	}

}

// gets the light list for the current scene
cherry::LightList* cherry::Game::GetSceneLightList() const { return lightList; }

// gets the lights for a given scene.
cherry::LightList* cherry::Game::GetSceneLightList(std::string sceneName)
{
	return LightManager::GetSceneLightListByName(sceneName);
}

// adds the light to the scene it's part of.
bool cherry::Game::AddLightToScene(cherry::Light* light)
{
	return LightManager::AddLightToSceneLightList(light);
}

// deletes a light from the scene.
bool cherry::Game::DeleteLightFromScene(cherry::Light* light)
{
	if (lightList != nullptr) // if the list hasn't been set, then it must be received.
	{
		return lightList->DeleteLightByPointer(light);
	}
	else
	{
		lightList = LightManager::GetSceneLightListByName(GetCurrentSceneName());
		return LightManager::GetSceneLightListByName(GetCurrentSceneName())->DeleteLightByPointer(light);
	}
}

// returns the running game.
cherry::Game* const cherry::Game::GetRunningGame() { return runningGame; }


void cherry::Game::Initialize() {

	// Initialize GLFW
	if (glfwInit() == GLFW_FALSE) {
		std::cout << "Failed to initialize GLFW" << std::endl;
		throw std::runtime_error("Failed to initialize GLFW");
	}
	// Enable transparent backbuffers for our windows (note that Windows expects our colors to be pre-multiplied with alpha)
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, true);

	// Create a new GLFW window
	if (fullScreen) // if the window is to be put in full screen
		myWindow = glfwCreateWindow(myWindowSize.x, myWindowSize.y, myWindowTitle, glfwGetPrimaryMonitor(), nullptr);

	else // no full screen
		myWindow = glfwCreateWindow(myWindowSize.x, myWindowSize.y, myWindowTitle, nullptr, nullptr);

	// We want GL commands to be executed for our window, so we make our window's context the current one
	glfwMakeContextCurrent(myWindow);
	// Let glad know what function loader we are using (will call gl commands via glfw)
	if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0) {
		std::cout << "Failed to initialize Glad" << std::endl;
		throw std::runtime_error("Failed to initialize GLAD");
	}

	// setting up window user pointer so that we can resize our window
	// Tie our game to our window, so we can access it via callbacks
	glfwSetWindowUserPointer(myWindow, this);

	// Set our window resized callback
	glfwSetWindowSizeCallback(myWindow, GlfwWindowResizedCallback);


	// callback for cursor entering the window
	glfwSetCursorEnterCallback(myWindow, CursorEnterCallback);

	// Setting cursor position callback function
	glfwSetCursorPosCallback(myWindow, CursorPosCallback);

	// Setting mouse button callback function
	glfwSetMouseButtonCallback(myWindow, MouseButtonCallback);

	// Setting keyboard callback function
	glfwSetKeyCallback(myWindow, KeyCallback);

	// Log our renderer and OpenGL version
	LOG_INFO(glGetString(GL_RENDERER));
	LOG_INFO(glGetString(GL_VERSION));

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(GlDebugMessage, this);

	// used for sky boxes, which needs to be manually turned on.
	// without this, we end up getting seams in our textures.
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE); // TODO: uncomment when showcasing game.
	glEnable(GL_SCISSOR_TEST); // used for rendering multiple windows (TODO: maybe turn off if we aren't using multiple windows?)

	// seeding the randomizer
	srand(time(0));

	// initalizies the audio engine
	audioEngine.Init();
}

// called when the game is shutting down
void cherry::Game::Shutdown() {
	// if this was the running game.
	if (runningGame == this)
		runningGame = nullptr;

	glfwTerminate();
}

// loads the content for the meshes and shaders
void cherry::Game::LoadContent()
{
	std::string engineSceneName = "Cherry - Debug Scene";

	// setting up the camera
	myCamera = std::make_shared<Camera>();
	myCamera->clearColor = myClearColor; // setting the clear colour
	myCamera->SetPosition(glm::vec3(0, 5, 12));
	myCamera->LookAt(glm::vec3(0));


	myCamera->SetPerspectiveMode(glm::radians(45.0f), 1.0f, 0.01f, 1000.0f);

	// sets the orthographic mode values. False is passed so that the camera starts in perspective mode.
	myCamera->SetOrthographicMode(-5.0f, 5.0f, -5.0f, 5.0f, 0.0f, 100.0f, false);
	myCamera->targetOffset = cherry::Vec3(0, 5, 12);

	// secondary camera, which is used for UI for the game.
	myCameraX = std::make_shared<Camera>();
	myCameraX->clearColor = myClearColor;
	myCameraX->SetPosition(0, 0.001F, 1.0F); // try adjusting the position of the perspecitve cam and orthographic cam
	myCameraX->Rotate(glm::vec3(0.0F, 0.0F, glm::radians(180.0f)));
	myCameraX->LookAt(glm::vec3(0));

	// this camera is used for UI elements
	myCameraX->SetPerspectiveMode(glm::radians(60.0f), 1.0f, 0.01f, 1000.0f, false);
	myCameraX->SetOrthographicMode(-myWindowSize.x / 2.0F, myWindowSize.x / 2.0F, -myWindowSize.y / 2.0F, myWindowSize.y / 2.0F, 0.0f, 1000.0f, true);


	// creating the scene
	if (openingScene != nullptr) // if there is a startup scene.
	{
		RegisterScene(new EngineScene(engineSceneName), false);

		if (!RegisterScene(openingScene, true)) // initialize with startup scene.
			SetCurrentScene(engineSceneName, false);
	}
	else // if there is no startup scene.
	{
		RegisterScene(new EngineScene(engineSceneName), true);
	}
}

void cherry::Game::UnloadContent() {
	audioEngine.Shutdown(); // shutdown the audio component.
	SceneManager::DestroyScenes(); // destroys all the scenes.
}

void cherry::Game::Update(float deltaTime) {
	// calling the scene update
	Scene* scene = CurrentScene();
	if (scene != nullptr)
		scene->Update(deltaTime);

	// updating the cameras
	myCamera->Update(deltaTime);
	myCameraX->Update(deltaTime);

	// updating all the cameras.
	for (int i = 0; i < exCameras.size(); i++)
		exCameras[i]->Update(deltaTime);

	// updates the object list
	objectList->Update(deltaTime);

	// moved to the bottom of the update.
	// called to Update the position and rotation of the sceneLists.
	// calling all of our functions for our Update behaviours.
	auto view = CurrentRegistry().view<UpdateBehaviour>();
	for (const auto& e : view) {
		auto& func = CurrentRegistry().get<UpdateBehaviour>(e);
		if (func.Function) {
			func.Function(e, deltaTime);
		}
	}

	// TODO: determine why this crashes.
	// updates the audio engine 
	audioEngine.Update();
}

void cherry::Game::InitImGui() {
	// Creates a new ImGUI context
	ImGui::CreateContext();
	// Gets our ImGUI input/output
	ImGuiIO& io = ImGui::GetIO();
	// Enable keyboard navigation
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	// Allow docking to our window
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	// Allow multiple viewports (so we can drag ImGui off our window)
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	// Allow our viewports to use transparent backbuffers
	io.ConfigFlags |= ImGuiConfigFlags_TransparentBackbuffers;
	// Set up the ImGui implementation for OpenGL
	ImGui_ImplGlfw_InitForOpenGL(myWindow, true);
	ImGui_ImplOpenGL3_Init("#version 410");

	// Dark mode FTW
	ImGui::StyleColorsDark();
	// Get our imgui style
	ImGuiStyle& style = ImGui::GetStyle();
	//style.Alpha = 1.0f;
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 0.8f;
	}
}

void cherry::Game::ShutdownImGui() {
	// Cleanup the ImGui implementation
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	// Destroy our ImGui context
	ImGui::DestroyContext();
}

void cherry::Game::ImGuiNewFrame() {
	// Implementation new frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	// ImGui context new frame
	ImGui::NewFrame();
}

void cherry::Game::ImGuiEndFrame() {
	// Make sure ImGui knows how big our window is
	ImGuiIO& io = ImGui::GetIO();
	int width{ 0 }, height{ 0 };
	glfwGetWindowSize(myWindow, &width, &height);
	io.DisplaySize = ImVec2(width, height);
	// Render all of our ImGui elements
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	// If we have multiple viewports enabled (can drag into a new window)
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		// Update the windows that ImGui is using
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		// Restore our gl context
		glfwMakeContextCurrent(myWindow);
	}
}

// called to run the game.
void cherry::Game::Run()
{
	runningGame = this; // this is the running game.

	Initialize();
	InitImGui();
	LoadContent();

	static float prevFrame = glfwGetTime();
	static float frameTime = 0; // the amount of time since the last frame.

	// Run as long as the window is open
	while (!glfwWindowShouldClose(myWindow)) {
		// Poll for events from windows
		// clicks, key presses, closing, all that
		glfwPollEvents();
		float thisFrame = glfwGetTime(); // returns 'time' in seconds.
		float deltaTime = thisFrame - prevFrame;

		frameTime += deltaTime;

		// if there is no frame rate cap, or if enough time has passed.
		if (FPS == 0 || frameTime > 1.0F / ((float)FPS))
		{
			Update(deltaTime);
			Draw(deltaTime);

			if (imguiMode) // if 'true', then the imGui frame is shown.
			{
				ImGuiNewFrame();
				DrawGui(deltaTime);
				ImGuiEndFrame();
			}

			prevFrame = thisFrame;
			frameTime = 0; // resetting frame time.

			// Present our image to windows
			glfwSwapBuffers(myWindow);
		}
		else
		{
			// std::cout << "FPS: " << FPS << std::endl;
		}

	}

	UnloadContent(); // unload all content
	ShutdownImGui(); // shutdown imGui
	Shutdown();
}

// resizes the window without skewing the sceneLists, and changes the cameras accordingly.
void cherry::Game::Resize(int newWidth, int newHeight)
{
	// for some reason, calling the functions and having them be used directly didn't work.
	// so all the values are being saved first.
	glm::vec2 orthoSizePro{ (float)newWidth / myWindowSize.x ,(float)newHeight / myWindowSize.y };
	Scene* currScene = CurrentScene(); // gets the current scene.

	// perspective variables
	float p_fovy = myCamera->GetFieldOfView();
	float p_aspect = newWidth / (float)newHeight; // aspect ratio  
	float p_zNear = myCamera->GetNearPerspective(); // near plane (distance)
	float p_zFar = myCamera->GetFarPerspective(); // far plane (distance)

	// orthographic variables
	float o_left = myCamera->GetLeftOrthographic() * orthoSizePro.x;
	float o_right = myCamera->GetRightOrthographic() * orthoSizePro.x;
	float o_bottom = myCamera->GetBottomOrthographic() * orthoSizePro.y;
	float o_top = myCamera->GetTopOrthographic() * orthoSizePro.y;
	float o_zNear = myCamera->GetNearOrthographic();
	float o_zFar = myCamera->GetFarOrthographic();

	// changing the camera modes to adjust for the new window size. 
	// The camera mode isn't changed, just it's values (i.e. if it's in perspective mode, it stays in perspective mode).

	// resizing the camera's perspective mode and orthographic mode.
	myCamera->SetPerspectiveMode(p_fovy, p_aspect, p_zNear, p_zFar, myCamera->InPerspectiveMode());
	myCamera->SetOrthographicMode(o_left, o_right, o_bottom, o_top, o_zNear, o_zFar, myCamera->InOrthographicMode());

	// secondary camera settings
	// resizing the ui/hud camera (camera x)
	p_fovy = myCameraX->GetFieldOfView();
	// p_aspect = newWidth / (float)newHeight; // aspect ratio  
	p_zNear = myCameraX->GetNearPerspective(); // near plane (distance)
	p_zFar = myCameraX->GetFarPerspective(); // far plane (distance)

	// orthographic variables
	o_left = myCameraX->GetLeftOrthographic() * orthoSizePro.x;
	o_right = myCameraX->GetRightOrthographic() * orthoSizePro.x;
	o_bottom = myCameraX->GetBottomOrthographic() * orthoSizePro.y;
	o_top = myCameraX->GetTopOrthographic() * orthoSizePro.y;
	o_zNear = myCameraX->GetNearOrthographic();
	o_zFar = myCameraX->GetFarOrthographic();

	myCameraX->SetPerspectiveMode(p_fovy, p_aspect, p_zNear, p_zFar, myCameraX->InPerspectiveMode());
	myCameraX->SetOrthographicMode(o_left, o_right, o_bottom, o_top, o_zNear, o_zFar, myCameraX->InOrthographicMode());

	// updating the extra cameras
	for (Camera::Sptr cam : exCameras)
	{
		p_fovy = cam->GetFieldOfView();
		p_zNear = cam->GetNearPerspective(); // near plane (distance)
		p_zFar = cam->GetFarPerspective(); // far plane (distance)

		// orthographic variables
		o_left = cam->GetLeftOrthographic() * orthoSizePro.x;
		o_right = cam->GetRightOrthographic() * orthoSizePro.x;
		o_bottom = cam->GetBottomOrthographic() * orthoSizePro.y;
		o_top = cam->GetTopOrthographic() * orthoSizePro.y;
		o_zNear = cam->GetNearOrthographic();
		o_zFar = cam->GetFarOrthographic();

		// updating the extra cameras
		cam->SetPerspectiveMode(p_fovy, p_aspect, p_zNear, p_zFar, cam->InPerspectiveMode());
		cam->SetOrthographicMode(o_left, o_right, o_bottom, o_top, o_zNear, o_zFar, cam->InOrthographicMode());
	}

	// resizes the layers
	if (currScene != nullptr)
	{
		// if the frame buffer is being used, then it is resized.
		if (currScene->IsUsingFrameBuffers())
		{
			FrameBuffer::Sptr& fb = CurrentRegistry().ctx<FrameBuffer::Sptr>();
			fb->Resize(newWidth, newHeight);

			// gets the layers
			std::vector<cherry::PostLayer*> layers = currScene->GetPostLayers();

			for (PostLayer* layer : layers)
				layer->OnWindowResize(newWidth, newHeight);
		}
	}

	// resizing the objects in screen space.
	if (objectList != nullptr)
		objectList->OnWindowResize(newWidth, newHeight);

	// saving the new window size.
	myWindowSize = { newWidth, newHeight }; // updating window size
}

// draws to a given viewpoint. The code that was originally here was moved to _RenderScene
void cherry::Game::Draw(float deltaTime) {
	// if the camera exists
	if (myCameraEnabled)
	{
		// viewport size (full screen)
		myCamera->viewport = glm::ivec4{
		0, 0,
		myWindowSize.x, myWindowSize.y
		};

		myCamera->clearColor = myClearColor;

		// renders the scene
		__RenderScene(myCamera);
	}
 
	// renders all the other cameras
	for (Camera::Sptr cam : exCameras)
		__RenderScene(cam);
}

// drawing ImGui
void cherry::Game::DrawGui(float deltaTime) {
	Scene* scene = CurrentScene();

	// now calls a scene for ImGui
	if (scene != nullptr)
		scene->DrawGui(deltaTime);

	// // Open a new ImGui window
	// ImGui::Begin("Colour Picker");
	// 
	// // Draw widgets here
	// // ImGui::SliderFloat4("Color", &myClearColor.x, 0, 1); // Original
	// ImGui::ColorPicker4("Color", &myClearColor.x); // new version
	// // ImGui::SetWindowSize(ImVec2(500.0F, 500.0F)); // window size for ImGUI Colour Picker (perament)
	// // ImGui::SetNextWindowCollapsed(false);
	// // ImGui::SetNextWindowPos(ImVec2(-225.0F, 1.0F));
	// ImGui::SetNextWindowSize(ImVec2(500.0F, 500.0F)); // window size for ImGUI ColorPicker (variable)
	// if (ImGui::InputText("Title", myWindowTitle, 31))
	// {
	// 	glfwSetWindowTitle(myWindow, myWindowTitle);
	// }
	// 
	// if (ImGui::Button("Apply")) // adding another button, which allows for the application of the window title.
	// {
	// 	glfwSetWindowTitle(myWindow, myWindowTitle);
	// }
	// if (ImGui::Button("Wireframe/Fill Toggle"))
	// {
	// 	for (cherry::Object* obj : objectList->objects)
	// 		obj->SetWireframeMode();
	// }
	// 
	// // changing the camera mode
	// std::string camMode = myCamera->InPerspectiveMode() ? "Perspective" : "Orthographic";
	// ImGui::InputText((std::string("CAMERA MODE (\'SPACE\')") + camMode).c_str(), myWindowTitle, WINDOW_TITLE_CHAR_MAX);
	// 
	// ImGui::End();
}

// renders the scene
void cherry::Game::__RenderScene(const Camera::Sptr& camera)
{
	__RenderScene(camera->viewport, camera, camera->drawSkybox, camera->borderSize, camera->borderColor, camera->clearBuffers);
}

// Now handles rendering the scene.
void cherry::Game::__RenderScene(glm::ivec4 viewport, const Camera::Sptr& camera, bool drawSkybox, int borderSize, glm::vec4 borderColor, bool clear)
{
	// frame buffer for the renderer
	FrameBuffer::Sptr fb; // frame buffer for the game.
	Scene* scene = CurrentScene(); // gets the current scene
	
	bool usingFrameBuffers = false; // if 'true', the frame buffer is being used.
	std::vector<PostLayer*> layers;

	// vector for post-post-process renders
	std::vector<MeshRenderer> postRenders;
	// vector for post-post-process transform
	std::vector<TempTransform> postTransforms;

	// no scene has been set to run.
	if (scene == nullptr)
		std::runtime_error("No scene is currently running.");

	// gets whether to use frame buffers or not, as well as the post layers.
	usingFrameBuffers = scene->IsUsingFrameBuffers();
	layers = scene->GetPostLayers();
	
	// if the frame buffers are being used, and they have values.
	if (usingFrameBuffers && !layers.empty())
	{
		fb = CurrentRegistry().ctx<FrameBuffer::Sptr>();
		fb->Bind();
		usingFrameBuffers = true;
	}

	// Set viewport to entire region
	// glViewport(viewport.x, viewport.y, viewport.z, viewport.w); // not neded since viewpoint doesn't change the clear call.
	glScissor(viewport.x, viewport.y, viewport.z, viewport.w);

	// Clear with the border color
	glClearColor(borderColor.x, borderColor.y, borderColor.z, borderColor.w);
	if (clear)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// Set viewport to be inset slightly (the amount is the border width)
	// the offsets are used to move the border relative to the viewpoint.
	glViewport(viewport.x + borderSize, viewport.y + borderSize, viewport.z - 2 * borderSize, viewport.w - 2 * borderSize);
	glScissor(viewport.x + borderSize, viewport.y + borderSize, viewport.z - 2 * borderSize, viewport.w - 2 * borderSize);

	// TODO: set border colour, border size, and clear colour in cameras instead. 
	// Clear our new inset area with the scene clear color
	glClearColor(myClearColor.x, myClearColor.y, myClearColor.z, myClearColor.w);
	if (clear)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// no longer needed?
	// myShader->Bind();

	// SKYBOX //
	// Draw the skybox after everything else, if the scene has one
	if (scene->Skybox)
	{
		// Disable culling
		glDisable(GL_CULL_FACE); // we disable face culling if the cube map is screwed up.
		// Set our depth test to less or equal (because we are at 1.0f)
		glDepthFunc(GL_LEQUAL);
		// Disable depth writing
		glDepthMask(GL_FALSE);

		// Make sure no samplers are bound to slot 0
		TextureSampler::Unbind(0);
		// Set up the shader
		scene->SkyboxShader->Bind();

		// casting the mat4 down to a mat3, then putting it back into a mat4, which is done to remove the camera's translation.
		scene->SkyboxShader->SetUniform("a_View", glm::mat4(glm::mat3(
			camera->GetView()
		)));
		scene->SkyboxShader->SetUniform("a_Projection", camera->Projection);

		scene->Skybox->Bind(0);
		scene->SkyboxShader->SetUniform("s_Skybox", 0); // binds our skybox to slot 0.

		// draws the skybox if it is to be visible.
		if (scene->SkyboxMesh->IsVisible())
			scene->SkyboxMesh->Draw();

		// Restore our state
		glDepthMask(GL_TRUE);
		glEnable(GL_CULL_FACE);
		glDepthFunc(GL_LESS);
	}

	// We'll grab a reference to the ecs to make things easier
	auto& ecs = CurrentRegistry();

	// copy past mesh renderer component and make ui rendere component?
	ecs.sort<MeshRenderer>([&](const MeshRenderer& lhs, const MeshRenderer& rhs) {
		if (rhs.Material == nullptr || rhs.Mesh == nullptr)
			return false;
		else if (lhs.Material == nullptr || lhs.Mesh == nullptr)
			return true;
		else if (lhs.Material->HasTransparency & !rhs.Material->HasTransparency) //
			return false; // This section is new
		else if (!lhs.Material->HasTransparency & rhs.Material->HasTransparency) // The order IS important
			return true; //
		else if (lhs.Material->GetShader() != rhs.Material->GetShader())
			return lhs.Material->GetShader() < rhs.Material->GetShader();
		else
			return lhs.Material < rhs.Material;
		});


	// These will keep track of the current shader and material that we have bound
	Material::Sptr mat = nullptr;
	Shader::Sptr boundShader = nullptr;
	// A view will let us iterate over all of our entities that have the given component types
	auto view = ecs.view<MeshRenderer>();

	for (const auto& entity : view) {
		// Get our shader 
		const MeshRenderer& renderer = ecs.get<MeshRenderer>(entity);
		// Early bail if mesh is invalid
		if (renderer.Mesh == nullptr || renderer.Material == nullptr)
			continue;

		// if the mesh shouldn't be post processed, it's drawn after the post processed meshes.
		if (!renderer.Mesh->postProcess)
		{
			postRenders.push_back(renderer); // gets the renderer
			postTransforms.push_back(ecs.get_or_assign<TempTransform>(entity)); // getting the transformation
			continue;
		}

		// If our shader has changed, we need to bind it and Update our frame-level uniforms
		if (renderer.Material->GetShader() != boundShader) {
			boundShader = renderer.Material->GetShader();
			boundShader->Bind();

			// if the object is to have a fixed screen position.
			if (renderer.Mesh->GetWindowChild())
				boundShader->SetUniform("a_CameraPos", myCameraX->GetPosition()); // uses Hud/UI camera
			else
				boundShader->SetUniform("a_CameraPos", camera->GetPosition()); // uses provided camera position.

			boundShader->SetUniform("a_Time", static_cast<float>(glfwGetTime())); // passing in the time.
		}
		// If our material has changed, we need to apply it to the shader
		if (renderer.Material != mat) {
			mat = renderer.Material;
			mat->Apply();
		}

		// We'll need some info about the entities position in the world
		const TempTransform& transform = ecs.get_or_assign<TempTransform>(entity);

		// Get the object's transformation
		// TODO: set up parent system
		glm::mat4 worldTransform = transform.GetWorldTransform();

		// Our normal matrix is the inverse-transpose of our object's world rotation
		// Recall that everything's backwards in GLM
		glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(worldTransform)));

		// Update the MVP using the item's transform
		if (renderer.Mesh->GetWindowChild())
		{
			mat->GetShader()->SetUniform("a_ModelViewProjection", myCameraX->GetViewProjection() * worldTransform);
		}
		else
		{
			mat->GetShader()->SetUniform("a_ModelViewProjection", camera->GetViewProjection() * worldTransform);
		}


		// Update the model matrix to the item's world transform
		mat->GetShader()->SetUniform("a_Model", worldTransform);
		// Update the model matrix to the item's world transform
		mat->GetShader()->SetUniform("a_NormalMatrix", normalMatrix);

		// Draw the item
		if (renderer.Mesh->IsVisible())
		{
			// if the mesh is in wireframe mode, and the draw call isn't set to that already.
			if (renderer.Mesh->IsWireframe() != wireframe)
			{
				wireframe = !wireframe;

				// switches between wireframe mode and fill mode.
				(wireframe) ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}

			// if the mesh should be drawn in a different mode from what is currently set.
			// if ((renderer.Mesh->IsPerspectiveMesh() && !camera->InPerspectiveMode()) ^
			// 	renderer.Mesh->IsOrthographicMesh() && !camera->InOrthographicMode())
			// {
			// 	camera->SwitchViewMode();
			// }

			// the faces should or should not be culled. Since faces should be culled by default, it's turned back on.
			if (!renderer.Mesh->cullFaces)
			{
				glDisable(GL_CULL_FACE);
				renderer.Mesh->Draw();
				glEnable(GL_CULL_FACE);
			}
			else // faces are set to be culled automatically
			{
				renderer.Mesh->Draw();
			}
		}
		else
		{
			// std::cout << "INVISIBLE" << std::endl;
		}
	}

	// post-processing layers
	if (usingFrameBuffers && !layers.empty())
	{
		fb->UnBind();
		FrameBuffer::Sptr& fbx = fb; // the most recent buffer

		// applies each layer
		for (PostLayer* layer : layers)
		{
			// in order to use multiple layers (each with their own post passes), the last buffer used is saved in fbx.
			// fbx then gives it to the new layer so that the image can continue to be changed.

			// layer->initialBuffer = fbx; // setting it to use the most recent buffer
			layer->PostRender(camera);
			fbx = layer->GetLastPassBuffer(); // saves the last frame buffer
			
		}
		
	}

	// making the bound shader null for the post draw.
	boundShader = nullptr;

	// post-post processing renders
	for (int i = 0; i < postRenders.size(); i++) {
		// Early bail if mesh is invalid
		const MeshRenderer& renderer = postRenders[i]; // getting the post render
		if (renderer.Mesh == nullptr || renderer.Material == nullptr)
			continue;

		// If our shader has changed, we need to bind it and Update our frame-level uniforms
		if (renderer.Material->GetShader() != boundShader) {
			boundShader = renderer.Material->GetShader();
			boundShader->Bind();

			// if the object is to have a fixed screen position.
			if (renderer.Mesh->GetWindowChild())
				boundShader->SetUniform("a_CameraPos", myCameraX->GetPosition()); // uses Hud/UI camera
			else
				boundShader->SetUniform("a_CameraPos", camera->GetPosition()); // uses provided camera position.

			boundShader->SetUniform("a_Time", static_cast<float>(glfwGetTime())); // passing in the time.
		}
		// If our material has changed, we need to apply it to the shader
		if (renderer.Material != mat) {
			mat = renderer.Material;
			mat->Apply();
		}

		// We'll need some info about the entities position in the world
		const TempTransform& transform = postTransforms[i];

		// Get the object's transformation
		// TODO: set up parent system
		glm::mat4 worldTransform = transform.GetWorldTransform();

		// Our normal matrix is the inverse-transpose of our object's world rotation
		// Recall that everything's backwards in GLM
		glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(worldTransform)));

		// Update the MVP using the item's transform
		if (renderer.Mesh->GetWindowChild())
		{
			mat->GetShader()->SetUniform("a_ModelViewProjection", myCameraX->GetViewProjection() * worldTransform);
		}
		else
		{
			mat->GetShader()->SetUniform("a_ModelViewProjection", camera->GetViewProjection() * worldTransform);
		}

		// Update the model matrix to the item's world transform
		mat->GetShader()->SetUniform("a_Model", worldTransform);
		// Update the model matrix to the item's world transform
		mat->GetShader()->SetUniform("a_NormalMatrix", normalMatrix);

		// Draw the item
		if (renderer.Mesh->IsVisible())
		{
			// if the mesh is in wireframe mode, and the draw call isn't set to that already.
			if (renderer.Mesh->IsWireframe() != wireframe)
			{
				wireframe = !wireframe;

				// switches between wireframe mode and fill mode.
				(wireframe) ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}

			// the faces should or should not be culled. Since faces should be culled by default, it's turned back on.
			if (!renderer.Mesh->cullFaces)
			{
				glDisable(GL_CULL_FACE);
				renderer.Mesh->Draw();
				glEnable(GL_CULL_FACE);
			}
			else // faces are set to be culled automatically 
			{
				renderer.Mesh->Draw(); 
			}
		}
	}
	
}