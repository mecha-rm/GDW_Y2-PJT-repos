#pragma once
#include "Enemy.h"

//Marauder class
namespace cnz {
	class Marauder : public cnz::Enemy
	{
	public:
		// copy constructor.
		Marauder(const cnz::Marauder&);

		Marauder(cnz::Enemy* obj, std::string scene);

		void Attack(cherry::Vec3 startPos, cherry::Vec3 aimPos);

		void Update(float dt);
	};
}

