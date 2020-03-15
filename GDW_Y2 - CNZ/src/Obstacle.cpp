#include "Obstacle.h"

cnz::Obstacle::Obstacle(std::string filePath, std::string scene, bool loadMtl) : Object(filePath, scene, loadMtl) {
}

cnz::Obstacle::Obstacle(std::string filePath, std::string scene, cherry::Vec3 pbodySize, bool loadMtl) : Object(filePath, scene, loadMtl) {
	this->pBodySize = pbodySize;
}

cnz::Obstacle::Obstacle(Obstacle* obj, std::string sceneName)
	: Obstacle(*obj)
{
	SetScene(sceneName);
}

// copy constructor
cnz::Obstacle::Obstacle(const Obstacle& obj)
	: Object(obj)
{
	pBodySize = obj.pBodySize;
	drawPBody = obj.drawPBody;

	degreeAngle = obj.degreeAngle;
	radianAngle = obj.radianAngle;

	worldAngle = obj.worldAngle;
}

// update
void cnz::Obstacle::Update(float deltaTime) 
{
	cherry::Object::Update(deltaTime);
}

void cnz::Obstacle::SetAngle(float angle, bool isDegrees) {
	if (isDegrees) {
		this->degreeAngle = angle;
		this->radianAngle = (this->degreeAngle) * (M_PI / 180.0f);
	}
	else if (!isDegrees) {
		this->radianAngle = angle;
		this->degreeAngle = (this->radianAngle) * (180.0f / M_PI);
	}
}

void cnz::Obstacle::SetAngle(glm::vec3 angle) {
	this->worldAngle = angle;
}

bool cnz::Obstacle::SetDrawPBody(bool draw) {
	if (this->GetPhysicsBodyCount() == 0) {
		this->drawPBody = false;
		return false;
	}
	else {
		this->drawPBody = true;
		return true;
	}
}

bool cnz::Obstacle::GetDrawPBody() const {
	return this->drawPBody;
}

cherry::Vec3 cnz::Obstacle::GetPBodySize() const {
	return this->pBodySize;
}

void cnz::Obstacle::SetPBodySize(cherry::Vec3 newSize) {
	this->pBodySize = newSize;
}

float cnz::Obstacle::GetPBodyWidth() const {
	return this->GetPBodySize().GetX() / 2;
}

float cnz::Obstacle::GetPBodyHeight() const {
	return this->GetPBodySize().GetY() / 2;
}

float cnz::Obstacle::GetPBodyDepth() const {
	return this->GetPBodySize().GetZ() / 2;
}
