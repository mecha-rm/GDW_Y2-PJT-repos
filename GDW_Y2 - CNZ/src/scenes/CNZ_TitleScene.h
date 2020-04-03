#pragma once

#include "..\cherry/scenes/SceneManager.h"
#include "..\cherry/scenes/MenuScene.h"
#include "..\cherry/post/KernelLayer.h"
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

		// called when a key has been pressed
		void KeyPressed(GLFWwindow* window, int key) override;

		// update loop
		void Update(float deltaTime) override;

	private:
		cherry::Button* entryButton = nullptr;
		cherry::Button* rankButton = nullptr;

		// buttons 
		cherry::Button* map1Button = nullptr;
		cherry::Button* map2Button = nullptr;
		cherry::Button* map3Button = nullptr;

		// text for the loading screen, and blur for the layer
		cherry::Text* loadingText = nullptr;
		cherry::PostLayer::Sptr loadLayer;

		// if 'load effect is false', then the loading effect isn't used.
		const bool enableLoadEffect = true;
		// if 'true', then the scene is loading.
		
		bool loading = false;
		
		// image of controls.
		cherry::Image* controls = nullptr;
		
		// gets the string for the next scene.
		// this is only used if the loading screen is active.
		std::string nextScene = "";

	protected:
	};
}