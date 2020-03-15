#include "CNZ_TitleScene.h"
#include "..\cherry/Game.h"

// title scene
cnz::CNZ_TitleScene::CNZ_TitleScene(std::string sceneName)
	: cherry::MenuScene(sceneName)
{
}

// scene open
void cnz::CNZ_TitleScene::OnOpen()
{
	using namespace cherry;

	cherry::MenuScene::OnOpen();

	std::string sceneName = GetName();

	{
		Image * image = new Image("res/images/codename_zero_logo_small.png", sceneName, false, false);
		image->SetWindowChild(true);
		cherry::Vec3 size = image->GetMeshBodyMaximum() - image->GetMeshBodyMinimum();
		image->SetScale(1.0F);
		image->AddPhysicsBody(new PhysicsBodyBox(Vec3(0, 0, 0), size)); // collision box

		entryButton = new Button();
		entryButton->object = image;

		AddButton(entryButton, false);
	}
}

// closing
void cnz::CNZ_TitleScene::OnClose()
{
	cherry::MenuScene::OnClose();
}

// update loop
void cnz::CNZ_TitleScene::Update(float deltaTime)
{
	MenuScene::Update(deltaTime);

	using namespace cherry;

	Game* const game = Game::GetRunningGame();

	// button has been hit
	if (enteredButton == entryButton && mousePressed)
	{
		game->SetCurrentScene(destScene, true);
	}
}
