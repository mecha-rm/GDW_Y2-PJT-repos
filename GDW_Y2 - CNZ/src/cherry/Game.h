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

namespace cherry
{
	class Game {
	public:
		// constructor
		Game();

		// creates the game with a width, height, and in fullscreen if requested.
		// _debug is used to start the game in debug mode.
		// variable '_default' opens the project with default settings for the camera, objects, and more.
		Game(float _width, float _height, bool _fullScreen, bool _defaults = false, bool _debug = false);

		// destructor
		~Game();

		// gets the window width
		float GetWindowWidth() const;

		// gets the window height
		float GetWindowHeight() const;

		// gets whether the window is full-screen or not. 
		bool IsFullScreen() const;

		// handles resizing the window without skewing the objects in the scene.
		void HandleResize(int width, int height);


		// returns 'true' if the cursor is in the window content, and false if it's not.
		bool GetCursorInWindow() const;

		// called to set whether or not the mouse cursor is in the window. This is excluively for the glfwCursorEnterCallback.
		void SetCursorInWindow(bool inWindow);

		// gets the cursor position as a cherry::Vec2
		cherry::Vec2 GetCursorPos() const;

		// gets the current cursor position as a glm vector
		glm::vec2 GetCursorPosGLM() const;

		// gets the cursor position on the x-axis
		float GetCursorPosX() const;

		// gets the cursor position on the y-axis
		float GetCursorPosY() const;

		// updates the cursor position variables when callback fuciton is called
		virtual void UpdateCursorPos(double xpos, double ypos);
		

		// called when a mouse button has been pressed
		virtual void MouseButtonPressed(GLFWwindow* window, int button);

		// called when a mouse button has been pressed
		virtual void MouseButtonReleased(GLFWwindow* window, int button);


		// called when a key has been pressed
		virtual void KeyPressed(GLFWwindow* window, int key);

		// called when a key is being held down
		virtual void KeyHeld(GLFWwindow* window, int key);

		// called when a key has been released
		virtual void KeyReleased(GLFWwindow* window, int key);

		// adds an object to the scene. Only call this if the object being passed already has a scene registered.
		// if false is returned, then the object is already in the scene.
		bool addObject(cherry::Object* obj);

		// adds an object to the current registry of the game.
		bool addObject(cherry::Object* obj, std::string scene);

		// removes an object from the game. If a 'false' is returned, then the object was never in the scene.
		bool removeObject(cherry::Object* obj);

		// runs the game
		void Run();

		// if 'true', then the objects keep their scale when the window is resized.
		// If false, the objects skew with the size of the window.
		bool changeImageAspectOnWindowResize = true;

		// the object used for the camera
		Camera::Sptr myCamera;
		

	protected:
		void Initialize();

		void Shutdown();

		virtual void LoadContent();

		void UnloadContent();

		void InitImGui();

		void ShutdownImGui();

		void ImGuiNewFrame();

		void ImGuiEndFrame();

		virtual void Update(float deltaTime);

		void Draw(float deltaTime);

		void DrawGui(float deltaTime);

		// gets the current scene
		std::string getCurrentScene() const;

		// set to 'true' for debug functionality.
		bool debugMode = false;

		// list of scenes
		std::vector<std::string> scenes;

		// the scene material
		Material::Sptr material;

	private:

		// Stores the main window that the game is running in
		GLFWwindow* myWindow;

		// static glm::vec2 resolution;

		// Stores the clear color of the game's window
		glm::vec4 myClearColor;

		// Stores the title of the game's window
		char myWindowTitle[32];

		// the camera position
		cherry::Vec3 cameraPos;

		// A shared pointer to our mesh; this is no longer used, and has been replaced with a vector of meshes.
		// Mesh::Sptr myMesh;

		// a vector of all the meshes in the scene. I'm using a vector for this project so that I can re-use this code later.
		// std::vector<Mesh::Sptr> myMeshes;

		// player mesh
		// Mesh::Sptr playerMesh;

		//player object
		// Object* playerObj;

		// A shared pointer to our shader.
		Shader::Sptr myShader;

		std::string currentScene = ""; // the current scene

		// a vector of the objects created for the game.
		std::vector<Object*> objects;

		// Model transformation matrix
		glm::mat4 myModelTransform;

		// if 'loadDefaults' is true, then default objects will be loaded up
		bool loadDefaults = false;

		// movement
		bool w = false, a = false, s = false, d = false;

		// gets the cursor position
		cherry::Vec2 mousePos;

		// double XcursorPos, YcursorPos;
		bool mbLeft = false, mbMiddle = false, mbRight = false;

		// window size
		unsigned int windowWidth = 850;
		unsigned int windowHeight = 850;
		
		// boolean for full screen
		bool fullScreen;
		
		// returns 'true' if the mouse is in the window content, false otherwise.
		bool mouseEnter = false;

		
	};


}
