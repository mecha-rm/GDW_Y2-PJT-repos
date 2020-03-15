#include "EngineMenuScene.h"
#include "..\Game.h"

// constructor
cherry::EngineMenuScene::EngineMenuScene(std::string name)
	: MenuScene(name)
{

}

// loading the content
void cherry::EngineMenuScene::OnOpen()
{
	MenuScene::OnOpen();

	Game* const game = Game::GetRunningGame();

	if (game == nullptr)
		return;

	std::string sceneName = GetName();
	
	// gets the window size
	glm::ivec2 myWindowSize = game->GetWindowSize();

	// setting up the camera
	Camera::Sptr& myCamera = game->myCamera; // camera reference
	myCamera->clearColor = game->myClearColor; // setting the clear colour

	myCamera->SetPosition(glm::vec3(0, 0.001F, 1.0F)); 
	// myCamera->SetPosition(glm::vec3(0, 5, 12));
	myCamera->LookAt(glm::vec3(0));
	myCamera->SetPerspectiveMode(glm::radians(45.0f), 1.0f, 0.01f, 1000.0f);
	myCamera->SetOrthographicMode(
		-myWindowSize.x / 2.0F, myWindowSize.x / 2.0F, -myWindowSize.y / 2.0F, myWindowSize.y / 2.0F, 0.0f, 1000.0f, true);
	// myCamera->SetOrthographicMode(-5.0f, 5.0f, -5.0f, 5.0f, 0.0f, 100.0f, false);
	
	myCamera->targetOffset = cherry::Vec3(0, 5, 12);

	// secondary camera, which is used for UI for the game.
	Camera::Sptr& myCameraX = game->myCameraX;

	myCameraX->clearColor = game->myClearColor;
	myCameraX->SetPosition(0, 0.001F, 1.0F); // try adjusting the position of the perspecitve cam and orthographic cam
	myCameraX->Rotate(glm::vec3(0.0F, 0.0F, glm::radians(180.0f)));
	myCameraX->LookAt(glm::vec3(0));

	// this camera is used for UI elements
	myCameraX->SetPerspectiveMode(glm::radians(60.0f), 1.0f, 0.01f, 1000.0f, false);
	myCameraX->SetOrthographicMode(
		-myWindowSize.x / 2.0F, myWindowSize.x / 2.0F, -myWindowSize.y / 2.0F, myWindowSize.y / 2.0F, 0.0f, 1000.0f, true);


	// image 1
	{
		Button* button = new Button();
		Image* image = new Image("res/images/bonus_fruit_logo.png", sceneName, false, false);
		PhysicsBodyBox* pbb = new PhysicsBodyBox(image->GetWidth(), image->GetHeight(), 0.01F);
		image->AddPhysicsBody(pbb); // if addded here, collision doesn't trigger.
		image->SetWindowChild(true);
		image->SetScale(0.2F);
		// pbb->SetVisible(true);
		button->object = image;

		button->text = new Text("John", sceneName, FONT_ARIAL, Vec4(1.0F, 1.0F, 1.0F, 1.0F), 1.0F);
		button->text->SetPosition(0.0F, 0.0F, -10.0F);
		button->text->SetScale(0.01F);
		// button->text->SetWindowChild(true);

		// button->text->GetMesh()->cullFaces = false;
		button->text->SetVisible(true);
		objectList->AddObject(button->text);
		// button-

		// image->SetPositionByWindowSize(myWindowSize.x / 4.0F, myWindowSize.y / 2.0F * 3);
		// image->AddPhysicsBody(pbb);
		
		// objectList->AddObject(image);

		AddButton(button);
		// UpdateButton(button);
	}

	game->imguiMode = true;
}

// unloading the scene
void cherry::EngineMenuScene::OnClose()
{
	MenuScene::OnClose();

}

// update loop
void cherry::EngineMenuScene::Update(float deltaTime)
{
	MenuScene::Update(deltaTime);

	if (enteredButton != nullptr)
		std::cout << "STOP" << std::endl;

	if (enteredButton != nullptr && mousePressed == true)
	{
		Game::GetRunningGame()->SetCurrentScene(nextScene, false);
	}
}
