// LightManager - manages all lights for the game
#include "LightManager.h"
#include "utils/Utils.h"

// instantiation
std::vector<std::string> cherry::LightManager::scenes;

std::vector<std::vector<cherry::Light>*> cherry::LightManager::sceneLights;

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
		// adds a scene, and an associated list of lights.
		scenes.push_back(sceneName);
		sceneLights.push_back(new std::vector<cherry::Light>());
		return true;
	}
}

// checks if a scene name has been taken.
bool cherry::LightManager::SceneExists(const std::string sceneName)
{
	// checks if the scene name has been used.
	for (std::string scn : scenes)
	{
		if (scn == sceneName)
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

	// if the scene should be added if it doesn't already exist.
	if (addScene)
	{
		AddScene(sceneName); // adds the scene
		sceneLights[sceneLights.size() - 1]->push_back(light); // adds the light
		return true;
	}

	return false;
}

// returns a light list for a scene.
std::vector<cherry::Light>* cherry::LightManager::GetLightList(std::string sceneName)
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

// gets all lights in the scene as a single light.
cherry::Light* cherry::LightManager::GetSceneLightsMerged(std::string sceneName)
{
	Light* sceneLight = nullptr; // holds the lights when they get merged together
	std::vector<Light> * lights = GetLightList(sceneName); // the list of lights


	cherry::Vec3 lightPos; // light position
	cherry::Vec3 lightClr; // colour

	cherry::Vec3 ambiClr; // ambient colour
	float ambiPwr = 0; // ambient power

	float lightSpecPwr = 0; // specular power
	float lightShine = 0; // shininess
	float lightAtten = 0; // attenuation

	// adds together all the values for each light.
	for (int i = 0; i < lights->size(); i++)
	{
		lightPos += lights->at(i).GetLightPosition();
		lightClr += lights->at(i).GetLightColor();

		ambiClr += lights->at(i).GetAmbientColor();
		ambiPwr += lights->at(i).GetAmbientPower();

		lightSpecPwr += lights->at(i).GetLightSpecularPower();
		lightShine += lights->at(i).GetLightShininess();
		lightAtten += lights->at(i).GetLightAttenuation();
	}

	// creates a single light for the whole scene by averaging the values.
	sceneLight = new Light(sceneName,
		lightPos / lights->size(),
		lightClr / lights->size(),
		ambiClr / lights->size(),
		ambiPwr / lights->size(),
		lightSpecPwr / lights->size(),
		lightShine / lights->size(),
		lightAtten / lights->size()
	);

	return sceneLight;
}
