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
		Player(std::string modelPath, std::string scene);

		// creates the player, taking in the scene, material, and position as well.
		Player(std::string modelPath, std::string scene, cherry::Vec3 pos);

		// creates a player object using an Object *. Used for loader, don't use manually
		Player(cherry::Object obj);

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
		float GetDegreeAngle() const;

		// gets object angle in screen space in radians
		float GetRadianAngle() const;

		// gets object angle in world space in vector 3
		glm::vec3 GetVec3Angle() const;

		// Update function to get new angle based on mouse position
		void UpdateAngle(cherry::Camera::Sptr camera, double xpos, double ypos, unsigned int width, unsigned int height);

		// Update
		void Update(float deltaTime);

		// sets object angle in degrees or radians. bool is true if degrees, false, if radians
		void SetAngle(float angle, bool isDegrees);

		// sets object angle in vec3
		void SetAngle(glm::vec3 angle);

		// get dash vector3 from angle and distance to dash
		glm::vec3 GetDash(float dist) const;

		// get dash dist
		float GetDashDist() const { return this->dashDist; };

		// set draw pbody
		bool SetDrawPBody(bool draw);

		// get draw pbody
		bool GetDrawPBody() const;

		// get pbody size
		cherry::Vec3 GetPBodySize() const;

		// get pbody width, height and depth.
		float GetPBodyWidth() const;

		float GetPBodyHeight() const;

		float GetPBodyDepth() const;

		// setter and getter for state
		void SetState(int newState) { this->state = newState; };

		int GetState() { return this->state; };

	private:
		bool dash = false;
		float dashDist = 0.1f;
		float dashTime = 0.0F;

		bool cT, cB, cL, cR;

		cherry::Vec3 pBodySize = cherry::Vec3(2, 2, 3);
		bool drawPBody = false;

		// TODO: possibly delete these variables? Object has its own rotation now.
		// object angle in screen space (degrees or radians)
		float degreeAngle, radianAngle;

		// object angle in world space (vec3, so 3d angle)
		glm::vec3 worldAngle;

		std::vector<cherry::PhysicsBody*> bodies;

		/*
		state of the player.
		0 -> idle/no anim
		1 -> walking
		2 -> charging dash
		3 -> dashing
		TODO: include other anims such as dashing, dodging and special attack
		*/
		int state = 0;

	protected:

	};
}

