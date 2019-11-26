#include "../Enemies.h"
#pragma once

//Sentry class
namespace cnz {
	class Sentry : public cnz::Enemies
	{
	public:
		Sentry(cnz::Enemies* obj, std::string scene) : cnz::Enemies(obj, scene) {};

		void shoot(cherry::Vec3 startPos, cherry::Vec3 aimPos, float speed);
	};
}

