// Scene (Header) - holds scene information
#pragma once


#include <entt/entt.hpp>

#include "..\textures/TextureCube.h"
#include "..\Shader.h"
#include "..\Mesh.h"
#include "..\post/PostLayer.h"

#include <GLFW/glfw3.h>
namespace cherry
{
	class Scene {
	public:
		TextureCube::Sptr Skybox = nullptr;
		Shader::Sptr SkyboxShader = nullptr;
		Mesh::Sptr SkyboxMesh = nullptr;

		// Scene() = default;

		// EX: creates a scene, providing it its name.
		Scene(const std::string name);

		// destructor.
		virtual ~Scene() = default;

		// this is the equivalent of 'LoadContent' in Game.cpp.
		virtual void OnOpen();

		// called when a scene is being closed.
		virtual void OnClose();

		// returns the main/primary registry
		entt::registry& Registry();
		
		// gets the name of the scene
		const std::string& GetName() const;

		// if 'true', then the scene is using the imGui draw.
		bool IsUsingImgui() const;

		// if 'true', then the frame buffer is being used.
		bool IsUsingFrameBuffers() const;

		// returns the post layers.
		std::vector<cherry::PostLayer::Sptr> GetPostLayers() const;

		// creates a new scene with the same name as the current scene.
		// this can be used to reset a scene entirely by deleting the old scene and replacing it with a new instance of the same scene.
		// do note that allowNewInstances must be set to 'true' so that the program knows that GetNewInstance() has been overwritten.
		// otherwise, data would be lost by generating a scene of the base type rather than the derived type.
		virtual Scene* GenerateNewInstance() const;

		// checked to see if the scene allows for new instances to be generated.
		// if 'true', a new isntance of the scene is made instead of reopening the same scene over and over.
		// reusing the same scene causes problems, so this is meant to fix that.
		bool IsAllowingNewInstances() const;

		// these functions get called by the game class by default, but they can be overwritten.
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

		// draws the ImGUI window for the scene. useImgui must be 'true' in order for this to be called.
		// include <imgui/imgui.h> for ImGUI functionality.
		virtual void DrawGui(float deltaTime);

		// the update loop.
		// the game update is called before this update.
		virtual void Update(float deltaTime);

		// post processing layers
		std::vector<PostLayer::Sptr> layers;

	private:
		entt::registry myRegistry; // registry

		// this should be const since it can't be changed.
		const std::string myName; // name

	protected:

		// tells the scene to use (or not use) its frame buffers.
		void SetUsingFrameBuffers(bool useFbs);

		// determines whether the scene can create new instances or not.
		// if 'true', then a new instance is made rather than reopening the same scene over and over.
		// if false, the same scene is used over and over again. This seems to cash crashes.
		void SetAllowingNewInstances(bool newInstances);

		// TODO: set it to use the frame buffer.
		// if 'true', then the game renders to the frame buffer.
		bool useFrameBuffers = false;

		// use Imgui window
		bool useImgui = false;

		// allow new instances to be
		bool allowNewInstances = false;
	};
}