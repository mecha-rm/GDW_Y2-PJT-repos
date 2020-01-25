#include "Mechaspider.h"

// copy constructor
cnz::Mechaspider::Mechaspider(const cnz::Mechaspider& enemy) : Enemy(enemy)
{
}

cnz::Mechaspider::Mechaspider(cnz::Enemy* obj, std::string scene) : Enemy(obj, scene)
{
	description = "Mechaspider";
}

void cnz::Mechaspider::Attack(cherry::Vec3 startPos, cherry::Vec3 aimPos)
{
}

void cnz::Mechaspider::Update(float dt)
{
	Enemy::Update(dt);
}
