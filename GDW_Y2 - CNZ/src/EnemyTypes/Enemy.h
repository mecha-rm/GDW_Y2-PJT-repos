#pragma once

#include "..\cherry\objects/Object.h"
#include "..\AnimationLoader.h"


// class for the Enemy
namespace cnz
{
	// Enemy class
	class Enemy : public cherry::Object
	{
	public:
		//Default Contructor
		Enemy();

		// Copy Constructor
		Enemy(const Enemy&);

		// copies an enemy pointer and places the in the provided scene.
		Enemy(const Enemy* obj, std::string scene);

		// creates an Enemy, and the file path of the user.
		Enemy(std::string modelFile);

		// sets the model for the Enemy, and the position.
		Enemy(std::string modelPath, float posX, float posY, float posZ);

		// creates an Enemy, and the file path of the user, which also saves the position of hte user.
		Enemy(std::string modelPath, cherry::Vec3 pos);

		// creates the Enemy, taking in the scene and material as well.
		Enemy(std::string modelPath, std::string scene, cherry::Material::Sptr material);

		// creates the Enemy, taking in the scene, material, and position as well.
		Enemy(std::string modelPath, std::string scene, cherry::Material::Sptr material, cherry::Vec3 pos);

		// loads a material if 'true' is passed.
		Enemy(std::string modelPath, std::string scene, bool loadMtl);

		// can load an mtl file and its position.
		Enemy(std::string modelPath, std::string scene, bool loadMtl, cherry::Vec3 pos);

		// loads in the Enemy using a default model. The Enemy takes this model's information.
		// Enemy(const cherry::Primitive * model);

		// the enemy's update
		virtual void Update(float dt);

		// the enemy's attack
		virtual void Attack(cherry::Vec3 startPos, cherry::Vec3 aimPos);

		// the type of enemy
		virtual std::string WhoAmI() { return description; }

		// Holdovers from the Object class.
		// gets object angle in screen space in degrees
		float GetDegreeAngle() const;

		// gets object angle in screen space in radians
		float GetRadianAngle() const;

		// gets object angle in world space in vector 3
		glm::vec3 GetVec3Angle() const;

		//Update Angle given enemy position and what they should look at
		void UpdateAngle(cherry::Vec3 one, cherry::Vec3 two);

		// sets object angle in degrees or radians. bool is true if degrees, false, if radians
		void SetAngle(float angle, const bool isDegrees);

		// sets object angle in vec3
		void SetAngle(glm::vec3 angle);

		// set draw pbody
		bool SetDrawPBody(bool draw);

		// get draw pbody
		bool GetDrawPBody() const;

		// get pbody size
		cherry::Vec3 GetPBodySize() const;

		// gets the width of the pbody
		float GetPBodyWidth() const;

		// gets the height of the pbody
		float GetPBodyHeight() const;

		// gets the depth of the pbody
		float GetPBodyDepth() const;

		// getter for state
		int GetState() const;

		// setting the state
		void SetState(int newState);

		// gets the amount of points for killing the enemy.
		int GetPoints() const;

		// the enemy's description.
		std::string description = "Enemy";

		// variable for attacking
		bool attacking = false;

		// no longer used.
		cherry::Object* arrow = nullptr;

		// bool for alive
		bool alive = false;

		// bool for stunned
		bool stunned = false;

		// stun timer
		float stunTimer = 0.0F;
		int state = 0;

	private:

										//		   z?		something is really funky here...
		cherry::Vec3 pBodySize = cherry::Vec3(1, 2, 1);
		bool drawPBody = false;

		// TODO: possibly delete these variables? Object has its own rotation now.
		// object angle in screen space (degrees or radians)
		float degreeAngle = 0, radianAngle = 0;

		// object angle in world space (vec3, so 3d angle)
		glm::vec3 worldAngle;

	protected:
		// loads in all animations. This is a pure virtual function since all enemies need animations.
		virtual void LoadAnimations() = 0;

		// becomes 'true' if the animations have been loaded.
		// TODO: add this
		// bool animsLoaded = false;

		// the points recieved for killing the enemy
		int points = 1;
	};
}