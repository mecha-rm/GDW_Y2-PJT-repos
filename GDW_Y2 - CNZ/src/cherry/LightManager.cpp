#include "LightManager.h"
#include "utils/Utils.h"


// adds a scene to the light manager.
bool cherry::LightManager::AddScene(const std::string sceneName)
{
	// if the scene name already exists in the list.
	if (SceneExists(sceneName))
	{
		return false;
	}
	else // if the scene doesn't exist in the list.
	{
		scenes.push_back(sceneName);
		return true;
	}
}

// checks if a scene name has been taken.
bool cherry::LightManager::SceneExists(const std::string scene)
{
	// checks if the scene name has been used.
	for (std::string scn : scenes)
	{
		if (scn == scene)
			return true;
	}

	return false;
}

// adds a light to the scene.
bool cherry::LightManager::AddLight(const std::string sceneName, const cherry::Light & light, bool addScene)
{
	// finds the scene
	for (int i = 0; i < scenes.size(); i++)
	{
		if (scenes[i] == sceneName) // if the scene exists
		{
			sceneLights[i]->push_back(light); // returns a pointer to the light list.
			return true;
			break;
		}
	}

	// if the scene should be added if it doesn't exist.
	if (addScene)
	{
		AddScene(sceneName); // adds the scene
		sceneLights[sceneLights.size() - 1]->push_back(light); // adds the light
		return true;
	}

	return false;
}

// returns a light list for a scene.
std::vector<cherry::Light>* cherry::LightManager::GetLightList(std::string sceneName) const
{
	// finds if a scene exists
	for (int i = 0; i < scenes.size(); i++)
	{
		if (scenes[i] == sceneName) // if the scene exists
		{
			return sceneLights[i]; // returns a pointer to the light list.
			break;
		}
	}

	return nullptr;
}
