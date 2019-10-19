// GAME CLASS (SOURCE)

#include "Game.h"

#include <stdexcept>
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>
#include <GLM/gtc/matrix_transform.hpp>

// call this function to resize the window.
void GlfwWindowResizedCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);

	Game* game = (Game*)glfwGetWindowUserPointer(window);

	if (game != nullptr)
	{
		if (game->changeImageAspectOnWindowResize) // if the aspect ratio should change with the window
			game->HandleResize(width, height);
	}
}

// called when a key has been pressed, held down, or released. This function figures out which, and calls the appropriate function to handle it.
// KeyCallback(Window, Keyboard Key, Platform-Specific Scancode, Key Action, and Modifier Bits)
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Game* game = (Game*)glfwGetWindowUserPointer(window);

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

// key has been pressed
void Game::KeyPressed(GLFWwindow* window, int key)
{
	Game* game = (Game*)glfwGetWindowUserPointer(window);

	if (game == nullptr) // if game is 'null', then nothing happens
		return;

	// checks key value.
	switch (key)
	{
	case GLFW_KEY_SPACE:
		game->myCamera->SwitchViewMode();
		break;
	case GLFW_KEY_W:
		game->w = true;
		break;
	case GLFW_KEY_S:
		game->s = true;
		break;
	case GLFW_KEY_A:
		game->a = true;
		break;
	case GLFW_KEY_D:
		game->d = true;
		break;
	}
}

// key is being held
void Game::KeyHeld(GLFWwindow* window, int key)
{
	Game* game = (Game*)glfwGetWindowUserPointer(window);

	if (game == nullptr) // if game is 'null', then it is returned
		return;
}

// key hs been released
void Game::KeyReleased(GLFWwindow* window, int key)
{
	Game* game = (Game*)glfwGetWindowUserPointer(window);

	if (game == nullptr) // if game is 'null', then it is returned
		return;

	switch (key)
	{
	case GLFW_KEY_SPACE:
		game->myCamera->SwitchViewMode();
		break;
	case GLFW_KEY_W:
		game->w = false;
		break;
	case GLFW_KEY_S:
		game->s = false;
		break;
	case GLFW_KEY_A:
		game->a = false;
		break;
	case GLFW_KEY_D:
		game->d = false;
		break;
	}
}

// constructor
Game::Game() :
	myWindow(nullptr),
	myWindowTitle("Bonus Fruit Engine"),
	myClearColor(glm::vec4(0.1f, 0.7f, 0.5f, 1.0f))
{ }

// destructor
Game::~Game() { }

void Game::Initialize() {

	// Initialize GLFW
	if (glfwInit() == GLFW_FALSE) {
		std::cout << "Failed to initialize GLFW" << std::endl;
		throw std::runtime_error("Failed to initialize GLFW");
	}
	// Enable transparent backbuffers for our windows (note that Windows expects our colors to be pre-multiplied with alpha)
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, true);

	// Create a new GLFW window
	myWindow = glfwCreateWindow(850, 850, myWindowTitle, nullptr, nullptr);

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

	// Setting keyboard callback function
	glfwSetKeyCallback(myWindow, KeyCallback);
}

void Game::Shutdown() {
	glfwTerminate();
}

// loads the content for the meshes and shaders
void Game::LoadContent()
{
	// setting up the camera
	myCamera = std::make_shared<Camera>();
	myCamera->SetPosition(glm::vec3(0, 5, 10));
	myCamera->LookAt(glm::vec3(0));

	// sets the camera to perspective mode for the scene.
	//myCamera->SetPerspectiveMode(glm::perspective(glm::radians(60.0f), 1.0f, 0.01f, 1000.0f));
	myCamera->SetPerspectiveMode(glm::perspective(glm::radians(60.0f), 1.0f, 0.01f, 1000.0f));
	// myCamera->SetPerspectiveMode(glm::perspective(glm::radians(10.0f), 1.0f, 0.01f, 1000.0f));

	// sets the orthographic mode values. False is passed so that the camera starts in perspective mode.
	myCamera->SetOrthographicMode(glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.0f, 100.0f), false);

	// This is leftover code from the tutorials; myMesh isn't drawn to the screen, but it still gets set up.
	// Create our 4 vertices ~ we're using an initalizer list inside an initializer list to get the data
	Vertex vertices[4] = {
		// Position Color
		//  x      y	 z		   r	 g	   b	 a
		{{ -0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }},
		{{ 0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }},
		{{ -0.5f, 0.5f, 0.0f }, { 1.0f, 0.0f, 1.0f, 1.0f }},
		{{ 0.5f, 0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }},
	};

	// Create our 6 indices
	uint32_t indices[6] = {
	0, 1, 2,
	2, 1, 3
	};

	// Create a new mesh from the data; as I said, this isn't drawn.
	myMesh = std::make_shared<Mesh>(vertices, 4, indices, 0);

	// Creating the object, and stroing it in the vector.
	objects.push_back(new Object("res/cube.obj")); // cube
	// objects.push_back(new Object("res/sphere.obj")); // sphere
	playerObj = new Object("res/monkeyT.obj");
	
	// objects.push_back(new Object("res/monkey.obj")); // monkey

	// objects.push_back(new PrimitiveCube());
	//objects.push_back(new PrimitivePlane());
	// objects.push_back(new PrimitiveSphere(10, 5, 5));

	myMeshes.push_back(objects[objects.size() - 1]->getMesh()); // storing the mesh
	playerMesh = playerObj->getMesh();

	// Create and compile shader
	myShader = std::make_shared<Shader>();
	myShader->Load("res/shader.vert.glsl", "res/shader.frag.glsl");

	myModelTransform = glm::mat4(1.0f); // initializing the model matrix
}

