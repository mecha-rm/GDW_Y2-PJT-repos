// handles keeping track of what lights are for what scenes
#pragma once

#include "Light.h"
// #include "SceneManager" // gets current m_Scene so it can switch automatically

namespace cherry
{
	class LightManager
	{
	public:

		// LightManager();

		// adds a scene to the light manager. This will generate a list of lights for said scene.
		// if 'true' is returned, then the scene was added.
		bool AddScene(const std::string sceneName);
		
		// checks to see if a scene exists, returning true if it does.
		bool SceneExists(const std::string scene);

		// adds a light to a given scene. If this scene doesn't exist, then a false is retuned.
		// if 'addScene' is true, then the scene is added if its name couldn't be found, and the light is added to it.
		bool AddLight(const std::string sceneName, const cherry::Light & light, bool addScene = false);

		// removes a scene
		// LightList

		// gets a pointer to the list of lights for a scene. If this scene doesn't exist, then a nullptr is returned.
		std::vector<cherry::Light>* GetLightList(std::string sceneName) const;

		

		// averageLights(); // averages all lights in the m_Scene

	private:

		// std::string currentScene; // the current scene being used.

		std::vector<std::string> scenes; // a vector of scenes

		// a vector that holds pointer vectors
		std::vector<std::vector<cherry::Light> *>sceneLights;

	protected:

	};
}

