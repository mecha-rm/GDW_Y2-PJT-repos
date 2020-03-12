//#include "MenuScene.h"
//#include <cherry\Game.cpp>
//
//// constructor
//cherry::MenuScene::MenuScene(const std::string name)
//	: Scene(name)
//{
//}
//
//
//// opening the scene
//void cherry::MenuScene::OnOpen()
//{
//	Game* game = Game::GetRunningGame();
//	glm::ivec2 myWindowSize = game->GetWindowSize();
//
//	ObjectManager::CreateSceneObjectList(GetName());
//	objectList = ObjectManager::GetSceneObjectListByName(GetName());
//
//	LightManager::CreateSceneLightList(GetName());
//	lightList = LightManager::GetSceneLightListByName(GetName());
//}
//
//// caled when the scene is being closed.
//void cherry::MenuScene::OnClose()
//{
//	ObjectManager::DestroySceneObjectListByPointer(objectList);
//	LightManager::DestroySceneLightListByPointer(lightList);
//}
//
//// updates the buttons
//void cherry::MenuScene::Update(float deltaTime)
//{
//	// updates the buttons.
//	// for (Button& button : buttons)
//	// {
//	// 
//	// }
//}
