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
		int GetWhichGroup() const;

		// sets the group
		void SetWhichGroup(int i);

		// gets which enemy the projectile is for.
		int GetWhichEnemy() const;

		// sets the enemy
		void SetWhichEnemy(int i);

		
		// set draw pbody
		bool SetDrawPBody(bool draw);

		// get draw pbody
		bool GetDrawPBody() const;

		// sets the pbody size
		void SetPBodySize(cherry::Vec3 size);

		// get pbody size
		cherry::Vec3 GetPBodySize() const;

		// get pbody width, height and depth.
		float GetPBodyWidth() const;
		float GetPBodyHeight() const;
		float GetPBodyDepth() const;

		bool active = false;

	private:
		cherry::Vec3 pBodySize = cherry::Vec3(0.1f, 0.1f, 0.1f);
		bool drawPBody = false;

		cherry::Vec3 arrowDirVec = cherry::Vec3(0, 0, 0);
		int whichGroup = -1;
		int whichEnemy = -1;

	};
}
