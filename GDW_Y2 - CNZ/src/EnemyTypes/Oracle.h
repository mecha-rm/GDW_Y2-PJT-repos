#pragma once
#include "Enemy.h"

//Oracle class
namespace cnz {
	class Oracle : public cnz::Enemy
	{
	public:
		// copy constuctor
		Oracle(const cnz::Oracle&);

		Oracle(cnz::Enemy* obj, std::string scene);

		void Attack(cherry::Vec3 startPos, cherry::Vec3 aimPos);

		void Update(float dt);
	};
}

