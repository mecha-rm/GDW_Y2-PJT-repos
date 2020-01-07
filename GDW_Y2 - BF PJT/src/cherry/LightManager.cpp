// LightManager - manages all lights for the game
#include "LightManager.h"
#include "utils/Utils.h"

std::vector<cherry::LightList*> cherry::LightManager::lightLists = std::vector<cherry::LightList*>();

// destructor
cherry::LightManager::~LightManager()
{
	for (LightList* ll : lightLists)
	{
		delete ll;
	}

	lightLists.clear();
}

// checks if a scene name has been taken.
bool cherry::LightManager::SceneLightListExists(const std::string sceneName)
{
	// checks if the scene name has been used.
	for (cherry::LightList * ll : lightLists)
	{
		if (ll->GetSceneName() == sceneName)
			return true;
	}

	return false;
}

// gets a light list via its index in the list.
cherry::LightList* cherry::LightManager::GetSceneLightListByIndex(unsigned int index)
{
	if (index >= lightLists.size()) // index out of bounds
		return nullptr;

	return lightLists[index];
}

// returns a light list for a scene.
cherry::LightList* cherry::LightManager::GetSceneLightListByName(std::string sceneName)
{
	// finds if a scene exists
	for (cherry::LightList* ll : lightLists)
	{
		if (ll->GetSceneName() == sceneName) // if the scene exists
		{
			return ll; // returns a pointer to the light list.
			break;
		}
	}

	return nullptr;
}

// adds a scene to the light manager.
bool cherry::LightManager::CreateSceneLightList(const std::string sceneName)
{
	// if the scene name already exists in the list.
	if (SceneLightListExists(sceneName))
	{
		return false;
	}
	else // if the scene doesn't exist in the list.
	{
		// adds a scene, and an associated list of lights.
		lightLists.push_back(new LightList(sceneName));
		return true;
	}
}

// adds a light to the scene.
bool cherry::LightManager::AddLightToSceneLightList(cherry::Light * light, bool addScene)
{
	if (light == nullptr) // light is nullptr
		return false;

	std::string sceneName = light->GetScene();
	// finds the scene
	for (cherry::LightList* ll : lightLists)
	{
		if (ll->GetSceneName() == sceneName) // if the scene exists
		{
			ll->AddLight(light); // returns a pointer to the light list.
			return true;
			break;
		}
	}

	// if the scene should be added if it doesn't already exist.
	if (addScene)
	{
		CreateSceneLightList(sceneName); // adds the scene
		GetSceneLightListByName(sceneName)->AddLight(light); // adds the light
		return true;
	}

	return false;
}

// gets all lights in the scene as a single light.
cherry::Light* cherry::LightManager::GetSceneLightsMerged(std::string sceneName)
{
	// gets the light list
	cherry::LightList* sceneLight = GetSceneLightListByName(sceneName);

	if (sceneLight != nullptr)
	{
		return sceneLight->GetLightsMerged();
	}
	else
	{
		return nullptr;
	}
}

// destroys a light list via its index
bool cherry::LightManager::DestroySceneLightListByIndex(unsigned int index)
{
	LightList* lgtList = GetSceneLightListByIndex(index);

	if (lgtList != nullptr) // if the object isn't a nullptr
	{
		util::removeFromVector(lightLists, lgtList);
		delete lgtList;

		return true;
	}
	else // if the object is a nullptr
	{
		return false;
	}
}

// destroys the light list via its pointer.
bool cherry::LightManager::DestroySceneLightListByPointer(cherry::LightList* ll)
{
	if (util::removeFromVector(lightLists, ll)) // in list
	{
		delete ll;
		return true;
	}
	else // was not in list
	{
		return false;
	}
}

// destroys a scene light list via its name
bool cherry::LightManager::DestroySceneLightListByName(std::string sceneName)
{
	LightList* ll = GetSceneLightListByName(sceneName); // destroys the light list.

	return DestroySceneLightListByPointer(ll);
}

// LIGHT LIST
// constructor
cherry::LightList::LightList(std::string scene) : scene(scene) {}

// gets the name of the scene the light list is for.
std::string cherry::LightList::GetSceneName() const { return scene; }

// gets the light list
int cherry::LightList::GetLightCount() const { return lights.size(); }

// gets the lights as a vector
std::vector<cherry::Light*>& cherry::LightList::GetLights() { return lights; }

// adds a light to the list
bool cherry::LightList::AddLight(cherry::Light* light)
{
	if(light == nullptr) // checking for nullptr
		return false;

	// adding to the vector
	return util::addToVector(lights, light);
}

