#include "Sentry.h"

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
	Enemies::Update(dt);
}
