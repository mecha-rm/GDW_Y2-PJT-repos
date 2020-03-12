// Scene (Source) - currently holds nothing
#include "Scene.h"

// constructor
cherry::Scene::Scene(const std::string name) : myName(name)
{
}

// on open
void cherry::Scene::OnOpen()
{
}

// on close
void cherry::Scene::OnClose()
{
	// now uses shared pointers
	// for (PostLayer* layer : layers)
	// 	delete layer;
	
	layers.clear();
}

// returns the main/primary registry
entt::registry& cherry::Scene::Registry() { return myRegistry; }

const std::string& cherry::Scene::GetName() const { return myName; }

// returns 'true' if using imGui
bool cherry::Scene::IsUsingImgui() const { return useImgui; }

// returns 'true' if the frame buffer(s) are being used.
bool cherry::Scene::IsUsingFrameBuffers() const { return useFrameBuffers; }

// sets whether to use frame buffers or not.
void cherry::Scene::SetUsingFrameBuffers(bool useFbs) { useFrameBuffers = useFbs; }

// returns the post layers.
std::vector<cherry::PostLayer::Sptr> cherry::Scene::GetPostLayers() const { return layers; }

// void cherry::Scene::SetName(const std::string& name) { myName = name; }

// mouse button pressed.
void cherry::Scene::MouseButtonPressed(GLFWwindow* window, int button)
{
}

// mouse button held
void cherry::Scene::MouseButtonHeld(GLFWwindow* window, int button)
{
}

// mouse button released
void cherry::Scene::MouseButtonReleased(GLFWwindow* window, int button)
{
}

// key pressed
void cherry::Scene::KeyPressed(GLFWwindow* window, int key)
{
}

// key held
void cherry::Scene::KeyHeld(GLFWwindow* window, int key)
{
}

// key released
void cherry::Scene::KeyReleased(GLFWwindow* window, int key)
{
}

// DrawGUI
void cherry::Scene::DrawGui(float deltaTime)
{
}

// update loop.
void cherry::Scene::Update(float deltaTime)
{
}

