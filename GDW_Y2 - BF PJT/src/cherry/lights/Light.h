// Light (Source) - used to create lights
#pragma once

#include "..\VectorCRY.h" // includes cherry::Vectors, and glm::vectors
#include "..\Material.h"


namespace cherry
{
	class Light
	{
	public:

		// constructor
		Light(const std::string a_Scene, cherry::Vec3 a_LightPos, cherry::Vec3 a_LightColor, cherry::Vec3 a_AmbientColor, float a_AmbientPower, float a_LightSpecPower, float a_LightShininess, float a_LightAttenuation);

		// constructor
		Light(const std::string a_Scene, glm::vec3 a_LightPos, glm::vec3 a_LightColor, glm::vec3 a_AmbientColor, float a_AmbientPower, float a_LightSpecPower, float a_LightShininess, float a_LightAttenuation);

		
		// gets the m_Scene the light is part of.
		std::string GetScene() const;

		// gets the position of the light
		cherry::Vec3 GetLightPosition() const;

		// gets the position of the light as a GLM vector
		glm::vec3 GetLightPositionGLM() const;

		// sets the position of the light
		void SetLightPosition(float x, float y, float z);

		// sets hte position of the light
		void SetLightPosition(cherry::Vec3 pos);

		// sets hte position of the light
		void SetLightPosition(glm::vec3 pos);

		// gets the position of the light on the x-axis
		float GetLightPositionX() const;

		// sets the position of the light on the x-axis
		void SetLightPositionX(float x);

		// gets the position of the light on the y-axis
		float GetLightPositionY() const;

		// sets the position of the light on the y-axis
		void SetLightPositionY(float y);

		// gets the position of the light on the z-axis
		float GetLightPositionZ() const;

		// sets the position of the light on the z-axis
		void SetLightPositionZ(float z);



		// returns the light colour, which is in decimal form
		cherry::Vec3 GetLightColor() const;

		// returns the light colour, which is in decimal form (GLM version)
		glm::vec3 GetLightColorGLM() const;

		// sets the light colour in decimal form [0, 1]
		void SetLightColor(cherry::Vec3 clr);

		// sets the light colour, which is set in decimal form [0, 1]
		void SetLightColor(glm::vec3 clr);

		// gets the ambient color of the light in decimal form [0, 1]
		// these values are used for textureless sceneLists
		cherry::Vec3 GetAmbientColor() const;

		// gets the ambient color of the light in decimal form [0, 1]
		// these values are used for textureless sceneLists
		glm::vec3 GetAmbientColorGLM() const;

		// sets the ambient colour (in RGB decimal, [0, 1])
		// these values are used for textureless sceneLists
		void SetAmbientColor(float x, float y, float z);

		// sets the ambient colour (in RGB decimal, [0, 1])
		// these values are used for textureless sceneLists
		void SetAmbientColor(cherry::Vec3 ambientClr);

		// sets the ambient colour (in RGB decimal, [0, 1])
		// these values are used for textureless sceneLists
		void SetAmbientColor(glm::vec3 ambientClr);

		// gets the ambient power
		float GetAmbientPower() const;

		// sets the ambient power; thi cannot be negative.
		void SetAmbientPower(float ambientPower);




		// gets the light specular power
		float GetLightSpecularPower() const;

		// sets the light specular power
		void SetLightSpecularPower(float specPower);

		// gets the light shininess (ranges from [0 to 256])
		float GetLightShininess() const;

		// sets the light shininess
		void SetLightShininess(float shininess);

		// gets the light attenuation
		float GetLightAttenuation() const;

		// sets hte light attenuation
		void SetLightAttenuation(float attenuation);
		
		
		// void SetSampler(TextureSampler::Sptr sampler);
		// generates a material with the default shader values
		cherry::Material::Sptr GenerateMaterial(const TextureSampler::Sptr& sampler = nullptr) const;

		// generates a material with the current values provided. No texture is applied, and default values are used.
		cherry::Material::Sptr GenerateMaterial(std::string vs, std::string fs, const TextureSampler::Sptr& sampler = nullptr) const;
	
		// generates a material using the lighting setup, and a material template library file.
		// if a sampler isn't being used, it should just be set to nullptr
		cherry::Material::Sptr GenerateMaterial(std::string vs, std::string fs, std::string mtllib, const TextureSampler::Sptr & sampler = nullptr);

		// generates a material with the current light values, as well as a texture. Set the weight to 1.0 to make the texture fully 
		// if a sampler isn't being used, it should just be set to nullptr
		// the texture path should be an image. Weight ranges from [0, 1]
		cherry::Material::Sptr GenerateMaterial(std::string vs, std::string fs, std::string texturePath, float weight, const TextureSampler::Sptr& sampler = nullptr) const;

		// generates a material with the current light values, as well as two provided texture.
		// the weights determine how they're mixed. Weights cannot be greater than 1.
		// if a sampler isn't being used, it should just be set to nullptr
		cherry::Material::Sptr GenerateMaterial(std::string vs, std::string fs, std::string txt0, float wgt0, std::string txt1, float wgt1, const TextureSampler::Sptr& sampler = nullptr) const;

		// generates a material with the current light values, as well as two provided texture.
		// the weights determine how they're mixed. Weights must be in the [0, 1] range.
		// if a sampler isn't being used, it should just be set to nullptr
		cherry::Material::Sptr GenerateMaterial(std::string vs, std::string fs, std::string txt0, float wgt0, std::string txt1, float wgt1, std::string txt2, float wgt2, const TextureSampler::Sptr& sampler = nullptr) const;

		// toString function for a light
		std::string ToString() const;

	private:

		std::string m_Scene; // the m_Scene the light is in.

		cherry::Vec3 m_LightPos; // light position
		cherry::Vec3 m_LightColor; // light colour

		cherry::Vec3 m_AmbientColor; // darkest colour of the object (i.e. no light present)
		float m_AmbientPower; // power of ambient light

		float m_LightSpecPower; // specular power
		float m_LightShininess; // ranges from 0 to 256.
		float m_LightAttenuation; // light area of effect/distance

	protected:
	};
}

