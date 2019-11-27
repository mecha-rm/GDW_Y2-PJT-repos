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

void cnz::Sentry::updateArrow(cherry::Vec3 dirVec, float speed, float dt)
{
	arrow->SetPosition(arrow->GetPosition() + (dirVec * (speed * dt)));
	if (arrow->GetPosition().GetX() >= (dirVec * 200.0f).GetX() && arrow->GetPosition().GetY() >= (dirVec * 200.0f).GetY()) {
		//attacking = false;
	}
}

void cnz::Sentry::Update(float dt)
{
	if (attacking == true) {
		updateArrow(arrowDirVec, 20.0f, dt);
	}
}
