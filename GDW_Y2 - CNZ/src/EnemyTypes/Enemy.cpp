#include "Enemy.h"

#include "cherry/objects/Primitive.h"
#include "cherry/physics/PhysicsBody.h"

// constructor
cnz::Enemy::Enemy() {}

// copy constructor
cnz::Enemy::Enemy(const Enemy& emy) : Object(emy)
{
	type = emy.type; // enemy type
	description = emy.description; // enemy description (name)
	attacking = emy.attacking;

	// since the physics bodies are remade, the enemy gets its own physics body as the primary.
	if (GetPhysicsBodyCount() > 0)
		primaryBody = GetPhysicsBody(0);
	
	alive = emy.alive;
	stunned = emy.alive;
	stunTimer = emy.stunTimer;

	pBodySize = emy.GetPBodySize();
	drawPBody = emy.GetDrawPBody();
	degreeAngle = emy.GetDegreeAngle();
	radianAngle = emy.GetRadianAngle();
	worldAngle = emy.GetVec3Angle();

	// projectiles - does not copy the current amount charged.
	useProjs = emy.useProjs;
	projChargeTimeMax = emy.projChargeTimeMax;
	projTimerFactor = emy.projTimerFactor;

	points = emy.points;
	speedMult = emy.speedMult;

	// tracking animations
	{
		int index = 0;

		// the entity has a walk animation
		index = emy.GetWalkAnimationIndex();
		if (index != -1)
		{
			aniWalk.index = index;
			aniWalk.animation = GetAnimation(index);
		}


		// the entity has an attack animation
		index = emy.GetAttackAnimationIndex();
		if (index != -1)
		{
			aniAttack.index = index;
			aniAttack.animation = GetAnimation(index);
		}
	}
}

// copies the enemy and provides it with the values.
cnz::Enemy::Enemy(const Enemy* obj, std::string scene)
	: Enemy(*obj)
{
	// *this = *obj; // todo: not doing this screws up the mesh. Fix it.
	SetScene(scene); // changes the scene to the provided scene.
}

// creates an Enemy with 
cnz::Enemy::Enemy(std::string modelFile) : Enemy(modelFile, cherry::Vec3()) {}

// sets the Enemy file, and position
cnz::Enemy::Enemy(std::string modelPath, float posX, float posY, float posZ)
	: Enemy(modelPath, cherry::Vec3(posX, posY, posZ)) {}

// sets the Enemy file, and position
cnz::Enemy::Enemy(std::string modelFile, cherry::Vec3 pos) : Object(modelFile, false, true)
{
	position = pos;

	// TODO: replace with a more accurate hitbox
	cherry::PhysicsBody* body = new cherry::PhysicsBodyBox(3.5F, 3.5F, 3.5F);
	AddPhysicsBody(body);
	primaryBody = body;
}

// creates the Enemy at the world's origin
cnz::Enemy::Enemy(std::string modelPath, std::string scene, cherry::Material::Sptr material)
	: Enemy(modelPath, scene, material, cherry::Vec3())
{}

// creates the Enemy in the provided scene
cnz::Enemy::Enemy(std::string modelPath, std::string scene, cherry::Material::Sptr material, cherry::Vec3 pos)
	: Object(modelPath, scene, material, false, true)
{
	position = pos;
}

// creates an enemy and loads an mtl file.
cnz::Enemy::Enemy(std::string modelPath, std::string scene, bool loadMtl)
	: Object(modelPath, scene, loadMtl, true)
{
}

// creates the enemy
cnz::Enemy::Enemy(std::string modelPath, std::string scene, bool loadMtl, cherry::Vec3 pos)
	: Enemy(modelPath, scene, loadMtl)
{
	position = pos;
}

// attack function
void cnz::Enemy::Attack(cherry::Vec3 startPos, cherry::Vec3 aimPos)
{
	// Nothing here since this behaviour is set by the enemies.
}

// returns the enemy type.
cnz::enemy_t cnz::Enemy::GetType() const 
{ 
	return type; 
}

// returns the enemy type.
void cnz::Enemy::SetType(cnz::enemy_t et) 
{ 
	type = et; 
}

// TODO: holdovers from object class. May not be needed?
float cnz::Enemy::GetDegreeAngle() const 
{ 
	return degreeAngle; 
}

float cnz::Enemy::GetRadianAngle() const 
{ 
	return radianAngle; 
}

glm::vec3 cnz::Enemy::GetVec3Angle() const 
{ 
	return worldAngle; 
}

//Update Angle given enemy position and what they should look at
void cnz::Enemy::UpdateAngle(cherry::Vec3 one, cherry::Vec3 two) {

	cherry::Vec3 firstLine = (one + cherry::Vec3(0, -5, 0)) - one;
	cherry::Vec3 secLine = two - one;

	float dotProduct = firstLine.GetX() * secLine.GetX() + firstLine.GetY() * secLine.GetY();
	float determinant = firstLine.GetX() * secLine.GetY() - firstLine.GetY() * secLine.GetX();
	SetAngle(atan2(determinant, dotProduct), false);
}

