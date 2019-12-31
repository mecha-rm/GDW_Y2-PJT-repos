#pragma once
// GAME CLASS (HEADER)

// External Library Includes
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>

// File Includes
#include "Shader.h"
#include "Mesh.h"
#include "objects/ObjectManager.h"
#include "Camera.h" // camera
#include "LightManager.h"

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
		// variable '_default' opens the project with default settings for the camera, sceneLists, and more.
		Game(const char windowTitle[32], float _width, float _height, bool _fullScreen, bool _defaults = false, bool _debug = false);

		// destructor
		~Game();

		// gets the window width
		float GetWindowWidth() const;

		// gets the window height
		float GetWindowHeight() const;

		// gets whether the window is full-screen or not. 
		bool IsFullScreen() const;

		// resize has been moved to the bottom of the code to be accurate to where it is in the framework.


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

		// called when a mouse button is being held.
		virtual void MouseButtonHeld(GLFWwindow* window, int button);

		// called when a mouse button has been pressed
		virtual void MouseButtonReleased(GLFWwindow* window, int button);


		// called when a key has been pressed
		virtual void KeyPressed(GLFWwindow* window, int key);

		// called when a key is being held down
		virtual void KeyHeld(GLFWwindow* window, int key);

		// called when a key has been released
		virtual void KeyReleased(GLFWwindow* window, int key);

		// adds an object to the m_Scene. Only call this if the object being passed already has a m_Scene registered.
		// if false is returned, then the object is already in the m_Scene.
		bool AddObject(cherry::Object* obj);

		// adds an object to the current registry of the game.
		bool AddObject(cherry::Object* obj, std::string scene);

		// TODO: rename to DeleteObject?
		// removes an object from the game. If a 'false' is returned, then the object was never in the m_Scene.
		bool RemoveObject(cherry::Object* obj);

		// replace with object manager
		// gets an object from the current scene
		cherry::Object* GetSceneObject(unsigned int index) const;

		// gets an object from the provided scene
		cherry::Object * GetSceneObject(unsigned int index, std::string scene) const;

		// gets a scene object, finding it via its name (must be in the current scene)
		cherry::Object* GetSceneObjectByName(std::string name) const;

		// gets the total amount of sceneLists
		unsigned int GetObjectCount() const;

		// runs the game
		void Run();

		// handles resizing the window without skewing the sceneLists in the m_Scene.
		void Resize(int newWidth, int newHeight);

		// if 'true', then the sceneLists keep their scale when the window is resized.
		// If false, the sceneLists skew with the size of the window.
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

		// used for rendering the scene to multiple viewpoints.
		void __RenderScene(glm::ivec4 viewport, Camera::Sptr camera);

		// gets the current m_Scene
		std::string GetCurrentScene() const;


		// set to 'true' for debug functionality.
		bool debugMode = false;

		// list of scenes
		std::vector<std::string> scenes;

		// the m_Scene material
		Material::Sptr matStatic; // the static material
		Material::Sptr matDynamic; // the dynamic material
		 
		SamplerDesc description; // texture description 
		TextureSampler::Sptr sampler; // texture sampler

		glm::ivec2 myWindowSize; // saves the window size

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

		// A shared pointer to our shader.
		Shader::Sptr myShader;

		std::string currentScene = ""; // the current m_Scene

		// a vector of the sceneLists created for the game.
		// std::vector<Object*> objects;

		// object manager
		std::shared_ptr<cherry::ObjectManager> objManager;

		// object list
		cherry::ObjectList* objList = nullptr;

		// the lights in the current scene
		// std::vector<Light*>* lights; // TODO: replace with light manager

		// light manager
		std::shared_ptr<cherry::LightManager> lightManager;

		// holds the list of lights
		cherry::LightList* lightList;

		// Model transformation matrix
		glm::mat4 myModelTransform;

		// if 'loadDefaults' is true, then default sceneLists will be loaded up
		bool loadDefaults = false;

		// enables the skybox. TODO: change for final build.
		bool enableSkybox = false;

		// movement
		bool w = false, a = false, s = false, d = false;

		// gets the cursor position
		cherry::Vec2 mousePos;

		// double XcursorPos, YcursorPos;
		bool mbLeft = false, mbMiddle = false, mbRight = false;

		// window size
		// unsigned int windowWidth = 850;
		// unsigned int windowHeight = 850;
		
		// boolean for full screen
		bool fullScreen;
		
		// returns 'true' if the mouse is in the window content, false otherwise.
		bool mouseEnter = false;

		unsigned int hitBoxIndex = -1;
	};


}
