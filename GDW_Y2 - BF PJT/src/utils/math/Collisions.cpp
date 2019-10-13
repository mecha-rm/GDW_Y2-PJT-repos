#include "../Utils.h"
#include "Collisions.h"
#include "Rotation.h"

#include <iostream>
#include <math.h>

bool util::math::circleCollision(Vec2 circ1, float radius1, Vec2 circ2, float radius2)
{
	return sphereCollision(Vec3(circ1.x, circ1.y, 0.0F), radius1, Vec3(circ2.x, circ2.y, 0.0F), radius2);
}

// calculates sphere-sphere collision
bool util::math::sphereCollision(Vec3 sphr1, float radius1, Vec3 sphr2, float radius2)
{
	// calclates the distance between the two spheres.
	float dist = sqrt(pow(sphr2.x - sphr1.x, 2) + pow(sphr2.y - sphr1.y, 2) + pow(sphr2.z - sphr1.z, 2));
	
	// if the distance between the two spheres is less than their combined radii, then there is collison.
	return dist <= radius1 + radius2;
}

// calculates sphere-sphere collision using individual floats and not vectors. Re-uses the other sphereCollision function.
bool util::math::sphereCollision(float x1, float y1, float z1, float r1, float x2, float y2, float z2, float r2) { return sphereCollision(Vec3(x1, y1, z1), r1, Vec3(x2, y2, z2), r2); }

// resues Vec3 version
bool util::math::aabbCollision(const Vec2 minA, const Vec2 maxA, const Vec2 minB, const Vec2 maxB)
{
	return aabbCollision(Vec3(minA.x, minA.y, 0.0F), Vec3(maxA.x, maxA.y, 0.0F), Vec3(minB.x, minB.y, 0.0F), Vec3(maxB.x, maxB.y, 0.0F));
}

// calculates cube-cube collision with the provided minimum (x, y, z) and maximum (x, y, z) of cube A and cubeB.
bool util::math::aabbCollision(Vec3 minA, Vec3 maxA, Vec3 minB, Vec3 maxB)
{
	// booleans for collision checks
	bool xCol = false, yCol = false, zCol = false;

	// If the projection on the x-axis shows collision.
	if ((minB.x > minA.x && minB.x < maxA.x) ^ (minA.x > minB.x && minA.x < maxB.x))
		xCol = true;

	// If the projection on the y-axis shows collision.
	if ((minB.y > minA.y && minB.y < maxA.y) ^ (minA.y > minB.y && minA.y < maxB.y))
		yCol = true;

	// If the projection on the z-axis shows collision, or if there is no z-value (i.e. a square is being used and not a cube)
	if ((minB.z > minA.z && minB.z < maxA.z) ^ (minA.z > minB.z && minA.z < maxB.z) ^ (minA.z == 0.0F && maxA.z == 0.0F && minB.z == 0.0F && maxB.z == 0.0F))
		zCol = true;
	
	// returns if all booleans have been set to 'true'. If so, then there is collision.
	return (xCol && yCol && zCol);
}

// calculates axis-aligned bounding box collision. The 'w' parameter isn't used, and set the 'z' parameter to 0.
// row 0 = top left corner, row 1 = top right corner, row 2 = bottom left corner, row 3 = bottom right corner
bool util::math::aabbCollision(Mat4 cubeA, Mat4 cubeB)
{
	// gets maximum and minimum values
	Vec3 cubeAMin(cubeA[2].x, cubeA[2].y, cubeA[2].z), cubeAMax(cubeA[1].x, cubeA[1].y, cubeA[1].z);
	Vec3 cubeBMin(cubeB[2].x, cubeB[2].y, cubeB[2].z), cubeBMax(cubeB[1].x, cubeB[1].y, cubeB[1].z);

	return (aabbCollision(cubeAMin, cubeAMax, cubeBMin, cubeBMax));
}

