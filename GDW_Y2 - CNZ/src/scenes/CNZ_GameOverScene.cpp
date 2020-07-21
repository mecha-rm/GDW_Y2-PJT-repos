/*
 * Name: Bonus Fruit
 * Date: 07/20/2020
 * Description: game over scene
 * References:
	* https://stackoverflow.com/questions/478075/creating-files-in-c
	* http://www.cplusplus.com/doc/tutorial/files/
*/

#include "CNZ_GameOverScene.h"
#include "..\CNZ_Game.h"
#include "..\cherry/utils/sort/SelectionSort.h"

#include <toolkit/Logging.h>
#include <imgui/imgui.h>


// game over scene
cnz::CNZ_GameOverScene::CNZ_GameOverScene(std::string sceneName)
	: cherry::MenuScene(sceneName)
{
}

// on open
void cnz::CNZ_GameOverScene::OnOpen()
{
	// reading in the scores
	std::string fileName = RANKING_FILE;
	std::ifstream file(fileName, std::ios::in);
	std::string line; // line from file.

	// an imgui window should be used.
	useImgui = true;

	// if the file does not exist, or otherwise could not be opened.
	if (!file)
	{
		LOG_ERROR("Ranking file not found. New file needs to be created.");
		file.close();

		// creates the file.
		std::ofstream newFile(fileName);
		newFile << "Blank" << std::endl;
		newFile.close();

		// adds dummy scores
		for (int i = 0; i < RANKING_SCORES; i++)
			scores.push_back(Score{});
		
	}
	else
	{
		// while the file still has lines, get scores.
		while (std::getline(file, line))
		{
			// splits the line between the score and the name.
			std::vector<std::string> splitStr = util::splitString<std::string>(line);

			// adds in a score.
			Score newScore;


			// if the line is of the right length.
			// [0] = name, [1] = score
			if (splitStr.size() == 2)
			{
				// name 
				newScore.name = splitStr[0];

				// points
				if (util::isInt(splitStr[1]))
					newScore.points = util::stringToInt(splitStr[1]);
				else
					newScore.points = 0;

			}

			// saves the score.
			scores.push_back(newScore);
		}
		file.close();
	}

	// going through all the scores, from last to first.
	// this gets the rank of the player.
	for (int i = scores.size() - 1; i >= 0; i--)
	{
		if (playerScore > scores[i].points)
			playerRank = i + 1;
	}

	// if the player rank is less than or equal to the amount of available scores, their score will be saved.
	if (playerRank <= RANKING_SCORES)
	{
		useImgui = true;
	}
}

// on close
void cnz::CNZ_GameOverScene::OnClose()
{
}

// generates a new instance.
cherry::Scene* cnz::CNZ_GameOverScene::GenerateNewInstance() const
{
	return new CNZ_GameOverScene(GetName());
}

// called when a mouse button as been pressed.
void cnz::CNZ_GameOverScene::MouseButtonPressed(GLFWwindow* window, int button)
{
}

// draw gui for entering in a new score.
void cnz::CNZ_GameOverScene::DrawGui(float deltaTime)
{
	cherry::Game* game = cherry::Game::GetRunningGame();

	// window title (char array)
	char myWindowTitle[WINDOW_TITLE_CHAR_MAX];

	// the window title (as a string)
	// std::string wtStr = game->GetWindowTitle();
	std::string wtStr = "Score Enterer";
	
	char entryNameChr[NAME_CHAR_LIMIT];
	std::string entryNameStr = "";

	// Open a new ImGui window
	ImGui::Begin("Score Entry");

	// Draw Widgits
	// ImGui::SetWindowSize(ImVec2(500.0F, 500.0F)); // window size for ImGUI Colour Picker (perament)
	// ImGui::SetNextWindowCollapsed(false);
	// ImGui::SetNextWindowPos(ImVec2(-225.0F, 1.0F));
	ImGui::SetNextWindowSize(ImVec2(500.0F, 500.0F)); // window size for ImGUI ColorPicker (variable)

	ImGui::Text("Ranking: " + playerRank);
	if (ImGui::InputText("Name: ", entryNameChr, NAME_CHAR_LIMIT))
	{
		entryNameStr = entryNameChr; // saves the name.
	}
	
	if (ImGui::Button("Enter")) // adding another button, which allows for the application of the window title.
	{
		// if characters were entered that aren't all spaces.
		if (util::replaceSubstring(entryNameStr, " ", "") != "")
		{
			entryNameStr = util::replaceSubstring(entryNameStr, " ", "_"); // replaces all spaces with underscores.

			// saves the name and points for the current player.
			scores.at(playerRank - 1).name = entryNameStr;
			scores.at(playerRank - 1).points = playerScore;

			// saves the scores.
			SaveScores();

			// the imgui window can be turned off now.
			useImgui = false;
		}
	}
	// if (ImGui::Button("Wireframe/Fill Toggle"))
	// {
	// 	for (cherry::Object* obj : objectList->objects)
	// 		obj->SetWireframeMode();
	// }
	// 
	// // changing the camera mode
	// std::string camMode = myCamera->InPerspectiveMode() ? "Perspective" : "Orthographic";
	// ImGui::InputText((std::string("CAMERA MODE (\'SPACE\')") + camMode).c_str(), myWindowTitle, WINDOW_TITLE_CHAR_MAX);

	ImGui::End();

	Scene::DrawGui(deltaTime);
}

// sets the score.
void cnz::CNZ_GameOverScene::SetScore(float score)
{
	playerScore = score;
}

// saves the scores in the vector.
void cnz::CNZ_GameOverScene::SaveScores()
{
	// TODO: save scores
}

// update loop
void cnz::CNZ_GameOverScene::Update(float deltaTime)
{
	cherry::MenuScene::Update(deltaTime);
}


