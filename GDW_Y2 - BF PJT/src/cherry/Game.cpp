// GAME CLASS (SOURCE)

#include "Game.h"
#include "SceneManager.h"
#include "MeshRenderer.h"

#include "Physics.h"

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
void MouseButtomCallback(GLFWwindow* window, int button, int action, int mods) {
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

// called when a mouse button has been pressed
void cherry::Game::MouseButtonPressed(GLFWwindow* window, int button) {
	Game* game = (Game*)glfwGetWindowUserPointer(window);

	if (game == nullptr) // if game is 'null', then nothing happens
		return;

	// checks each button
	switch (button) {
	case GLFW_MOUSE_BUTTON_LEFT:
		game->mbLP = true;
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
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
		game->mbLR = true;
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		break;
	}
}

// called when the mouse moves over the screen
void CursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
	cherry::Game* game = (cherry::Game*)glfwGetWindowUserPointer(window);
	
	if (game == nullptr) {
		return;
	}
	else {
		game->UpdateCursorPos(xpos, ypos);
	}
}

// updates cursor pos variables
void cherry::Game::UpdateCursorPos(double xpos, double ypos) {
	Game* game = (Game*)glfwGetWindowUserPointer(myWindow);

	//this->XcursorPos = xpos;
	//this->YcursorPos = ypos;
	xpos = xpos - this->wX / 2;
	ypos = ypos - this->wY / 2;

	//Logger::GetLogger()->info(xpos);
	//Logger::GetLogger()->info(ypos);
	// update the player object's angle
	playerObj->updateAngle(*game->myCamera, xpos, ypos, this->wX, this->wY);
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
void cherry::Game::KeyHeld(GLFWwindow* window, int key)
{
	Game* game = (Game*)glfwGetWindowUserPointer(window);

	if (game == nullptr) // if game is 'null', then it is returned
		return;
}

// key hs been released
void cherry::Game::KeyReleased(GLFWwindow* window, int key)
{
	Game* game = (Game*)glfwGetWindowUserPointer(window);

	if (game == nullptr) // if game is 'null', then it is returned
		return;

	switch (key)
	{
	// case GLFW_KEY_SPACE:
		// game->myCamera->SwitchViewMode();
		// break;
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
cherry::Game::Game() :
	myWindow(nullptr),
	myWindowTitle("Bonus Fruit - Cherry Engine"),
	myClearColor(glm::vec4(0.1f, 0.7f, 0.5f, 1.0f))
{ }

// destructor
cherry::Game::~Game() { }

void cherry::Game::Initialize() {

	// Initialize GLFW
	if (glfwInit() == GLFW_FALSE) {
		std::cout << "Failed to initialize GLFW" << std::endl;
		throw std::runtime_error("Failed to initialize GLFW");
	}
	// Enable transparent backbuffers for our windows (note that Windows expects our colors to be pre-multiplied with alpha)
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, true);

	// Create a new GLFW window
	myWindow = glfwCreateWindow(this->wX, this->wY, myWindowTitle, nullptr, nullptr);

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

	// Setting cursor pos callback function
	glfwSetCursorPosCallback(myWindow, CursorPosCallback);

	// Setting mouse button callback function
	glfwSetMouseButtonCallback(myWindow, MouseButtomCallback);
}

void cherry::Game::Shutdown() {
	glfwTerminate();
}

// loads the content for the meshes and shaders
void cherry::Game::LoadContent()
{
	// setting up the camera
	myCamera = std::make_shared<Camera>();
	myCamera->SetPosition(glm::vec3(0, 5, 10));
	myCamera->LookAt(glm::vec3(0));

	// sets the camera to perspective mode for the scene.
	// myCamera->SetPerspectiveMode(glm::perspective(glm::radians(60.0f), 1.0f, 0.01f, 1000.0f));
	//myCamera->SetPerspectiveMode(glm::perspective(glm::radians(60.0f), 1.0f, 0.01f, 1000.0f));
	myCamera->SetPerspectiveMode(glm::perspective(glm::radians(60.0f), 1.0f, 0.01f, 1000.0f));
	// myCamera->SetPerspectiveMode(glm::perspective(glm::radians(10.0f), 1.0f, 0.01f, 1000.0f));

	// sets the orthographic mode values. False is passed so that the camera starts in perspective mode.
	myCamera->SetOrthographicMode(glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.0f, 100.0f), false);

	// This is leftover code from the tutorials; myMesh isn't drawn to the screen, but it still gets set up.
	// Create our 4 vertices ~ we're using an initalizer list inside an initializer list to get the data
	
	Vertex vertices[4] = {
		// Position				  Color							Normal
		//  x      y	 z		   r	 g	   b	 a		   x  y  z
		{{ -2.5f, -2.5f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }, {0, 0, 1}},
		{{ 2.5f, -2.5f, 0.0f },  { 1.0f, 1.0f, 0.0f, 1.0f }, {0, 0, 1}},
		{{ -2.5f, 2.5f, 0.0f },  { 1.0f, 0.0f, 1.0f, 1.0f }, {0, 0, 1}},
		{{ 2.5f, 2.5f, 0.0f },   { 0.0f, 1.0f, 0.0f, 1.0f }, {0, 0, 1}},
	};

	// Create our 6 indices
	uint32_t indices[6] = {
	0, 1, 2,
	2, 1, 3
	};

	// Create a new mesh from the data; as I said, this isn't drawn.
	// myMesh = std::make_shared<Mesh>(vertices, 4, indices, 0);

	// Creating the object, and stroing it in the vector.
	// objects.push_back(new Object("res/cube.obj")); // cube
	// objects.push_back(new Object("res/sphere.obj")); // sphere
	// playerObj = new Object("res/monkeyT.obj");
	playerObj = new PrimitiveSphere(3, 5, 5);
	playerMesh = playerObj->getMesh();
	// playerObj->addPhysicsBody(new cherry::PhysicsBodyBox(1.0F, 1.0F, 1.0F));

	objects.push_back(playerObj);

	// objects.push_back(new Object("res/monkey.obj")); // monkey

	// objects.push_back(new PrimitiveCube());
	// objects.push_back(new PrimitivePlane());
	// objects.push_back(new PrimitiveCube(2.5, 2.5, 2.5));
	// objects.at(objects.size() - 1)->setPosition(0.0, 0.0, 0.0F);
	// objects.push_back(new PrimitiveCube(2.5, 2.5, 2.5));
	// objects.at(objects.size() - 1)->setPosition(0.0, 0.0, 0.0F);
	// objects.push_back(new PrimitiveSphere(3, 5, 5));
	// objects.at(objects.size() - 1)->setPosition(1.5, 0.0, 0.0F);
    // objects.push_back(new PrimitiveSphere(10, 5, 5));

	// objects[objects.size() - 1]->setColor(12, 24, 111);
	// myMeshes.push_back(objects[objects.size() - 1]->getMesh()); // storing the mesh
	
	// before the mesh in the original code
	Shader::Sptr phong = std::make_shared<Shader>();
	phong->Load("res/lighting.vs.glsl", "res/blinn-phong.fs.glsl");

	Material::Sptr testMat = std::make_shared<Material>(phong);
	testMat->Set("a_LightPos", { 0, 0, 1 });
	testMat->Set("a_LightColor", { 1.0f, 1.0f, 0 });
	testMat->Set("a_AmbientColor", { 1.0f, 1.0f, 1.0f });
	testMat->Set("a_AmbientPower", 0.1f); // change this to change the main lighting power (originally value of 0.1F)
	testMat->Set("a_LightSpecPower", 0.5f);
	testMat->Set("a_LightShininess", 256);
	testMat->Set("a_LightAttenuation", 1.0f);

	SceneManager::RegisterScene("Test");
	SceneManager::RegisterScene("Test2");
	SceneManager::SetCurrentScene("Test");

	{
		// adds an entity to one of the scenes.
		auto& ecs = GetRegistry("Test");
		entt::entity e1 = ecs.create();
		MeshRenderer& m1 = ecs.assign<MeshRenderer>(e1);
		m1.Material = testMat;
		m1.Mesh = myMesh;

		auto rotate = [](entt::entity e, float dt) {
			auto& transform = CurrentRegistry().get_or_assign<TempTransform>(e);
			transform.EulerRotation += glm::vec3(0, 0, 90 * dt);

			// does the same thing, except all in one linel.
			// CurrentRegistry().get_or_assign<TempTransform>(e).EulerRotation += glm::vec3(0, 0, 90 * dt);
		};
		auto& up = ecs.get_or_assign<UpdateBehaviour>(e1);
		up.Function = rotate;
	}

	// Create and compile shader
	myShader = std::make_shared<Shader>();
	myShader->Load("res/shader.vert.glsl", "res/shader.frag.glsl");

	myModelTransform = glm::mat4(1.0f); // initializing the model matrix
}

void cherry::Game::UnloadContent() {
}

void cherry::Game::Update(float deltaTime) {

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

	// check if mouse left button is being held down
	if (this->dashTime >= 1.0f) {
		this->dash = true;
		this->dashTime = 0.0f;
	}
	else if (mbLP == true && mbLR == false) {
		this->dashTime += 0.01f;
		//Logger::GetLogger()->info(this->dashTime);
	}
	else if (mbLP == true && mbLR == true) {
		this->dashTime = 0.0f;
		//Logger::GetLogger()->info(this->dashTime);
		this->mbLP = false;
		this->mbLR = false;
	}

	// updates each object
	//for (cherry::Object* obj : objects)
	//{
	//	// obj->update();
	//	obj->setIntersection(false);
	//}
		
	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->update();
		objects[i]->setIntersection(false);
		// std::cout << "[" + std::to_string(i) + "] : " << *cherry::Vec3(objects[i]->getPosition()).v << std::endl;
	}

	// std::cout << std::endl;
	// collisions
	mainLoop:
	for (cherry::Object* obj1 : objects) // object 1
	{
		if (obj1 == nullptr)
			continue;
		if (obj1->getIntersection() == true) // already colliding with something.
			continue;

		for (cherry::Object* obj2 : objects) // object 2
		{
			if (obj1 == obj2 || obj2 == nullptr) // if the two objects are the same.
				continue;

			if (obj2->getIntersection() == true) // if the object is already intersecting with something.
				continue;

			// gets the vectors from both objects
			std::vector<cherry::PhysicsBody*> pbods1 = obj1->getPhysicsBodies();
			std::vector<cherry::PhysicsBody*> pbods2 = obj2->getPhysicsBodies();

			// goes through each collision body
			for (cherry::PhysicsBody* pb1 : pbods1)
			{
				for (cherry::PhysicsBody* pb2 : pbods2)
				{
					bool col = PhysicsBody::Collision(pb1, pb2);
					
					if (col == true) // if collision has occurred.
					{
						obj1->setIntersection(true);
						// obj1->setColor(255, 0, 0);
						obj2->setIntersection(true);
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
void cherry::Game::HandleResize(int width, int height)
{
	// changing the camera modes to adjust for the new window size. 
	// The camera mode isn't changed, just it's values (i.e. if it's in perspective mode, it stays in perspective mode).
	myCamera->SetPerspectiveMode(glm::perspective(glm::radians(60.0f), width / (float)height, 0.01f, 1000.0f), myCamera->InPerspectiveMode());
	myCamera->SetOrthographicMode(glm::ortho(-5.0f * width / (float)height, 5.0f * width / (float)height, -5.0f, 5.0f, 0.0f, 100.0f), myCamera->InOrthographicMode());
}

void cherry::Game::Draw(float deltaTime) {
	// Clear our screen every frame
	glClearColor(myClearColor.x, myClearColor.y, myClearColor.z, myClearColor.w);
	glClear(GL_COLOR_BUFFER_BIT);

	myShader->Bind();

	glm::mat4 playerModelTransform = glm::mat4(1.0F);
	
	if (this->dash) {
		glm::vec3 dash = playerObj->getDash(0.05f);
		//Logger::GetLogger()->info(playerObj->getPosition().x);
		//Logger::GetLogger()->info(playerObj->getPosition().y);
		//Logger::GetLogger()->info(playerObj->getPosition().z);
		playerObj->setPosition(playerObj->getPosition() + dash);
		this->dash = false;
	}
	playerModelTransform = glm::translate(playerModelTransform, playerObj->getPosition());
	playerModelTransform = glm::rotate(playerModelTransform, glm::radians(playerObj->getDegreeAngle()), glm::vec3(0, 1, 0));

	myShader->SetUniform("a_ModelViewProjection", myCamera->GetViewProjection() * playerModelTransform); // transforms the mesh.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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
		/*if (mesh == myMeshes[myMeshes.size() - 1]) {
			mesh->enableWireframe();
		}*/
		
		if (mesh->isWireframeMode()) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		myShader->SetUniform("a_ModelViewProjection", myCamera->GetViewProjection() * myModelTransform); // transforms the mesh.

		// mesh->Draw(); // draws the mesh.

		//myShader->SetUniform("a_ModelViewProjection", myCamera->GetViewProjection() * myModelTransform); // transforms the mesh. //Moved above second draw because uniform needs to be set before object is redrawn
		myShader->SetUniform("a_ModelViewProjection", myCamera->GetViewProjection() * tempModelTransform); // moving the second drawing so that it's visible.

		mesh->Draw(); // re-drawing the mesh so that it shows up twice.

		// myShader->SetUniform("a_ModelViewProjection", myCamera->GetViewProjection() * tempModelTransform); // moving the second drawing so that it's visible. //Moved above first draw because uniform needs to be set before object is redrawn
		// mesh->Draw();
	}

	// goes through each object and gets their meshes.
	for (cherry::Object* obj : objects)
	{
		/*if (mesh == myMeshes[myMeshes.size() - 1]) {
			mesh->enableWireframeMode();
		}*/

		if (obj->getMesh()->isWireframeMode()) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else if (!obj->getMesh()->isWireframeMode()) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		// std::cout << "Position: (" << playerObj->getPosition().x << ", " << playerObj->getPosition().y << ") " << std::endl;

		// myShader->SetUniform("a_ModelViewProjection", myCamera->GetViewProjection() * myModelTransform); // transforms the mesh.

		// obj->getMesh()->Draw(); // draws the mesh.

		//myShader->SetUniform("a_ModelViewProjection", myCamera->GetViewProjection() * myModelTransform); // transforms the mesh. //Moved above second draw because uniform needs to be set before object is redrawn
		myShader->SetUniform("a_ModelViewProjection", myCamera->GetViewProjection() * tempModelTransform); // moving the second drawing so that it's visible.

		obj->getMesh()->Draw(); // re-drawing the mesh so that it shows up twice.

		
	}
	// TODO : finish lighting setup
	//// We'll grab a reference to the ecs to make things easier
	//auto& ecs = CurrentRegistry();
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

	//// These will keep track of the current shader and material that we have bound
	//Material::Sptr mat = nullptr;
	//Shader::Sptr boundShader = nullptr;
	//// A view will let us iterate over all of our entities that have the given component types
	//auto view = ecs.view<MeshRenderer>();

	//for (const auto& entity : view) {
	//	// Get our shader
	//	const MeshRenderer& renderer = ecs.get<MeshRenderer>(entity);
	//	// Early bail if mesh is invalid
	//	if (renderer.Mesh == nullptr || renderer.Material == nullptr)
	//		continue;
	//	// If our shader has changed, we need to bind it and update our frame-level uniforms
	//	if (renderer.Material->GetShader() != boundShader) {
	//		boundShader = renderer.Material->GetShader();
	//		boundShader->Bind();
	//		boundShader->SetUniform("a_CameraPos", myCamera->GetPosition());
	//	}
	//	// If our material has changed, we need to apply it to the shader
	//	if (renderer.Material != mat) {
	//		mat = renderer.Material;
	//		mat->Apply();
	//	}

	//	// We'll need some info about the entities position in the world
	//	const TempTransform& transform = ecs.get_or_assign<TempTransform>(entity);
	//	// Get the object's transformation
	//	glm::mat4 worldTransform = transform.GetWorldTransform();
	//	// Our normal matrix is the inverse-transpose of our object's world rotation
	//	// Recall that everything's backwards in GLM
	//	glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(worldTransform)));

	//	// Update the MVP using the item's transform
	//	mat->GetShader()->SetUniform(
	//		"a_ModelViewProjection",
	//		myCamera->GetViewProjection() *
	//		worldTransform);
	//	// Update the model matrix to the item's world transform
	//	mat->GetShader()->SetUniform("a_Model", worldTransform);
	//	// Update the model matrix to the item's world transform
	//	mat->GetShader()->SetUniform("a_NormalMatrix", normalMatrix);
	//	// Draw the item
	//	renderer.Mesh->Draw();
	//}
}

void cherry::Game::DrawGui(float deltaTime) {
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
		for (cherry::Mesh::Sptr mesh : myMeshes)
			mesh->setWireframeMode(!mesh->isWireframeMode());

		for (cherry::Object* obj : objects)
			obj->getMesh()->setWireframeMode(!obj->getMesh()->isWireframeMode());
	}

	myCamera->SetPosition(glm::vec3(playerObj->getPosition().x, playerObj->getPosition().y + 5, playerObj->getPosition().z + 10));
	myCamera->LookAt(playerObj->getPosition()); //Looks at player
	// changing the camera mode
	std::string camMode = myCamera->InPerspectiveMode() ? "Perspective" : "Orthographic";
	ImGui::InputText((std::string("CAMERA MODE (\'SPACE\')") + camMode).c_str(), myWindowTitle, 32);

	ImGui::End();
}