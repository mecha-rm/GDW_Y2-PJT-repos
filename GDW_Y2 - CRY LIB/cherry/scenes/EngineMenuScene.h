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

		// generates a new instance of the engine menu scene.
		virtual Scene* GenerateNewInstance() const override;

		// update loop
		void Update(float deltaTime) override;

		// the next scene to be travelled to.
		std::string nextScene = "";

	private:
		// Button b1;

	protected:
	};
}