// calculates axis-aligned bounding box collision. You input the four corner points.
bool util::math::aabbCollision(Vec2 topLeftA, Vec2 topRightA, Vec2 bottomLeftA, Vec2 bottomRightA, Vec2 topLeftB, Vec2 topRightB, Vec2 bottomLeftB, Vec2 bottomRightB)
{
	return aabbCollision(
		Vec3(topLeftA.x, topLeftA.y, 0.0F), Vec3(topRightA.x, topRightA.y, 0.0F), Vec3(bottomLeftA.x, bottomLeftA.y, 0.0F), Vec3(bottomRightA.x, bottomRightA.y, 0.0F),
		Vec3(topLeftB.x, topLeftB.y, 0.0F), Vec3(topRightB.x, topRightB.y, 0.0F), Vec3(bottomLeftB.x, bottomLeftB.y, 0.0F), Vec3(bottomRightB.x, bottomRightB.y, 0.0F)
	);
}

// calculates axis-aligned bounding box collision.
bool util::math::aabbCollision(Vec3 topLeftA, Vec3 topRightA, Vec3 bottomLeftA, Vec3 bottomRightA, Vec3 topLeftB, Vec3 topRightB, Vec3 bottomLeftB, Vec3 bottomRightB)
{
	// creates the first cube
	Mat4 cubeA = Mat4(
		Vec4(topLeftA.x, topLeftA.y, topLeftA.z, 0),
		Vec4(topRightA.x, topRightA.y, topRightA.z, 0),
		Vec4(bottomLeftA.x, bottomLeftA.y, bottomLeftA.z, 0),
		Vec4(bottomRightA.x, bottomRightA.y, bottomRightA.z, 0)
	);

	// creates the second cube
	Mat4 cubeB = Mat4(
		Vec4(topLeftB.x, topLeftB.y, topLeftB.z, 0),
		Vec4(topRightB.x, topRightB.y, topRightB.z, 0),
		Vec4(bottomLeftB.x, bottomLeftB.y, bottomLeftB.z, 0),
		Vec4(bottomRightB.x, bottomRightB.y, bottomRightB.z, 0)
	);

	return aabbCollision(cubeA, cubeB);
}

// OBB Collision; rotation factor is in radians.
bool util::math::obbCollision(const Vec2 topLeftA, const Vec2 topRightA, const Vec2 bottomLeftA, const Vec2 bottomRightA, const float thetaA, const Vec2 topLeftB, const Vec2 topRightB, const Vec2 bottomLeftB, const Vec2 bottomRightB, const float thetaB, const bool IS_ROTATED)
{
	Vec2 posA = ((bottomLeftA + topRightA) / 2); // the position of rectangle A
	Vec2 posB = ((bottomLeftB + topRightB) / 2); // the position of rectangle B
	Vec2 rPos = (posA + posB) / 2; // the centre position that the two rectangles are rotated around.

	// makes copies of all the passed vector values.
	Vec2 tempTLA(topLeftA), tempTRA(topRightA), tempBLA(bottomLeftA), tempBRA(bottomRightA);
	Vec2 tempTLB(topLeftB), tempTRB(topRightB), tempBLB(bottomLeftB), tempBRB(bottomRightB);

	bool intersects = false; // saves the results of intersection checks.

	if (IS_ROTATED == false) // if the boxes have not been rotated, the function does so around their centre
	{
		tempTLA = util::math::rotate(topLeftA - posA, thetaA) + posA;
		tempTRA = util::math::rotate(topRightA - posA, thetaA) + posA;
		tempBLA = util::math::rotate(bottomLeftA - posA, thetaA) + posA;
		tempBRA = util::math::rotate(bottomRightA - posA, thetaA) + posA;

		tempTLB = util::math::rotate(topLeftB - posB, thetaB) + posB;
		tempTRB = util::math::rotate(topRightB - posB, thetaB) + posB;
		tempBLB = util::math::rotate(bottomLeftB - posB, thetaB) + posB;
		tempBRB = util::math::rotate(bottomRightB - posB, thetaB) + posB;
	}

	// checks for collision with rectangle A aligned with the axis.
	intersects = aabbCollision(
		util::math::rotate(tempTLA - rPos, -thetaA) + rPos, util::math::rotate(tempTRA - rPos, -thetaA) + rPos, util::math::rotate(tempBLA - rPos, -thetaA) + rPos, util::math::rotate(tempBRA - rPos, -thetaA) + rPos,
		util::math::rotate(tempTLB - rPos, -thetaA) + rPos, util::math::rotate(tempTRB - rPos, -thetaA) + rPos, util::math::rotate(tempBLB - rPos, -thetaA) + rPos, util::math::rotate(tempBRB - rPos, -thetaA) + rPos
	);

	if (intersects == false) // if there was no intersection, a false is returned.
		return false;

	// checks for collision with rectangle B aligned with the axis.
	intersects = aabbCollision(
		util::math::rotate(tempTLA - rPos, -thetaB) + rPos, util::math::rotate(tempTRA - rPos, -thetaB) + rPos, util::math::rotate(tempBLA - rPos, -thetaB) + rPos, util::math::rotate(tempBRA - rPos, -thetaB) + rPos,
		util::math::rotate(tempTLB - rPos, -thetaB) + rPos, util::math::rotate(tempTRB - rPos, -thetaB) + rPos, util::math::rotate(tempBLB - rPos, -thetaB) + rPos, util::math::rotate(tempBRB - rPos, -thetaB) + rPos
	);

	// if intersects is true, then there is collision. If not, then there is no intersection.
	return intersects;
}

