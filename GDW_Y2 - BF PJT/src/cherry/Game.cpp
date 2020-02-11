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
#include "Texture2D.h"

#include "physics/PhysicsBody.h"
#include "utils/Utils.h"
#include "WorldTransform.h"

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
cherry::Game::Game(const char windowTitle[32], float _width, float _height, bool _fullScreen, bool _defaults, bool _debug, bool _imgui) 
	: Game()
{
	// setting the values
	memcpy(myWindowTitle, windowTitle, strlen(windowTitle) + 1);
	myWindowSize = glm::ivec2(_width, _height);
	fullScreen = _fullScreen;
	loadDefaults = _defaults; // loads the engine default values
	debugMode = _debug; // debug functionality.
	imguiMode = _imgui;
}

// destructor
cherry::Game::~Game() { }

// gets the window width
float cherry::Game::GetWindowWidth() const { return myWindowSize.x; }

// gets the window height
float cherry::Game::GetWindowHeight() const { return myWindowSize.y; }

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

	mousePos = cherry::Vec2(xpos - this->myWindowSize.x / 2.0F, ypos - this->myWindowSize.y / 2.0F);

	//this->XcursorPos = xpos;
	//this->YcursorPos = ypos;
	// xpos = xpos - this->windowWidth / 2.0F;
	// ypos = ypos - this->windowHeight / 2.0F;

	//Logger::GetLogger()->info(xpos);
	//Logger::GetLogger()->info(ypos);

}

// gets the cursor position
cherry::Vec2 cherry::Game::GetCursorPos() const { return mousePos; }

// gets the cursor position as a glm vector
glm::vec2 cherry::Game::GetCursorPosGLM() const { return glm::vec2(mousePos.v.x, mousePos.v.y); }

float cherry::Game::GetCursorPosX() const {	return mousePos.v.x; }

// returns the cursor position on the y-axis
float cherry::Game::GetCursorPosY() const { return mousePos.v.y; }

// called when a mouse button has been pressed
void cherry::Game::MouseButtonPressed(GLFWwindow* window, int button) {
	Game* game = (Game*)glfwGetWindowUserPointer(window);

	if (game == nullptr) // if game is 'null', then nothing happens
		return;

	// checks each button
	switch (button) {
	case GLFW_MOUSE_BUTTON_LEFT:
		mbLeft = true;
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		mbMiddle = true;
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		mbRight = true;
		break;
	}
}

// called when a mouse button is being held
void cherry::Game::MouseButtonHeld(GLFWwindow* window, int button) {
	Game* game = (Game*)glfwGetWindowUserPointer(window);

	if (game == nullptr) // if game is 'null', then nothing happens
		return;

	// checks each button
	switch (button) {
	case GLFW_MOUSE_BUTTON_LEFT:
		mbLeft = true;
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		mbMiddle = true;
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		mbRight = true;
		break;
	}
}

// called when a mouse button has been released
void cherry::Game::MouseButtonReleased(GLFWwindow* window, int button) {
	Game* game = (Game*)glfwGetWindowUserPointer(window);

	if (game == nullptr) // if game is 'null', then nothing happens
		return;

	// checks each button
	switch (button) {
	case GLFW_MOUSE_BUTTON_LEFT:
		mbLeft = true;
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		mbMiddle = true;
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		mbRight = true;
		break;
	}
}

// key has been pressed
void cherry::Game::KeyPressed(GLFWwindow* window, int key)
{
	Game* game = (Game*)glfwGetWindowUserPointer(window);

	if (game == nullptr) // if game is 'null', then nothing happens
		return;

	// checks key value.
	switch (key)
	{
	case GLFW_KEY_SPACE:
		myCamera->SwitchViewMode();
		break;
	case GLFW_KEY_W:
		w = true;
		break;
	case GLFW_KEY_S:
		s = true;
		break;
	case GLFW_KEY_A:
		a = true;
		break;
	case GLFW_KEY_D:
		d = true;
		break;
	case GLFW_KEY_V:
		if (hitBoxIndex >= 0 && hitBoxIndex < objectList->objects.size())
			objectList->objects[hitBoxIndex]->GetPhysicsBodies()[0]->SetVisible();
		break;
	case GLFW_KEY_P:
		if (hitBoxIndex >= 0 && hitBoxIndex < objectList->objects.size())
			objectList->objects[hitBoxIndex]->followPath = !objectList->objects[hitBoxIndex]->followPath;
	case GLFW_KEY_I:
		if (hitBoxIndex >= 0 && hitBoxIndex < objectList->objects.size())
		{
			if (objectList->objects[hitBoxIndex]->GetPath().GetInterpolationMode() == 0)
			{
				objectList->objects[hitBoxIndex]->GetPath().SetInterpolationMode(1);
			}
			else if (objectList->objects[hitBoxIndex]->GetPath().GetInterpolationMode() == 1)
			{
				objectList->objects[hitBoxIndex]->GetPath().SetInterpolationMode(0);
			}
			
		}
		break;
	}
}

// key is being held
void cherry::Game::KeyHeld(GLFWwindow* window, int key)
{
	Game* game = (Game*)glfwGetWindowUserPointer(window);

	if (game == nullptr) // if game is 'null', then it is returned
		return;

	switch (key)
	{
	case GLFW_KEY_W:
		w = true;
		break;
	case GLFW_KEY_S:
		s = true;
		break;
	case GLFW_KEY_A:
		a = true;
		break;
	case GLFW_KEY_D:
		d = true;
		break;
	}

}

