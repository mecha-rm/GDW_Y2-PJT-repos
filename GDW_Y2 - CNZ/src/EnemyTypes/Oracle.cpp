#include "Oracle.h"

// copy constructor
cnz::Oracle::Oracle(const cnz::Oracle& enemy) : Enemy(enemy)
{
}

cnz::Oracle::Oracle(cnz::Enemy* obj, std::string scene) : cnz::Enemy(obj, scene) 
{ 
	description = "Oracle"; 
}

void cnz::Oracle::Attack(cherry::Vec3 startPos, cherry::Vec3 aimPos)
{
}

void cnz::Oracle::Update(float dt)
{
	Enemy::Update(dt);
}
