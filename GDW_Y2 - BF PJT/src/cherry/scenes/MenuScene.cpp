#include "MenuScene.h"

#include <toolkit/Logging.h>
#include "..\Game.h"
#include "..\utils/math/Rotation.h"

// collision for the mouse
// cherry::PhysicsBodyBox* cherry::MenuScene::mouseBox = new cherry::PhysicsBodyBox(1.0F, 1.0F, 0.1F);

// constructor
cherry::MenuScene::MenuScene(const std::string name)
	: Scene(name)
{
}


// opening the scene
void cherry::MenuScene::OnOpen()
{
	Game* game = Game::GetRunningGame();
	glm::ivec2 myWindowSize = game->GetWindowSize();

	ObjectManager::CreateSceneObjectList(GetName());
	objectList = ObjectManager::GetSceneObjectListByName(GetName());

	LightManager::CreateSceneLightList(GetName());
	lightList = LightManager::GetSceneLightListByName(GetName());

	// cursor hitbox
	cursorBox = new PhysicsBodyBox(2.0F, 2.0F, 0.1F);

	// doesn't do anything.
	// cursorBox->SetVisible(true);
}

// caled when the scene is being closed.
void cherry::MenuScene::OnClose()
{
	// mouse collision
	// delete cursorObject; // deletes cursorBox as well.
	delete cursorBox;

	// deletes all buttons
	// the objects, physics bodies, and texts will be deleted by their resepctive managers.
	for (Button* button : buttons)
		delete button;

	buttons.clear();

	// destroys the scene and light list. The objects part of these lights get removed as well.
	ObjectManager::DestroySceneObjectListByPointer(objectList);
	LightManager::DestroySceneLightListByPointer(lightList);

	// resetting mouse key values.
	enteredButton = nullptr;
	mousePressed = false;
	mouseKey = -1;

	// now uses shared pointers
	// for (PostLayer* layer : layers)
	// 	delete layer;

	layers.clear();

	// this is empty
	Scene::OnClose();
}

// generates a new instance.
cherry::Scene* cherry::MenuScene::GenerateNewInstance() const
{
	return new MenuScene(GetName());
}

// mouse button pressed
void cherry::MenuScene::MouseButtonPressed(GLFWwindow* window, int button)
{
	// saving the button
	// mouseKeyLog.push(button);
	mouseKey = button;
	mousePressed = true;
}

// mouse button held
void cherry::MenuScene::MouseButtonHeld(GLFWwindow* window, int button)
{
}

// mouse button released
void cherry::MenuScene::MouseButtonReleased(GLFWwindow* window, int button)
{
	// if the button released is the same as the one most recently pressed.
	if(button == mouseKey)
		mousePressed = false;
}

// key pressed
void cherry::MenuScene::KeyPressed(GLFWwindow* window, int key)
{
}

// key held
void cherry::MenuScene::KeyHeld(GLFWwindow* window, int key)
{
}

// key released
void cherry::MenuScene::KeyReleased(GLFWwindow* window, int key)
{
}

// imgui draw
void cherry::MenuScene::DrawGui(float deltaTime)
{
	Scene::DrawGui(deltaTime);
}

// adds a button to the scene
cherry::Button* cherry::MenuScene::AddButton(cherry::Object* object, cherry::PhysicsBody* body, cherry::Text* text)
{
	// object doesn't exist.
	if (object == nullptr)
	{
		LOG_ERROR("Button could not be created");
		return nullptr;
	}

	objectList->AddObject(object);

	// creates the body if it doesn't exist.
	if (body == nullptr)
		body = new PhysicsBodyBox(Vec3(), object->GetMeshBodyMaximum() - object->GetMeshBodyMinimum());
	
	object->AddPhysicsBody(body);

	// if the text doesn't exist.
	if (text == nullptr)
		text = new Text("[?]", GetName(), FONT_ARIAL, Vec4(1.0F, 1.0F, 1.0F, 1.0F), 1.0F);
	
	// adds in the text.
	objectList->AddObject(text);

	cherry::Button * button = new Button();
	button->object = object;
	button->text = text;

	buttons.push_back(button);
	return button;
}

// adds a button->
void cherry::MenuScene::AddButton(Button* button, bool addedToList)
{
	// if the button hasn't been added to hte object lists
	if (!addedToList)
	{
		objectList->AddObject(button->object);
		objectList->AddObject(button->text);
	}

	buttons.push_back(button);
}

// removes a button, and returns it.
cherry::Button* cherry::MenuScene::RemoveButtonByPointer(Button* button)
{
	// removes object and text
	if (button != nullptr)
	{
		// removes object, and text
		objectList->RemoveObjectByPointer(button->object);
		objectList->RemoveObjectByPointer(button->text);
	}

	// removes the button
	util::removeFromVector(buttons, button);

	return button;
}

// removes the button by its index in the list.
cherry::Button* cherry::MenuScene::RemoveButtonByIndex(int index)
{
	// button not in list
	if (index < 0 || index >= buttons.size())
		return nullptr;

	return RemoveButtonByPointer(buttons[index]);
}

// deletes the button
bool cherry::MenuScene::DeleteButtonByPointer(Button* button)
{
	Button * b = RemoveButtonByPointer(button);

	// button removed successfully.
	if (b != nullptr)
	{
		delete b;
		return true;
	}
	else
	{
		return false;
	}
}

// deletes a button by index
bool cherry::MenuScene::DeleteButtonByIndex(int index)
{
	Button* b = RemoveButtonByIndex(index);

	if (b != nullptr)
	{
		delete b;
		return true;
	}
	else
	{
		return false;
	}
}

