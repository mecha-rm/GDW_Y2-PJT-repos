#pragma once
#include "Enemy.h"


//Bastion class
namespace cnz {
	class Bastion : public cnz::Enemy
	{
	public:
		Bastion(const cnz::Bastion&);

		Bastion(cnz::Enemy* obj, std::string scene);

		void Attack(cherry::Vec3 startPos, cherry::Vec3 aimPos);

		void Update(float dt);
	};
}