void Game::UnloadContent() {
}

void Game::Update(float deltaTime) {

	float rotate_inc = glm::radians(20.0F); // saves the increment of the rotation in radians.

	// saves rotation to the model matrix.
	//myModelTransform = glm::rotate(myModelTransform, rotate_inc * deltaTime, glm::vec3(0, 0, 1));
	if (w) {
		playerObj->setPosition(glm::vec3(playerObj->getPosition().x, playerObj->getPosition().y - 0.05, playerObj->getPosition().z));
	}
	if (s) {
		playerObj->setPosition(glm::vec3(playerObj->getPosition().x, playerObj->getPosition().y + 0.05, playerObj->getPosition().z));
	}
	if (a) {
		playerObj->setPosition(glm::vec3(playerObj->getPosition().x + 0.05, playerObj->getPosition().y, playerObj->getPosition().z));
	}
	if (d) {
		playerObj->setPosition(glm::vec3(playerObj->getPosition().x - 0.05, playerObj->getPosition().y, playerObj->getPosition().z));
	}
}

void Game::InitImGui() {
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

void Game::ShutdownImGui() {
	// Cleanup the ImGui implementation
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	// Destroy our ImGui context
	ImGui::DestroyContext();
}

void Game::ImGuiNewFrame() {
	// Implementation new frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	// ImGui context new frame
	ImGui::NewFrame();
}

void Game::ImGuiEndFrame() {
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

void Game::Run()
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
void Game::HandleResize(int width, int height)
{
	// changing the camera modes to adjust for the new window size. 
	// The camera mode isn't changed, just it's values (i.e. if it's in perspective mode, it stays in perspective mode).
	myCamera->SetPerspectiveMode(glm::perspective(glm::radians(60.0f), width / (float)height, 0.01f, 1000.0f), myCamera->InPerspectiveMode());
	myCamera->SetOrthographicMode(glm::ortho(-5.0f * width / (float)height, 5.0f * width / (float)height, -5.0f, 5.0f, 0.0f, 100.0f), myCamera->InOrthographicMode());
}

void Game::Draw(float deltaTime) {
	// Clear our screen every frame
	glClearColor(myClearColor.x, myClearColor.y, myClearColor.z, myClearColor.w);
	glClear(GL_COLOR_BUFFER_BIT);

	myShader->Bind();

	glm::mat4 playerModelTransform = glm::mat4(1.0F);
	playerModelTransform = glm::translate(playerModelTransform, playerObj->getPosition());
	myShader->SetUniform("a_ModelViewProjection", myCamera->GetViewProjection() * playerModelTransform); // transforms the mesh.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	playerMesh->Draw();

	glm::mat4 tempModelTransform = glm::mat4(1.0F); // makes a copy of the transform matrix for making another copy of the mesh.
	//tempModelTransform = glm::translate(tempModelTransform, playerObj->getPosition());

	// translates the temporary model matrix
	tempModelTransform = glm::translate(tempModelTransform, glm::vec3(-4.0F, 0.0F, 0.0F)); // translates the model matrix.

	// copies the rotation factor from the original matrix into this one so that the two objects both rotate around their origins.
	tempModelTransform[0] = myModelTransform[0];
	tempModelTransform[1] = myModelTransform[1];

	// wireframe if needed
	

	// draws each mesh
	for (Mesh::Sptr mesh : myMeshes)
	{
		if (mesh == myMeshes[myMeshes.size() - 1]) {
			mesh->enableWireframe();
		}
		
		if (mesh->isWireframe()) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		myShader->SetUniform("a_ModelViewProjection", myCamera->GetViewProjection() * myModelTransform); // transforms the mesh.

		mesh->Draw(); // draws the mesh.

		//myShader->SetUniform("a_ModelViewProjection", myCamera->GetViewProjection() * myModelTransform); // transforms the mesh. //Moved above second draw because uniform needs to be set before object is redrawn
		myShader->SetUniform("a_ModelViewProjection", myCamera->GetViewProjection() * tempModelTransform); // moving the second drawing so that it's visible.

		mesh->Draw(); // re-drawing the mesh so that it shows up twice.

		//myShader->SetUniform("a_ModelViewProjection", myCamera->GetViewProjection() * tempModelTransform); // moving the second drawing so that it's visible. //Moved above first draw because uniform needs to be set before object is redrawn
		// mesh->Draw();
	}
}

void Game::DrawGui(float deltaTime) {
	// Open a new ImGui window
	ImGui::Begin("Colour Picker");

	// Draw widgets here
	// ImGui::SliderFloat4("Color", &myClearColor.x, 0, 1); // Original
	ImGui::ColorPicker4("Color", &myClearColor.x); // new version
	// ImGui::SetWindowSize(ImVec2(500.0F, 500.0F)); // window size for ImGUI Colour Picker
	if (ImGui::InputText("Title", myWindowTitle, 31))
	{
		glfwSetWindowTitle(myWindow, myWindowTitle);
	}

	if (ImGui::Button("Apply")) // adding another button, which allows for the application of the window title.
	{
		glfwSetWindowTitle(myWindow, myWindowTitle);
	}

	myCamera->SetPosition(glm::vec3(playerObj->getPosition().x, playerObj->getPosition().y + 5, playerObj->getPosition().z + 10));
	myCamera->LookAt(playerObj->getPosition()); //Looks at player
	// changing the camera mode
	std::string camMode = myCamera->InPerspectiveMode() ? "Perspective" : "Orthographic";
	ImGui::InputText((std::string("CAMERA MODE (\'SPACE\')") + camMode).c_str(), myWindowTitle, 32);

	ImGui::End();
}