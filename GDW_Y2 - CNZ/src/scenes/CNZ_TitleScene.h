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

		// generates a new instance of the engine gameplay scene.
		virtual Scene* GenerateNewInstance() const override;

		// called when a mouse button has been pressed.
		void MouseButtonPressed(GLFWwindow* window, int button) override;

		// called when a key has been pressed.
		void KeyPressed(GLFWwindow* window, int key) override;

		// called when a key has been released.
		void KeyReleased(GLFWwindow* window, int key) override;

		// update loop
		void Update(float deltaTime) override;

	private:
		cherry::Button* entryButton = nullptr;
		cherry::Button* rankButton = nullptr;

		// buttons 
		cherry::Button* map1Button = nullptr;
		cherry::Button* map2Button = nullptr;
		cherry::Button* map3Button = nullptr;

		// map 0 (debug) button combo (CTRL + SHIFT + D) 
		// doing CTRL + SHIFT + 0 causes 0 not to go through for some reason. It seems to not happen with other numbers.
		// either way, that's why this had to be changed.
		std::queue<int> map0keys;

		// if 'true', the debug map (map 0) is available for use.
		bool DEBUG_MAP_OPEN = true;

		// text for the loading screen, and blur for the layer
		cherry::Text* loadingText = nullptr;
		cherry::PostLayer::Sptr loadLayer;

		// if 'load effect is false', then the loading effect isn't used.
		// it seems like turning off this effect causes things to crash now.
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