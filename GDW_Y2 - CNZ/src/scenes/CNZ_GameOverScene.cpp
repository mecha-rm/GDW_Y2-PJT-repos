/*
 * Name: Bonus Fruit
 * Date: 07/20/2020
 * Description: game over scene
 * References:
	* https://stackoverflow.com/questions/478075/creating-files-in-c
	* http://www.cplusplus.com/doc/tutorial/files/
	* https://stackoverflow.com/questions/8960087/how-to-convert-a-char-array-to-a-string
*/

// TODO: change the skybox, and format the text better.

#include "CNZ_GameOverScene.h"
#include "..\CNZ_Game.h"
#include "..\cherry/utils/sort/SelectionSort.h"
#include "..\cherry/utils/math/Rotation.h"

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
	cherry::MenuScene::OnOpen();

	const std::string SCENE_NAME = GetName();

	// reading in the scores
	std::string fileName = RANKING_FILE;
	std::ifstream file(fileName, std::ios::in);
	std::string line; // line from file.

	// an imgui window should be used.
	// useImgui = true;

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
			// [0] = name, [1] = map, [2] = score
			if (splitStr.size() == 3)
			{
				// name 
				newScore.name = splitStr[0];

				// map number
				if (util::isInt(splitStr[1]))
					newScore.mapNumber = util::stringToInt(splitStr[1]);

				// points
				if (util::isInt(splitStr[2]))
					newScore.points = util::stringToInt(splitStr[2]);
				else
					newScore.points = 0;

			}

			// saves the score.
			scores.push_back(newScore);
		}
		file.close();
	}

	// new instances allowed
	SetAllowingNewInstances(true);

	// skybox
	{
		cherry::Skybox skybox(
			"res/images/cubemaps/checkerboard_black-grey_d.jpg",
			"res/images/cubemaps/checkerboard_black-grey_d.jpg",
			"res/images/cubemaps/checkerboard_black-grey_d.jpg",
			"res/images/cubemaps/checkerboard_black-grey_d.jpg",
			"res/images/cubemaps/checkerboard_black-grey_d.jpg",
			"res/images/cubemaps/checkerboard_black-grey_d.jpg"
		);

		skybox.AddSkyboxToScene(this);
		cherry::Game::GetRunningGame()->SetSkyboxVisible(true);
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
		cherry::Game::GetRunningGame()->imguiMode = true; // TODO: make this more convenient.
	}

	// Text
	const cherry::Vec4 WHITE(1.0F, 1.0F, 1.0F, 1.0F);

	// GAME OVER
	{
		cherry::Text* text = new cherry::Text("GAME OVER", GetName(), FONT_ARIAL, WHITE, 25.0F);
		text->SetWindowChild(true);
		text->SetPositionByWindowSize(0.80F, 0.40F);

		objectList->AddObject(text);
	}

	// Final Score
	{
		cherry::Text* text = new cherry::Text("FINAL SCORE: " + std::to_string(playerScore), SCENE_NAME, FONT_ARIAL, WHITE, 25.0F);
		text->SetWindowChild(true);
		text->SetPositionByWindowSize(0.82F, 0.7F);

		objectList->AddObject(text);
	}

	// Button
	{
		cherry::Image* image = new cherry::Image("res/images/continue_button.png", SCENE_NAME, false, false);
		image->SetWindowChild(true);
		image->SetPositionByWindowSize(0.5F, 0.1F);
		image->SetScale(0.45F);

		// physics body.
		cherry::Vec3 size = image->GetMeshBodyMaximum() - image->GetMeshBodyMinimum();
		cherry::PhysicsBodyBox* pbb = new cherry::PhysicsBodyBox(cherry::Vec3(0, 0, 0), size * 2);
		image->AddPhysicsBody(pbb); // collision box

		// adds button
		exitButton = new cherry::Button();
		exitButton->object = image;
		AddButton(exitButton);
	}

	// Extras
	//{
	//	// path values
	//	cherry::Path path; // path
	//	const float RADIUS = 50.0F;
	//	const int NODE_COUNT = 10; // number of nodes

	//	// path
	//	// making nodes to travel between
	//	for (int i = 0; i < NODE_COUNT; i++)
	//	{
	//		util::math::Vec2 rotVec{ 0.0F, RADIUS };

	//		rotVec = util::math::rotate(rotVec, 360.0F / NODE_COUNT * i, true);
	//		path.AddNode(rotVec.x, rotVec.y, 0.0F);
	//	}
	//	
	//	// TODO: make path adjustments so different objects can have different starting points.

	//	cherry::Object * obj = new cherry::PrimitiveCube(10.0F);
	//	obj->SetPath(path);
	//	obj->UsePath(true);

	//	objectList->AddObject(obj);
	//}
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
	cherry::MenuScene::MouseButtonPressed(window, button);
}

