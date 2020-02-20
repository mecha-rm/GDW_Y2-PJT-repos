#include "EngineScene.h"

#include "..\objects/ObjectManager.h"
#include "..\lights/LightManager.h"
#include "..\Game.h"

// creating the engine scene.
cherry::EngineScene::EngineScene() : Scene("Engine")
{
	ObjectManager::CreateSceneObjectList(GetName());
	objectList = ObjectManager::GetSceneObjectListByName(GetName());

	LightManager::CreateSceneLightList(GetName());
	lightList = LightManager::GetSceneLightListByName(GetName());
}

void cherry::EngineScene::OnOpen()
{
	Game* const game = Game::GetRunningGame();
	
	if (game == nullptr)
		return;

	// added for mip mapping. As long as its above the material, it's fine.
	SamplerDesc description = SamplerDesc();
	description.MinFilter = MinFilter::LinearMipNearest;
	description.MagFilter = MagFilter::Linear;
	description.WrapS = description.WrapT = WrapMode::Repeat;

	// TODO: make linear and NearestMipNearest different variables?
	// called 'Linear' in the original code
	TextureSampler::Sptr sampler = std::make_shared<TextureSampler>(description);

	// materials
	Material::Sptr matStatic = lightList->GenerateMaterial(STATIC_VS, STATIC_FS, sampler);
	Material::Sptr matDynamic = lightList->GenerateMaterial(DYNAMIC_VS, DYNAMIC_FS, sampler);

	game->SetCurrentScene(GetName(), true);

	// setting up the camera
	game->myCamera = std::make_shared<Camera>();
	game->myCamera->SetPosition(glm::vec3(0, 5, 12));
	game->myCamera->LookAt(glm::vec3(0));

	// sets the camera to perspective mode for the m_Scene.
	// myCamera->SetPerspectiveMode(glm::perspective(glm::radians(60.0f), 1.0f, 0.01f, 1000.0f));
	//myCamera->SetPerspectiveMode(glm::perspective(glm::radians(60.0f), 1.0f, 0.01f, 1000.0f));
	game->myCamera->SetPerspectiveMode(glm::radians(45.0f), 1.0f, 0.01f, 1000.0f);
	// myCamera->SetPerspectiveMode(glm::perspective(glm::radians(10.0f), 1.0f, 0.01f, 1000.0f));

	// sets the orthographic mode values. False is passed so that the camera starts in perspective mode.
	game->myCamera->SetOrthographicMode(-5.0f, 5.0f, -5.0f, 5.0f, 0.0f, 100.0f, false);
	// myCamera->followTarget = true;
	// myCamera->fixedTargetDistance = true;
	game->myCamera->targetOffset = cherry::Vec3(0, 5, 12);

	// secondary camera, which is used for UI for the game.
	game->myCameraX = std::make_shared<Camera>();
	game->myCameraX->SetPosition(0, 0.001F, 1.0F); // try adjusting the position of the perspecitve cam and orthographic cam
	game->myCameraX->Rotate(glm::vec3(0.0F, 0.0F, glm::radians(180.0f)));
	game->myCameraX->LookAt(glm::vec3(0));

	Material::Sptr objMat; // used for custom materials
	float offset = 3.0F; // position offset

	  //sceneLists.push_back(new PrimitiveCube(5));
	  //sceneLists.at(sceneLists.size() - 1)->CreateEntity(currentScene, matStatic);
	  //sceneLists.at(sceneLists.size() - 1)->SetPosition(0.0F, 0.0F, 0.0F);

	// Creating the sceneLists, storing them, and making them part of the default m_Scene.
	objectList->objects.push_back(new PrimitiveCapsule());
	objectList->objects.at(objectList->objects.size() - 1)->CreateEntity(GetName(), matStatic);
	objectList->objects.at(objectList->objects.size() - 1)->SetPosition(-offset, -offset, 0.0F);



	objectList->objects.push_back(new PrimitiveCircle());
	objectList->objects.at(objectList->objects.size() - 1)->CreateEntity(GetName(), matStatic);
	objectList->objects.at(objectList->objects.size() - 1)->SetPosition(-offset, 0.0f, 0.0F);

	objectList->objects.push_back(new PrimitiveCone());
	objectList->objects.at(objectList->objects.size() - 1)->CreateEntity(GetName(), matStatic);
	objectList->objects.at(objectList->objects.size() - 1)->SetPosition(-offset, offset, 0.0F);

	objectList->objects.push_back(new PrimitiveCube());
	objectList->objects.at(objectList->objects.size() - 1)->CreateEntity(GetName(), matStatic);
	objectList->objects.at(objectList->objects.size() - 1)->SetPosition(0.0F, -offset, 0.0F);

	objectList->objects.push_back(new PrimitiveCylinder());
	objectList->objects.at(objectList->objects.size() - 1)->CreateEntity(GetName(), matStatic);
	objectList->objects.at(objectList->objects.size() - 1)->SetPosition(0.0F, 0.0F, 0.0F);

	objectList->objects.push_back(new PrimitiveDiamond());
	objectList->objects.at(objectList->objects.size() - 1)->CreateEntity(GetName(), matStatic);
	objectList->objects.at(objectList->objects.size() - 1)->SetPosition(0.0F, offset, 0.0F);

	objectList->objects.push_back(new PrimitiveUVSphere());
	objectList->objects.at(objectList->objects.size() - 1)->CreateEntity(GetName(), matStatic);
	objectList->objects.at(objectList->objects.size() - 1)->SetPosition(offset, -offset, 0.0F);

	objectList->objects.push_back(new PrimitivePlane());
	objectList->objects.at(objectList->objects.size() - 1)->CreateEntity(GetName(), matStatic);
	objectList->objects.at(objectList->objects.size() - 1)->SetPosition(offset, 0.0F, 0.0F);

}

void cherry::EngineScene::OnClose()
{
}

void cherry::EngineScene::MouseButtonPressed(GLFWwindow* window, int button)
{
}

void cherry::EngineScene::MouseButtonHeld(GLFWwindow* window, int button)
{
}

void cherry::EngineScene::MouseButtonReleased(GLFWwindow* window, int button)
{
}

void cherry::EngineScene::KeyPressed(GLFWwindow* window, int key)
{
}

void cherry::EngineScene::KeyHeld(GLFWwindow* window, int key)
{
}

void cherry::EngineScene::KeyReleased(GLFWwindow* window, int key)
{
}

// update loop
void cherry::EngineScene::Update(float deltaTime)
{
}
