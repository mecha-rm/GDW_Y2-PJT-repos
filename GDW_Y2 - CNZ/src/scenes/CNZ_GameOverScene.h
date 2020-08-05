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

		// gets the set score that will be saved if it breaks the top performers list.
		int GetScore() const;

		// sets the score for the player.
		void SetScore(int score);

		// gets the map the player was on.
		int GetMap() const;

		// sets the map the score is for.
		void SetMapNumber(int map);

		// update loop
		void Update(float deltaTime) override;

	private:

		//  saves the scores.
		void SaveScores();

		// struct for scores
		// TODO: change limit on how many characters can be used for the name.
		// TODO: add in value for the map that the score was gotten on.
		struct Score
		{
			std::string name = "XXXXXXXXXXXXXXXX";
			int mapNumber = 0;
			int points = 0;
		};

		// array of scores
		std::vector<Score> scores;

		// player
		int playerScore = 0;

		// the map that the player was on.
		int mapNumber = 0;

		// the rank of the current player.
		int playerRank = 99;

		// Buttons
		cherry::Button* exitButton = nullptr;

		// used for imGUI
		std::string entryNameStr = ""; // string version

	protected:
	};
}