// key hs been released
void cherry::Game::KeyReleased(GLFWwindow* window, int key)
{
	Game* game = (Game*)glfwGetWindowUserPointer(window);

	if (game == nullptr) // if game is 'null', then it is returned
		return;

	switch (key)
	{
	case GLFW_KEY_W:
		w = false;
		break;
	case GLFW_KEY_S:
		s = false;
		break;
	case GLFW_KEY_A:
		a = false;
		break;
	case GLFW_KEY_D:
		d = false;
		break;
	case GLFW_KEY_0:
		DeleteObjectFromScene(objectList->objects.at(0));
		break;
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
		cherry::Scene * scene = SceneManager::Get(sceneName); // getting the scene

		skybox.AddSkyboxToScene(scene); // adds the skybox to the scene.

		// creating an object list.
		ObjectManager::CreateSceneObjectList(sceneName);
		
		if(makeCurrent) // if it should be the current object list.
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

// gets the requested scene via its name
cherry::Scene* cherry::Game::GetScene(std::string sceneName) const { return SceneManager::Get(sceneName); }

// gets the current scene.
cherry::Scene* cherry::Game::GetCurrentScene() const { return CurrentScene(); }

// sets the current scene
bool cherry::Game::SetCurrentScene(std::string sceneName, bool createScene) 
{
	if (SceneManager::HasScene(sceneName)) // sets the current scene.
	{
		if (SceneManager::SetCurrentScene(sceneName)) // if the scene switch was successful.
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

			return true;
		}
		else // scene switch failed
		{
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
const std::string & cherry::Game::GetCurrentSceneName() const
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

	if(scene != nullptr) // if the scene exists.
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


// adds an object to the m_Scene.
//bool cherry::Game::AddObjectToCurrentScene(cherry::Object* obj, std::string scene)
//{
//	// adds the object to the list of sceneLists.
//	bool added = util::addToVector(objectList->objects, obj);
//
//	if (added) // if the object was added, then an entity is created.
//		obj->CreateEntity(scene, matStatic);
//
//	return added; // returns 
//}

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


void cherry::Game::Initialize() {

	// Initialize GLFW
	if (glfwInit() == GLFW_FALSE) {
		std::cout << "Failed to initialize GLFW" << std::endl;
		throw std::runtime_error("Failed to initialize GLFW");
	}
	// Enable transparent backbuffers for our windows (note that Windows expects our colors to be pre-multiplied with alpha)
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, true);

	// Create a new GLFW window
	if(fullScreen) // if the window is to be put in full screen
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

void cherry::Game::Shutdown() {
	glfwTerminate();
}

// loads the content for the meshes and shaders
void cherry::Game::LoadContent()
{
	// setting up the camera
	myCamera = std::make_shared<Camera>();
	myCamera->SetPosition(glm::vec3(0, 5, 12));
	myCamera->LookAt(glm::vec3(0));

	// sets the camera to perspective mode for the m_Scene.
	// myCamera->SetPerspectiveMode(glm::perspective(glm::radians(60.0f), 1.0f, 0.01f, 1000.0f));
	//myCamera->SetPerspectiveMode(glm::perspective(glm::radians(60.0f), 1.0f, 0.01f, 1000.0f));
	myCamera->SetPerspectiveMode(glm::radians(45.0f), 1.0f, 0.01f, 1000.0f);
	// myCamera->SetPerspectiveMode(glm::perspective(glm::radians(10.0f), 1.0f, 0.01f, 1000.0f));

	// sets the orthographic mode values. False is passed so that the camera starts in perspective mode.
	myCamera->SetOrthographicMode(-5.0f, 5.0f, -5.0f, 5.0f, 0.0f, 100.0f, false);
	// myCamera->followTarget = true;
	// myCamera->fixedTargetDistance = true;
	myCamera->targetOffset = cherry::Vec3(0, 5, 12);

	// secondary camera, which is used for UI for the game.
	myCameraX = std::make_shared<Camera>();
	myCameraX->SetPosition(0, 0.001F, 1.0F); // try adjusting the position of the perspecitve cam and orthographic cam
	myCameraX->Rotate(glm::vec3(0.0F, 0.0F, glm::radians(180.0f)));
	myCameraX->LookAt(glm::vec3(0));
	
	// this camera is used for UI elements
	myCameraX->SetPerspectiveMode(glm::radians(60.0f), 1.0f, 0.01f, 1000.0f, false);
	myCameraX->SetOrthographicMode(-50.0f, 50.0f, -50.0f, 50.0f, 0.0f, 1000.0f, true);

	// creating the object manager and light manager
	// objManager = std::make_shared<ObjectManager>();
	// lightManager = std::make_shared<LightManager>();


	// SAMPLER FOR MIP MAPPING
	// added for mip mapping. As long as its above the material, it's fine.
	
	// OLD VERSION
	// TODO: remove upon final submission
	// description = SamplerDesc();
	// description.MinFilter = MinFilter::NearestMipNearest;

	// description.MagFilter = MagFilter::Nearest;
	// sampler = std::make_shared<TextureSampler>(description);

	// added for mip mapping. As long as its above the material, it's fine.
	description = SamplerDesc();
	description.MinFilter = MinFilter::LinearMipNearest;
	description.MagFilter = MagFilter::Linear;
	description.WrapS = description.WrapT = WrapMode::Repeat;

	// TODO: make linear and NearestMipNearest different variables?
	// called 'Linear' in the original code
	sampler = std::make_shared<TextureSampler>(description);

	// TODO: remove upon submission
	//desc1 = SamplerDesc();
	//desc1.MinFilter = MinFilter::NearestMipNearest;
	//desc1.MagFilter = MagFilter::Nearest;

	//desc2 = SamplerDesc();
	//desc2.MinFilter = MinFilter::LinearMipLinear;
	//desc2.MagFilter = MagFilter::Linear;

	//samplerEX = std::make_shared<TextureSampler>(desc1);
	


	// before the mesh in the original code
	Shader::Sptr phong = std::make_shared<Shader>();
	// TODO: make version without UVs?
	phong->Load("res/lighting.vs.glsl", "res/blinn-phong.fs.glsl");

	// TODO: change this so that it uses the light manager.
	// used to make the albedo
	// dedicated variable no longer needed?
	
	// no longer needed since GenerateMaterial() exists.
	// matStatic = std::make_shared<Material>(phong);
	// matStatic->Set("a_LightCount", 1);
	// matStatic->Set("a_LightPos[0]", { 0, 0, 3 });
	// matStatic->Set("a_LightColor[0]", { 0.5f, 0.1f, 0.9f});
	// matStatic->Set("a_AmbientColor[0]", { 0.9f, 0.1f, 0.01f });
	// matStatic->Set("a_AmbientPower[0]", 0.4f); // change this to change the main lighting power (originally value of 0.1F)
	// matStatic->Set("a_LightSpecPower[0]", 0.5f);
	// matStatic->Set("a_LightShininess[0]", 256.0f); // MUST be a float
	// matStatic->Set("a_LightAttenuation[0]", 0.15f);
	// material->Set("s_Albedo", albedo, sampler); // sceneLists will just be blank if no texture is set.
	
	// testMat->Set("s_Albedo", albedo); // right now, this is using the texture state.

		// Shader was originally compiled here.
	// // Create and compile shader
	// myShader = std::make_shared<Shader>();
	// myShader->Load("res/shader.vs.glsl", "res/shader.fs.glsl");
	// 
	// myModelTransform = glm::mat4(1.0f); // initializing the model matrix
	// testMat->Set("s_Albedo", albedo, Linear); // now uses mip mapping

	// No longer needed since GenerateMaterial exists.
	// Texture2D::Sptr albedo = Texture2D::LoadFromFile("res/images/default.png");
	// matStatic->Set("s_Albedos[0]", albedo, sampler);
	// matStatic->Set("s_Albedos[1]", albedo, sampler);
	// matStatic->Set("s_Albedos[2]", albedo,sampler);
	
	
	

	// creating a skybox for the scene.
	Skybox skybox(
		"res/images/cubemaps/checkerboard_black-red.jpg",
		"res/images/cubemaps/checkerboard_black-green.jpg",
		"res/images/cubemaps/checkerboard_black-blue.jpg",
		"res/images/cubemaps/checkerboard_red-white.jpg",
		"res/images/cubemaps/checkerboard_green-white.jpg",
		"res/images/cubemaps/checkerboard_blue-white.jpg"
	);

	CreateScene("Cherry", skybox, true); // creates the scene
	GetCurrentScene()->SkyboxMesh->SetVisible(true); // makes the skybox invisible

	ObjectManager::CreateSceneObjectList(GetCurrentSceneName()); // creating an object list for the scene
	objectList = ObjectManager::GetSceneObjectListByName(GetCurrentSceneName()); // getting the object list.

	// TODO: add sampler for light list?
	LightManager::CreateSceneLightList(GetCurrentSceneName());
	lightList = LightManager::GetSceneLightListByName(GetCurrentSceneName()); // getting the light list

	lightList->AddLight(new Light(GetCurrentSceneName(), Vec3(-7.0F, 0.0F, 0.0F), Vec3(1.0F, 0.1F, 0.1F),
		Vec3(0.1F, 1.0F, 0.4F), 0.4F, 0.2F, 250.0F, 0.15F));

	lightList->AddLight(new Light(GetCurrentSceneName(), Vec3(7.0F, 0.0F, 0.0F), Vec3(0.1, 0.1F, 1.0F),
		Vec3(0.2F, 0.7F, 0.9F), 0.3F, 0.5F, 256.0F, 0.15F)); 

	AddLightToScene(new Light(GetCurrentSceneName(), Vec3(0.0F, 7.0F, 0.0F), Vec3(0.3, 0.9F, 0.1F),
		Vec3(0.8F, 0.2F, 0.95F), 0.9F, 0.7F, 100.0F, 0.85F));

	// material = LightManager::GetLightList(currentScene)->at(1).GenerateMaterial(sampler);
	// replace the shader for the material if using morph tagets.
	matStatic = lightList->GenerateMaterial(STATIC_VS, STATIC_FS, sampler);
	matDynamic = lightList->GenerateMaterial(DYNAMIC_VS, DYNAMIC_FS, sampler);
	 
	// loads in default sceneLists
	if (loadDefaults)
	{
		Material::Sptr objMat; // used for custom materials
		float offset = 3.0F; // position offset

		  //sceneLists.push_back(new PrimitiveCube(5));
		  //sceneLists.at(sceneLists.size() - 1)->CreateEntity(currentScene, matStatic);
		  //sceneLists.at(sceneLists.size() - 1)->SetPosition(0.0F, 0.0F, 0.0F);

		// Creating the sceneLists, storing them, and making them part of the default m_Scene.
		 objectList->objects.push_back(new PrimitiveCapsule());
		 objectList->objects.at(objectList->objects.size() - 1)->CreateEntity(GetCurrentSceneName(), matStatic);
		 objectList->objects.at(objectList->objects.size() - 1)->SetPosition(-offset, -offset, 0.0F);
		 
		 
		 
		 objectList->objects.push_back(new PrimitiveCircle());
		 objectList->objects.at(objectList->objects.size() - 1)->CreateEntity(GetCurrentSceneName(), matStatic);
		 objectList->objects.at(objectList->objects.size() - 1)->SetPosition(-offset, 0.0f, 0.0F);
		  
		 objectList->objects.push_back(new PrimitiveCone());
		 objectList->objects.at(objectList->objects.size() - 1)->CreateEntity(GetCurrentSceneName(), matStatic);
		 objectList->objects.at(objectList->objects.size() - 1)->SetPosition(-offset, offset, 0.0F);
		 
		 objectList->objects.push_back(new PrimitiveCube());
		 objectList->objects.at(objectList->objects.size() - 1)->CreateEntity(GetCurrentSceneName(), matStatic);
		 objectList->objects.at(objectList->objects.size() - 1)->SetPosition(0.0F, -offset, 0.0F);
		 
		 objectList->objects.push_back(new PrimitiveCylinder());
		 objectList->objects.at(objectList->objects.size() - 1)->CreateEntity(GetCurrentSceneName(), matStatic);
		 objectList->objects.at(objectList->objects.size() - 1)->SetPosition(0.0F, 0.0F, 0.0F);
		 
		 objectList->objects.push_back(new PrimitiveDiamond());
		 objectList->objects.at(objectList->objects.size() - 1)->CreateEntity(GetCurrentSceneName(), matStatic);
		 objectList->objects.at(objectList->objects.size() - 1)->SetPosition(0.0F, offset, 0.0F);
		 
		 objectList->objects.push_back(new PrimitiveUVSphere());
		 objectList->objects.at(objectList->objects.size() - 1)->CreateEntity(GetCurrentSceneName(), matStatic);
		 objectList->objects.at(objectList->objects.size() - 1)->SetPosition(offset, -offset, 0.0F);
		 
		 objectList->objects.push_back(new PrimitivePlane());
		 objectList->objects.at(objectList->objects.size() - 1)->CreateEntity(GetCurrentSceneName(), matStatic);
		 objectList->objects.at(objectList->objects.size() - 1)->SetPosition(offset, 0.0F, 0.0F);

		 // testing the copy constructor.
		  // objectList->objects.push_back(new PrimitivePlane(*(PrimitivePlane *)objectList->objects.at(objectList->objects.size() - 1)));
		  // objectList->objects.at(objectList->objects.size() - 1)->SetPosition(0.0F, 3.0F, -20.0F);
		  // objectList->objects.at(objectList->objects.size() - 1)->SetScale(45.0F);

		// liquid
		{
			Liquid* water = new Liquid(GetCurrentSceneName(), 20.0f, 100);
			water->SetEnabledWaves(3);
			water->SetGravity(9.81F);

			water->SetWave(0, 1.0f, 0.0f, 0.50f, 6.0f);
			water->SetWave(1, 0.0f, 1.0f, 0.25f, 3.1f);
			water->SetWave(2, 1.0f, 1.4f, 0.20f, 1.8f);

			water->SetColor(0.5f, 0.5f, 0.95f, 0.75f);
			water->SetClarity(0.9f);

			water->SetFresnelPower(0.5f);
			water->SetRefractionIndex(1.0f, 1.34f);
			water->SetEnvironment(GetCurrentScene()->Skybox);

			water->SetPosition(0.0F, 0.0F, -70.0F);
			water->SetVisible(true);
			AddObjectToScene(water);
		}
		  
		// Height Map
		{
			Terrain* terrain = new Terrain(GetCurrentSceneName(), "res/images/heightmaps/heightmap.bmp", 30.0f, 50, false);
			terrain->SetTexture(0, "res/images/red.png");
			terrain->SetTexture(1, "res/images/green.png");
			terrain->SetTexture(2, "res/images/blue.png");  
			terrain->SetMinimumHeight(-5.0F);
			terrain->SetMaximumHeight(10.0F); 
			terrain->SetPosition(0.0F, 0.0F, -15.0F); 
			terrain->SetVisible(true); 
			AddObjectToScene(terrain); 
		}
		//// sceneLists.push_back(new Object("res/sceneLists/monkey.obj", currentScene, material));
		{
			// images don't need CreateEntity called.
			// Image Test (1)
			// cherry::Image* image = new Image("res/images/bonus_fruit_logo_v01.png", GetCurrentSceneName(), true, false);

			// Image Test (2)
			// cherry::Image* image = new Image("res/images/bonus_fruit_logo_ss.png", GetCurrentSceneName(), false, false);

			// sprite sheet (currently 21 frames)

			// Image Test (3)
			// cherry::Image* image = new Image("res/images/bonus_fruit_logo_ss_bw.png", GetCurrentSceneName(), 
			// 	Image::ConvertImagePixelsToUVSpace(Vec4(0, 0, 1185, 594), 5925, 594, false), true, false);
			// 
			// // ..ss_bw and ..ss_rb are the same size, and are good for showing image switching. However, it's slow to siwtch them.
			// cherry::ImageAnimation* imgAnime = new ImageAnimation();
			// imgAnime->AddFrame(new cherry::ImageAnimationFrame("res/images/bonus_fruit_logo_ss_bw.png", Image::ConvertImagePixelsToUVSpace(Vec4(1185 * 0, 0, 1185 * 1, 594), 5925, 594, false), 0.5F));
			// imgAnime->AddFrame(new cherry::ImageAnimationFrame("res/images/bonus_fruit_logo_ss_rb.png", Image::ConvertImagePixelsToUVSpace(Vec4(1185 * 1, 0, 1185 * 2, 594), 5925, 594, false), 0.5F));
			// imgAnime->AddFrame(new cherry::ImageAnimationFrame("res/images/bonus_fruit_logo_ss_bw.png", Image::ConvertImagePixelsToUVSpace(Vec4(1185 * 2, 0, 1185 * 3, 594), 5925, 594, false), 0.5F));
			// imgAnime->AddFrame(new cherry::ImageAnimationFrame("res/images/bonus_fruit_logo_ss_rb.png", Image::ConvertImagePixelsToUVSpace(Vec4(1185 * 3, 0, 1185 * 4, 594), 5925, 594, false), 0.5F));
			// imgAnime->AddFrame(new cherry::ImageAnimationFrame("res/images/bonus_fruit_logo_ss_bw.png", Image::ConvertImagePixelsToUVSpace(Vec4(1185 * 4, 0, 1185 * 5, 594), 5925, 594, false), 0.5F));
			// imgAnime->SetInfiniteLoop(true);
			// imgAnime->Play();
			// image->AddAnimation(imgAnime, false);

			// Image Test (4)
			// cherry::Image* image = new Image("res/images/bonus_fruit_logo_ss_bw.png", GetCurrentSceneName(), 
			// 	Image::ConvertImagePixelsToUVSpace(Vec4(0, 0, 1185, 594), 5925, 594, false), true, false);
			cherry::Image* image = new Image("res/images/bonus_fruit_logo_ss_sml.png", GetCurrentSceneName(),
				Image::ConvertImagePixelsToUVSpace(Vec4(0, 0, 395, 198), 5530, 198, false), true, false);

			// ..ss_bw and ..ss_rb are the same size, and are good for showing image switching. However, it's slow to siwtch them.
			cherry::ImageAnimation* imgAnime = new ImageAnimation(); 
			
			// 14 frames
			imgAnime->AddFrame(new cherry::ImageAnimationFrame("res/images/bonus_fruit_logo_ss_sml.png", Image::ConvertImagePixelsToUVSpace(Vec4(395 * 0, 0, 395 * 1, 198), 5530, 198, false), 0.5F));
			imgAnime->AddFrame(new cherry::ImageAnimationFrame("res/images/bonus_fruit_logo_ss_sml.png", Image::ConvertImagePixelsToUVSpace(Vec4(395 * 1, 0, 395 * 2, 198), 5530, 198, false), 0.0F));
			imgAnime->AddFrame(new cherry::ImageAnimationFrame("res/images/bonus_fruit_logo_ss_sml.png", Image::ConvertImagePixelsToUVSpace(Vec4(395 * 2, 0, 395 * 3, 198), 5530, 198, false), 0.0F));
			imgAnime->AddFrame(new cherry::ImageAnimationFrame("res/images/bonus_fruit_logo_ss_sml.png", Image::ConvertImagePixelsToUVSpace(Vec4(395 * 3, 0, 395 * 4, 198), 5530, 198, false), 0.0F));
			imgAnime->AddFrame(new cherry::ImageAnimationFrame("res/images/bonus_fruit_logo_ss_sml.png", Image::ConvertImagePixelsToUVSpace(Vec4(395 * 4, 0, 395 * 5, 198), 5530, 198, false), 0.0F));
																							  
			imgAnime->AddFrame(new cherry::ImageAnimationFrame("res/images/bonus_fruit_logo_ss_sml.png", Image::ConvertImagePixelsToUVSpace(Vec4(395 * 5, 0, 395 * 6, 198), 5530, 198, false), 0.0F));
			imgAnime->AddFrame(new cherry::ImageAnimationFrame("res/images/bonus_fruit_logo_ss_sml.png", Image::ConvertImagePixelsToUVSpace(Vec4(395 * 6, 0, 395 * 7, 198), 5530, 198, false), 0.0F));
			imgAnime->AddFrame(new cherry::ImageAnimationFrame("res/images/bonus_fruit_logo_ss_sml.png", Image::ConvertImagePixelsToUVSpace(Vec4(395 * 7, 0, 395 * 8, 198), 5530, 198, false), 0.0F));
			imgAnime->AddFrame(new cherry::ImageAnimationFrame("res/images/bonus_fruit_logo_ss_sml.png", Image::ConvertImagePixelsToUVSpace(Vec4(395 * 8, 0, 395 * 9, 198), 5530, 198, false), 0.0F));
			imgAnime->AddFrame(new cherry::ImageAnimationFrame("res/images/bonus_fruit_logo_ss_sml.png", Image::ConvertImagePixelsToUVSpace(Vec4(395 * 9, 0, 395 * 10, 198), 5530, 198, false), 0.0F));
																							  
			imgAnime->AddFrame(new cherry::ImageAnimationFrame("res/images/bonus_fruit_logo_ss_sml.png", Image::ConvertImagePixelsToUVSpace(Vec4(395 * 10, 0, 395 * 11, 198), 5530, 198, false), 0.0F));
			imgAnime->AddFrame(new cherry::ImageAnimationFrame("res/images/bonus_fruit_logo_ss_sml.png", Image::ConvertImagePixelsToUVSpace(Vec4(395 * 11, 0, 395 * 12, 198), 5530, 198, false), 0.0F));
			imgAnime->AddFrame(new cherry::ImageAnimationFrame("res/images/bonus_fruit_logo_ss_sml.png", Image::ConvertImagePixelsToUVSpace(Vec4(395 * 12, 0, 395 * 13, 198), 5530, 198, false), 0.0F));
			imgAnime->AddFrame(new cherry::ImageAnimationFrame("res/images/bonus_fruit_logo_ss_sml.png", Image::ConvertImagePixelsToUVSpace(Vec4(395 * 13, 0, 395 * 14, 198), 5530, 198, false), 0.0F));
			
			
			imgAnime->SetInfiniteLoop(true);
			imgAnime->Play();
			image->AddAnimation(imgAnime, false);
			image->SetVisible(true);

			objectList->objects.push_back(image);
			objectList->objects.at(objectList->GetObjectCount() - 1)->SetPosition(0.0F, 0.0F, 1.0F);
			objectList->objects.at(objectList->GetObjectCount() - 1)->SetScale(0.01F);
		
			// image->GetAnimation(0)->Play();
			
		} 

		// image (UI element)
		{
			cherry::Image* image = new Image("res/images/codename_zero_logo.png", GetCurrentSceneName(), false, false);
			image->SetPosition(-88.0F, -47.0F, 0.0F);
			image->SetFixedScreenPosition(true);
			// image->SetPositionByScreenPortion(Vec2(0.5, 0.5), Vec2(myWindowSize), Vec2(0.5, 0.5));
			// image->SetPosition(myCamera->GetPosition() + glm::vec3(0.0F, 0.0F, -10.0F));
			image->SetScale(0.02F);
			image->SetAlpha(0.8F);
			image->SetVisible(true);
			objectList->objects.push_back(image); 
		}

		// version 1 (finds .mtl file automatically)
		objectList->objects.push_back(new Object("res/objects/charactoereee.obj", GetCurrentSceneName(),
			lightList->GenerateMaterial(DYNAMIC_VS, DYNAMIC_FS, sampler), true, true));

		// objectList->objects.push_back(new Object("res/objects/charactoereee.obj", currentScene,
		// LightManager::GetSceneLightsMerged(currentScene)->GenerateMaterial(sampler), true, true));


		objectList->objects.at(objectList->objects.size() - 1)->SetScale(10.0F);
		hitBoxIndex = objectList->objects.size() - 1;

		// sceneLists.push_back();

		// version 2 (.mtl file manually added)
		//sceneLists.push_back(new Object("res/sceneLists/MAS_1 - QIZ04 - Textured Hammer.obj", currentScene, 
		// 	LightManager::GetSceneLightsMerged(currentScene)->GenerateMaterial(STATIC_VS, STATIC_FS, sampler),
		// 	"res/sceneLists/MAS_1 - QIZ04 - Textured Hammer.mtl", false));
		
		// PhysicsBodyBox* temp = new PhysicsBodyBox(Vec3(0.0F, 0.0F, 0.0F), 1.0F, 3.0F, 1.0F);
		PhysicsBodyBox* temp = new PhysicsBodyBox(Vec3(0.0F, 1.0F, 0.0F), 1.0F, 3.0F, 1.0F);
		// temp->SetRotationDegrees(Vec3(0, 0, 30.0F));
		// temp->SetScale(Vec3(2.0F, 2.0F, 2.0F));
		objectList->objects.at(objectList->objects.size() - 1)->AddPhysicsBody(temp);
		objectList->objects.at(objectList->objects.size() - 1)->GetPhysicsBodies()[0]->SetVisible(true);

		// objectList->objects.at(objectList->objects.size() - 1)->SetScale(Vec3(2.0F, 2.0F, 2.0F));
		// objectList->objects.at(objectList->objects.size() - 1)->SetRotationZDegrees(45.0F);
		
		// path
		Path path = Path();
		path.AddNode(8.0F, 0.0F, 0.0F);
		path.AddNode(-8.0F, 8.0F, 0.0F);
		path.AddNode(20.0F, 8.0F, 8.0F);
		path.AddNode(7.0F, 9.0F, 7.0F);
		path.AddNode(8.0F, -8.0F, -8.0F);
		path.SetIncrementer(0.1f);

		path.SetInterpolationMode(1);
		path.SetOpenPath(false);
		path.SetSpeedControl(true);

		objectList->objects.at(objectList->objects.size() - 1)->SetPath(path, true);

		objectList->objects.at(objectList->objects.size() - 1)->SetScale(0.7);

		// sceneLists.at(sceneLists.size() - 1)->CreateEntity(currentScene, objMat);
		// sceneLists.at(sceneLists.size() - 1)->SetPosition(0.0F, 0.0F, -10.0F);
		// sceneLists.at(sceneLists.size() - 1)->SetScale(2.0F);

		//material->SetShader(shdr);
		// VER 1
		//sceneLists.push_back(new Object("res/sceneLists/cube_morph_target_0.obj", currentScene, matDynamic, false, true));
		//sceneLists.at(sceneLists.size() - 1)->SetPosition(offset, offset, 0.0F);
		////

		//MorphAnimation* mph = new MorphAnimation();
		//mph->AddFrame(new MorphAnimationFrame("res/sceneLists/cube_morph_target_0.obj", 2.0F));
		//mph->AddFrame(new MorphAnimationFrame("res/sceneLists/cube_morph_target_1.obj", 2.0F));

		// VER 2
		objectList->objects.push_back(new Object("res/objects/hero pose one.obj", GetCurrentSceneName(), matDynamic, false, true));
		objectList->objects.at(objectList->objects.size() - 1)->SetPosition(offset, offset, 0.0F);
		//

		MorphAnimation * mph = new MorphAnimation();
		mph->AddFrame(new MorphAnimationFrame("res/objects/hero pose one.obj", 2.0F));
		mph->AddFrame(new MorphAnimationFrame("res/objects/hero pose two.obj", 2.0F));
		mph->AddFrame(new MorphAnimationFrame("res/objects/hero pose three.obj", 2.0F));
		// mph->AddFrame(new MorphAnimationFrame("res/sceneLists/cube_target_0.obj", 2.0F));
		mph->SetInfiniteLoop(true);
		// TODO: set up ability to return to pose 0, t-pose, or stay on ending frame.
		//mph->SetLoopsTotal(3);
		mph->Play();
		objectList->objects.at(objectList->objects.size() - 1)->AddAnimation(mph, true);
		// sceneLists.at(sceneLists.size() - 1)->GetMesh()->SetVisible(false);

	} 
	
	// Switching a scene.
	// CreateScene("AIS", false);
	// objectList->objects.at(0)->SetScene("AIS");
	// SetCurrentScene("AIS", false);

	// Create and compile shader
	// myShader = std::make_shared<Shader>();
	// myShader->Load("res/shader.vs.glsl", "res/shader.fs.glsl");

	// myModelTransform = glm::mat4(1.0f); // initializing the model matrix

	// TODO: streamline, and replace audio file (WE DON'T OWN IT)
	// Load a bank (Use the flag FMOD_STUDIO_LOAD_BANK_NORMAL)
	// TODO: put in dedicated folder with ID on it?
	audioEngine.LoadBank("res/audio/Master", FMOD_STUDIO_LOAD_BANK_NORMAL);

	// Load an event
	audioEngine.LoadEvent("Music", "{13471b17-f4bd-4cd5-afaa-e9e60eb1ee67}");
	// Play the event
	audioEngine.PlayEvent("Music");
}

void cherry::Game::UnloadContent() {
}

void cherry::Game::Update(float deltaTime) {

	glm::vec3 camTranslate{}; // movement for the camera this given frame.
	float camTransInc = 8.0F; // increment for camera movement

	// TODO: remove this line.
	// <the update loop for all sceneLists was originally here.>

	// updates the camera
	if (debugMode) // moves the camera with button presses if in debug mode.
	{
		//// moving the camera
		camTranslate.x = (a) ? -camTransInc * deltaTime : (d) ? camTransInc * deltaTime : 0.0F; // x-axis
		camTranslate.y = (w) ? camTransInc * deltaTime : (s) ? -camTransInc * deltaTime : 0.0F; // y-axis

		myCamera->SetPosition(myCamera->GetPosition() + camTranslate); // setting the new cmaera position
		myCamera->LookAt(glm::vec3(0, 0, 0)); //Looks at player
	} 

	// if (w)
	// 	objectList->objects.at(0)->Translate(0.0F, 10.0F * deltaTime, 0.0F);
	// else if (s)
	// 	objectList->objects.at(0)->Translate(0.0F, -10.0F * deltaTime, 0.0F);
	// if (a)
	// 	objectList->objects.at(0)->Translate(-10.0F * deltaTime, 0.0F, 0.0F);
	// else if (d)
	// 	objectList->objects.at(0)->Translate(10.0F * deltaTime, 0.0F, 0.0F);

	myCamera->Update(deltaTime);
	myCameraX->Update(deltaTime);

	// updates the object list
	objectList->Update(deltaTime);

	// if collisions should be checked.
	if (collisionMode)
	{
		// collision calculations
	mainLoop:
		for (cherry::Object* obj1 : objectList->objects) // object 1
		{
			if (obj1 == nullptr)
				continue;

			if (obj1->GetIntersection() == true) // already colliding with something.
				continue;

			for (cherry::Object* obj2 : objectList->objects) // object 2
			{
				if (obj1 == obj2 || obj2 == nullptr) // if the two sceneLists are the same.
					continue;

				if (obj2->GetIntersection() == true) // if the object is already intersecting with something.
					continue;

				// gets the vectors from both sceneLists
				std::vector<cherry::PhysicsBody*> pbods1 = obj1->GetPhysicsBodies();
				std::vector<cherry::PhysicsBody*> pbods2 = obj2->GetPhysicsBodies();

				// goes through each collision body
				for (cherry::PhysicsBody* pb1 : pbods1)
				{
					for (cherry::PhysicsBody* pb2 : pbods2)
					{
						bool col = PhysicsBody::Collision(pb1, pb2);

						if (col == true) // if collision has occurred.
						{
							obj1->SetIntersection(true);
							// obj1->setColor(255, 0, 0);
							obj2->SetIntersection(true);
							// obj2->setColor(255, 0, 0);
							// std::cout << "Hit!" << std::endl;

							goto mainLoop; // goes back to the main loop
						}
					}
				}
			}
		}
	}

	// moved to the bottom of the update.
	// called to Update the position and rotation of hte sceneLists.
	// calling all of our functions for our Update behaviours.
	auto view = CurrentRegistry().view<UpdateBehaviour>();
	for (const auto& e : view) {
		auto& func = CurrentRegistry().get<UpdateBehaviour>(e);
		if (func.Function) {
			func.Function(e, deltaTime);
		}
	}

	// // secondary registry
	// view = CurrentSecondaryRegistry().view<UpdateBehaviour>();
	// for (const auto& e : view) {
	// 	auto& func = CurrentSecondaryRegistry().get<UpdateBehaviour>(e);
	// 	if (func.Function) {
	// 		func.Function(e, deltaTime);
	// 	}
	// }

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

void cherry::Game::Run()
{
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
			std::cout << "Test" << std::endl;
		}

	}

	UnloadContent(); // unload all content
	ShutdownImGui(); // shutdown imGui
	audioEngine.Shutdown(); // shutdown the audio component.
	Shutdown();
}

// resizes the window without skewing the sceneLists, and changes the cameras accordingly.
void cherry::Game::Resize(int newWidth, int newHeight)
{
	// for some reason, calling the functions and having them be used directly didn't work.
	// so all the values are being saved first.
	glm::vec2 orthoSizePro{ (float)newWidth / myWindowSize.x ,(float)newHeight / myWindowSize.y };

	// TODO: keep objects in proper place. Do note that this goes through all objects and should probably be optimized somehow.
	// moving all the object
	// for (Object* obj : objectList->GetObjects())
	// {
	// 	obj->SetPositionX(obj->GetPositionX() / myWindowSize.x * newWidth);
	// 	obj->SetPositionY(obj->GetPositionY() / myWindowSize.y * newHeight);
	// }


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

	// saving the new window size.
	myWindowSize = { newWidth, newHeight }; // updating window size
}

// draws to a given viewpoint. The code that was originally here was moved to _RenderScene
void cherry::Game::Draw(float deltaTime) {
	// viewport size (full screen)
	glm::ivec4 viewport = {
	0, 0,
	myWindowSize.x, myWindowSize.y
	};
	
	// renders the scene
	__RenderScene(viewport, myCamera, true, 0, glm::vec4(1.0F), true);
}

void cherry::Game::DrawGui(float deltaTime) {
	if (debugMode)
	{
		// Open a new ImGui window
		ImGui::Begin("Colour Picker");

		// Draw widgets here
		// ImGui::SliderFloat4("Color", &myClearColor.x, 0, 1); // Original
		ImGui::ColorPicker4("Color", &myClearColor.x); // new version
		// ImGui::SetWindowSize(ImVec2(500.0F, 500.0F)); // window size for ImGUI Colour Picker (perament)
		// ImGui::SetNextWindowCollapsed(false);
		// ImGui::SetNextWindowPos(ImVec2(-225.0F, 1.0F));
		ImGui::SetNextWindowSize(ImVec2(500.0F, 500.0F)); // window size for ImGUI ColorPicker (variable)
		if (ImGui::InputText("Title", myWindowTitle, 31))
		{
			glfwSetWindowTitle(myWindow, myWindowTitle);
		}

		if (ImGui::Button("Apply")) // adding another button, which allows for the application of the window title.
		{
			glfwSetWindowTitle(myWindow, myWindowTitle);
		}
		if (ImGui::Button("Wireframe/Fill Toggle"))
		{
			for (cherry::Object* obj : objectList->objects)
				obj->SetWireframeMode();
		}

		// changing the camera mode
		std::string camMode = myCamera->InPerspectiveMode() ? "Perspective" : "Orthographic";
		ImGui::InputText((std::string("CAMERA MODE (\'SPACE\')") + camMode).c_str(), myWindowTitle, 32);

		ImGui::End();
	}
}

// Now handles rendering the scene.
void cherry::Game::__RenderScene(glm::ivec4 viewport, Camera::Sptr camera, bool drawSkybox, int borderSize, glm::vec4 borderColor, bool clear)
{
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

	// Clear our new inset area with the scene clear color
	glClearColor(myClearColor.x, myClearColor.y, myClearColor.z, myClearColor.w);
	if (clear)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// no longer needed?
	// myShader->Bind();

	// SKYBOX //
	auto scene = CurrentScene();
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
		// If our shader has changed, we need to bind it and Update our frame-level uniforms
		if (renderer.Material->GetShader() != boundShader) {
			boundShader = renderer.Material->GetShader();
			boundShader->Bind();

			// if the object is to have a fixed screen position.
			if(renderer.Mesh->GetFixedScreenPosition())
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
		if (renderer.Mesh->GetFixedScreenPosition())
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

		// TODO: add ability to turn face culling on and off for a given object
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
}