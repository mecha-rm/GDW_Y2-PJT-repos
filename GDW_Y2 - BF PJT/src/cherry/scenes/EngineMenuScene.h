#pragma once
#include "MenuScene.h"

namespace cherry
{
	// menu scene
	class EngineMenuScene : public MenuScene
	{
	public:
		EngineMenuScene(std::string name);

		// opening scene
		void OnOpen() override;

		// closing scene
		void OnClose() override;

		// update loop
		void Update(float deltaTime) override;

		std::string nextScene = "";

	private:
		// Button b1;

	protected:
	};
}