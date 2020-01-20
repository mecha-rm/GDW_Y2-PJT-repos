#include "Enemies.h"
#include "cherry/objects/Primitive.h"
#include "cherry/PhysicsBody.h"

cnz::Enemies::Enemies()
{
}

cnz::Enemies::Enemies(Enemies* obj, std::string scene)
	: Object()
{
	*this = *obj;
	pBodySize = obj->GetPBodySize();
	drawPBody = obj->GetDrawPBody();
	degreeAngle = obj->GetDegreeAngle();
	radianAngle = obj->GetRadianAngle();
	worldAngle = obj->GetVec3Angle();
}

// creates an Enemies with 
cnz::Enemies::Enemies(std::string modelFile) : Enemies(modelFile, cherry::Vec3()) {}

// sets the Enemies file, and position
cnz::Enemies::Enemies(std::string modelPath, float posX, float posY, float posZ)
	: Enemies(modelPath, cherry::Vec3(posX, posY, posZ)) {}

// sets the Enemies file, and position
cnz::Enemies::Enemies(std::string modelFile, cherry::Vec3 pos) : Object(modelFile)
{
	position = pos;
	// TODO: replace with a more accurate hitbox
	AddPhysicsBody(new cherry::PhysicsBodyBox(3.5F, 3.5F, 3.5F));
}

// creates the Enemies at the world's origin
cnz::Enemies::Enemies(std::string modelPath, std::string scene, cherry::Material::Sptr material)
	: Enemies(modelPath, scene, material, cherry::Vec3())
{}

// creates the Enemies in hte provided scene
cnz::Enemies::Enemies(std::string modelPath, std::string scene, cherry::Material::Sptr material, cherry::Vec3 pos)
	: Object(modelPath, scene, material)
{
	position = pos;
}

void cnz::Enemies::Update(float dt) {
	Object::Update(dt);
}

void cnz::Enemies::Attack(cherry::Vec3 startPos, cherry::Vec3 aimPos)
{
}

// TODO: holdovers from object class. May not be needed?
float cnz::Enemies::GetDegreeAngle() { return degreeAngle; }

float cnz::Enemies::GetRadianAngle() { return radianAngle; }

glm::vec3 cnz::Enemies::GetVec3Angle() { return this->worldAngle; }

//Update Angle given enemy position and what they should look at
void cnz::Enemies::UpdateAngle(cherry::Vec3 one, cherry::Vec3 two) {

	cherry::Vec3 firstLine = (one + cherry::Vec3(0, 5, 0)) - one;
	cherry::Vec3 secLine = two - one;

	//if (one.GetX() < two.GetX()) {
		//Clockwise
		float dot = one.GetX() * two.GetX() + one.GetY() * two.GetY();  // dot product between[x1, y1] and [x2, y2]
		float det = one.GetX() * two.GetY() - one.GetY() * two.GetX();  // determinant
		SetAngle(atan2(det, dot), true);  // atan2(y, x) or atan2(sin, cos)
	//}
	//else {
		//Counter-clockwise
	//}
}

// sets the angle
void cnz::Enemies::SetAngle(float angle, bool isDegrees) {
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

void cnz::Enemies::SetAngle(glm::vec3 angle) { this->worldAngle = angle; }

bool cnz::Enemies::SetDrawPBody(bool draw)
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

bool cnz::Enemies::GetDrawPBody()
{
	return this->drawPBody;
}

cherry::Vec3 cnz::Enemies::GetPBodySize()
{
	return this->pBodySize;
}

float cnz::Enemies::GetPBodyWidth()
{
	return this->GetPBodySize().GetX() / 2;
}

float cnz::Enemies::GetPBodyHeight()
{
	return this->GetPBodySize().GetY() / 2;
}

float cnz::Enemies::GetPBodyDepth()
{
	return this->GetPBodySize().GetZ() / 2;
}