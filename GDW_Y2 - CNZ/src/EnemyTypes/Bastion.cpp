#include "Bastion.h"

// TODO: replace with proper file path.
// constructor
cnz::Bastion::Bastion(std::string scene)
	: Enemy("res/objects/weapons/shield.obj", scene, true)
{
	description = "Bastion";
}

// scene and material
cnz::Bastion::Bastion(std::string scene, cherry::Material::Sptr mat)
	: Enemy("res/objects/weapons/shield.obj", scene, mat)
{
	description = "Bastion";
}

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
