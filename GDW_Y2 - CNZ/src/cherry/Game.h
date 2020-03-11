#pragma once
// GAME CLASS (HEADER)
#define WINDOW_TITLE_CHAR_MAX 50

// External Library Includes
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>

// File Includes
#include "Camera.h" // camera
#include "Shader.h"
#include "Mesh.h"

// managers
#include "scenes/SceneManager.h"
#include "Skybox.h"
#include "objects/ObjectManager.h"
#include "lights/LightManager.h"
#include "audio/AudioComponent.h"

// post-processing
#include "post/PostLayer.h"

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
		// if _imgui is 'true', then the _imgui functions are used.
		Game(const char windowTitle[WINDOW_TITLE_CHAR_MAX], float _width, float _height, bool _fullScreen, 
			cherry::Scene * _openingScene = nullptr, bool _imgui = false);

		// destructor
		~Game();

		// returns the title of the game window. It's maximum letter count is determined by WINDOW_TITLE_CHAR_MAX
		const std::string GetWindowTitle() const;

		// gets the length of the window title.
		// if this is to be called multiple times, it's recommended that its value be saved in a variable instead.
		int GetWindowTitleLength() const;

		// gets the GLFW window.
		GLFWwindow* GetWindow() const;

		// gets the window width
		int GetWindowWidth() const;

		// gets the window height
		int GetWindowHeight() const;

		// gets the size of the window.
		glm::ivec2 GetWindowSize() const;

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
		glm::dvec2 GetCursorPosGLM() const;

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

		// creates a scene. This is the static verison, so it cannot set the current scene.
		static bool CreateScene(const std::string sceneName);

		// creates a scene with a provided skybox. This is the static verison, so it cannot set the current scene.
		static bool CreateScene(const std::string sceneName, const cherry::Skybox skybox);

		// creates a scene. If 'makeCurrent' is true, then this scene is made the current scene.
		// if the scene already exists, a false is returned. It will not be set to the current scene.
		bool CreateScene(const std::string sceneName, const bool makeCurrent);

		// nothing false if the scene already exists, but will still set it to the current scene if possible.
		// if the scene already exists, a false is returned. It will not be set to the current scene.
		bool CreateScene(const std::string sceneName, const cherry::Skybox skybox, const bool makeCurrent);

		// adds a scene, and registers it as being part of the game.
		static bool RegisterScene(cherry::Scene* scene);

		// registered a scene with a provided skybox. This is the static verison, so it cannot set the current scene.
		static bool RegisterScene(cherry::Scene * scene, const cherry::Skybox skybox);

		// adds the scene using a pre-existing scene object and makes it current if requested.
		// if the scene is already registered, a false is returned. It will not be set to the current scene.
		bool RegisterScene(cherry::Scene* scene, const bool makeCurrent);

		// adds  the scene using a pre-existing scene object.
		// if the scene is already registered, a false is returned. It will not be set to the current scene.
		bool RegisterScene(cherry::Scene* scene, const cherry::Skybox skybox, const bool makeCurrent);

		

		// gets the current scene.
		cherry::Scene* GetScene(std::string sceneName) const;

		// gets the current scene.
		cherry::Scene* GetCurrentScene() const;

		// sets the current scene. If the scene doesn't exist, then nothing happens.
		// if 'createScene' is true, then a new scene is made if it doesn't exist, which causes 'true' to be returned.
		bool SetCurrentScene(std::string sceneName, bool createScene);

		// returns the name of the current scene. If the scene doesn't exist, an empty string of "" is returned.
		const std::string & GetCurrentSceneName() const;

		// destroys all scenes.
		void DestroyScenes();


		// sets the skybox for the current scene, and whether it should be visible or not.
		void SetSkybox(cherry::Skybox & skybox, const bool visible = true);

		// sets the skybox for the provided scene, and whether it should be visible or not.
		void SetSkybox(cherry::Skybox& skybox, const std::string sceneName, const bool visible = true);

		// gets whether the skybox is visible for the current scene or not.
		bool GetSkyboxVisible() const;

		// gets whether the skybox is visible in the provided scene or not.
		bool GetSkyboxVisible(std::string sceneName) const;

		// changes whether the skybox is visible or not for the current scene.
		void SetSkyboxVisible(bool skybox);

		// changes whether the skybox is visible or not for the provided scene.
		void SetSkyboxVisible(bool skybox, std::string sceneName);

		// gets the total amount of sceneLists
		unsigned int GetObjectCount() const;

		// gets the object list for the scene.
		cherry::ObjectList * GetSceneObjectList() const;

		// returns the object list for the provided scene.
		cherry::ObjectList* GetSceneObjectList(std::string scene);

		// replace with object manager
		// gets an object from the current scene
		cherry::Object * GetCurrentSceneObjectByIndex(unsigned int index) const;

		// gets an object from the provided scene
		cherry::Object * GetSceneObjectByIndex(std::string scene, unsigned int index) const;

		// gets a scene object, finding it via its name (must be in the current scene)
		cherry::Object* GetCurrentSceneObjectByName(std::string name) const;

		// gets a scene object from the provided scene, finding it via its name (must be in the current scene)
		cherry::Object* GetSceneObjectByName(std::string scene, std::string name) const;

		// TODO: add function; adds an object to the requested scen's list.
		// bool AddObjectToScene(std::string sceneName, cherry::Object* obj);

		// adds an object to the scene stored in it. Only call this if the object has already been given a scene..
		// if false is returned, then the object is already in the requested scene, or the scene didn't exist.
		bool AddObjectToScene(cherry::Object* obj);

		// TODO: rename to DeleteObject?
		// removes an object from the game. If a 'false' is returned, then the object was never in the m_Scene.
		bool DeleteObjectFromScene(cherry::Object* obj);

		// LIGHTS
		// gets the light list for the current scene.
		cherry::LightList* GetSceneLightList() const;

		// gets the light list for the provided scene.
		cherry::LightList* GetSceneLightList(std::string sceneName);

		// adds a light to the scene
		bool AddLightToScene(cherry::Light * light);

		// removes a light from the scene.
		bool DeleteLightFromScene(cherry::Light * light);

		// gets the game object that's currently running.
		// if 'nullptr' is returned, then no game is running.
		static cherry::Game* const GetRunningGame();

		// runs the game
		void Run();

		// handles resizing the window without skewing the sceneLists in the m_Scene.
		void Resize(int newWidth, int newHeight);

		// the frame rate of the game.
		// set the frame rate to 0 (or anything less), to have no framerate cap.
		static short int FPS;

		// the scene created on start up.
		std::string startupScene = "";

		// if 'true', then the sceneLists keep their scale when the window is resized.
		// If false, the sceneLists skew with the size of the window.
		bool changeImageAspectOnWindowResize = true;
		 
		// the object used for the camera, which ALWAYs has a viewport of the screen size.
		// if you do not want this camera to be used, set myCameraEnabled to false.
		Camera::Sptr myCamera;

		// if 'true', the full-screen camera (myCamera) gets used. If false, then that camera is not used.
		bool myCameraEnabled = true;

		// the secondary camera, which is used for overlaying a hud.
		Camera::Sptr myCameraX;

		// extra cameras.
		std::vector<Camera::Sptr> exCameras;

		// TODO: make private?
		// audio component for the scene
		cherry::AudioComponent audioEngine = cherry::AudioComponent();

		// stores the main clear color of the game's window
		// each camera has its own clear colour, which can be set to this value if it should remain the same for all cameras.
		glm::vec4 myClearColor;

		// if 'true', the  imgui window functions are used.
		// if false, then they are not used.
		bool imguiMode = false;

	protected:
		void Initialize();

		void Shutdown();

		virtual void LoadContent();

		void UnloadContent();

		void InitImGui();

		void ShutdownImGui();

		void ImGuiNewFrame();

		void ImGuiEndFrame();

		// update loop
		virtual void Update(float deltaTime);

		// draw loop
		void Draw(float deltaTime);

		// draw ImGUI
		virtual void DrawGui(float deltaTime);

		// renders the scene. It calls the other __RenderScene and takes in the values saved to the camera.
		void __RenderScene(const Camera::Sptr& camera);

		/*
		 * used for rendering the scene to multiple viewpoints.
		 * Variables:
			* viewport: the size of the viewport.
			* camera: the camera to be used for rendering.
			* drawSkybox: if 'true', the skybox is drawn. Since each registry has its own camera, it's recommended that this is only used once.
				* Do note that if there is no skybox or if scene->SkyboxMesh->IsVisible() returns 'false', the skybox won't be rendered anyway.
			* clear: if 'true', then anything previously rendered is cleared, which is needed for the border.
		*/
		void __RenderScene(glm::ivec4 viewport, const Camera::Sptr& camera, bool drawSkybox = true,
			int borderSize = 0, glm::vec4 borderColor = glm::vec4(1.0F, 1.0F, 1.0F, 1.0F), bool clear = true);


		// list of scenes
		// std::vector<std::string> scenes;

	private:

		// Stores the main window that the game is running in
		GLFWwindow* myWindow;

		glm::ivec2 myWindowSize; // saves the window size

		// static glm::vec2 resolution;

		// Stores the title of the game's window
		char myWindowTitle[WINDOW_TITLE_CHAR_MAX];

		// saves the game that's currently running.
		static cherry::Game* runningGame;

		// A shared pointer to our shader.
		// Shader::Sptr myShader;

		// the opening scene of the game.
		Scene* openingScene = nullptr;

		// object list
		cherry::ObjectList* objectList = nullptr;

		// light list
		cherry::LightList* lightList = nullptr;

		// checks for wireframe being active.
		bool wireframe = false;

		// Model transformation matrix
		glm::mat4 myModelTransform;

		// enables the skybox. TODO: change for final build.
		bool enableSkybox = false;

		// gets the cursor position
		glm::dvec2 mousePos;

		// boolean for full screen
		bool fullScreen;
		
		// returns 'true' if the mouse is in the window content, false otherwise.
		bool mouseEnter = false;

	};

}
