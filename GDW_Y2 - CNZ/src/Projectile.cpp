#include "Projectile.h"

cnz::Projectile::Projectile(const Projectile & arrow) : Object(arrow)
{
	// *this = arrow;
	pBodySize = arrow.GetPBodySize();
	drawPBody = arrow.GetDrawPBody();
}

void cnz::Projectile::SetDirVec(cherry::Vec3 startPos, cherry::Vec3 endPos)
{
	arrowDirVec = endPos - startPos;

	float temp = arrowDirVec.GetX() * arrowDirVec.GetX() + arrowDirVec.GetZ() * arrowDirVec.GetZ() + arrowDirVec.GetZ() * arrowDirVec.GetZ();
	arrowDirVec.SetX(arrowDirVec.GetX() / temp);
	arrowDirVec.SetY(arrowDirVec.GetY() / temp);
	arrowDirVec.SetZ(arrowDirVec.GetZ() / temp);
}

void cnz::Projectile::SetDirVec(cherry::Vec3 dirVec) {
	arrowDirVec = dirVec;
}

bool cnz::Projectile::SetDrawPBody(bool draw)
{
	if (this->GetPhysicsBodyCount() == 0) {
		this->drawPBody = false;
		return false;
	}
	else {
		this->drawPBody = true;
		return true;
	}
}

bool cnz::Projectile::GetDrawPBody() const
{
	return this->drawPBody;
}

cherry::Vec3 cnz::Projectile::GetPBodySize() const
{
	return this->pBodySize;
}

float cnz::Projectile::GetPBodyWidth() const
{
	return this->GetPBodySize().GetX() / 2;
}

float cnz::Projectile::GetPBodyHeight() const
{
	return this->GetPBodySize().GetY() / 2;
}

float cnz::Projectile::GetPBodyDepth() const
{
	return this->GetPBodySize().GetZ() / 2;
}