// checks for collision between a circle and a point.
bool util::math::circlePointCollision(const Vec2 circPos, const float circRadius, const Vec2 point)
{
	// if the distance between the point and the circle are less than the circle's radius, there's collision.
	return (sqrt(pow(circPos.x - point.x, 2) + pow(circPos.y - point.y, 2)) <= abs(circRadius));
}

// checks collision between a circle and a line.
bool util::math::circleLineCollision(const Vec2 circPos, const float circRadius, const Vec2 lineStart, const Vec2 lineEnd)
{
	// Tutorial: http://www.jeffreythompson.org/collision-detection/table_of_contents.php

	float proj = 0;
	float projLen = 0;
	bool col = false;

	Vec2 vec1 = lineEnd - lineStart; // gets the first vector
	Vec2 vec2 = circPos - lineStart; // gets the second vector, which is used to check for projections

	// if either of the two points lie within the circle, then there is collision.
	if (circlePointCollision(circPos, circRadius, lineStart) || circlePointCollision(circPos, circRadius, lineEnd))
		return true;

	proj = vec1.dot(vec2); // gets the projection
	projLen = proj / vec1.length(); // gets the length of the projection of vec2 on vec1, which will be used to help with the pythgorean theorem

	col = sqrt(pow(vec2.length(), 2) - pow(projLen, 2)) <= abs(circRadius); // calculates the distance between the circle and the closest point on the line, and sees if there's collision.

	// if the circle's point is within the line, then the distance between the circle's position and the two ending points of the line should be equal to the length of the line itself.
	// this would return false if the circle's position isn't within the line, but this case has already been handled.
	if (!(abs(circPos.x - lineStart.x) + abs(lineEnd.x - circPos.x) == abs(lineEnd.x - lineStart.x) ||
		  abs(circPos.y - lineStart.y) + abs(lineEnd.y - circPos.y) == abs(lineEnd.y - lineStart.y)))
	{
		col = false;
	}


	return col; // calculates the distance between the circle and the closest point on the line.
}

// circle collision with AABB.
bool util::math::circleAABBCollision(const Vec2 circPos, const float circRadius, Vec2 topLeftCorner, Vec2 topRightCorner, Vec2 bottomLeftCorner, Vec2 bottomRightCorner)
{
	// calculates the position of the AABB.
	Vec2 aabbPos = Vec2((topLeftCorner.x + topRightCorner.x) / 2.0F, (topLeftCorner.y + bottomLeftCorner.y) / 2.0F);

	// if the position of the circle is inside the aabb, it may not intersect with any of the square's sides. This checks for that form of collision.
	if (circPos.x >= topLeftCorner.x && circPos.x <= topRightCorner.x && circPos.y >= bottomLeftCorner.y && circPos.y <= topLeftCorner.y)
		return true;

	// circle collision with each line that makes up the AABB.
	if (circleLineCollision(circPos, circRadius, topLeftCorner, topRightCorner))
		return true;

	if (circleLineCollision(circPos, circRadius, topRightCorner, bottomRightCorner))
		return true;

	if (circleLineCollision(circPos, circRadius, bottomRightCorner, bottomLeftCorner))
		return true;

	if (circleLineCollision(circPos, circRadius, bottomLeftCorner, topLeftCorner))
		return true;

	return false;
}
