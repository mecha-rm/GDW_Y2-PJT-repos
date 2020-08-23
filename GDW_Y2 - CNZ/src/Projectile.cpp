#include "Projectile.h"

// constructor
cnz::Projectile::Projectile(std::string modelFile, std::string sceneName) : Projectile(modelFile, sceneName, cherry::Vec3())
{
}

// constructor
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

	arrowDirVec = proj.arrowDirVec;
	// whichGroup = proj.whichGroup;
	// whichEnemy = proj.whichEnemy;

	active = proj.active;
	maxLifeTime = proj.maxLifeTime;
	currLifeTime = proj.currLifeTime;
	ageFactor = proj.ageFactor;
}

// set direction vector
void cnz::Projectile::SetDirVec(cherry::Vec3 startPos, cherry::Vec3 endPos)
{
	arrowDirVec = endPos - startPos;

	float temp = arrowDirVec.GetX() * arrowDirVec.GetX() + arrowDirVec.GetZ() * arrowDirVec.GetZ() + arrowDirVec.GetZ() * arrowDirVec.GetZ();
	arrowDirVec.SetX(arrowDirVec.GetX() / temp);
	arrowDirVec.SetY(arrowDirVec.GetY() / temp);
	arrowDirVec.SetZ(arrowDirVec.GetZ() / temp);
}

// set direction vector
void cnz::Projectile::SetDirVec(cherry::Vec3 dirVec) {
	arrowDirVec = dirVec;
}

// gets which group
// int cnz::Projectile::GetWhichGroup() const { return whichGroup; }
// 
// // sets which group the projectile is part of.
// void cnz::Projectile::SetWhichGroup(int i) { whichGroup = i; }
// 
// // sets which enemy
// void cnz::Projectile::SetWhichEnemy(int i) { whichEnemy = i; }
// 
// // gets which enemy the projectile is for.
// int cnz::Projectile::GetWhichEnemy() const { return whichEnemy; }

// set draw pbody
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

// get draw pbody
bool cnz::Projectile::GetDrawPBody() const
{
	return this->drawPBody;
}

// sets the pbody size.
void cnz::Projectile::SetPBodySize(cherry::Vec3 size)
{
	pBodySize = size;
}

// get pbody size
cherry::Vec3 cnz::Projectile::GetPBodySize() const
{
	return this->pBodySize;
}

// get pbody width
float cnz::Projectile::GetPBodyWidth() const
{
	return this->GetPBodySize().GetX() / 2;
}

// get pbody height
float cnz::Projectile::GetPBodyHeight() const
{
	return this->GetPBodySize().GetY() / 2;
}

// get pbody depth
float cnz::Projectile::GetPBodyDepth() const
{
	return this->GetPBodySize().GetZ() / 2;
}

// gets maximum life time.
float cnz::Projectile::GetMaximumLifeTime() const
{
	return maxLifeTime;
}

// sets maximum life time.
void cnz::Projectile::SetMaximumLifeTime(float mt)
{
	maxLifeTime = (mt >= 0) ? mt : 0;
}

// returns the current life time.
float cnz::Projectile::GetCurrentLifeTime() const
{
	return currLifeTime;
}

// resets current life time.
void cnz::Projectile::ResetCurrentLifeTime()
{
	currLifeTime = 0;
}

// gets the age incrementer.
float cnz::Projectile::GetAgeFactor() const { return ageFactor; }

// sets the age factor.
void cnz::Projectile::SetAgeFactor(float ai)
{
	ageFactor = (ai > 0) ? ai : 1.0F;
}

// returns 'true' if maximum life time as been reached.
bool cnz::Projectile::ReachedMaximumLifeTime()
{
	return currLifeTime >= maxLifeTime;
}

// update
void cnz::Projectile::Update(float deltaTime)
{
	Object::Update(deltaTime);

	currLifeTime += deltaTime * ageFactor;
}
