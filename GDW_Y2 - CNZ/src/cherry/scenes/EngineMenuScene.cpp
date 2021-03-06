#include "EngineMenuScene.h"
#include "..\Game.h"

// used for ADS - GDW Component
#include <chrono>
#include <stack>

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
	
	// myCamera->SetPerspectiveMode(true);
	// myCamera->SetOrthographicMode(-5.0f, 5.0f, -5.0f, 5.0f, 0.0f, 100.0f, false);
	
	myCamera->targetOffset = cherry::Vec3(0, 5, 12);

	// secondary camera, which is used for UI for the game.
	Camera::Sptr& myCameraX = game->myCameraX;

	myCameraX->clearColor = game->myClearColor;
	myCameraX->SetPosition(0, 0.001F, 1.0F); // try adjusting the position of the perspecitve cam and orthographic cam
	myCameraX->Rotate(glm::vec3(0.0F, 0.0F, 0.0f));
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

		button->text = new Text("DEBUG SCENE", sceneName, FONT_ARIAL, Vec4(1.0F, 1.0F, 1.0F, 1.0F), 10.0F);
		// button->text->ClearText();
		button->text->SetText("DEBUG MENU");
		// button->text->SetWindowChild(false);
		button->text->SetPosition(40.0F, 0.0F, 0.0F);
		
		
		button->text->SetWindowChild(true);
		button->text->SetPositionByWindowSize(Vec2(0.9F, 0.30F));
		button->text->SetRotationZDegrees(15.0F);


		// button->text->GetMesh()->cullFaces = false;
		button->text->SetVisible(true);
		// objectList->AddObject(button->text);
		// button-

		// image->SetPositionByWindowSize(myWindowSize.x / 4.0F, myWindowSize.y / 2.0F * 3);
		// image->AddPhysicsBody(pbb);
		
		// objectList->AddObject(image);

		AddButton(button);
		// UpdateButton(button);

		Text* tester = new Text("Click Title to Enter Scene", GetName(), FONT_ARIAL, Vec4(0.0F, 0.0F, 0.0F, 1.0F), 8);
		tester->SetWindowChild(true);
		tester->SetPositionByWindowSize(0.82F, 0.83F);

		// tester->SetPosition(5.0F, 10.0F, -2.0F);
		// tester->SetScale(1.9F);
		// tester->SetRotationZDegrees(14.0F);
		objectList->AddObject(tester);

	}

	// Algorithm Test (Algorithms and Data Structures GDW Component)
	// for(int n = 1000; n <= 10000; n += 1000)
	// {
	// 	std::stack<Object*> delStack;
	// 	
	// 	// shader
	// 	Shader::Sptr shader = std::make_shared<Shader>();
	// 	shader->Load("res/shaders/shader.vs.glsl", "res/shaders/shader.fs.glsl");
	// 
	// 	// material
	// 	Material::Sptr mat = std::make_shared<Material>(shader);
	// 
	// 	// trial
	// 	const int N = n;
	// 
	// 	// adding values
	// 	for (int i = 1; i <= N; i++)
	// 	{
	// 		Object* obj = new PrimitiveCube();
	// 		obj->CreateEntity(sceneName, mat);
	// 		
	// 		objectList->AddObject(obj);
	// 		delStack.push(obj);
	// 	}
	// 
	// 	// Deletion
	// 	auto start = std::chrono::high_resolution_clock::now(); // current time.
	// 
	// 	// while the stack isn't empty.
	// 	while (!delStack.empty())
	// 	{
	// 		// this is a function that removes an object from its object list, then deletes it.
	// 		objectList->DeleteObjectByPointer(delStack.top());
	// 
	// 		// pops the top of the stack.
	// 		delStack.pop();
	// 	}
	// 
	// 	auto end = std::chrono::high_resolution_clock::now(); // current time.
	// 
	// 	// printing
	// 	std::cout << "\n" << "(n = " << N << ") - Time: "
	// 		<< std::chrono::duration<double, std::milli>(end - start).count() << "\n";
	// 
	// }
	// std::cout << std::endl;

	game->imguiMode = true;

	// calling on window resize to fix aspect ratio
	Game::GetRunningGame()->Resize(myWindowSize.x, myWindowSize.y);
}

// unloading the scene
void cherry::EngineMenuScene::OnClose()
{
	nextScene = "";

	MenuScene::OnClose();

}

// generates a new instance of the engine menu scene.
cherry::Scene* cherry::EngineMenuScene::GenerateNewInstance() const
{
	return new EngineMenuScene(GetName());
}

// update loop
void cherry::EngineMenuScene::Update(float deltaTime)
{
	MenuScene::Update(deltaTime);

	// if (enteredButton != nullptr)
	// 	std::cout << "STOP" << std::endl;

	if (enteredButton != nullptr && mousePressed == true)
	{
		Game::GetRunningGame()->SetCurrentScene(nextScene, false);
	}
}
