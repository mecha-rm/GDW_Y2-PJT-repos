#include "Marauder.h"

// constructor
cnz::Marauder::Marauder(std::string scene) 
	: Enemy("res/objects/enemies/Enemy_Sword.obj", scene, true)
{
}

cnz::Marauder::Marauder(std::string scene, cherry::Material::Sptr mat)
	: Enemy("res/objects/enemies/Enemy_Sword.obj", scene, mat)
{
}


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
