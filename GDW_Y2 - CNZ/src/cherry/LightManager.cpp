// LightManager - manages all lights for the game
#include "LightManager.h"
#include "utils/Utils.h"

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

// returns a light list for a scene.
cherry::LightList* cherry::LightManager::GetSceneLightList(std::string sceneName)
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
bool cherry::LightManager::AddSceneLightList(const std::string sceneName)
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
bool cherry::LightManager::AddLightToSceneLightList(const std::string sceneName, cherry::Light * light, bool addScene)
{
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
		AddSceneLightList(sceneName); // adds the scene
		GetSceneLightList(sceneName)->AddLight(light); // adds the light
		return true;
	}

	return false;
}

// gets all lights in the scene as a single light.
cherry::Light* cherry::LightManager::GetSceneLightsMerged(std::string sceneName)
{
	// gets the light list
	cherry::LightList* sceneLight = GetSceneLightList(sceneName);

	if (sceneLight != nullptr)
	{
		return sceneLight->GetLightsMerged();
	}
	else
	{
		return nullptr;
	}
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