// gets lights averaged
cherry::Light* cherry::LightList::GetLightsMerged()
{
	cherry::Light* sceneLight = nullptr; // returns the scene light

	cherry::Vec3 lightPos; // light position
	cherry::Vec3 lightClr; // colour

	cherry::Vec3 ambiClr; // ambient colour
	float ambiPwr = 0; // ambient power

	float lightSpecPwr = 0; // specular power
	float lightShine = 0; // shininess
	float lightAtten = 0; // attenuation

	// adds together all the values for each light.
	for (int i = 0; i < lights.size(); i++)
	{
		lightPos += lights.at(i)->GetLightPosition();
		lightClr += lights.at(i)->GetLightColor();

		ambiClr += lights.at(i)->GetAmbientColor();
		ambiPwr += lights.at(i)->GetAmbientPower();

		lightSpecPwr += lights.at(i)->GetLightSpecularPower();
		lightShine += lights.at(i)->GetLightShininess();
		lightAtten += lights.at(i)->GetLightAttenuation();
	}

	// creates a single light for the whole scene by averaging the values.
	sceneLight = new Light(scene,
		lightPos / lights.size(),
		lightClr / lights.size(),
		ambiClr / lights.size(),
		ambiPwr / lights.size(),
		lightSpecPwr / lights.size(),
		lightShine / lights.size(),
		lightAtten / lights.size()
	);

	return sceneLight;
}

// generates a material
cherry::Material::Sptr cherry::LightList::GenerateMaterial(std::string vs, std::string fs) const
{
	return GenerateMaterial(vs, fs, nullptr);
}

// generates the material
cherry::Material::Sptr cherry::LightList::GenerateMaterial(std::string vs, std::string fs, const TextureSampler::Sptr& sampler) const
{
	if(lights.size() == 0) // no lights
		return cherry::Material::Sptr();

	Material::Sptr material; // the material
	Shader::Sptr phong = std::make_shared<Shader>(); // shader
	glm::vec3 temp; // temporary vector
	
	int lightCount = 0; // total amount of lights

	// Must align with MAX_LIGHTS. It is set to allow for 10 lights.
	// TODO: add const to cap lights instead
	lightCount = (lights.size() > 10) ? 10 : lights.size();

	// used to make the albedo // TODO: fix shaders
	phong->Load(vs.c_str(), fs.c_str()); // the shader
	material = std::make_shared<Material>(phong); // loads in the shader.
	 
	material->Set("a_LightCount", lightCount);

	// goes through each light, getting the values.
	for (int i = 0; i < lightCount; i++)
	{
		temp = lights.at(i)->GetAmbientColorGLM();
		material->Set("a_AmbientColor[" + std::to_string(i) + "]", { temp[0], temp[1], temp[2] }); // ambient colour

		material->Set("a_AmbientPower[" + std::to_string(i) + "]", lights.at(i)->GetAmbientPower()); // ambient power
		material->Set("a_LightSpecPower[" + std::to_string(i) + "]", lights.at(i)->GetLightSpecularPower()); // specular power

		temp = lights.at(i)->GetLightPositionGLM();
		material->Set("a_LightPos[" + std::to_string(i) + "]", { temp[0], temp[1], temp[2] }); // position

		temp = lights.at(i)->GetLightColorGLM();
		material->Set("a_LightColor[" + std::to_string(i) + "]", { temp[0], temp[1], temp[2] }); // light colour

		material->Set("a_LightShininess[" + std::to_string(i) + "]", lights.at(i)->GetLightShininess()); // shininess
		material->Set("a_LightAttenuation[" + std::to_string(i) + "]", lights.at(i)->GetLightAttenuation()); // attenuation
	}

	// albedo values (with sampler)
	if (sampler != nullptr)
	{
		material->Set("s_Albedos[0]", Texture2D::LoadFromFile("res/images/default.png"), sampler);
		material->Set("s_Albedos[1]", Texture2D::LoadFromFile("res/images/default.png"), sampler);
		material->Set("s_Albedos[2]", Texture2D::LoadFromFile("res/images/default.png"), sampler);
	}
	else // no sampler provided
	{
		material->Set("s_Albedos[0]", Texture2D::LoadFromFile("res/images/default.png"));
		material->Set("s_Albedos[1]", Texture2D::LoadFromFile("res/images/default.png"));
		material->Set("s_Albedos[2]", Texture2D::LoadFromFile("res/images/default.png"));
	}

	 
	// returns the material
	return material;
}

// removes a light via its index
cherry::Light* cherry::LightList::RemoveLightByIndex(unsigned int index)
{
	if (index >= lights.size()) // bounds checking
		return nullptr;

	Light * ll = lights.at(index);

	if (util::removeFromVector(lights, ll)) // success
	{
		return ll;
	}
	else // failure
	{
		return nullptr;
	}
}

// removes a light via its pointer
cherry::Light* cherry::LightList::RemoveLightByPointer(cherry::Light* light)
{
	if (light == nullptr) // nullptr passed.
		return nullptr;

	if (util::removeFromVector(lights, light)) // removed successfully
	{
		return light;
	}
	else // failure
	{
		return nullptr;
	}
}

// deletes a light via its index.
bool cherry::LightList::DeleteLightByIndex(unsigned int index)
{
	Light* ll = RemoveLightByIndex(index);

	if (ll != nullptr) // light exists
	{
		delete ll;
		return true;
	}
	else // light doesn't exist
	{
		return false;
	}
}

// deletes the light via its pointer.
bool cherry::LightList::DeleteLightByPointer(cherry::Light* ll)
{
	if (ll == nullptr) // nullptr passed
		return false;

	if (RemoveLightByPointer(ll) != nullptr) // if the light was removed successfully
	{
		delete ll;
		return true;
	}
	else // not removed successfully
	{
		return false;
	}
}
