// GAME CLASS (SOURCE)

#include "Game.h"

#include <stdexcept>
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>
#include <GLM/gtc/matrix_transform.hpp>
#include <toolkit/Logging.h>

#include "SceneManager.h"
#include "MeshRenderer.h"
#include "Texture2D.h"

#include "PhysicsBody.h"
#include "utils/Utils.h"

#include<functional>

struct TempTransform {
	glm::vec3 Position = glm::vec3(0.0f);
	glm::vec3 EulerRotation = glm::vec3(0.0f);
	glm::vec3 Scale = glm::vec3(1.0f);

	// does our TRS for us.
	glm::mat4 GetWorldTransform() const {
		return
			glm::translate(glm::mat4(1.0f), Position) *
			glm::mat4_cast(glm::quat(glm::radians(EulerRotation))) *
			glm::scale(glm::mat4(1.0f), Scale)
			;
	}
};

struct UpdateBehaviour {
	std::function<void(entt::entity e, float dt)> Function;
};

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



// Game
// constructor
cherry::Game::Game() :
	myWindow(nullptr),
	myWindowTitle("GDW_Y2 - BnsFt - Cherry Engine"),
	myClearColor(glm::vec4(0.1f, 0.7f, 0.5f, 1.0f)), // default clear colour
	myModelTransform(glm::mat4(1)), // my model transform
	myWindowSize(600, 600) // window size (default)
{ }

// creates window with a width, height, and whether or not it's in full screen.
cherry::Game::Game(const char windowTitle[32], float _width, float _height, bool _fullScreen, bool _defaults, bool _debug) : Game()
{
	// setting the values
	memcpy(myWindowTitle, windowTitle, strlen(windowTitle) + 1);
	myWindowSize = glm::ivec2(_width, _height);
	fullScreen = _fullScreen;
	loadDefaults = _defaults; // loads the engine default values
	debugMode = _debug; // debug functionality.
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

	// Update the player object's angle
	// playerObj->updateAngle(*game->myCamera, xpos, ypos, this->windowWidth, this->windowHeight);
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

// called when a mouse button has been pressed
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
	}
}

// adds an object to the m_Scene
bool cherry::Game::addObject(cherry::Object* obj) { return util::addToVector(objects, obj); }


// adds an object to the m_Scene.
bool cherry::Game::addObject(cherry::Object* obj, std::string scene)
{
	// adds the object to the list of objects.
	bool added = util::addToVector(objects, obj);

	if (added) // if the object was added, then an entity is created.
		obj->CreateEntity(scene, material);

	return added; // returns 
}

// removes an object from the objects vector.
bool cherry::Game::removeObject(cherry::Object* obj) 
{ 
	return util::removeFromVector(objects, obj); 
	delete obj;
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
	// glEnable(GL_CULL_FACE); // TODO: uncomment when showcasing game.
	glEnable(GL_SCISSOR_TEST); // used for rendering multiple windows (TODO: maybe turn off if we aren't using multiple windows?)
}

void cherry::Game::Shutdown() {
	glfwTerminate();
}

// makes the faces face outward.
cherry::Mesh::Sptr MakeInvertedCube() {
	// Create our 4 vertices
	cherry::Vertex verts[8] = {
		// Position
		// x y z
		{{ -1.0f, -1.0f, -1.0f }}, {{ 1.0f, -1.0f, -1.0f }}, {{ -1.0f, 1.0f, -1.0f }}, {{ 1.0f, 1.0f, -1.0f }},
		{{ -1.0f, -1.0f, 1.0f }}, {{ 1.0f, -1.0f, 1.0f }}, {{ -1.0f, 1.0f, 1.0f }}, {{ 1.0f, 1.0f, 1.0f }}
	};
	// Create our 6 indices
	uint32_t indices[36] = {
	0, 1, 2, 2, 1, 3, 4, 6, 5, 6, 7, 5, // bottom / top
	0, 1, 4, 4, 1, 5, 2, 3, 6, 6, 3, 7, // front /back
	2, 4, 0, 2, 6, 4, 3, 5, 1, 3, 7, 5 // left / right
	};
	// Create a new mesh from the data
	return std::make_shared<cherry::Mesh>(verts, 8, indices, 36);
}

