// the default scene the engine loads into
#pragma once

#include "Scene.h"

namespace cherry
{
	class ObjectList;
	class LightList;

	// the scene the engine uses.
	class EngineScene : public Scene
	{
	public:
		// constructor
		EngineScene(std::string sceneName);

		void OnOpen() override;

		void OnClose() override;

		// these functions get called by the game class by default, but they can be overwritten.
		void MouseButtonPressed(GLFWwindow* window, int button) override;

		// called when a mouse button is being held.
		void MouseButtonHeld(GLFWwindow* window, int button) override;

		// called when a mouse button has been pressed
		void MouseButtonReleased(GLFWwindow* window, int button) override;


		// called when a key has been pressed
		void KeyPressed(GLFWwindow* window, int key) override;

		// called when a key is being held down
		void KeyHeld(GLFWwindow* window, int key) override;

		// called when a key has been released
		void KeyReleased(GLFWwindow* window, int key) override;

		// update loop.
		void Update(float deltaTime) override;

	private:
		// object list
		cherry::ObjectList* objectList = nullptr;

		// light list
		cherry::LightList* lightList = nullptr;

		// translation and rotation direction
		glm::vec3 t_Dir = glm::vec3(0, 0, 0);
		glm::vec3 r_Dir = glm::vec3(0, 0, 0);

		// translation and rotation speeds
		float t_Inc = 22.50F;
		float r_Inc = 55.0F;

		// hitbox
		int hitBoxIndex = -1;
	protected:
	};
}
