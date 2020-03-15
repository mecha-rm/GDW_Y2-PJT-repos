#include "Oracle.h"

// constructor
cnz::Oracle::Oracle(std::string scene)
	: Enemy("res/objects/enemies/Enemy_Spear.obj", scene, true)
{
	description = "Oracle";
}

// constructor
cnz::Oracle::Oracle(std::string scene, cherry::Material::Sptr mat)
	: Enemy("res/objects/enemies/Enemy_Spear.obj", scene, mat)
{
	description = "Oracle";
}

// copy constructor
cnz::Oracle::Oracle(const cnz::Oracle& enemy) : Enemy(enemy)
{
	description = "Oracle";
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
