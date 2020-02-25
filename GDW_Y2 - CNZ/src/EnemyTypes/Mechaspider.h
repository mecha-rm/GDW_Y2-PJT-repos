#pragma once
#include "Enemy.h"

//Mechaspider class
namespace cnz {
	class Mechaspider : public cnz::Enemy
	{
	public:
		// constructor
		Mechaspider(std::string scene);

		// constructor
		Mechaspider(std::string scene, cherry::Material::Sptr mat);

		// copy constructor
		Mechaspider(const cnz::Mechaspider&);

		Mechaspider(cnz::Enemy* obj, std::string scene);

		void Attack(cherry::Vec3 startPos, cherry::Vec3 aimPos);

		void Update(float dt);
	};
}

