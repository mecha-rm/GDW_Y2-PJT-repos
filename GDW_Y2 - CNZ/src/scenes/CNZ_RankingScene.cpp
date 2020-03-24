#include "CNZ_RankingScene.h"

#include "..\CNZ_Game.h"
#include <toolkit/Logging.h>

// constructor
cnz::CNZ_RankingScene::CNZ_RankingScene(std::string sceneName)
	: cherry::MenuScene(sceneName)
{
}

void cnz::CNZ_RankingScene::OnOpen()
{
	using namespace cherry;
	cherry::MenuScene::OnOpen();

	fileName = "res/data/ranking.txt";

	std::ifstream file(fileName, std::ios::in);
	std::string line; // line from file.
	std::string sceneName = GetName();
	std::string font = FONT_ARIAL;

	glm::vec2 startPos{}; // start
	Vec2 offset{ 0.0F, 0.15F }; // offset

	// making the texts
	if (!file)
	{
		LOG_ERROR("Ranking file not found.");
		file.close();
		return;
	}

	// while the file still has lines.
	while (std::getline(file, line))
	{
		Text* text = new Text(line, sceneName, font, Vec4(1.0F, 1.0F, 1.0F, 1.0F), 9.0F);
		text->SetWindowChild(true);
		text->SetPositionByWindowSize(Vec2(0.8F, 0.6F) + offset);
		
		objectList->AddObject(text);
	}

	file.close();
	
	// exit button
	{
		Image* image = new Image("res/images/codename_zero_logo_small.png", sceneName, false, false);
		image->SetWindowChild(true);
		image->SetPositionByWindowSize(Vec2(0.5F, 0.1F));

		// collision box
		PhysicsBodyBox* pbb = new PhysicsBodyBox(Vec3(0, 0, 0), image->GetMeshBodyMaximum() - image->GetMeshBodyMinimum());
		pbb->SetVisible(true);

		image->AddPhysicsBody(pbb);

		exitButton = new Button();
		exitButton->object = image; // setting image as the object.

		// creating the text.
		exitButton->text = new Text("Back To Menu", sceneName, font, Vec4(1.0F, 1.0F, 1.0F, 1.0F), 10.0F);
		exitButton->text->SetWindowChild(true);
		exitButton->localTextPos = glm::vec3(0.0F, 0.0F, 1.0F);

		AddButton(exitButton, false);
		UpdateButton(exitButton);
	}
}

void cnz::CNZ_RankingScene::OnClose()
{
	cherry::MenuScene::OnClose();
}

void cnz::CNZ_RankingScene::Update(float deltaTime)
{
	using namespace cherry;
	cherry::MenuScene::Update(deltaTime);

	CNZ_Game* const game = (CNZ_Game*)Game::GetRunningGame();

	// a button has been entered and the mouse has been pressed.
	if (enteredButton == exitButton && mousePressed)
	{
		game->SetCurrentScene(game->titleSceneName, true);
	}
}
