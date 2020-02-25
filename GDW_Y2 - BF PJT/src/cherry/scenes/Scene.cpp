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
}

// returns the main/primary registry
entt::registry& cherry::Scene::Registry() { return myRegistry; }

const std::string& cherry::Scene::GetName() const { return myName; }

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

// update loop.
void cherry::Scene::Update(float deltaTime)
{
}

