#include "Sentry.h"

// enemy
cnz::Sentry::Sentry(std::string scene)
	: Enemy("res/objects/enemies/Enemy_Bow.obj", scene, false)
{
}

// constructor
cnz::Sentry::Sentry(std::string scene, cherry::Material::Sptr mat)
	: Enemy("res/objects/enemies/Enemy_Bow.obj", scene, mat)
{
}

// copy constructor
cnz::Sentry::Sentry(const cnz::Sentry& sentry)
	: Enemy(sentry)
{
}

// copies the enemy and replaces its scene.
cnz::Sentry::Sentry(const cnz::Enemy* obj, std::string scene) : cnz::Enemy(obj, scene)
{
	description = "Sentry";
}

void cnz::Sentry::Attack(cherry::Vec3 startPos, cherry::Vec3 aimPos)
{
	arrowDirVec = aimPos - startPos;

	float temp = arrowDirVec.GetX() * arrowDirVec.GetX() + arrowDirVec.GetZ() * arrowDirVec.GetZ() + arrowDirVec.GetZ() * arrowDirVec.GetZ();
	arrowDirVec.SetX(arrowDirVec.GetX() / temp);
	arrowDirVec.SetY(arrowDirVec.GetY() / temp);
	arrowDirVec.SetZ(arrowDirVec.GetZ() / temp);

	arrow->SetPosition(startPos);
	arrow->SetRotation(cherry::Vec3(0, 0, 0), true);
	attacking = true;
}

void cnz::Sentry::Update(float dt)
{
	Enemy::Update(dt);
}
