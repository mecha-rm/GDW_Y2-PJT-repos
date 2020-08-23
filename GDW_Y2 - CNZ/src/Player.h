#pragma once

#include "cherry/objects/Object.h"
#include "AnimationLoader.h"

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

		// calls copy constructor and sets the scene.
		Player(Player* player, std::string sceneName);

		// creates a player object using an Object *. Used for loader, don't use manually
		Player(const Player& obj);

		// loads in the player using a default model. The player takes this model's information.
		// Player(const cherry::Primitive * model);

		// generates and returns the default player character.
		static Player* GenerateDefault(std::string scene, cherry::Vec3 position = cherry::Vec3());

		// returns whether the player is currently charging their dash.
		bool IsChargingDash() const;

		// sets whether the player is charging their dash.
		// this also sets the current dash time to 0.0F if it is going from 'true' to 'false'.
		void SetChargingDash(bool dash);

		// returns 'true' if the dash is fully charged, false otherwise.
		bool IsDashFullyCharged() const;

		// gets the maximum dash charge time
		float GetMaximumDashChargeTime() const;

		// sets the maximum dash charge time
		void SetMaximumDashChargeTime(float mdt);

		// returns the current time of the 
		float GetCurrentDashChargeTime() const;

		// sets the current dash charge time. It cannot be negative.
		void SetCurrentDashChargeTime(float dct);

		// returns the charging factor for the dash.
		float GetDashChargeFactor() const;

		// sets the dash charge factor. If a factor of 0.0F or less is passed, then it is set to 1.0F.
		void SetDashChargeFactor(float dcf);

		// Holdovers from the Object class.
		// gets object angle in screen space in degrees
		float GetDegreeAngle() const;

		// gets object angle in screen space in radians
		float GetRadianAngle() const;

		// gets object angle in world space in vector 3
		glm::vec3 GetVec3Angle() const;

		// Update function to get new angle based on mouse position
		void UpdateAngle(cherry::Camera::Sptr camera, double xpos, double ypos, unsigned int width, unsigned int height);



		// sets object angle in degrees or radians. bool is true if degrees, false, if radians
		void SetAngle(float angle, bool isDegrees);

		// sets object angle in vec3
		void SetAngle(glm::vec3 angle);

		// get dash vector3 from angle and distance to dash
		glm::vec3 GetDash(float dist) const;

		// get dash dist
		float GetDashDist() const { return dashDist; };

		// gets the primary physics body of the player.
		// if no primary body is set, then the first body in the list is returned. If there are no bodies, it returns nullptr.
		cherry::PhysicsBody* GetPrimaryPhysicsBody() const;


		// get the animation. Nullptr if not set.
		cherry::Animation* GetChargingAnimation() const;
		
		// get the index of the animation. (-1) if not in list.
		int GetChargingAnimationIndex() const;

		// get the animation. Nullptr if not set.
		cherry::Animation* GetChargedAnimation() const;

		// get the index of the animation. (-1) if not in list.
		int GetChargedAnimationIndex() const;

		// get the animation. Nullptr if not set.
		cherry::Animation* GetDashingAnimation() const;

		// get the index of the animation. (-1) if not in list.
		int GetDashingAnimationIndex() const;

		// get the animation. Nullptr if not set.
		cherry::Animation* GetRunFAnimation() const;

		// get the index of the animation. (-1) if not in list.
		int GetRunFAnimationIndex() const;

		// get the animation. Nullptr if not set.
		cherry::Animation* GetRunBAnimation() const;

		// get the index of the animation. (-1) if not in list.
		int GetRunBAnimationIndex() const;

		// get the animation. Nullptr if not set.
		cherry::Animation* GetRunLAnimation() const;

		// get the index of the animation. (-1) if not in list.
		int GetRunLAnimationIndex() const;

		// get the animation. Nullptr if not set.
		cherry::Animation* GetRunRAnimation() const;

		// get the index of the animation. (-1) if not in list.
		int GetRunRAnimationIndex() const;
		
		// get the animation. Nullptr if not set.
		cherry::Animation* GetDodgeFAnimation() const;

		// get the index of the animation. (-1) if not in list.
		int GetDodgeFAnimationIndex() const;

		// get the animation. Nullptr if not set.
		cherry::Animation* GetDodgeBAnimation() const;

		// get the index of the animation. (-1) if not in list.
		int GetDodgeBAnimationIndex() const;

		// get the animation. Nullptr if not set.
		cherry::Animation* GetDodgeLAnimation() const;

		// get the index of the animation. (-1) if not in list.
		int GetDodgeLAnimationIndex() const;

		// get the animation. Nullptr if not set.
		cherry::Animation* GetDodgeRAnimation() const;

		// get the index of the animation. (-1) if not in list.
		int GetDodgeRAnimationIndex() const;

		// get the animation. Nullptr if not set.
		cherry::Animation* GetDeathAnimation() const;

		// get the index of the animation. (-1) if not in list.
		int GetDeathAnimationIndex() const;

		// get the animation. Nullptr if not set.
		cherry::Animation* GetSpecialAnimation() const;

		// get the index of the animation. (-1) if not in list.
		int GetSpecialAnimationIndex() const;

		// get the animation. Nullptr if not set.
		cherry::Animation* GetBashAnimation() const;

		// get the index of the animation. (-1) if not in list.
		int GetBashAnimationIndex() const;



		// set draw pbody
		bool SetDrawPBody(bool draw);

		// get draw pbody
		bool GetDrawPBody() const;

		// get pbody size
		cherry::Vec3 GetPBodySize() const;

		// get pbody width, height and depth.
		// gets the width of the pbody.
		float GetPBodyWidth() const;

		// gets the height of the pbody.
		float GetPBodyHeight() const;

		// gets the depth of the pbody.
		float GetPBodyDepth() const;

		// gets the state
		int GetState() const;

		/*
		* sets the player state, which determines the current animation.
		* it should be noted that this currently does NOT change the actual animation.
		* 0 -> idle/no anim
		* 1 -> walking
		* 2 -> charging dash
		* 3 -> dash charged 
		* 4 -> dashing
		*/
		void SetState(int newState);

		// update
		void Update(float deltaTime) override;

		// if set to 'true', then the dahs is being charged.
		bool chargingDash = false;

	private:
		// dash distance
		float dashDist = 0.1f;
		
		// the maximum charge time for the dash (maximum charge)
		float dashChargeFactor = 1.0F; // factor
		float dashChargeTimeMax = 1.0F; // desired time
		float dashChargeTimeCurr = 0.0F; // current time

		// if 'true', then the dash is being charged.
		bool dashCharging = false;

		// bool cT, cB, cL, cR;

		cherry::Vec3 pBodySize = cherry::Vec3(2, 2, 3);
		bool drawPBody = false;

		// TODO: possibly delete these variables? Object has its own rotation now.
		// object angle in screen space (degrees or radians)
		float degreeAngle, radianAngle;

		// object angle in world space (vec3, so 3d angle)
		glm::vec3 worldAngle;

		//current update move vector
		cherry::Vec3 playerCurMovement;

		// gets the primary physics body of the player.
		cherry::PhysicsBody* primaryBody = nullptr;
		
		// std::vector<cherry::PhysicsBody*> bodies;

		/*
		state of the player.
		0 -> idle/no anim
		1 -> walking
		2 -> charging dash
		3 -> charged 
		4 -> dashing
		TODO: include other anims such as dashing, dodging and special attack
		*/
		int state = 0;

		// anims

	protected:
		// cnz animation
		struct CNZ_Animation
		{
			cherry::Animation* animation = nullptr;
			int index = -1;
		};

		// player animations
		// most go unused.
		CNZ_Animation aniCharging;
		CNZ_Animation aniCharged;
		CNZ_Animation aniDashing;
		CNZ_Animation aniRun_F;
		CNZ_Animation aniRun_B;
		CNZ_Animation aniRun_L;
		CNZ_Animation aniRun_R;
		CNZ_Animation aniDodge_F;
		CNZ_Animation aniDodge_B;
		CNZ_Animation aniDodge_L;
		CNZ_Animation aniDodge_R;
		CNZ_Animation aniDeath;
		CNZ_Animation aniSpecial;
		CNZ_Animation aniBash;

	};
}

