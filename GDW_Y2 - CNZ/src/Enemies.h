#include "cherry/objects/Object.h"
#pragma once

// class for the Enemies
namespace cnz
{
	// Enemies class
	class Enemies : public cherry::Object
	{
	public:
		// creates an Enemies object. A cube is used to represent the Enemies
		// Enemies();

		// creates an Enemies, and the file path of the user.
		Enemies(std::string modelFile);

		// sets the model for the Enemies, and the position.
		Enemies(std::string modelPath, float posX, float posY, float posZ);

		// creates an Enemies, and the file path of the user, which also saves the position of hte user.
		Enemies(std::string modelPath, cherry::Vec3 pos);

		// creates the Enemies, taking in the scene and material as well.
		Enemies(std::string modelPath, std::string scene, cherry::Material::Sptr material);

		// creates the Enemies, taking in the scene, material, and position as well.
		Enemies(std::string modelPath, std::string scene, cherry::Material::Sptr material, cherry::Vec3 pos);

		// loads in the Enemies using a default model. The Enemies takes this model's information.
		// Enemies(const cherry::Primitive * model);

		virtual void Update(float dt);

		// Holdovers from the Object class.
		// gets object angle in screen space in degrees
		float GetDegreeAngle();

		// gets object angle in screen space in radians
		float GetRadianAngle();

		// gets object angle in world space in vector 3
		glm::vec3 GetVec3Angle();

		// Update function to get new angle based on mouse position
		void UpdateAngle(cherry::Camera::Sptr camera, double xpos, double ypos, unsigned int width, unsigned int height);

		// sets object angle in degrees or radians. bool is true if degrees, false, if radians
		void SetAngle(float angle, bool isDegrees);

		// sets object angle in vec3
		void SetAngle(glm::vec3 angle);

		// set draw pbody
		bool setDrawPBody(bool draw);

		// get draw pbody
		bool getDrawPBody();

		// get pbody size
		cherry::Vec3 getPBodySize();

		// get pbody width, height and depth.
		float getPBodyWidth();
		float getPBodyHeight();
		float getPBodyDepth();

	private:
		cherry::Vec3 pBodySize = cherry::Vec3(2, 2, 4);
		bool drawPBody = false;

		// TODO: possibly delete these variables? Object has its own rotation now.
		// object angle in screen space (degrees or radians)
		float degreeAngle, radianAngle;

		// object angle in world space (vec3, so 3d angle)
		glm::vec3 worldAngle;

	protected:

	};
}