// used to create lights
#pragma once

#include "VectorCRY.h" // includes cherry::Vectors, and glm::vectors
#include "Material.h"

namespace cherry
{
	class Light
	{
	public:

		// constructor
		Light(const std::string a_Scene, cherry::Vec3 a_LightColor, cherry::Vec3 a_LightPos, cherry::Vec3 a_AmbientColor, float a_AmbientPower, float a_LightSpecPower, float a_LightShininess, float a_LightAttenuation);

		// constructor
		Light(const std::string a_Scene, glm::vec3 a_LightColor, glm::vec3 a_LightPos, glm::vec3 a_AmbientColor, float a_AmbientPower, float a_LightSpecPower, float a_LightShininess, float a_LightAttenuation);

		
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
		cherry::Vec3 GetAmbientColor() const;

		// gets the ambient color of the light in decimal form [0, 1]
		glm::vec3 GetAmbientColorGLM() const;

		// sets the ambient colour (in RGB decimal, [0, 1])
		void SetAmbientColor(float x, float y, float z);

		// sets the ambient colour (in RGB decimal, [0, 1])
		void SetAmbientColor(cherry::Vec3 ambientClr);

		// sets the ambient colour (in RGB decimal, [0, 1])
		void SetAmbientColor(glm::vec3 ambientClr);

		// gets the light specular power
		float GetLightSpecularPower() const;

		// sets the light specular power
		void SetLightSpecularPower(float specPower);

		// gets the light shininess (ranges from [0 to 256])
		float GetLightShiniess() const;

		// sets the light shininess
		void SetLightShininess(float shininess);

		// gets the light attenuation
		float GetLightAttenuation() const;

		// sets hte light attenuation
		void SetLightAttenuation(float attenuation);

		// generates a material with the current values provided. No texture is applied.
		cherry::Material::Sptr GenerateMaterial() const;
	
		// generates a material with the current light values, as well as a texture.
		cherry::Material::Sptr GenerateMaterial(std::string texturePath) const;

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

