#pragma once
#include "cherry/objects/Object.h"
#include "cherry/physics/PhysicsBody.h"

namespace cnz
{
	class Projectile : public cherry::Object
	{
	public:
		// copy constructor.
		Projectile(const Projectile& arrow);

		Projectile(std::string modelFile, std::string sceneName) : Projectile(modelFile, sceneName, cherry::Vec3()) {}

		// sets the Enemy file, and position
		Projectile(std::string modelFile, std::string sceneName, cherry::Vec3 pos) : Object(modelFile, sceneName, true)
		{
			position = pos;
			// TODO: replace with a more accurate hitbox
			//AddPhysicsBody(new cherry::PhysicsBodyBox(3.5F, 3.5F, 3.5F));
		}

		void SetDirVec(cherry::Vec3 startPos, cherry::Vec3 endPos);
		void SetDirVec(cherry::Vec3 dirVec);

		cherry::Vec3 GetDirectionVec() { return arrowDirVec; }

		void SetWhichGroup(int i) { whichGroup = i; }
		void SetWhichEnemy(int i) { whichEnemy = i; }
		int GetWhichGroup() const { return whichGroup; }
		int GetWhichEnemy() const { return whichEnemy; }

		bool active = false;
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

	private:
		cherry::Vec3 pBodySize = cherry::Vec3(0.1f, 0.1f, 0.1f);
		bool drawPBody = false;

		cherry::Vec3 arrowDirVec = cherry::Vec3(0, 0, 0);
		int whichGroup = -1;
		int whichEnemy = -1;

	};
}
