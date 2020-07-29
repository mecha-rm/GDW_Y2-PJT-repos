// scene for character rankings.
#pragma once

#include "..\cherry/scenes/SceneManager.h"
#include "..\cherry/scenes/MenuScene.h"
#include <string>

namespace cnz
{
	class CNZ_RankingScene : public cherry::MenuScene
	{
	public:
		// constructor
		CNZ_RankingScene(std::string sceneName);

		// loading the scene
		void OnOpen() override;

		// closing the scene
		void OnClose() override;

		// generates a new instance of the ranking scene.
		virtual Scene* GenerateNewInstance() const override;

		// called when a mouse button has been pressed.
		void MouseButtonPressed(GLFWwindow* window, int button) override;

		// update loop
		void Update(float deltaTime) override;

	private:
		// exit button
		cherry::Button* exitButton = nullptr;

		// line of scores.
		std::vector<cherry::Text*> scores;

		// name of the file that will be used.
		std::string fileName = "";

	protected:
	};
}