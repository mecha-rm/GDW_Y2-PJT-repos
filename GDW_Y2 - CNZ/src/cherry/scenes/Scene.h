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

		virtual ~Scene() = default;

		// this is the equivalent of 'LoadContent' in Game.cpp.
		virtual void OnOpen();

		virtual void OnClose();

		// returns the main/primary registry
		entt::registry& Registry();

		const std::string& GetName() const;

		// void SetName(const std::string& name);

		// if 'true', then the frame buffer is being used.
		bool IsUsingFrameBuffers() const;

		// returns the post layers.
		std::vector<cherry::PostLayer*> GetPostLayers() const;

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

		// draws the ImGUI window for the scene.
		// include <imgui/imgui.h> for ImGUI functionality.
		virtual void DrawGui(float deltaTime);

		// the update loop.
		// the game update is called before this update.
		virtual void Update(float deltaTime);



	private:
		entt::registry myRegistry; // registry

		std::string myName; // name

	protected:

		// tells the scene to use (or not use) its frame buffers.
		void SetUsingFrameBuffers(bool useFbs);

		// TODO: set it to use the frame buffer.
		// if 'true', then the game renders to the frame buffer.
		bool useFrameBuffers = false;

		// post processing layers
		std::vector<PostLayer*> layers;
	};
}