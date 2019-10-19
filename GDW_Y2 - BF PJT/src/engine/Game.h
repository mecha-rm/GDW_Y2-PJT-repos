#pragma once
// GAME CLASS (HEADER)

// External Library Includes
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>

// File Includes
#include "Shader.h"
#include "Mesh.h"
#include "objects/Primitives.h"
#include "Camera.h" // camera

// System Library Includes
#include <iostream>
#include <vector>

class Game {
public:
	// constructor
	Game();

	// destructor
	~Game();

	// runs the game
	void Run();

	// handles resizing the window without skewing the objects in the scene.
	void HandleResize(int width, int height);

	// called when a key has been pressed
	void KeyPressed(GLFWwindow* window, int key);

	// called when a key is being held down
	void KeyHeld(GLFWwindow* window, int key);

	// called when a key has been released
	void KeyReleased(GLFWwindow* window, int key);

	// if 'true', then the objects keep their scale when the window is resized.
	// If false, the objects skew with the size of the window.
	bool changeImageAspectOnWindowResize = true;

	// the object used for the camera
	Camera::Sptr myCamera;

protected:
	void Initialize();

	void Shutdown();

	void LoadContent();

	void UnloadContent();

	void InitImGui();

	void ShutdownImGui();

	void ImGuiNewFrame();

	void ImGuiEndFrame();

	void Update(float deltaTime);

	void Draw(float deltaTime);

	void DrawGui(float deltaTime);

private:

	// Stores the main window that the game is running in
	GLFWwindow* myWindow;

	// Stores the clear color of the game's window
	glm::vec4 myClearColor;

	// Stores the title of the game's window
	char myWindowTitle[32];

	// A shared pointer to our mesh; this is no longer used, and has been replaced with a vector of meshes.
	Mesh::Sptr myMesh;

	// a vector of all the meshes in the scene. I'm using a vector for this project so that I can re-use this code later.
	std::vector<Mesh::Sptr> myMeshes;

	// a vector of the objects created for the game.
	std::vector<Object*> objects;

	//player mesh
	Mesh::Sptr playerMesh;

	//player object
	Object* playerObj;

	// A shared pointer to our shader.
	Shader::Sptr myShader;

	// Model transformation matrix
	glm::mat4 myModelTransform;

	glm::vec3 testPlayPos = glm::vec3(0, 0, 0);
	bool w, a, s, d = false;
};