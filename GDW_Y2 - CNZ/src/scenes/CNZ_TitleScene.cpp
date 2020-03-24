#include "CNZ_TitleScene.h"
#include "..\CNZ_Game.h"
#include "..\LevelLoader.h"

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
	std::string buttonImage = "res/images/button_beta_sml.png";
	std::string textFnt = FONT_ARIAL;
	Vec4 textClr = Vec4(1.0F, 1.0F, 1.0F, 1.0F);

	glm::vec2 startPos{ 0.9, 0.8F };
	glm::vec2 offset{ -0.15F, 10.0F };

	// title screen
	{
		Image * image = new Image("res/images/codename_zero_logo_small.png", sceneName, false, false);
		image->SetWindowChild(true);
		cherry::Vec3 size = image->GetMeshBodyMaximum() - image->GetMeshBodyMinimum();
		image->SetScale(1.0F);

		PhysicsBodyBox* pbb = new PhysicsBodyBox(Vec3(0, 0, 0), size);
		pbb->SetVisible(true);

		image->AddPhysicsBody(pbb); // collision box

		entryButton = new Button();
		entryButton->object = image;

		AddButton(entryButton, false);
	}

	// ranking button
	{
		Image* image = new Image(buttonImage, sceneName, false, false);
		image->SetWindowChild(true);
		// image->SetPositionByWindowSize(Vec2(0.9, 0.8F));
		image->SetPositionByWindowSize(Vec2(0.5F, 0.915F));
		image->SetScale(1.2F);

		// size
		Vec3 size = image->GetMeshBodyMaximum() - image->GetMeshBodyMinimum();
		// collision box
		PhysicsBodyBox* pbb = new PhysicsBodyBox(Vec3(0, 0, 0), size);
		pbb->SetVisible(true);

		image->AddPhysicsBody(pbb);

		rankButton = new Button();
		rankButton->object = image; // setting image as the object.

		// creating the text.
		rankButton->text = new Text("Ranking", sceneName, textFnt, textClr, 10.0F);
		rankButton->text->SetWindowChild(true);
		rankButton->localTextPos = glm::vec3(0.0F, 0.0F, 1.0F);

		AddButton(rankButton, false);
		UpdateButton(rankButton);
	}

	// button 1
	{
		Image* image = new Image(buttonImage, sceneName, false, false);
		image->SetWindowChild(true);
		// image->SetPositionByWindowSize(Vec2(0.9, 0.8F));
		image->SetPositionByWindowSize(Vec2(startPos));

		// size
		Vec3 size = image->GetMeshBodyMaximum() - image->GetMeshBodyMinimum();
		// collision box
		PhysicsBodyBox* pbb = new PhysicsBodyBox(Vec3(0, 0, 0), size);
		pbb->SetVisible(true);

		image->AddPhysicsBody(pbb);

		map1Button = new Button();
		map1Button->object = image; // setting image as the object.

		// creating the text.
		map1Button->text = new Text("Map 1", sceneName, textFnt, textClr, 10.0F);
		map1Button->text->SetWindowChild(true);
		map1Button->localTextPos = glm::vec3(0.0F, 0.0F, 1.0F);
		
		AddButton(map1Button, false);
		UpdateButton(map1Button);
	}

	// button 2
	{
		Image* image = new Image(buttonImage, sceneName, false, false);
		image->SetWindowChild(true);
		image->SetPositionByWindowSize(Vec2(startPos.x + offset.x, startPos.y));

		// size
		Vec3 size = image->GetMeshBodyMaximum() - image->GetMeshBodyMinimum();
		// collision box
		PhysicsBodyBox* pbb = new PhysicsBodyBox(Vec3(0, 0, 0), size);
		pbb->SetVisible(true);

		image->AddPhysicsBody(pbb);

		map2Button = new Button();
		map2Button->object = image; // setting image as the object.

		// creating the text.
		map2Button->text = new Text("Map 2", sceneName, textFnt, textClr, 10.0F);
		map2Button->text->SetWindowChild(true);
		map2Button->localTextPos = glm::vec3(0.0F, 0.0F, 1.0F);

		AddButton(map2Button, false);
		UpdateButton(map2Button);
	}

	// button 3
	{
		Image* image = new Image(buttonImage, sceneName, false, false);
		image->SetWindowChild(true);
		image->SetPositionByWindowSize(Vec2(startPos.x + offset.x * 2, startPos.y));

		// size
		Vec3 size = image->GetMeshBodyMaximum() - image->GetMeshBodyMinimum();
		// collision box
		PhysicsBodyBox* pbb = new PhysicsBodyBox(Vec3(0, 0, 0), size);
		pbb->SetVisible(true);

		image->AddPhysicsBody(pbb);

		map3Button = new Button();
		map3Button->object = image; // setting image as the object.

		// creating the text.
		map3Button->text = new Text("Map 3", sceneName, textFnt, textClr, 10.0F);
		map3Button->text->SetWindowChild(true);
		map3Button->localTextPos = glm::vec3(0.0F, 0.0F, 1.0F);

		AddButton(map3Button, false);
		UpdateButton(map3Button);
	}

	Level::GenerateSources();
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

	CNZ_Game* const game = (CNZ_Game*)Game::GetRunningGame();

	// a button has been entered and the mouse has been pressed.
	if (enteredButton != nullptr && mousePressed)
	{
		if (enteredButton == entryButton) // TODO: change the entry button to something else.
		{
			game->SetCurrentScene(game->map1Info.sceneName, true);
		}
		else if (enteredButton == rankButton) // ranking list
		{
			game->SetCurrentScene(game->rankingSceneName, true);
		}
		else if (enteredButton == map1Button) // enters map 1
		{
			game->SetCurrentScene(game->map1Info.sceneName, true);
		}
		else if (enteredButton == map2Button) // enters map 2
		{
			game->SetCurrentScene(game->map2Info.sceneName, true);
		}
		else if (enteredButton == map3Button) // enters map 3
		{
			game->SetCurrentScene(game->map3Info.sceneName, true);
		}
	}

	// button has been hit
	// if (enteredButton == entryButton && mousePressed)
	// {
	// 	game->SetCurrentScene(game->map1Info.sceneName, true);
	// }

}
