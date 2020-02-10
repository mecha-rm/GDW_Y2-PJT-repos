#pragma once
// GAME CLASS (HEADER)

// External Library Includes
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>

// File Includes
#include "Camera.h" // camera
#include "Shader.h"
#include "Mesh.h"


#include "SceneManager.h"
#include "Skybox.h"
#include "objects/ObjectManager.h"
#include "lights/LightManager.h"
#include "audio/AudioComponent.h"

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
		Game(const char windowTitle[32], float _width, float _height, bool _fullScreen, 
			bool _defaults = false, bool _debug = false, bool _imgui = false);

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

		// creates a scene. If 'makeCurrent' is true, then this scene is made the current scene.
		// this also returns the scene created. Since no skybox is provided, a default one is made.
		bool CreateScene(const std::string sceneName, const bool makeCurrent);

		// nothing happens if the scene already exists
		// this also returns the scene created.
		bool CreateScene(const std::string sceneName, const cherry::Skybox skybox, const bool makeCurrent);

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

		// runs the game
		void Run();

		// handles resizing the window without skewing the sceneLists in the m_Scene.
		void Resize(int newWidth, int newHeight);

		// if 'true', then the sceneLists keep their scale when the window is resized.
		// If false, the sceneLists skew with the size of the window.
		bool changeImageAspectOnWindowResize = true;
		 
		// the object used for the camera
		Camera::Sptr myCamera;

		// the secondary camera, which is used for overlaying a hud.
		Camera::Sptr myCameraX;

		// Target;
		
		// TODO: make private?
		// audio component for the scene
		cherry::AudioComponent audioEngine = cherry::AudioComponent();

		// the frame rate of the game.
		// set the frame rate to 0 (or anything less), to have no framerate cap.
		static short int FPS;

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
		void DrawGui(float deltaTime);

		/*
		 * used for rendering the scene to multiple viewpoints.
		 * Variables:
			* viewport: the size of the viewport.
			* camera: the camera to be used for rendering.
			* drawSkybox: if 'true', the skybox is drawn. Since each registry has its own camera, it's recommended that this is only used once.
				* Do note that if there is no skybox or if scene->SkyboxMesh->IsVisible() returns 'false', the skybox won't be rendered anyway.
			* clear: if 'true', then anything previously rendered is cleared, which is needed for the border.
		*/
		void __RenderScene(glm::ivec4 viewport, Camera::Sptr camera, bool drawSkybox = true,
			int borderSize = 0, glm::vec4 borderColor = glm::vec4(1.0F, 1.0F, 1.0F, 1.0F), bool clear = true);

		// TODO: add in variables for borders.

		// set to 'true' for debug functionality.
		bool debugMode = false;

		// if 'true', the  imgui window functions are used.
		// if false, then they are not used.
		bool imguiMode = false;

		// if 'true', collisions are checked by the Game class.
		bool collisionMode = true;

		// list of scenes
		// std::vector<std::string> scenes;

		// the m_Scene material
		Material::Sptr matStatic; // the static material
		Material::Sptr matDynamic; // the dynamic material
		 
		SamplerDesc description; // texture description 
		TextureSampler::Sptr sampler; // texture sampler

	private:

		// Stores the main window that the game is running in
		GLFWwindow* myWindow;

		glm::ivec2 myWindowSize; // saves the window size

		// static glm::vec2 resolution;

		// Stores the clear color of the game's window
		glm::vec4 myClearColor;

		// Stores the title of the game's window
		char myWindowTitle[32];

		// A shared pointer to our shader.
		// Shader::Sptr myShader;

		// object list
		cherry::ObjectList* objectList = nullptr; // objManager deletion handles this

		// checks for wireframe being active.
		bool wireframe = false;

		// holds the list of lights
		cherry::LightList* lightList; // lightManager deletion handles this

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
		// TODO: change to array
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
