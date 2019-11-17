#include "cherry/objects/Object.h"
#pragma once

// class for the player
namespace cnz
{
	// player class
	class Player : public cherry::Object
	{
	public:
		// creates a player object. A cube is used to represent the player
		// Player();

		// creates a player, and the file path of the user.
		Player(std::string modelFile);

		// sets the model for the player, and the position.
		Player(std::string modelPath, float posX, float posY, float posZ);

		// creates a player, and the file path of the user, which also saves the position of hte user.
		Player(std::string modelPath, cherry::Vec3 pos);

		// creates the player, taking in the scene and material as well.
		Player(std::string modelPath, std::string scene, cherry::Material::Sptr material);

		// creates the player, taking in the scene, material, and position as well.
		Player(std::string modelPath, std::string scene, cherry::Material::Sptr material, cherry::Vec3 pos);

		// loads in the player using a default model. The player takes this model's information.
		// Player(const cherry::Primitive * model);

		// returns whether the player is currently dashing. The function getDash() does the same thing.
		bool IsDashing() const;

		// sets whether the player is dashing.
		void SetDash(bool dash);

		// returns the dash time.
		float GetDashTime() const;

		// sets dash time
		void SetDashTime(float dashTime);


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

		// get dash vector3 from angle and distance to dash
		glm::vec3 GetDash(float dist);

	private:
		bool dash = false;

		float dashTime = 0.0F;

		// TODO: possibly delete these variables? Object has its own rotation now.
		// object angle in screen space (degrees or radians)
		float degreeAngle, radianAngle;

		// object angle in world space (vec3, so 3d angle)
		glm::vec3 worldAngle;

	protected:

	};
}

