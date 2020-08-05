#pragma once
#include "cherry/objects/Object.h"
#include "cherry/physics/PhysicsBody.h"

namespace cnz
{
	class Projectile : public cherry::Object
	{
	public:
		// creates the projectile
		Projectile(std::string modelFile, std::string sceneName);

		// sets the Enemy file, and position
		Projectile(std::string modelFile, std::string sceneName, cherry::Vec3 pos);

		// copies the projectile and sets the scene.
		Projectile(const Projectile* proj, std::string scene);

		// copy constructor.
		Projectile(const Projectile& proj);

		// sets the direction vector using a starting point and ending point.
		void SetDirVec(cherry::Vec3 startPos, cherry::Vec3 endPos);

		// sets the direction vector.
		void SetDirVec(cherry::Vec3 dirVec);

		// returns the direction vector.
		cherry::Vec3 GetDirectionVec() { return arrowDirVec; }

		// gets which group the projectile is for.
		// int GetWhichGroup() const;
		// 
		// // sets the group
		// void SetWhichGroup(int i);
		// 
		// // gets which enemy the projectile is for.
		// int GetWhichEnemy() const;
		// 
		// // sets the enemy
		// void SetWhichEnemy(int i);

		
		// set draw pbody
		bool SetDrawPBody(bool draw);

		// get draw pbody
		bool GetDrawPBody() const;

		// sets the pbody size
		void SetPBodySize(cherry::Vec3 size);

		// get pbody size
		cherry::Vec3 GetPBodySize() const;

		// get the pbody width.
		float GetPBodyWidth() const;
		
		// gets the p-body height.
		float GetPBodyHeight() const;

		// gets the p-body depth.
		float GetPBodyDepth() const;

		// gets the maximum life time of the projectile.
		float GetMaximumLifeTime() const;

		// sets the maximum life time of the projectile.
		// if a negative number is provided, then the maximum life time is set to 0.
		void SetMaximumLifeTime(float mt);

		// gets the current life time of the projectile.
		float GetCurrentLifeTime() const;

		// resets the current life time, bringing it back to zero.
		void ResetCurrentLifeTime();

		// returns the factor multiplied by delta time to increase the age of the projectile.
		float GetAgeFactor() const;

		// sets the factor multiplied by delta time to increase the age of the projectile.
		// if a zero value or less is provided, then the age factor is set to 1 (i.e. no factor applied).
		void SetAgeFactor(float ai);

		// returns 'true' if the maximum life has been either reached or surpassed. 
		bool ReachedMaximumLifeTime();

		// update
		void Update(float deltaTime) override;

		bool active = false;

	private:
		cherry::Vec3 pBodySize = cherry::Vec3(0.1f, 0.1f, 0.1f);
		bool drawPBody = false;

		cherry::Vec3 arrowDirVec = cherry::Vec3(0, 0, 0);
		// int whichGroup = -1;
		// int whichEnemy = -1;

		// maximum life time.
		float maxLifeTime = 0;

		// current life time
		float currLifeTime = 0;

		// age incrementer.
		float ageFactor = 1.0F;
	};
}