// chopping up the plane.
cherry::Mesh::Sptr MakeSubdividedPlane(float size, int numSections, bool worldUvs = true) {
	LOG_ASSERT(numSections > 0, "Number of sections must be greater than 0!");
	LOG_ASSERT(size != 0, "Size cannot be zero!");
	// Determine the number of edge vertices, and the number of vertices and indices we'll need
	int numEdgeVerts = numSections + 1;
	size_t vertexCount = numEdgeVerts * numEdgeVerts;
	size_t indexCount = numSections * numSections * 6;
	// Allocate some memory for our vertices and indices
	cherry::Vertex* vertices = new cherry::Vertex[vertexCount];
	uint32_t* indices = new uint32_t[indexCount];
	// Determine where to start vertices from, and the step pre grid square
	float start = -size / 2.0f;
	float step = size / numSections;

	// vertices
	// Iterate over the grid's edge vertices
	for (int ix = 0; ix <= numSections; ix++) {
		for (int iy = 0; iy <= numSections; iy++) {
			// Get a reference to the vertex so we can modify it
			cherry::Vertex& vert = vertices[ix * numEdgeVerts + iy];
			// Set its position
			vert.Position.x = start + ix * step;
			vert.Position.y = start + iy * step;
			vert.Position.z = 0.0f;
			// Set its normal
			vert.Normal = glm::vec3(0, 0, 1);
			// The UV will go from [0, 1] across the entire plane (can change this later)
			if (worldUvs) {
				vert.UV.x = vert.Position.x;
				vert.UV.y = vert.Position.y;
			}
			else {
				vert.UV.x = vert.Position.x / size;
				vert.UV.y = vert.Position.y / size;
			}
			// Flat white color
			vert.Color = glm::vec4(1.0f);
		}
	}

	// indices
	// We'll just increment an index instead of calculating it
	uint32_t index = 0;
	// Iterate over the quads that make up the grid
	for (int ix = 0; ix < numSections; ix++) {
		for (int iy = 0; iy < numSections; iy++) {
			// Determine the indices for the points on this quad
			uint32_t p1 = (ix + 0) * numEdgeVerts + (iy + 0);
			uint32_t p2 = (ix + 1) * numEdgeVerts + (iy + 0);
			uint32_t p3 = (ix + 0) * numEdgeVerts + (iy + 1);
			uint32_t p4 = (ix + 1) * numEdgeVerts + (iy + 1);
			// Append the quad to the index list
			indices[index++] = p1;
			indices[index++] = p2;
			indices[index++] = p3;
			indices[index++] = p3;
			indices[index++] = p2;
			indices[index++] = p4;
		}
	}

	// returning the mesh
	// Create the result, then clean up the arrays we used
	cherry::Mesh::Sptr result = std::make_shared<cherry::Mesh>(vertices, vertexCount, indices, indexCount);
	delete[] vertices;
	delete[] indices;
	// Return the result
	return result;
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
	myCamera->SetPerspectiveMode(glm::perspective(glm::radians(60.0f), 1.0f, 0.01f, 1000.0f));
	// myCamera->SetPerspectiveMode(glm::perspective(glm::radians(10.0f), 1.0f, 0.01f, 1000.0f));

	// sets the orthographic mode values. False is passed so that the camera starts in perspective mode.
	myCamera->SetOrthographicMode(glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.0f, 100.0f), false);

	// SAMPLER FOR MIP MAPPING
	// added for mip mapping. As long as its above the material, it's fine.
	
	// OLD VERSION
	// TODO: remove upon submission
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
	Texture2D::Sptr albedo = Texture2D::LoadFromFile("res/images/default.png");
	material = std::make_shared<Material>(phong);
	material->Set("a_LightPos", { 0, 0, 3 });
	material->Set("a_LightColor", { 0.5f, 0.1f, 0.9f});
	material->Set("a_AmbientColor", { 0.9f, 0.1f, 0.01f });
	material->Set("a_AmbientPower", 0.4f); // change this to change the main lighting power (originally value of 0.1F)
	material->Set("a_LightSpecPower", 0.5f);
	material->Set("a_LightShininess", 256.0f); // MUST be a float
	material->Set("a_LightAttenuation", 0.15f);
	// material->Set("s_Albedo", albedo, sampler); // objects will just be blank if no texture is set.
	
	// testMat->Set("s_Albedo", albedo); // right now, this is using the texture state.
	// testMat->Set("s_Albedo", albedo, Linear); // now uses mip mapping
	
	material->Set("s_Albedos[0]", Texture2D::LoadFromFile("res/images/default.png"), sampler);
	material->Set("s_Albedos[1]", Texture2D::LoadFromFile("res/images/default.png"), sampler);
	material->Set("s_Albedos[2]", Texture2D::LoadFromFile("res/images/default.png"),sampler);

	currentScene = "Cherry"; // the name of the m_Scene
	scenes.push_back(currentScene); // saving the m_Scene

	//lights.push_back(new Light(currentScene, glm::vec3( 0.0F, 0.0F, 1.0F ), glm::vec3( 1.0F, 1.0F, 0.0F ), glm::vec3(0.2F, 0.5F, 0.01F
	//	), 0.9F, 0.5F, 256, 1.0F));
	//material = lights[0]->GenerateMaterial();

	SceneManager::RegisterScene(currentScene); // registering the m_Scene
	// SceneManager::RegisterScene("Test2");
	SceneManager::SetCurrentScene(currentScene);

	// SKYBOX
	// we need to make the scene before we can attach things to it.
	auto scene = CurrentScene();
	scene->SkyboxShader = std::make_shared<Shader>();
	scene->SkyboxShader->Load("res/cubemap.vs.glsl", "res/cubemap.fs.glsl");
	scene->SkyboxMesh = MakeInvertedCube();

	// loads in six files out of res, then making them into the cube map.

	std::string files[6] = {
	std::string("res/images/cubemap/graycloud_lf.jpg"),
	std::string("res/images/cubemap/graycloud_rt.jpg"),
	std::string("res/images/cubemap/graycloud_dn.jpg"),
	std::string("res/images/cubemap/graycloud_up.jpg"),
	std::string("res/images/cubemap/graycloud_ft.jpg"),
	std::string("res/images/cubemap/graycloud_bk.jpg")
	};
	scene->Skybox = TextureCube::LoadFromFiles(files);

	// Shader was originally compiled here.
	// // Create and compile shader
	// myShader = std::make_shared<Shader>();
	// myShader->Load("res/shader.vert.glsl", "res/shader.frag.glsl");
	// 
	// myModelTransform = glm::mat4(1.0f); // initializing the model matrix

		// loads in default objects
	if (loadDefaults)
	{
		float offset = 3.0F; // position offset

		// Creating the objects, storing them, and making them part of the default m_Scene.
		objects.push_back(new PrimitiveCapsule());
		objects.at(objects.size() - 1)->CreateEntity(currentScene, material);
		objects.at(objects.size() - 1)->SetPosition(-offset, -offset, 0.0F);
		

		objects.push_back(new PrimitiveCircle());
		objects.at(objects.size() - 1)->CreateEntity(currentScene, material);
		objects.at(objects.size() - 1)->SetPosition(-offset, 0.0f, 0.0F);

		objects.push_back(new PrimitiveCone());
		objects.at(objects.size() - 1)->CreateEntity(currentScene, material);
		objects.at(objects.size() - 1)->SetPosition(-offset, offset, 0.0F);

		objects.push_back(new PrimitiveCube());
		objects.at(objects.size() - 1)->CreateEntity(currentScene, material);
		objects.at(objects.size() - 1)->SetPosition(0.0F, -offset, 0.0F);

		objects.push_back(new PrimitiveCylinder());
		objects.at(objects.size() - 1)->CreateEntity(currentScene, material);
		objects.at(objects.size() - 1)->SetPosition(0.0F, 0.0F, 0.0F);

		objects.push_back(new PrimitiveDiamond());
		objects.at(objects.size() - 1)->CreateEntity(currentScene, material);
		objects.at(objects.size() - 1)->SetPosition(0.0F, offset, 0.0F);

		objects.push_back(new PrimitiveUVSphere());
		objects.at(objects.size() - 1)->CreateEntity(currentScene, material);
		objects.at(objects.size() - 1)->SetPosition(offset, -offset, 0.0F);

		objects.push_back(new PrimitivePlane());
		objects.at(objects.size() - 1)->CreateEntity(currentScene, material);
		objects.at(objects.size() - 1)->SetPosition(offset, 0.0F, 0.0F);

		// objects.push_back(new Object("res/objects/monkey.obj", currentScene, material));
	}

	// Create and compile shader
	myShader = std::make_shared<Shader>();
	myShader->Load("res/shader.vert.glsl", "res/shader.frag.glsl");

	// myModelTransform = glm::mat4(1.0f); // initializing the model matrix

	// WATER SHADER
	// Making the water shader
	{ // Push a new scope so that we don't step on other names
		if (loadDefaults) // the water will be considered one of the defaults.
		{
			Shader::Sptr waterShader = std::make_shared<Shader>();
			waterShader->Load("res/water-shader.vs.glsl", "res/water-shader.fs.glsl");
			Material::Sptr waterMaterial = std::make_shared<Material>(waterShader);
			waterMaterial->HasTransparency = true;


			waterMaterial->Set("a_EnabledWaves", 3); // number of waves
			waterMaterial->Set("a_Gravity", 9.81f);
			// Format is: [xDir, yDir, "steepness", wavelength] (note that the sum of steepness should be < 1 to avoid loops)
			waterMaterial->Set("a_Waves[0]", { 1.0f, 0.0f, 0.50f, 6.0f });
			waterMaterial->Set("a_Waves[1]", { 0.0f, 1.0f, 0.25f, 3.1f });
			waterMaterial->Set("a_Waves[2]", { 1.0f, 1.4f, 0.20f, 1.8f });
			waterMaterial->Set("a_WaterAlpha", 0.75f);
			waterMaterial->Set("a_WaterColor", { 0.5f, 0.5f, 0.95f });
			waterMaterial->Set("a_WaterClarity", 0.9f); // 0.9f so that it's fairly clear
			waterMaterial->Set("a_FresnelPower", 0.5f); // the higehr the power, the higher the reflection
			waterMaterial->Set("a_RefractionIndex", 1.0f / 1.34f); // bending light; 1.0/1.34 goes from air to water.
			waterMaterial->Set("s_Environment", scene->Skybox);

			auto& ecs = GetRegistry(currentScene); // If you've changed the name of the scene, you'll need to modify this!
			entt::entity e1 = ecs.create();
			MeshRenderer& m1 = ecs.assign<MeshRenderer>(e1);
			m1.Material = waterMaterial;
			m1.Mesh = MakeSubdividedPlane(20.0f, 100);

			auto tform = [](entt::entity e, float dt)
			{
				auto& transform = CurrentRegistry().get_or_assign<TempTransform>(e);

				transform.Position = { 0.0f, 0.0f, -50.0f };
			};

			auto& up = ecs.get_or_assign<UpdateBehaviour>(e1);
			up.Function = tform;
		}
	}
}

