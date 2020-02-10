// LightManager - manages all lights for the game
#pragma once

#include "Light.h"
#include <vector>

// the maximum amount of lights
// if changed, make sure it is adjusted in the shaders as well.
#define MAX_LIGHTS 16

namespace cherry
{
	class ObjectList;
	class LightList;

	class LightManager
	{
	public:

		// constructor
		LightManager() = default;

		// destructor
		~LightManager();

		// checks to see if the requested scene exists in the list, returning true if it does.
		static bool SceneLightListExists(const std::string sceneName);

		// gets the light list via its index.
		static cherry::LightList* GetSceneLightListByIndex(unsigned int index);

		// gets a pointer to the list of lights for a scene. If this scene doesn't exist, then a nullptr is returned.
		static cherry::LightList * GetSceneLightListByName(std::string sceneName);

		// adds a scene to the light manager. This will generate a list of lights for said scene.
		// if 'true' is returned, then the scene was added.
		static bool CreateSceneLightList(const std::string sceneName);
	

		// adds a light to the scene stored in the light.
		// if 'addScene' is true, then the scene is added if its name couldn't be found, and the light is added to said scene's list.
		static bool AddLightToSceneLightList(cherry::Light * light, bool addScene = false);
		
		// applies the lights to the object list. If a light list for this object list doesn't exist, nothing happens.
		static void ApplyLightsToObjects(cherry::ObjectList* objList);

		// returns the average of all lights in the scene, returning them as a single light.
		// TODO: create a lighting shader that takes multiple lights.
		// TODO: move this to the light list. Delete this?
		static cherry::Light* GetSceneLightsMerged(std::string sceneName);

		// deletes a scene object list by using its index.
		static bool DestroySceneLightListByIndex(unsigned int index);

		// deletes an scene list via finding its pointer.
		static bool DestroySceneLightListByPointer(cherry::LightList* ll);

		// deletes a scene list via the name of the scene.
		static bool DestroySceneLightListByName(std::string sceneName);

		

	private:

		// a vector of light list
		static std::vector<cherry::LightList *> lightLists;

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

		// gets the vector of lights.
		// adding or removing values
		std::vector<cherry::Light*> GetLights();

		// adds a light to the list.
		// if the list is full, a 'false' is returned, and the light isn't added.
		bool AddLight(cherry::Light * light);

		// gets the scene lights averaged together
		cherry::Light* GetLightsMerged();

		// generates a material out of the light list (first 10 in the list)
		// TODO: add indexes of lights to be used.
		// generates a material without a sampler
		cherry::Material::Sptr GenerateMaterial(std::string vs, std::string fs) const;
		
		// generates the material, providing it with the sampler.
		cherry::Material::Sptr GenerateMaterial(std::string vs, std::string fs, const TextureSampler::Sptr& sampler) const;

		// applies all the lights in the list.
		void ApplyLights(cherry::Material::Sptr& material);

		// applies the lights to the provided material.
		// TODO: remove this.
		void ApplyLights(cherry::Material::Sptr & material, int lightCount);

		// removes a light by its index.
		cherry::Light* RemoveLightByIndex(unsigned int index);

		// removes a light from the list via its pointer.
		cherry::Light* RemoveLightByPointer(cherry::Light * light);

		// removes the first light with // TODO: add tag
		// cherry::Light* RemoveLightByTag(std::string tag);

		// deletes an object from memory based on a provided index.
		bool DeleteLightByIndex(unsigned int index);

		// deletes a light from the list based on a provided pointer.
		bool DeleteLightByPointer(cherry::Light* ll);

		// deletes an object based on its name.
		// bool DeleteLightByTag(std::string name);

		// updates the lights for the objects its attachted it.
		void Update(float deltaTime);

		// vector of lights
		std::vector<cherry::Light *> lights;

	private:
		std::string scene = ""; // the scene te object is in.

	protected:
	};
}

