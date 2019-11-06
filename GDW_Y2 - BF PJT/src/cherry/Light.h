// used to create lights
#pragma once

#include "VectorCRY.h" // includes cherry::Vectors, and glm::vectors

namespace cherry
{
	class Light
	{
	public:

		// constructor
		Light(const std::string scene, glm::vec3 lightPos, glm::vec3 ambientColor, float ambientPower, float lightSpecPower, float lightShininess, float lightAttenuation);

		// gets the scene the light is part of.
		std::string GetScene();

		glm::vec3 GetLightPosition();

		void SetLightPosition(float x, float y, float z);

		void SetLightPosition(glm::vec3 pos);

		// returns the light colour, which is in decimal form
		glm::vec3 GetLightColor() const;

		// sets hte light colour, which is set in decimal form
		void SetLightColor(glm::vec3 clr);

		// gets the ambient color of the light (in decimal)
		glm::vec3 GetAmbientColor() const;

		// sets the ambient colour (in RGB decimal).
		void SetAmbientColor(glm::vec3 ambientClr);

		// gets the light specular power
		float GetLightSpecularPower() const;

		// sets the light specular power
		void SetLightSpecularPower(float specPower) const;

		// gets the light shininess
		float GetLightShiniess() const;

		// sets the light shininess
		void SetLightShininess(float shininess) const;

		// gets the light attenuation
		float GetLightAttenuation() const;

		// sets hte light attenuation
		void SetLightAttenuation(float attenuation) const;

	private:

		std::string scene; // the scene the light is in.

		glm::vec3 lightPos; // light position
		glm::vec3 lightColor; // light colour

		glm::vec3 ambientColor; // darkest colour of the object (i.e. no light present)
		float ambientPower; // power of ambient light

		float lightSpecPower; // specular power
		float lightShininess; // ranges from 0 to 256.
		float lightAttenuation; // light area of effect/distance
	protected:
	};
}