void cherry::Game::UnloadContent() {
}

void cherry::Game::Update(float deltaTime) {

	glm::vec3 camTranslate{}; // movement for the camera this given frame.
	float camTransInc = 0.05F; // increment for camera movement

	// TODO: remove this line.
	// <the update loop for all objects was originally here.>

	// updates the camera
	if (debugMode) // moves the camera with button presses if in debug mode.
	{
		// moving the camera
		camTranslate.x = (a) ? -camTransInc : (d) ? camTransInc : 0.0F; // x-axis
		camTranslate.y = (w) ? camTransInc : (s) ? -camTransInc : 0.0F; // y-axis

		myCamera->SetPosition(myCamera->GetPosition() + camTranslate); // setting the new cmaera position
		myCamera->LookAt(glm::vec3(0, 0, 0)); //Looks at player
	}

	// setting intersection to false for all objects
	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(deltaTime); // calls the Update loop
		objects[i]->SetIntersection(false); // sets intersection to false for all objects
		// std::cout << "[" + std::to_string(i) + "] : " << *cherry::Vec3(objects[i]->GetPosition()).v << std::endl;
	}

	// std::cout << std::endl;
	// collisions
mainLoop:
	for (cherry::Object* obj1 : objects) // object 1
	{
		if (obj1 == nullptr)
			continue;
		if (obj1->GetIntersection() == true) // already colliding with something.
			continue;

		for (cherry::Object* obj2 : objects) // object 2
		{
			if (obj1 == obj2 || obj2 == nullptr) // if the two objects are the same.
				continue;

			if (obj2->GetIntersection() == true) // if the object is already intersecting with something.
				continue;

			// gets the vectors from both objects
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

	// moved to the bottom of the update.
	// called to Update the position and rotation of hte objects.
	// calling all of our functions for our Update behaviours.
	auto view = CurrentRegistry().view<UpdateBehaviour>();
	for (const auto& e : view) {
		auto& func = CurrentRegistry().get<UpdateBehaviour>(e);
		if (func.Function) {
			func.Function(e, deltaTime);
		}
	}
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

	// Run as long as the window is open
	while (!glfwWindowShouldClose(myWindow)) {
		// Poll for events from windows
		// clicks, key presses, closing, all that
		glfwPollEvents();
		float thisFrame = glfwGetTime();
		float deltaTime = thisFrame - prevFrame;
		Update(deltaTime);
		Draw(deltaTime);
		ImGuiNewFrame();
		DrawGui(deltaTime);
		ImGuiEndFrame();
		prevFrame = thisFrame;
		// Present our image to windows
		glfwSwapBuffers(myWindow);
	}
	UnloadContent();
	ShutdownImGui();
	Shutdown();
}

// resizes the window without skewing the objects, and changes the cameras accordingly.
void cherry::Game::Resize(int newWidth, int newHeight)
{
	myWindowSize = { newWidth, newHeight }; // updating window size

	// changing the camera modes to adjust for the new window size. 
	// The camera mode isn't changed, just it's values (i.e. if it's in perspective mode, it stays in perspective mode).
	myCamera->SetPerspectiveMode(glm::perspective(glm::radians(60.0f), newWidth / (float)newHeight, 0.01f, 1000.0f), myCamera->InPerspectiveMode());
	myCamera->SetOrthographicMode(glm::ortho(-5.0f * newWidth / (float)newHeight, 5.0f * newWidth / (float)newHeight, -5.0f, 5.0f, 0.0f, 100.0f), myCamera->InOrthographicMode());
}

// draws to a given viewpoint. The code that was originally here was moved to _RenderScne
void cherry::Game::Draw(float deltaTime) {
	// TODO: set up ability to have multiple views.
	// bool singleView = true;

	glm::ivec4 viewport = {
	0, 0,
	myWindowSize.x, myWindowSize.y
	};
	__RenderScene(viewport, myCamera);
	
	// bottom of the window
	//glm::ivec4 viewport1 = {
	//	0, 0,
	//	myWindowSize.x, myWindowSize.y / 2
	//};
	//__RenderScene(viewport1, myCamera);


	//// top of the window
	//glm::ivec4 viewport2 = {
	//	0, myWindowSize.y / 2,
	//	myWindowSize.x, myWindowSize.y / 2
	//};
	//__RenderScene(viewport2, myCamera);

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
			for (cherry::Object* obj : objects)
				obj->SetWireframeMode();
		}

		// changing the camera mode
		std::string camMode = myCamera->InPerspectiveMode() ? "Perspective" : "Orthographic";
		ImGui::InputText((std::string("CAMERA MODE (\'SPACE\')") + camMode).c_str(), myWindowTitle, 32);

		ImGui::End();
	}
}

