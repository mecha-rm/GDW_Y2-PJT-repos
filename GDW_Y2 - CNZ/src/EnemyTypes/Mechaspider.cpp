#include "Mechaspider.h"

// constructor
cnz::Mechaspider::Mechaspider(std::string scene)
	:Enemy("res/objects/enemies/Spider.obj", scene, true)
{
}

// constructor
cnz::Mechaspider::Mechaspider(std::string scene, cherry::Material::Sptr mat)
	: Enemy("res/objects/enemies/Spider.obj", scene, mat)
{
}

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
