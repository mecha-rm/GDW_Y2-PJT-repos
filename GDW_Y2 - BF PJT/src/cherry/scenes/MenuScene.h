//// the default scene the engine loads into
//#pragma once
//
//#include "Scene.h"
//#include "..\objects\ObjectManager.h"
//#include "..\lights\LightManager.h"
//#include "..\objects\Text.h"
//
//namespace cherry
//{
//	// the scene the engine uses.
//	class MenuScene: public cherry::Scene
//	{
//	public:
//		// constructor
//		MenuScene(std::string sceneName);
//
//		// loading the scene
//		void OnOpen() override;
//
//		// closing the scene
//		void OnClose() override;
//
//		// // these functions get called by the game class by default, but they can be overwritten.
//		// void MouseButtonPressed(GLFWwindow* window, int button) override;
//		// 
//		// // called when a mouse button is being held.
//		// void MouseButtonHeld(GLFWwindow* window, int button) override;
//		// 
//		// // called when a mouse button has been pressed
//		// void MouseButtonReleased(GLFWwindow* window, int button) override;
//		// 
//		// 
//		// // called when a key has been pressed
//		// void KeyPressed(GLFWwindow* window, int key) override;
//		// 
//		// // called when a key is being held down
//		// void KeyHeld(GLFWwindow* window, int key) override;
//		// 
//		// // called when a key has been released
//		// void KeyReleased(GLFWwindow* window, int key) override;
//		// 
//		// // ImGUI draw function.
//		// void DrawGui(float deltaTime) override;
//
//		// update loop.
//		void Update(float deltaTime) override;
//
//	private:
//
//	protected:
//		// object list
//		cherry::ObjectList* objectList = nullptr;
//
//		// light list
//		cherry::LightList* lightList = nullptr;
//
//		// a button for the menu
//		//
//		struct Button
//		{
//			// an image for the button
//			cherry::Object* button = nullptr;
//
//			// the text for the button. Leave it as nullptr if it shouldn't be used.
//			cherry::Text* text = nullptr;
//
//			// the physics body for the textbox.
//			// this is not saved to the menu.
//			cherry::PhysicsBody* body;
//
//			// local transformation values for the text.
//			cherry::Vec3 localTextPos;
//
//			// text rotation (degrees)
//			cherry::Vec3 localTextRot;
//
//			// text scale
//			cherry::Vec3 localTextScale;
//		};
//
//		std::vector<Button> buttons;
//
//	};
//}
