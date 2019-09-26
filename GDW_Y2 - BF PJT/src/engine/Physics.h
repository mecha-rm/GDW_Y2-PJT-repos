#pragma once
#include <vector>
#include <string>

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

class Vec2 {
public:
	float getX();
	float getY();
	void setX(float newX);
	void setY(float newY);

	Vec2 operator+ (Vec2& vec);
	Vec2 operator- (Vec2& vec);

private:
	float x;
	float y;
};

class Object {
public:
	Object(const char* path);
	~Object();

	void Start();
	void Update();

	vector<Vec3> vertices;
	vector<Vec2> uvs;
	vector<Vec3> normals;
	vector<unsigned int> vertexIndices, uvIndices, normalIndices;
private:
	vector<Vec3> temp_vertices;
	vector<Vec2> temp_uvs;
	vector<Vec3> temp_normals;
};