// Scene (Header) - holds scene information
#pragma once

#include "Scene.h"
#include "..\objects/ObjectManager.h"
#include "..\lights/LightManager.h"

namespace cherry
{
	class GameplayScene : public Scene {
	public:
		// constructor
		GameplayScene(const std::string name);

		virtual ~GameplayScene() = default;

		// this is the equivalent of 'LoadContent' in Game.cpp.
		// this creates the scene object list and light object list, so use GameplayScene::OnOpen() to call the parent function beforehand.
		virtual void OnOpen() override;

		// called when the scene is closing. It deletes the lists for the scene. 
		// use GameplayScene::OnClose() to call the inherited function.
		virtual void OnClose() override;

	private:

	protected:
		// object list
		cherry::ObjectList* objectList = nullptr;

		// light list
		cherry::LightList* lightList = nullptr;

	};
}