// sets the angle
void cnz::Enemy::SetAngle(float angle, const bool isDegrees) {
	if (isDegrees) {
		degreeAngle = angle;
		radianAngle = (degreeAngle) * (M_PI / 180.0f);
	}
	// else if (!isDegrees) {
	else
	{
		radianAngle = angle;
		degreeAngle = (radianAngle) * (180.0f / M_PI);
	}

	// add code here to convert from screenspace to world space
}

// sets the angle
void cnz::Enemy::SetAngle(glm::vec3 angle) { this->worldAngle = angle; }

// gets the walk animation
cherry::Animation* cnz::Enemy::GetWalkAnimation() const
{
	return aniWalk.animation;
}

// gets the walk animation
int cnz::Enemy::GetWalkAnimationIndex() const
{
	return aniWalk.index;
}

// gets the attack animation
cherry::Animation* cnz::Enemy::GetAttackAnimation() const
{
	return aniAttack.animation;
}

// gets the attack animation index
int cnz::Enemy::GetAttackAnimationIndex() const
{
	return aniAttack.index;
}

// gets the primary physics body of the enemy.
cherry::PhysicsBody* cnz::Enemy::GetPrimaryPhysicsBody() const
{
	// gets the primary body if it is not set to nullptr.
	if (primaryBody != nullptr)
	{
		return primaryBody;
	}
	else // returns the first physics body in the list.
	{
		// will return nullptr if there are no bodies
		return GetPhysicsBody(0);
	}
}

// sets the draw pbody
bool cnz::Enemy::SetDrawPBody(bool draw)
{
	if (GetPhysicsBodyCount() == 0) {
		drawPBody = false;
		return false;
	}
	else {
		drawPBody = true;
		return true;
	}
}

// gets the draw pbody
bool cnz::Enemy::GetDrawPBody() const
{
	return drawPBody;
}

// gets the pbody size
cherry::Vec3 cnz::Enemy::GetPBodySize() const
{
	return pBodySize;
}

// gets the pbody width
float cnz::Enemy::GetPBodyWidth() const
{
	return GetPBodySize().GetX() / 2.0F;
}

// gets the pbody height
float cnz::Enemy::GetPBodyHeight() const
{
	return GetPBodySize().GetY() / 2.0F;
}

// gets the pbody depth
float cnz::Enemy::GetPBodyDepth() const
{
	return GetPBodySize().GetZ() / 2.0F;
}

// gets the state of the enemy
int cnz::Enemy::GetState() const 
{
	return state;
}

// sets the state of the enemy.
void cnz::Enemy::SetState(int newState) 
{
	state = newState;
}

// returns 'true' if the enemy uses projectiles.
bool cnz::Enemy::IsUsingProjectiles() const
{
	return useProjs;
}

// set using projectiles
void cnz::Enemy::SetUsingProjectiles(bool use)
{
	useProjs = use;
}

// gets if a projectile is available.
bool cnz::Enemy::GetProjectileAvailable() const
{
	// if using projectiles
	if (useProjs)
		return projChargeTimer >= projChargeTimeMax;
	else
		return false;
}

// returns the alloted charge time 
float cnz::Enemy::GetCurrentProjectileCharge() const
{
	return projChargeTimer;
}

// get maximum projectile charge time.
float cnz::Enemy::GetMaximumProjectileChargeTime() const
{
	return projChargeTimeMax;
}

// sets the maximum projectile charge time
void cnz::Enemy::SetMaximumProjectileChargeTime(float maxTime)
{
	if (maxTime > 0.0F)
		projChargeTimeMax = maxTime;
}

// return projectile charge factor
float cnz::Enemy::GetProjectileChargeFactor() const
{
	return projTimerFactor;
}

// set projectile charge factor
void cnz::Enemy::SetProjectileChargeFactor(float factor)
{
	// the factor only gets changed if it's above 0
	if (factor > 0.0F)
		projTimerFactor = factor;
}

// projectile fired.
void cnz::Enemy::ProjectileFired()
{
	projChargeTimer = 0.0F;
}



// gets the amount of points for killing the enemy.
int cnz::Enemy::GetPoints() const 
{ 
	return points; 
}

// sets the points for killing the enemy.
void cnz::Enemy::SetPoints(int pnts)
{
	points = (pnts > 0) ? pnts : points;
}

// gets the speed of multiplier
float cnz::Enemy::GetSpeedMultiplier() const
{
	return speedMult;
}

// sets the speed multipier
void cnz::Enemy::SetSpeedMultiplier(float speed)
{
	speedMult = (speed > 0.0F) ? speed : speedMult;
}

// update loop
void cnz::Enemy::Update(float dt) 
{
	Object::Update(dt);

	// if the enemy uses projectiles, charge the enemy if it can't use them yet.
	if (useProjs)
	{
		projChargeTimer += projTimerFactor * dt;

		if (projChargeTimer > projChargeTimeMax)
			projChargeTimer = projChargeTimeMax;
	}
}