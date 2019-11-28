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

bool Obstacle::setDrawPBody(bool draw) {
	if (this->GetPhysicsBodyCount() == 0) {
		this->drawPBody = false;
		return false;
	}
	else {
		this->drawPBody = true;
		return true;
	}
}

bool Obstacle::getDrawPBody() {
	return this->drawPBody;
}

cherry::Vec3 Obstacle::getPBodySize() {
	return this->pBodySize;
}

float Obstacle::getPBodyWidth() {
	return this->getPBodySize().GetX() / 2;
}

float Obstacle::getPBodyHeight() {
	return this->getPBodySize().GetY() / 2;
}

float Obstacle::getPBodyDepth() {
	return this->getPBodySize().GetZ() / 2;
}
