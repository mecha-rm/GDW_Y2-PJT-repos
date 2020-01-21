#include "../Enemies.h"
#pragma once

//Sentry class
namespace cnz {
	class Sentry : public cnz::Enemies
	{
	public:
		Sentry(cnz::Enemies* obj, std::string scene) : cnz::Enemies(obj, scene) { description = "Sentry"; };

		void Attack(cherry::Vec3 startPos, cherry::Vec3 aimPos);

		void Update(float dt);

	private:
		cherry::Vec3 arrowDirVec = cherry::Vec3(0, 0, 0);
		float arrowST = 0;
	};
}

