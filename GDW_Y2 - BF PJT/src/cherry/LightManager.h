// LightManager - manages all lights for the game
#pragma once

#include "Light.h"
#include <vector>


namespace cherry
{
	class LightList;

	class LightManager
	{
	public:

		// constructor
		LightManager() = default;

		// checks to see if the requested scene exists in the list, returning true if it does.
		bool SceneLightListExists(const std::string sceneName);

		// gets a pointer to the list of lights for a scene. If this scene doesn't exist, then a nullptr is returned.
		cherry::LightList * GetSceneLightList(std::string sceneName);

		// adds a scene to the light manager. This will generate a list of lights for said scene.
		// if 'true' is returned, then the scene was added.
		bool AddSceneLightList(const std::string sceneName);
	

		// adds a light to a given scene. If this scene doesn't exist, then a false is retuned.
		// if 'addScene' is true, then the scene is added if its name couldn't be found, and the light is added to said scene's list.
		bool AddLightToSceneLightList(const std::string sceneName, cherry::Light * light, bool addScene = false);
		

		// returns the average of all lights in the scene, returning them as a single light.
		// TODO: create a lighting shader that takes multiple lights.
		// TODO: move this to the light list
		cherry::Light* GetSceneLightsMerged(std::string sceneName);


	private:

		// a vector of light list
		std::vector<cherry::LightList *> lightLists;

	protected:

	};

	// light list
	class LightList
	{
	public:
		LightList(std::string scene);

		// gets the name of the scene the light list belongs to.
		std::string GetSceneName() const;

		// gets the light count
		int GetLightCount() const;

		// gets the vector of lights
		std::vector<cherry::Light*>& GetLights();

		// adds a light to the list.
		bool AddLight(cherry::Light * light);

		// gets the scene lights averaged together
		cherry::Light* GetLightsMerged();

		// generates a material out of the light list (first 10 in hte list)
		// TODO: add indexes of lights to be used.
		// generates a material without a sampler
		cherry::Material::Sptr GenerateMaterial(std::string vs, std::string fs) const;
		
		cherry::Material::Sptr GenerateMaterial(std::string vs, std::string fs, const TextureSampler::Sptr& sampler) const;

		// vector of lights
		std::vector<cherry::Light *> lights;

	private:
		std::string scene = ""; // the scene te object is in.

	protected:
	};
}