// Now handles rendering the scene.
void cherry::Game::__RenderScene(glm::ivec4 viewport, Camera::Sptr camera)
{
	static bool wireframe = false; // used to switch between fill mode and wireframe mode for draw calls.
	bool enableSkybox = false; // enables the skybox. TODO: change for final build.

	int border = 0; // the border for the viewpoint
	glm::vec4 borderColor = { 1.0F, 1.0F, 1.0F, 1.0F }; // border colour

	// Set viewport to entire region
	// glViewport(viewport.x, viewport.y, viewport.z, viewport.w); // not neded since viewpoint doesn't change the clear call.
	glScissor(viewport.x, viewport.y, viewport.z, viewport.w);

	// Clear with the border color
	glClearColor(borderColor.x, borderColor.y, borderColor.z, borderColor.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// Set viewport to be inset slightly (the amount is the border width)
	// the offsets are used to move the border relative to the viewpoint.
	glViewport(viewport.x + border, viewport.y + border, viewport.z - 2 * border, viewport.w - 2 * border);
	glScissor(viewport.x + border, viewport.y + border, viewport.z - 2 * border, viewport.w - 2 * border);

	// Clear our new inset area with the scene clear color
	glClearColor(myClearColor.x, myClearColor.y, myClearColor.z, myClearColor.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// no longer needed?
	// myShader->Bind();


	// We'll grab a reference to the ecs to make things easier
	auto& ecs = CurrentRegistry();
	
	// REPLACED: TODO: remove comments for submission
	//// We sort our mesh renderers based on material properties
	//// This will group all of our meshes based on shader first, then material second
	//ecs.sort<MeshRenderer>([](const MeshRenderer& lhs, const MeshRenderer& rhs) {
	//	if (rhs.Material == nullptr || rhs.Mesh == nullptr)
	//		return false;
	//	else if (lhs.Material == nullptr || lhs.Mesh == nullptr)
	//		return true;
	//	else if (lhs.Material->GetShader() != rhs.Material->GetShader())
	//		return lhs.Material->GetShader() < rhs.Material->GetShader();
	//	else
	//		return lhs.Material < rhs.Material;
	//	});

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

	// SKYBOX //
	auto scene = CurrentScene();
	// Draw the skybox after everything else, if the scene has one
	if (scene->Skybox && enableSkybox)
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
			myCamera->GetView()
		)));
		scene->SkyboxShader->SetUniform("a_Projection", myCamera->Projection);

		scene->Skybox->Bind(0);
		scene->SkyboxShader->SetUniform("s_Skybox", 0); // binds our skybox to slot 0.
		scene->SkyboxMesh->Draw();

		// Restore our state
		glDepthMask(GL_TRUE);
		glEnable(GL_CULL_FACE);
		glDepthFunc(GL_LESS);
	}



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
			boundShader->SetUniform("a_CameraPos", myCamera->GetPosition());
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
		glm::mat4 worldTransform = transform.GetWorldTransform();
		// Our normal matrix is the inverse-transpose of our object's world rotation
		// Recall that everything's backwards in GLM
		glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(worldTransform)));

		// Update the MVP using the item's transform
		mat->GetShader()->SetUniform(
			"a_ModelViewProjection",
			myCamera->GetViewProjection() *
			worldTransform);
		// Update the model matrix to the item's world transform
		mat->GetShader()->SetUniform("a_Model", worldTransform);
		// Update the model matrix to the item's world transform
		mat->GetShader()->SetUniform("a_NormalMatrix", normalMatrix);


		// Draw the item
		if (renderer.Mesh->IsVisible())
		{
			// if the mesh is in weireframe mode, and the draw call isn't set to that already.
			if (renderer.Mesh->IsWireframe() != wireframe)
			{
				wireframe = !wireframe;

				// switches between wireframe mode and fill mode.
				(wireframe) ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}

			renderer.Mesh->Draw();
		}
	}
}

// returns the current m_Scene
std::string cherry::Game::getCurrentScene() const { return currentScene; }
