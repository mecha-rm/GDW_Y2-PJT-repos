#include "Marauder.h"

// copy constructor
cnz::Marauder::Marauder(const cnz::Marauder& enemy) : Enemy(enemy)
{
}

cnz::Marauder::Marauder(cnz::Enemy* obj, std::string scene) : cnz::Enemy(obj, scene)
{
	description = "Marauder";
}

void cnz::Marauder::Attack(cherry::Vec3 startPos, cherry::Vec3 aimPos)
{
}

void cnz::Marauder::Update(float dt)
{
	Enemy::Update(dt);
}
