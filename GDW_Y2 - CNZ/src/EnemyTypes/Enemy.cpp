#include "Enemy.h"

#include "cherry/objects/Primitive.h"
#include "cherry/physics/PhysicsBody.h"

// constructor
cnz::Enemy::Enemy() {}

// copy constructor
cnz::Enemy::Enemy(const Enemy& emy) : Object(emy)
{
	pBodySize = emy.GetPBodySize();
	drawPBody = emy.GetDrawPBody();
	degreeAngle = emy.GetDegreeAngle();
	radianAngle = emy.GetRadianAngle();
	worldAngle = emy.GetVec3Angle();
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
	AddPhysicsBody(new cherry::PhysicsBodyBox(3.5F, 3.5F, 3.5F));
}

// creates the Enemy at the world's origin
cnz::Enemy::Enemy(std::string modelPath, std::string scene, cherry::Material::Sptr material)
	: Enemy(modelPath, scene, material, cherry::Vec3())
{}

// creates the Enemy in hte provided scene
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

void cnz::Enemy::Update(float dt) {
	Object::Update(dt);
}

void cnz::Enemy::Attack(cherry::Vec3 startPos, cherry::Vec3 aimPos)
{
}

// TODO: holdovers from object class. May not be needed?
float cnz::Enemy::GetDegreeAngle() const { return degreeAngle; }

float cnz::Enemy::GetRadianAngle() const { return radianAngle; }

glm::vec3 cnz::Enemy::GetVec3Angle() const { return this->worldAngle; }

//Update Angle given enemy position and what they should look at
void cnz::Enemy::UpdateAngle(cherry::Vec3 one, cherry::Vec3 two) {

	cherry::Vec3 firstLine = (one + cherry::Vec3(0, -5, 0)) - one;
	cherry::Vec3 secLine = two - one;

	float dotProduct = firstLine.GetX() * secLine.GetX() + firstLine.GetY() * secLine.GetY();
	float determinant = firstLine.GetX() * secLine.GetY() - firstLine.GetY() * secLine.GetX();
	SetAngle(atan2(determinant, dotProduct), false);
}

// sets the angle
void cnz::Enemy::SetAngle(float angle, bool isDegrees) {
	if (isDegrees) {
		this->degreeAngle = angle;
		this->radianAngle = (this->degreeAngle) * (M_PI / 180.0f);
	}
	else if (!isDegrees) {
		this->radianAngle = angle;
		this->degreeAngle = (this->radianAngle) * (180.0f / M_PI);
	}

	// add code here to convert from screenspace to world space
}

void cnz::Enemy::SetAngle(glm::vec3 angle) { this->worldAngle = angle; }

bool cnz::Enemy::SetDrawPBody(bool draw)
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

bool cnz::Enemy::GetDrawPBody() const
{
	return this->drawPBody;
}

cherry::Vec3 cnz::Enemy::GetPBodySize() const
{
	return this->pBodySize;
}

float cnz::Enemy::GetPBodyWidth() const
{
	return this->GetPBodySize().GetX() / 2;
}

float cnz::Enemy::GetPBodyHeight() const
{
	return this->GetPBodySize().GetY() / 2;
}

float cnz::Enemy::GetPBodyDepth() const
{
	return this->GetPBodySize().GetZ() / 2;
}