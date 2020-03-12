// the default scene the engine loads into
#pragma once

#include "Scene.h"
#include "..\objects\ObjectManager.h"
#include "..\lights\LightManager.h"
#include "..\objects\Text.h"
#include "..\physics\Physics.h"

namespace cherry
{
	// a button for the menu
	struct Button
	{
		// an image for the button
		cherry::Object* object = nullptr;

		// the text for the button. Leave it as nullptr if it shouldn't be used.
		cherry::Text* text = nullptr;

		// the physics body for the textbox.
		// this is not saved to the menu.
		cherry::PhysicsBody* body = nullptr;

		// local transformation values for the text.
		glm::vec3 localTextPos;

		// text rotation (degrees)
		glm::vec3 localTextRot;

		// text scale
		glm::vec3 localTextScale = glm::vec3(1.0F);
	};

	// the scene the engine uses.
	class MenuScene: public cherry::Scene
	{
	public:
		// constructor
		MenuScene(std::string sceneName);

		// loading the scene
		void OnOpen() override;

		// closing the scene
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
		
		// ImGUI draw function.
		void DrawGui(float deltaTime) override;

		// adds a button to the scene.
		// if a physics body is not passed, one is generated based on the mesh size of the object.
		// the object and text are both added to the object list. This adds the physics body to the object as well.
		cherry::Button& AddButton(cherry::Object * object, cherry::PhysicsBody * body = nullptr, cherry::Text * text = nullptr);

		// adds a button to the list. If 'addedToList' is equal to false, it adds the buttons to the list.
		void AddButton(Button& button, bool addedToList = false);

		// updates the provided button.
		// call this function when the local text position values have been changed.
		void UpdateButton(Button& button);

		// update loop.
		void Update(float deltaTime) override;

	private:

	protected:
		// object list
		cherry::ObjectList* objectList = nullptr;

		// light list
		cherry::LightList* lightList = nullptr;

		// vector of buttons
		std::vector<Button> buttons;

	};
}
