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

	SetAllowingNewInstances(true);

	// scene name
	std::string sceneName = GetName();

	// set skybox 
	{
		// default
		// cherry::Skybox skybox(
		// 	"res/images/cubemaps/checkerboard_black-grey_d.jpg",
		// 	"res/images/cubemaps/checkerboard_black-grey_d.jpg",
		// 	"res/images/cubemaps/checkerboard_black-grey_d.jpg",
		// 	"res/images/cubemaps/checkerboard_black-grey_d.jpg",
		// 	"res/images/cubemaps/checkerboard_black-grey_d.jpg",
		// 	"res/images/cubemaps/checkerboard_black-grey_d.jpg"
		// );

		// main
		cherry::Skybox skybox(
			"res/images/cubemaps/checkerboard_black-blue.jpg",
			"res/images/cubemaps/checkerboard_black-blue.jpg",
			"res/images/cubemaps/checkerboard_black-blue.jpg",
			"res/images/cubemaps/checkerboard_black-blue.jpg",
			"res/images/cubemaps/checkerboard_black-blue.jpg",
			"res/images/cubemaps/checkerboard_black-blue.jpg"
		);

		skybox.AddSkyboxToScene(this);
		Game::GetRunningGame()->SetSkyboxVisible(true);
	}

	// music trigger (if not already playing) - this doesn't work since isEventPlaying() doesn't actually work.
	// AudioEngine& audio = AudioEngine::GetInstance();
	// if (audio.isEventPlaying("bgm_01") == false) {
	// 	audio.PlayEvent("bgm_01");
	// }

	// text - reading in lines.
	{
		fileName = RANKING_FILE;

		std::ifstream file(fileName, std::ios::in);
		std::string line; // line from file.
		std::string font = FONT_ARIAL;

		glm::vec2 startPos{}; // start
		Vec2 offset{ 0.0F, 0.05F }; // offset per line
		int lNum = 0; // number of lines
		int spaces = 10;
		std::string spaceLine = "";

		// text size for the score board text.
		float textSize = 5.25f;

		// making the texts
		if (!file)
		{
			LOG_ERROR("Ranking file not found.");
			file.close();
			return;
		}

		// space for lines
		for (int i = 1; i <= spaces; i++)
			spaceLine += " ";

		// TODO: add numbers for ranks (currently built into names)
		// while the file still has lines.
		while (std::getline(file, line))
		{
			// splits the line between the score and the name.
			std::vector<std::string> splitStr = util::splitString<std::string>(line);
			std::string str = "";

			// forms the line for the text.

			// replaces underscores with spaces.
			if (splitStr.size() == 3) // name - map - score
				str = util::replaceSubstring(splitStr[0], "_", " ") + spaceLine + splitStr[1] + spaceLine + splitStr[2];

			// adding line number.
			if(lNum < 9)
				str = "0" + std::to_string(lNum + 1) + ". " + str;
			else
				str = "10. " + str;

			Text* text = new Text(str, sceneName, font, Vec4(1.0F, 1.0F, 1.0F, 1.0F), textSize);
			text->SetWindowChild(true);
			// the names line up with the center, but not the scores.
			text->SetPositionByWindowSize(Vec2(0.85F, 0.275F) + offset * lNum);

			// TODO: line 10 needs to be shifted over slightly.
			// if(lNum == 9)
			// 	text->SetPositionByWindowSize(Vec2(0.6F, 0.275F) + offset * lNum - 0.002F);

			scores.push_back(text);

			objectList->AddObject(text);
			lNum++;
		}

		file.close();

		// Other Text

		// TOP 10
		{
			Text* text = new Text("TOP 10 PLAYERS", sceneName, font, Vec4(1.0F, 1.0F, 1.0F, 1.0F), 10.0F);
			text->SetWindowChild(true);
			text->SetPositionByWindowSize(Vec2(0.65F, 0.1F));
			objectList->AddObject(text);
		}

		// DESCRIPTION
		{
			std::string str = "";
			str = "NAME      " + spaceLine + spaceLine + "MAP        "+ "SCORE";

			Text* text = new Text(str, sceneName, font, Vec4(1.0F, 1.0F, 1.0F, 1.0F), textSize);
			text->SetWindowChild(true);
			text->SetPositionByWindowSize(Vec2(0.85F, 0.20F));
			objectList->AddObject(text);
		}
	}

	// logo
	// {
	// 	Image* image = new Image("res/images/codename_zero_logo_small.png", sceneName, false, false);
	// 	image->SetWindowChild(true);
	// 	image->SetPositionByWindowSize(Vec2(0.5F, 0.1F));
	// 	image->SetScale(0.4F);
	// 	objectList->AddObject(image);
	// }

	// exit button
	{
		Image* image = new Image("res/images/level_select_button.png", sceneName, false, false);
		image->SetWindowChild(true);
		image->SetPositionByWindowSize(Vec2(0.5F, 0.875F));
		image->SetScale(0.4F);

		// collision box
		PhysicsBodyBox* pbb = new PhysicsBodyBox(Vec3(0, 0, 0), image->GetMeshBodyMaximum() - image->GetMeshBodyMinimum());
		pbb->SetVisible(true);

		image->AddPhysicsBody(pbb);

		exitButton = new Button();
		exitButton->object = image; // setting image as the object.

		// creating the text.
		// exitButton->text = new Text("Back To Menu", sceneName, font, Vec4(1.0F, 1.0F, 1.0F, 1.0F), 10.0F);
		// exitButton->text->SetWindowChild(true);
		// exitButton->localTextPos = glm::vec3(0.0F, 0.0F, 1.0F);

		AddButton(exitButton, false);
		// UpdateButton(exitButton);
	}

	// liquid
	// if(false)
	// {
	// 	// the liquid doesn't appear anything else.
	// 	Liquid* liquid = new Liquid(sceneName, 150.0F, 100.0F);
	// 	
	// 	liquid->SetEnabledWaves(3);
	// 	liquid->SetWave(0, 1.0f, 0.0f, 0.50f, 6.0f);
	// 	liquid->SetWave(1, 0.0f, 1.0f, 0.25f, 3.1f);
	// 	liquid->SetWave(2, 1.0f, 1.4f, 0.20f, 1.8f);
	// 
	// 	liquid->SetGravity(9.81F);
	// 	liquid->SetColor(0.1F, 0.0F, 9.0F);
	// 	liquid->SetClarity(0.90F);
	// 
	// 	liquid->SetFresnelPower(0.5f);
	// 	liquid->SetRefractionIndex(1.0f, 1.34f);
	// 	liquid->SetEnvironment(Game::GetRunningGame()->GetCurrentScene()->Skybox);
	// 
	// 	liquid->SetPosition(0.0F, 0.0F, -45.0F);
	// 	// liquid->SetAlpha(0.45F);
	// 	// liquid->SetVisible(true);
	// 	objectList->AddObject(liquid);
	// }
}

void cnz::CNZ_RankingScene::OnClose()
{
	fileName = "";
	exitButton = nullptr;
	scores.clear();

	cherry::MenuScene::OnClose();
}

// generates a new instance of the ranking scene.
cherry::Scene* cnz::CNZ_RankingScene::GenerateNewInstance() const
{
	return new CNZ_RankingScene(GetName());
}

// mouse button pressed
void cnz::CNZ_RankingScene::MouseButtonPressed(GLFWwindow* window, int button)
{
	cherry::MenuScene::MouseButtonPressed(window, button);

	// menu click
	cherry::AudioEngine::GetInstance().PlayEvent("menu_click");
}

void cnz::CNZ_RankingScene::Update(float deltaTime)
{
	using namespace cherry;
	cherry::MenuScene::Update(deltaTime);

	CNZ_Game* const game = (CNZ_Game*)Game::GetRunningGame();

	// a button has been entered and the mouse has been pressed.
	if (enteredButton == exitButton && mousePressed)
	{
		// menu accept.
		AudioEngine::GetInstance().PlayEvent("menu_accept");

		game->SetCurrentScene(game->titleSceneName, true);
	}
}