// draw gui for entering in a new score.
void cnz::CNZ_GameOverScene::DrawGui(float deltaTime)
{
	cherry::Game* game = cherry::Game::GetRunningGame();

	const int NAME_LEN_LIMIT = NAME_CHAR_LIMIT;

	// entry text for player name
	char entryNameChr[NAME_LEN_LIMIT]; // array version

	entryNameStr.resize(NAME_LEN_LIMIT, '\0'); // fill rest of string with null termination character.
	memcpy(entryNameChr, entryNameStr.c_str(), NAME_LEN_LIMIT);

	// Open a new ImGui window
	ImGui::Begin("Score Entry"); // window title

	// Draw Widgits
	// ImGui::SetWindowSize(ImVec2(500.0F, 500.0F)); // window size for ImGUI Colour Picker (perament)
	// ImGui::SetNextWindowCollapsed(false);
	// ImGui::SetWindowPos(ImVec2(-225.0F, 1.0F));
	// ImGui::SetNextWindowSize(ImVec2(500.0F, 500.0F)); // window size for ImGUI ColorPicker (variable)

	ImGui::Text(("Ranking: " + std::to_string(playerRank)).c_str());
	if (ImGui::InputText("Name", entryNameChr, NAME_LEN_LIMIT))
	{
		entryNameStr = entryNameChr; // saves the name.
	}
	
	if (ImGui::Button("Enter")) // adding another button, which allows for the application of the window title.
	{
		// if characters were entered that aren't all spaces.
		if (util::replaceSubstring(entryNameStr, " ", "") != "")
		{
			// TODO: this is actually handled when putting in the scores. So we can take this out.
			// entryNameStr = util::replaceSubstring(entryNameStr, " ", "_"); // replaces all spaces with underscores.

			// resizes the name and fills empty spaces with underscores.
			// it would still print otherwise, but it would be full of unrecognizable characters.
			// since the string is full of null termination characters, the resize function doesn't work.
			// so a loop must be used instead.
			for (int i = 0; i < entryNameStr.size(); i++)
			{
				if (entryNameStr[i] == '\0') // if this is equal to a null termination character.
					entryNameStr[i] = '_';
			}

			// saves the name, map, and points for the current player.
			scores.at(playerRank - 1).name = entryNameStr;
			scores.at(playerRank - 1).mapNumber = mapNumber;
			scores.at(playerRank - 1).points = playerScore;

			// saves the scores.
			SaveScores();

			// the imgui window can be turned off now.
			useImgui = false;
			game->imguiMode = false;
			ImGui::CloseCurrentPopup();
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

// gets the score.
int cnz::CNZ_GameOverScene::GetScore() const
{
	return playerScore;
}

// sets the score.
void cnz::CNZ_GameOverScene::SetScore(int score)
{
	// the score cannot be negative
	playerScore = (score > 0) ? score : 0;
}

// gets the map
int cnz::CNZ_GameOverScene::GetMap() const
{
	return mapNumber;
}

// sets the map the score is for.
void cnz::CNZ_GameOverScene::SetMapNumber(int map)
{
	this->mapNumber = map;
}

// saves the scores in the vector.
void cnz::CNZ_GameOverScene::SaveScores()
{
	// TODO: save scores
	std::string fileName = RANKING_FILE;
	std::ofstream fw(fileName, std::ios::out); // opens the file, creating it if necessary.

	// the file does not exist.
	// if (!fileWrite)
	// {
	// 	std::ofstream newFile(fileName);
	// 	newFile << "Blank" << std::endl;
	// 	newFile.close();

	// puts in all scores based on a fixed number of scores that can be saved.
	for (int i = 0; i < scores.size() && i < RANKING_SCORES; i++)
	{
		// doing tab or space doesn't makea difference.
		
		// puts in the 10 names (name, map, and points)
		fw << 
			util::replaceSubstring(scores[i].name, " ", "_") << " " << 
			std::to_string(scores[i].mapNumber) << " " <<
			std::to_string(scores[i].points) << "\n";
	}

	fw.close();
}
	

// update loop
void cnz::CNZ_GameOverScene::Update(float deltaTime)
{
	using namespace cherry;

	MenuScene::Update(deltaTime);

	CNZ_Game* const game = (CNZ_Game*)Game::GetRunningGame();

	// a button has been entered and the mouse has been pressed.
	if (enteredButton != nullptr && mousePressed && useImgui == false)
	// if (enteredButton != nullptr && mousePressed)
	{
		// if the entered button is the contnue button, and imGui is off.
		if (enteredButton == exitButton) // exit button
		{
			// leaves and goes back to the title scene.
			game->SetCurrentScene(game->titleSceneName, false);

		}
		
	}
}


