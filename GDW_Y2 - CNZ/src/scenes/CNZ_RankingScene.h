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

		// update loop
		void Update(float deltaTime) override;

	private:
		cherry::Button* exitButton = nullptr;

		// name of the file that will be used.
		std::string fileName = "";

	protected:
	};
}