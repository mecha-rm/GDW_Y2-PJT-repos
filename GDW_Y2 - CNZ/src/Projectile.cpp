#include "Projectile.h"

cnz::Projectile::Projectile(std::string modelFile, std::string sceneName) : Projectile(modelFile, sceneName, cherry::Vec3())
{
}

cnz::Projectile::Projectile(std::string modelFile, std::string sceneName, cherry::Vec3 pos)
	: Object(modelFile, sceneName, true)
{
	position = pos;
	// TODO: replace with a more accurate hitbox
	//AddPhysicsBody(new cherry::PhysicsBodyBox(3.5F, 3.5F, 3.5F));
}

// copies projectile and changes scene.
cnz::Projectile::Projectile(const Projectile* proj, std::string scene)
	: Projectile(*proj)
{
	SetScene(scene);
}

// copy constructor
cnz::Projectile::Projectile(const Projectile & proj) : Object(proj)
{
	// *this = arrow;
	pBodySize = proj.GetPBodySize();
	drawPBody = proj.GetDrawPBody();
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

// gets which group
int cnz::Projectile::GetWhichGroup() const { return whichGroup; }

// sets which group the projectile is part of.
void cnz::Projectile::SetWhichGroup(int i) { whichGroup = i; }

// sets which enemy
void cnz::Projectile::SetWhichEnemy(int i) { whichEnemy = i; }

// gets which enemy the projectile is for.
int cnz::Projectile::GetWhichEnemy() const { return whichEnemy; }

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

// sets the pbody size.
void cnz::Projectile::SetPBodySize(cherry::Vec3 size)
{
	pBodySize = size;
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