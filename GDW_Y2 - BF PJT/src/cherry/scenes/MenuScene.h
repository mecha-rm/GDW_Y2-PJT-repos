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

		// local transformation values for the text.
		glm::vec3 localTextPos;

		// text rotation (degrees)
		glm::vec3 localTextRot;

		// text scale
		glm::vec3 localTextScale = glm::vec3(1.0F);

	};

	// mouse
	// struct Mouse
	// {
	// 	// saves the button the mouse is currently hovering over
	// 	// nullptr if the mouse is not touching any button
	// 	Button* enteredButton = nullptr;
	// 
	// 	// set to 'true' if the mouse button has been pressed.
	// 	bool mousePressed = false;
	// 
	// 	// saves the keycode of the most recent mouse button pressed.
	// 	// check 'mousePressed' to see 
	// 	int mouseKeyCode = 0;
	// };

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
		cherry::Button* AddButton(cherry::Object * object, cherry::PhysicsBody * body = nullptr, cherry::Text * text = nullptr);

		// adds a button to the applicable lists. 
		// If 'addedToList' is equal to false, it adds the button and its text are both added to the object list.
		void AddButton(Button* button, bool addedToList = false);

		// removes a button and its components from the lists, and returns the button that was removed.
		// if it wasn't in the list, a nullptr is returned.
		cherry::Button* RemoveButtonByPointer(Button* button);

		// removes the button by its index in the list.
		cherry::Button* RemoveButtonByIndex(int index);

		// deletes the button
		bool DeleteButtonByPointer(Button* button);
		
		// delets the button by its index in the list
		bool DeleteButtonByIndex(int index);

		// updates the provided button.
		// call this function when the local text position values have been changed.
		void UpdateButton(Button* button);

		// update loop.
		void Update(float deltaTime) override;

		// saves the button the mouse is currently hovering over
		// nullptr if the mouse is not touching any button
		Button* enteredButton = nullptr;

		// set to 'true' if the mouse button has been clocked.
		bool mousePressed = false;

		// saves the keycode of the most recent mouse button pressed.
		// check 'mousePressed' to see if a mouse button has been clicked.
		// -1 by default since no buttons have been pressed yet.
		int mouseKey = -1;

	private:
		// mouse collision box
		// object representing the mouse
		// Object* cursorObject = nullptr;
		PhysicsBodyBox* cursorBox = nullptr;

	protected:
		// object list
		cherry::ObjectList* objectList = nullptr;

		// light list
		cherry::LightList* lightList = nullptr;

		// vector of buttons
		std::vector<Button*> buttons;

	};
}
