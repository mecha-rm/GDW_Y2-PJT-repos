// GAME CLASS (SOURCE)

#include "Game.h"
#include "SceneManager.h"
#include "MeshRenderer.h"
#include "Texture2D.h"

#include "PhysicsBody.h"
#include "utils/Utils.h"

#include <stdexcept>
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>
#include <GLM/gtc/matrix_transform.hpp>
#include <toolkit/Logging.h>

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

// call this function to resize the window.
void GlfwWindowResizedCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);

	cherry::Game* game = (cherry::Game*)glfwGetWindowUserPointer(window);

	if (game != nullptr)
	{
		if (game->changeImageAspectOnWindowResize) // if the aspect ratio should change with the window
			game->HandleResize(width, height);
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
	myWindowTitle("Bonus Fruit - Cherry Engine"),
	myClearColor(glm::vec4(0.1f, 0.7f, 0.5f, 1.0f))
{ }

// creates window with a width, height, and whether or not it's in full screen.
cherry::Game::Game(float _width, float _height, bool _fullScreen, bool _defaults, bool _debug) : Game()
{
	// setting the values
	windowWidth = _width;
	windowHeight = _height;
	fullScreen = _fullScreen;
	loadDefaults = _defaults; // loads the engine default values
	debugMode = _debug; // debug functionality.
}

// destructor
cherry::Game::~Game() { }

// gets the window width
float cherry::Game::GetWindowWidth() const { return windowWidth; }

// gets the window height
float cherry::Game::GetWindowHeight() const { return windowHeight; }

// shows whether the window is in full screen or not
bool cherry::Game::IsFullScreen() const { return fullScreen; }

// resizes the window without skewing the objects, and changes the cameras accordingly.
void cherry::Game::HandleResize(int width, int height)
{
	// changing the camera modes to adjust for the new window size. 
	// The camera mode isn't changed, just it's values (i.e. if it's in perspective mode, it stays in perspective mode).
	myCamera->SetPerspectiveMode(glm::perspective(glm::radians(60.0f), width / (float)height, 0.01f, 1000.0f), myCamera->InPerspectiveMode());
	myCamera->SetOrthographicMode(glm::ortho(-5.0f * width / (float)height, 5.0f * width / (float)height, -5.0f, 5.0f, 0.0f, 100.0f), myCamera->InOrthographicMode());
}


// gets whether the cursor is in the window content or not.
bool cherry::Game::GetCursorInWindow() const { return mouseEnter; }

// sets whether the cursor is in the window.
void cherry::Game::SetCursorInWindow(bool inWindow) { mouseEnter = inWindow; }

// updates cursor position variables
void cherry::Game::UpdateCursorPos(double xpos, double ypos)
{
	// Game* game = (Game*)glfwGetWindowUserPointer(myWindow);

	mousePos = cherry::Vec2(xpos - this->windowWidth / 2.0F, ypos - this->windowHeight / 2.0F);

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
		myWindow = glfwCreateWindow(this->windowWidth, this->windowHeight, myWindowTitle, glfwGetPrimaryMonitor(), nullptr);

	else // no full screen
		myWindow = glfwCreateWindow(this->windowWidth, this->windowHeight, myWindowTitle, nullptr, nullptr);

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
	myCamera->SetPerspectiveMode(glm::perspective(glm::radians(60.0f), 1.0f, 0.01f, 1000.0f));
	// myCamera->SetPerspectiveMode(glm::perspective(glm::radians(10.0f), 1.0f, 0.01f, 1000.0f));

	// sets the orthographic mode values. False is passed so that the camera starts in perspective mode.
	myCamera->SetOrthographicMode(glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.0f, 100.0f), false);

	// before the mesh in the original code
	Shader::Sptr phong = std::make_shared<Shader>();
	// TODO: make version without UVs?
	phong->Load("res/lighting.vs.glsl", "res/blinn-phong.fs.glsl");

	// TODO: change this so that it uses the light manager.
	// used to make the albedo
	Texture2D::Sptr albedo = Texture2D::LoadFromFile("res/blank.png");
	material = std::make_shared<Material>(phong);
	material->Set("a_LightPos", { 0, 0, 1 });
	material->Set("a_LightColor", { 1.0f, 1.0f, 0 });
	material->Set("a_AmbientColor", { 0.2f, 0.5f, 0.01f });
	material->Set("a_AmbientPower", 0.9f); // change this to change the main lighting power (originally value of 0.1F)
	material->Set("a_LightSpecPower", 0.5f);
	material->Set("a_LightShininess", 256);
	material->Set("a_LightAttenuation", 1.0f);
	material->Set("s_Albedo", albedo); // objects will just be blank if no texture is set.

	currentScene = "Cherry"; // the name of the m_Scene
	scenes.push_back(currentScene); // saving the m_Scene

	//lights.push_back(new Light(currentScene, glm::vec3( 0.0F, 0.0F, 1.0F ), glm::vec3( 1.0F, 1.0F, 0.0F ), glm::vec3(0.2F, 0.5F, 0.01F
	//	), 0.9F, 0.5F, 256, 1.0F));
	//material = lights[0]->GenerateMaterial();

	SceneManager::RegisterScene(currentScene); // registering the m_Scene
	// SceneManager::RegisterScene("Test2");
	SceneManager::SetCurrentScene(currentScene);


	// Create and compile shader
	myShader = std::make_shared<Shader>();
	myShader->Load("res/shader.vert.glsl", "res/shader.frag.glsl");

	myModelTransform = glm::mat4(1.0f); // initializing the model matrix

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

		objects.push_back(new PrimitiveSphere());
		objects.at(objects.size() - 1)->CreateEntity(currentScene, material);
		objects.at(objects.size() - 1)->SetPosition(offset, -offset, 0.0F);

		objects.push_back(new PrimitivePlane());
		objects.at(objects.size() - 1)->CreateEntity(currentScene, material);
		objects.at(objects.size() - 1)->SetPosition(offset, 0.0F, 0.0F);
	}
}

void cherry::Game::UnloadContent() {
}

void cherry::Game::Update(float deltaTime) {

	glm::vec3 camTranslate{}; // movement for the camera this given frame.
	float camTransInc = 0.05F; // increment for camera movement

	// called to Update the position and rotation of hte objects.
	// calling all of our functions for our Update behaviours.
	auto view = CurrentRegistry().view<UpdateBehaviour>();
	for (const auto& e : view) {
		auto& func = CurrentRegistry().get<UpdateBehaviour>(e);
		if (func.Function) {
			func.Function(e, deltaTime);
		}
	}

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

	glEnable(GL_DEPTH_TEST);
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


void cherry::Game::Draw(float deltaTime) {

	static bool wireframe = false; // used to switch between fill mode and wireframe mode for draw calls.

	// Clear our screen every frame
	glClearColor(myClearColor.x, myClearColor.y, myClearColor.z, myClearColor.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	myShader->Bind();


	// We'll grab a reference to the ecs to make things easier
	auto& ecs = CurrentRegistry();
	// We sort our mesh renderers based on material properties
	// This will group all of our meshes based on shader first, then material second
	ecs.sort<MeshRenderer>([](const MeshRenderer& lhs, const MeshRenderer& rhs) {
		if (rhs.Material == nullptr || rhs.Mesh == nullptr)
			return false;
		else if (lhs.Material == nullptr || lhs.Mesh == nullptr)
			return true;
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
			boundShader->SetUniform("a_CameraPos", myCamera->GetPosition());
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

// returns the current m_Scene
std::string cherry::Game::getCurrentScene() const { return currentScene; }
