#pragma once
#include <vector>
using namespace std;

class Vec3 {
public:
	float getX();
	float getY();
	float getZ();
	void setX(float newX);
	void setY(float newY);
	void setZ(float newZ);

	Vec3 operator+ (Vec3 & vec);
	Vec3 operator- (Vec3 & vec);

private:
	float x;
	float y;
	float z;
};

class Object {
public:
private:
	unsigned int numOfVertices;
	vector<Vec3> vertices;
};