// updates the button
void cherry::MenuScene::UpdateButton(Button* button)
{
	// button has no parent object, or no text child
	if (button->object == nullptr || button->text == nullptr)
		return;

	// gets the new rotation
	Vec3 newRot = button->object->GetRotationDegrees() + button->localTextRotationDeg;
	button->text->SetRotationDegrees(newRot);

	// gets the new scale
	Vec3 newScl = button->object->GetScale() + button->localTextScale;
	button->text->SetScale(newScl);

	
	// updating position if any of the values have been changed.
	{
		// the text box is the parent.
		glm::mat4 parent = glm::mat4(1.0F);
	
		// the resulting matrix.
		glm::mat4 result = glm::mat4(1.0F);
	
		// rotation and scale
		util::math::Mat3 rotScale{
			1.0F, 0.0F, 0.0F,
			0.0F, 1.0F, 0.0F,
			0.0F, 0.0F, 1.0F
		};
	
		// scale
		util::math::Mat3 scale = rotScale;
	
		// rotations
		util::math::Mat3 rotX = rotScale;
		util::math::Mat3 rotY = rotScale;
		util::math::Mat3 rotZ = rotScale;
	
		// parent translation (from world origin)
		{
			glm::vec3 parentPos = button->object->GetPositionGLM();
	
			parent[0][3] = parentPos.x;
			parent[1][3] = parentPos.y;
			parent[2][3] = parentPos.z;
			parent[3][3] = 1.0F;
		}
	
		// parent rotation
		{
			glm::vec3 parentRot = button->object->GetRotationDegreesGLM();
			
			rotX = util::math::getRotationMatrixX(parentRot.x, true);
			rotY = util::math::getRotationMatrixY(parentRot.y, true);
			rotZ = util::math::getRotationMatrixZ(parentRot.z, true);
		}
	
		// parent scale
		{
			glm::vec3 parentScale = button->object->GetScaleGLM();
	
			scale[0][0] = parentScale.x;
			scale[1][1] = parentScale.y;
			scale[2][2] = parentScale.z;
		}
	
		// rotation and scale.
		rotScale = scale * (rotZ * rotX * rotY);
		
		{
			// saving the rotation and scale transformations.
			parent[0][0] = rotScale[0][0];
			parent[0][1] = rotScale[0][1];
			parent[0][2] = rotScale[0][2];

			parent[1][0] = rotScale[1][0];
			parent[1][1] = rotScale[1][1];
			parent[1][2] = rotScale[1][2];

			parent[2][0] = rotScale[2][0];
			parent[2][1] = rotScale[2][1];
			parent[2][2] = rotScale[2][2];

			// text transformation matrix
			glm::mat4 child;

			child[0][3] = button->localTextPos.x;
			child[1][3] = button->localTextPos.y;
			child[2][3] = button->localTextPos.z;

			result = parent * child;

			button->text->SetPosition(result[0][3], result[1][3], result[2][3]);
		}
	}
}

// updates the buttons
void cherry::MenuScene::Update(float deltaTime)
{
	Scene::Update(deltaTime); // update loop

	// game and window
	Game* const game = Game::GetRunningGame();
	glm::ivec2 windowSize = game->GetWindowSize();

	// cursor position (screen space)
	glm::dvec2 cursorPos = game->GetCursorViewPositionGLM();

	// checks to see if the cursor has been flipped on the x-axis.
	bool xFlip = false;

	// cursor world space position
	// the button positions are based on world space, but use an orthographic camera.
	// the camera is looking at the origin of the game world, so the centre of the screen is (0, 0).
	// the mouse position is given in screen space, where the bottom left-hand corner is the origin (0, 0)
	// glm::vec3 cursor_wpos = { cursorPos.x, cursorPos.y, 0.0F };

	// std::cout << "C1: " << Vec3(cursorPos.x, cursorPos.y, 0.0F).ToString() << std::endl;
	// std::cout << "C2: " << Vec3(cursor_wpos).ToString() << std::endl;
	// std::cout << "" << std::endl;


	// sets the position of the mouse box so that it's aligned with the mouse position.
	// if the window direction is still reversed, this reverses the cursor position for the hitbox.
	cursorBox->SetLocalPosition(cursorPos.x, cursorPos.y, 0.0F);
	// cursorObject->Update(deltaTime); // update object representing mouse

	// checks for collision
	bool collision = false;

	// button collision check
	for (Button* button : buttons)
	{
		if (WINDOW_X_DIR_REVERSED)
		{
			// if the button is a window child, but the x-position has not been flipped for the cursor hitbox, it gets flipped.
			if (button->object->IsWindowChild() && xFlip == false)
			{
				// reverses position
				cursorBox->SetLocalPosition(-cursorPos.x, cursorPos.y, 0.0F);
				xFlip = true;
			}
			// if the button isn't a window child, but the x-position of the cursor has been flipped, it flips it back.
			else if (!button->object->IsWindowChild() && xFlip == true)
			{
				// back to normal position
				cursorBox->SetLocalPosition(cursorPos.x, cursorPos.y, 0.0F);
				xFlip = false;
			}
		}

		// all physics bodies
		std::vector<PhysicsBody*> bodies = button->object->GetPhysicsBodies();
		
		for (PhysicsBody* body : bodies)
		{
			collision = PhysicsBody::Collision(body, cursorBox);
			if (collision) // button collided with.
			{
				enteredButton = button;
				break;
			}
		}
		
		if (collision)
			break;

		// collision check
		// collision = PhysicsBody::Collision(button->body, cursorBox);
	}

	// no button has been entered yet.
	if (collision == false)
		enteredButton = nullptr;
	
}