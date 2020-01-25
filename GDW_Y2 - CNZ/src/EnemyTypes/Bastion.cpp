#include "Bastion.h"

// copy constructor
cnz::Bastion::Bastion(const cnz::Bastion& emy) : cnz::Enemy(emy)
{
	description = "Bastion";
}

// copying from a pointer
cnz::Bastion::Bastion(cnz::Enemy* obj, std::string scene) : cnz::Enemy(obj, scene)
{
	description = "Bastion";
}

void cnz::Bastion::Attack(cherry::Vec3 startPos, cherry::Vec3 aimPos)
{
}

void cnz::Bastion::Update(float dt)
{
	Enemy::Update(dt);
}
