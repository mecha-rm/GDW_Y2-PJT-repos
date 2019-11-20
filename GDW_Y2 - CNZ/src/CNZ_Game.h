#pragma once
#include "cherry/Game.h"
#include "Player.h"

// inherits from the game class.
namespace cnz
{
	// class for the codename zero game.
	class CNZ_Game : public cherry::Game
	{
	public:
		// creates the game with default values for the screen size
		CNZ_Game();

		// creates a game window with the provided screen size
		CNZ_Game(float windowWidth, float windowHeight, bool fullScreen);

		// called when a mouse button has been pressed
		virtual void MouseButtonPressed(GLFWwindow* window, int button);

		// called when a mouse button has been pressed
		virtual void MouseButtonReleased(GLFWwindow* window, int button);

		// called when a key has been pressed; overloaded from the game's keyPressed
		virtual void KeyPressed(GLFWwindow* window, int key);

		// called when a key is being held down; overloaded from the game's keyHeld
		virtual void KeyHeld(GLFWwindow* window, int key);

		// called when a key has been released; overloaded from the game's keyReleased
		virtual void KeyReleased(GLFWwindow* window, int key);

	protected:

		// overwritten function for loading in game content.
		virtual void LoadContent();

		// overwritten from the game class's data.
		virtual void Update(float deltaTime);

	private:
		
		cnz::Player * playerObj = nullptr; // object for the player.
		cnz::Player* testObj = nullptr; // object for the player.
		bool mbLP = false, mbLR = false;

		float camLerpPercent = 0.0f;

		glm::vec3 testPlayPos = glm::vec3(0, 0, 0);
		cherry::Vec3 playerPrevPos;

		// player movement
		bool w = false;
		bool a = false;
		bool s = false;
		bool d = false;

		// camera
		bool debugMode = true; // allows for debug mode.

	};
}
