#pragma once

#include "..\cherry/scenes/SceneManager.h"
#include "..\cherry/scenes/MenuScene.h"
#include <string>

namespace cnz
{
	class CNZ_TitleScene : public cherry::MenuScene
	{
	public:
		// constructor
		CNZ_TitleScene(std::string sceneName);

		// loading the scene
		void OnOpen() override;

		// closing the scene
		void OnClose() override;

		// update loop
		void Update(float deltaTime) override;

	private:
		cherry::Button* entryButton = nullptr;
		cherry::Button* rankButton = nullptr;

		// buttons 
		cherry::Button* map1Button = nullptr;
		cherry::Button* map2Button = nullptr;
		cherry::Button* map3Button = nullptr;

	protected:
	};
}