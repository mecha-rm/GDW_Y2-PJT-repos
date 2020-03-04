#include "Obstacle.h"

Obstacle::Obstacle(std::string filePath, std::string scene, bool loadMtl) : Object(filePath, scene, loadMtl) {
}

Obstacle::Obstacle(std::string filePath, std::string scene, cherry::Vec3 pbodySize, bool loadMtl) : Object(filePath, scene, loadMtl) {
	this->pBodySize = pbodySize;
}

void Obstacle::Update(float deltaTime) {

}

void Obstacle::SetAngle(float angle, bool isDegrees) {
	if (isDegrees) {
		this->degreeAngle = angle;
		this->radianAngle = (this->degreeAngle) * (M_PI / 180.0f);
	}
	else if (!isDegrees) {
		this->radianAngle = angle;
		this->degreeAngle = (this->radianAngle) * (180.0f / M_PI);
	}
}

void Obstacle::SetAngle(glm::vec3 angle) {
	this->worldAngle = angle;
}

bool Obstacle::SetDrawPBody(bool draw) {
	if (this->GetPhysicsBodyCount() == 0) {
		this->drawPBody = false;
		return false;
	}
	else {
		this->drawPBody = true;
		return true;
	}
}

bool Obstacle::GetDrawPBody() const {
	return this->drawPBody;
}

cherry::Vec3 Obstacle::GetPBodySize() const {
	return this->pBodySize;
}

void Obstacle::SetPBodySize(cherry::Vec3 newSize) {
	this->pBodySize = newSize;
}

float Obstacle::GetPBodyWidth() const {
	return this->GetPBodySize().GetX() / 2;
}

float Obstacle::GetPBodyHeight() const {
	return this->GetPBodySize().GetY() / 2;
}

float Obstacle::GetPBodyDepth() const {
	return this->GetPBodySize().GetZ() / 2;
}
