#include "../Enemies.h"
#pragma once

//Marauder class
namespace cnz {
	class Marauder : public cnz::Enemies
	{
	public:
		Marauder(cnz::Enemies* obj, std::string scene) : cnz::Enemies(obj, scene) { description = "Marauder"; };

		void Attack(cherry::Vec3 startPos, cherry::Vec3 aimPos);

		void Update(float dt);
	};
}

