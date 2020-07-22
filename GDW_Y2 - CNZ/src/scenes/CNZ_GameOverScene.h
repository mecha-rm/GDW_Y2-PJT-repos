#pragma once

#include "..\cherry/scenes/SceneManager.h"
#include "..\cherry/scenes/MenuScene.h"
#include "..\cherry/post/KernelLayer.h"
#include <string>

namespace cnz
{
	class CNZ_GameOverScene : public cherry::MenuScene
	{

	public:
		// constructor
		CNZ_GameOverScene(std::string sceneName);

		// loading the scene
		void OnOpen() override;

		// closing the scene
		void OnClose() override;

		// generates a new instance of the engine gameplay scene.
		virtual Scene* GenerateNewInstance() const override;

		// called when a mouse button has been pressed.
		void MouseButtonPressed(GLFWwindow* window, int button) override;

		// called when a key has been pressed.
		// void KeyPressed(GLFWwindow* window, int key) override;

		// ImGUI draw function.
		void DrawGui(float deltaTime) override;

		// sets the score for the player.
		void SetScore(int score);


		// update loop
		void Update(float deltaTime) override;

	private:

		//  saves the scores.
		void SaveScores();

		// struct for scores
		// TODO: change limit on how many characters can be used for the name.
		struct Score
		{
			std::string name = "XXXXXXXXXXXXXXXX";
			int points = 0;
		};

		// array of scores
		std::vector<Score> scores;

		// player
		int playerScore = 0;

		// the rank of the current player.
		int playerRank = 99;

		// Buttons
		cherry::Button* exitButton = nullptr;

		// used for imGUI
		std::string entryNameStr = ""; // string version

	protected:
	};
}