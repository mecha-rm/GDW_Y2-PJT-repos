#include "Projectile.h"

Projectile::Projectile(Projectile* arrow)
{
	this->arrow = arrow;
	*this = *arrow;
	pBodySize = arrow->GetPBodySize();
	drawPBody = arrow->GetDrawPBody();
}

void Projectile::SetDirVec(cherry::Vec3 startPos, cherry::Vec3 endPos)
{
	arrowDirVec = endPos - startPos;

	float temp = arrowDirVec.GetX() * arrowDirVec.GetX() + arrowDirVec.GetZ() * arrowDirVec.GetZ() + arrowDirVec.GetZ() * arrowDirVec.GetZ();
	arrowDirVec.SetX(arrowDirVec.GetX() / temp);
	arrowDirVec.SetY(arrowDirVec.GetY() / temp);
	arrowDirVec.SetZ(arrowDirVec.GetZ() / temp);
}

bool Projectile::SetDrawPBody(bool draw)
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

bool Projectile::GetDrawPBody()
{
	return this->drawPBody;
}

cherry::Vec3 Projectile::GetPBodySize()
{
	return this->pBodySize;
}

float Projectile::GetPBodyWidth()
{
	return this->GetPBodySize().GetX() / 2;
}

float Projectile::GetPBodyHeight()
{
	return this->GetPBodySize().GetY() / 2;
}

float Projectile::GetPBodyDepth()
{
	return this->GetPBodySize().GetZ() / 2;
}