#include "Physics.h"

float Vec3::getX() {
	return this->x;
}

float Vec3::getY() {
	return this->y;
}

float Vec3::getZ() {
	return this->z;
}

void Vec3::setX(float newX) {
	this->x = newX;
}

void Vec3::setY(float newY) {
	this->y = newY;
}

void Vec3::setZ(float newZ) {
	this->z = newZ;
}

Vec3 Vec3::operator+(Vec3 & vec) {
	Vec3 temp;
	temp.setX(this->getX() + vec.getX());
	temp.setY(this->getY() + vec.getY());
	temp.setZ(this->getZ() + vec.getZ());
	return temp;
}

Vec3 Vec3::operator-(Vec3 & vec) {
	Vec3 temp;
	temp.setX(this->getX() - vec.getX());
	temp.setY(this->getY() - vec.getY());
	temp.setZ(this->getZ() - vec.getZ());
	return temp;
}
