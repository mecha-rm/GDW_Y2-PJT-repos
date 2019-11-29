// LightManager - manages all lights for the game
#pragma once

#include "Light.h"

#include <vector>


namespace cherry
{
	class LightManager
	{
	public:

		// constructor
		LightManager() = default;

		// adds a scene to the light manager. This will generate a list of lights for said scene.
		// if 'true' is returned, then the scene was added.
		static bool AddScene(const std::string sceneName);
		
		// checks to see if the requested scene exists in the list, returning true if it does.
		static bool SceneExists(const std::string sceneName);

		// adds a light to a given scene. If this scene doesn't exist, then a false is retuned.
		// if 'addScene' is true, then the scene is added if its name couldn't be found, and the light is added to said scene's list.
		static bool AddLight(const std::string sceneName, const cherry::Light & light, bool addScene = false);

		// gets a pointer to the list of lights for a scene. If this scene doesn't exist, then a nullptr is returned.
		static std::vector<cherry::Light>* GetLightList(std::string sceneName);

		// returns the average of all lights in the scene, returning them as a single light.
		// TODO: create a lighting shader that takes multiple lights
		static Light* GetSceneLightsMerged(std::string sceneName);


	private:

		// a vector of scene names
		static std::vector<std::string> scenes;

		// a vector that holds a vector of Light pointers. 
		// It's structured this way so that scenes can get a reference to the light list attachted to them.
		static std::vector<std::vector<cherry::Light> *>sceneLights;

	protected:

	};
}

