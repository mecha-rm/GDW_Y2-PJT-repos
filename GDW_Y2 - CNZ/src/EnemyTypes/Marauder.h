#pragma once
#include "Enemy.h"

//Marauder class
namespace cnz {
	class Marauder : public cnz::Enemy
	{
	public:
		// constructor
		Marauder(std::string scene);

		// constructor
		Marauder(std::string scene, cherry::Material::Sptr mat);

		// copy constructor.
		Marauder(const cnz::Marauder&);

		Marauder(cnz::Enemy* obj, std::string scene);

		void Attack(cherry::Vec3 startPos, cherry::Vec3 aimPos);

		void Update(float dt);
	};
}

