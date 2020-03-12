#include "MenuScene.h"

#include <toolkit/Logging.h>
#include "..\Game.h"
#include "..\utils/math/Rotation.h"

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
}

// caled when the scene is being closed.
void cherry::MenuScene::OnClose()
{
	ObjectManager::DestroySceneObjectListByPointer(objectList);
	LightManager::DestroySceneLightListByPointer(lightList);

	Scene::OnClose();
}

// mouse button pressed
void cherry::MenuScene::MouseButtonPressed(GLFWwindow* window, int button)
{
}

// mouse button held
void cherry::MenuScene::MouseButtonHeld(GLFWwindow* window, int button)
{
}

// mouse button released
void cherry::MenuScene::MouseButtonReleased(GLFWwindow* window, int button)
{
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
cherry::Button& cherry::MenuScene::AddButton(cherry::Object* object, cherry::PhysicsBody* body, cherry::Text* text)
{
	// object doesn't exist.
	if (object == nullptr)
	{
		LOG_ERROR("Button could not be created");
		cherry::Button temp;
		return temp;
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

	cherry::Button button;
	button.object = object;
	button.body = body;
	button.text = text;

	buttons.push_back(button);
	return button;
}

// adds a button.
void cherry::MenuScene::AddButton(Button& button, bool addedToList)
{
	// if the button hasn't been added to hte object lists
	if (!addedToList)
	{
		objectList->AddObject(button.object);
		objectList->AddObject(button.text);
	}

	buttons.push_back(button);
}

// updates the button
void cherry::MenuScene::UpdateButton(Button& button)
{
	// button has no parent object, or no text child
	if (button.object == nullptr || button.text == nullptr)
		return;

	// updating text scale
	{
		// the scale of the text.
		glm::vec3 parentScale = button.object->GetScaleGLM();
		glm::vec3 childScale = button.localTextScale;

		button.text->SetScale(parentScale * childScale);
	}

	// updating rotation
	{
		button.text->SetRotationDegrees(button.object->GetRotationDegreesGLM() + button.localTextRot);
	}

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
			glm::vec3 parentPos = button.object->GetPositionGLM();

			parent[0][3] = parentPos.x;
			parent[1][3] = parentPos.y;
			parent[2][3] = parentPos.z;
			parent[3][3] = 1.0F;
		}

		// parent rotation
		{
			glm::vec3 parentRot = button.object->GetRotationDegreesGLM();
			
			rotX = util::math::getRotationMatrixX(parentRot.x, true);
			rotY = util::math::getRotationMatrixY(parentRot.y, true);
			rotZ = util::math::getRotationMatrixZ(parentRot.z, true);
		}

		// parent scale
		{
			glm::vec3 parentScale = button.object->GetScaleGLM();

			scale[0][0] = parentScale.x;
			scale[1][1] = parentScale.y;
			scale[2][2] = parentScale.z;
		}

		// rotation and scale.
		rotScale = scale * (rotZ * rotX * rotY);

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
		glm::mat4 child
		{
			button.localTextPos.x, 0, 0, 0,
			button.localTextPos.y, 0, 0, 0,
			button.localTextPos.z, 0, 0, 0,
			0, 0, 0, 0
		};

		result = parent * child;

		button.text->SetPosition(result[0][3], result[1][3], result[2][3]);
	}
}

// updates the buttons
void cherry::MenuScene::Update(float deltaTime)
{
	Scene::Update(deltaTime);
}