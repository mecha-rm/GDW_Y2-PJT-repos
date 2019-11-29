#include "../Utils.h"
#include "Collision.h"
#include "Rotation.h"

#include <iostream>
#include <math.h>
// #include <algorithm> // max/min function

// SPHERE/CIRCLE COLLISION
bool util::math::circleCollision(Vec2 circ1, float radius1, Vec2 circ2, float radius2)
{
	return sphereCollision(Vec3(circ1), radius1, Vec3(circ2), radius2);
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


// AABB Colliion
// calculates AABB collision by reusing the 3D calculation
bool util::math::aabbCollision(const Vec2 posA, float widthA, float heightA, float depthA, const Vec2 posB, float widthB, float heightB, float depthB)
{
	return aabbCollision(Vec3(posA), widthA, heightA, depthA, Vec3(posB), widthB, heightB, depthB);
}

// calculates aabb collision between two bounding boxes
bool util::math::aabbCollision(const Vec3 posA, float widthA, float heightA, float depthA, const Vec3 posB, float widthB, float heightB, float depthB)
{
	// maximum and minimum values for object A
	Vec3 aMin = posA - Vec3(widthA / 2.0F, heightA / 2.0f, depthA / 2.0F);
	Vec3 aMax = posA + Vec3(widthA / 2.0F, heightA / 2.0f, depthA / 2.0F);

	// maximum and minimum values for object B
	Vec3 bMin = posB - Vec3(widthB / 2.0F, heightB / 2.0f, depthB / 2.0F);
	Vec3 bMax = posB + Vec3(widthB / 2.0F, heightB / 2.0f, depthB / 2.0F);

	// calculates collision.
	return aabbCollision(aMin, aMax, bMin, bMax);
}

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
	if ((minB.x > minA.x&& minB.x < maxA.x) ^ (minA.x > minB.x&& minA.x < maxB.x))
		xCol = true;

	// If the projection on the y-axis shows collision.
	if ((minB.y > minA.y&& minB.y < maxA.y) ^ (minA.y > minB.y&& minA.y < maxB.y))
		yCol = true;

	// If the projection on the z-axis shows collision, or if there is no z-value (i.e. a square is being used and not a cube)
	if ((minB.z > minA.z&& minB.z < maxA.z) ^ (minA.z > minB.z&& minA.z < maxB.z) ^ (minA.z == 0.0F && maxA.z == 0.0F && minB.z == 0.0F && maxB.z == 0.0F))
		zCol = true;

	// returns if all booleans have been set to 'true'. If so, then there is collision.
	return (xCol && yCol && zCol);
}

// calculates axis-aligned bounding box collision. The 'w' parameter isn't used, and set the 'z' parameter to 0.
// row 0 = top left corner, row 1 = top right corner, row 2 = bottom left corner, row 3 = bottom right corner
bool util::math::aabbCollision(Mat4 squareA, Mat4 squareB)
{
	// gets maximum and minimum values
	Vec3 aMin(squareA[2].x, squareA[2].y, squareA[2].z);
	Vec3 aMax(squareA[1].x, squareA[1].y, squareA[1].z);

	Vec3 bMin(squareB[2].x, squareB[2].y, squareB[2].z);
	Vec3 bMax(squareB[1].x, squareB[1].y, squareB[1].z);

	return (aabbCollision(aMin, aMax, bMin, bMax));
}

// aabb collision with cubes
// row 0 = top left corner, row 1 = top right corner, row 2 = bottom left corner, row 3 = bottom right corner
// this is from the perspective of the front of the cube. So top left for the back face is based on if you were looking at the cube from its front.
bool util::math::aabbCollision(Mat4 aFrontFace, Mat4 aBackFace, Mat4 bFrontFace, Mat4 bBackFace)
{
	Vec3 aMax(aFrontFace[1].x, aFrontFace[1].y, aFrontFace[1].z); // front face, top right
	Vec3 aMin(aBackFace[2].x, aBackFace[2].y, aBackFace[2].z); // back face, bottom left

	Vec3 bMax(bFrontFace[1].x, bFrontFace[1].y, bFrontFace[1].z); // front face, top right
	Vec3 bMin(bBackFace[2].x, bBackFace[2].y, bBackFace[2].z); // back face, bottom left

	return aabbCollision(aMin, aMax, bMin, bMax);
}

// calculates axis-aligned bounding box collision. You input the four corner points.
bool util::math::aabbCollision(Vec2 topLeftA, Vec2 topRightA, Vec2 bottomLeftA, Vec2 bottomRightA, Vec2 topLeftB, Vec2 topRightB, Vec2 bottomLeftB, Vec2 bottomRightB)
{
	return aabbCollision(Mat4(Vec3(topLeftA), Vec3(topRightA), Vec3(bottomLeftA), Vec3(bottomRightA)),
		Mat4(Vec3(topLeftB), Vec3(topRightB), Vec3(bottomLeftB), Vec3(bottomRightB)));
}

// calculates axis-aligned bounding box collision.
bool util::math::aabbCollision(Vec3 topLeftFrontA, Vec3 topRightFrontA, Vec3 bottomLeftFrontA, Vec3 bottomRightFrontA, Vec3 topLeftBackA, Vec3 topRightBackA, Vec3 bottomLeftBackA, Vec3 bottomRightBackA,
	Vec3 topLeftFrontB, Vec3 topRightFrontB, Vec3 bottomLeftFrontB, Vec3 bottomRightFrontB, Vec3 topLeftBackB, Vec3 topRightBackB, Vec3 bottomLeftBackB, Vec3 bottomRightBackB)
{
	return util::math::aabbCollision(bottomLeftBackA, topRightFrontA, bottomLeftBackB, topRightFrontB);
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
	return (sqrt(powf(circPos.x - point.x, 2) + powf(circPos.y - point.y, 2)) <= abs(circRadius));
}

// distance between sphere and point to check for collision.
bool util::math::spherePointCollision(const Vec3 spherePos, const float sphereRadius, const Vec3 point)
{
	// if the distance between the point and the circle are less than the circle's radius, there's collision.
	return (sqrt(powf(spherePos.x - point.x, 2) + powf(spherePos.y - point.y, 2) + powf(spherePos.z - point.z, 2)) <= abs(sphereRadius));
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

	col = sqrt(powf(vec2.length(), 2) - powf(projLen, 2)) <= abs(circRadius); // calculates the distance between the circle and the closest point on the line, and sees if there's collision.

	// if the circle's point is within the line, then the distance between the circle's position and the two ending points of the line should be equal to the length of the line itself.
	// this would return false if the circle's position isn't within the line, but this case has already been handled.
	if (!(abs(circPos.x - lineStart.x) + abs(lineEnd.x - circPos.x) == abs(lineEnd.x - lineStart.x) ||
		abs(circPos.y - lineStart.y) + abs(lineEnd.y - circPos.y) == abs(lineEnd.y - lineStart.y)))
	{
		col = false;
	}

	return col; // calculates the distance between the circle and the closest point on the line.
}

// checks collision between a sphere and a line.
bool util::math::sphereLineCollision(const Vec3 spherePos, const float sphereRadius, const Vec3 lineStart, const Vec3 lineEnd)
{
	// Tutorial: http://www.jeffreythompson.org/collision-detection/table_of_contents.php

	float proj = 0;
	float projLen = 0;
	bool col = false;

	Vec3 vec1 = lineEnd - lineStart; // gets the first vector
	Vec3 vec2 = spherePos - lineStart; // gets the second vector, which is used to check for projections

	// if either of the two points lie within the circle, then there is collision.
	if (spherePointCollision(spherePos, sphereRadius, lineStart) || spherePointCollision(spherePos, sphereRadius, lineEnd))
		return true;

	proj = vec1.dot(vec2); // gets the projection
	projLen = proj / vec1.length(); // gets the length of the projection of vec2 on vec1, which will be used to help with the pythgorean theorem

	col = sqrt(powf(vec2.length(), 2) - powf(projLen, 2)) <= abs(sphereRadius); // calculates the distance between the circle and the closest point on the line, and sees if there's collision.

	// if the sphere's point is within the line, then the distance between the sphere's position and the two ending points of the line should be equal to the length of the line itself.
	// this would return false if the sphere's position isn't within the line, but this case has already been handled.
	if (!(abs(spherePos.x - lineStart.x) + abs(lineEnd.x - spherePos.x) == abs(lineEnd.x - lineStart.x) ||
		abs(spherePos.y - lineStart.y) + abs(lineEnd.y - spherePos.y) == abs(lineEnd.y - lineStart.y)))
	{
		col = false;
	}

	return col; // calculates the distance between the sphere and the closest point on the line.
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

// sphere-AABB Collision
bool util::math::sphereAABBCollision(const Vec3 spherePos, const float sphereRadius, const Vec3 aabbPos, const float aabbWidth, const float aabbHeight, const float aabbDepth)
{
	// source: https://developer.mozilla.org/en-US/docs/Games/Techniques/3D_collision_detection
	// source 2: http://www.jeffreythompson.org/collision-detection/circle-rect.php

	Vec3 closest; // a Vec3 for the closest point on the sphere.
	float dist; // the distance between the sphere and the cloest point on the aabb box.

	// calculates the closet point on the box to the sphere's current location.
	// first, it takes the minimum of the largest point for the box on a given axis, and the sphere's position.
	// it then checks against the smallest point on the box and the sphere's position, which gets the closest point to the sphere.
	closest.x = std::fmaxf(aabbPos.x - aabbWidth / 2.0F, std::fminf(spherePos.x, aabbPos.x + aabbWidth / 2.0F)); // width (x-axis)
	closest.y = std::fmaxf(aabbPos.y - aabbHeight / 2.0F, std::fminf(spherePos.y, aabbPos.y + aabbHeight / 2.0F)); // height (y-axis)
	closest.z = std::fmaxf(aabbPos.z - aabbDepth / 2.0F, std::fminf(spherePos.z, aabbPos.z + aabbDepth / 2.0F));; // depth (z-axis)

	dist = (closest - spherePos).length();
	
	return dist < sphereRadius; // checks against the sphere's radius.
}

// sphere - OBB collision
bool util::math::sphereOBBCollision(const Vec3 spherePos, const float sphereRadius, 
	const Vec3 obbPos, const float obbWidth, const float obbHeight, const float obbDepth, const Mat3 rMatrix)
{
	const unsigned int vertsTotal = 8;
	// front -> back
	// top left -> top right -> bottom left -> bottom right
	Vec3 obb[vertsTotal]; // a matrix that holds all 8 vertices for the obb.

	Vec3 obbMax; // the maximum point on the obb
	Vec3 obbMin; // the minimum point on the obb
	Mat3 pMatrix; // a matrix used for saving the position.

	Vec3 closest; // a Vec3 for the closest point on the sphere to the obb.
	float dist; // the distance between the sphere and the cloest point on the obb box.

	// getting the vertices of the cube. Labels are based on if you were facing the front of the cube.
	obb[0] = obbPos + Vec3(-obbWidth / 2.0F, obbHeight / 2.0F, -obbDepth / 2.0F); // top left, front face
	obb[1] = obbPos + Vec3(obbWidth / 2.0F, obbHeight / 2.0F, -obbDepth / 2.0F); // top right, front face
	obb[2] = obbPos + Vec3(-obbWidth / 2.0F, -obbHeight / 2.0F, -obbDepth / 2.0F); // bottom left, front face
	obb[3] = obbPos + Vec3(obbWidth / 2.0F, -obbHeight / 2.0F, -obbDepth / 2.0F); // bottom right, front face

	obb[4] = obbPos + Vec3(-obbWidth / 2.0F, obbHeight / 2.0F, obbDepth / 2.0F); // top left, back face
	obb[5] = obbPos + Vec3(obbWidth / 2.0F, obbHeight / 2.0F, obbDepth / 2.0F); // top right, back face
	obb[6] = obbPos + Vec3(-obbWidth / 2.0F, -obbHeight / 2.0F, obbDepth / 2.0F); // bottom left, back face
	obb[7] = obbPos + Vec3(obbWidth / 2.0F, -obbHeight / 2.0F, obbDepth / 2.0F); // bottom right, back face

	// applying the rotations aorund the origin.
	for (int i = 0; i < vertsTotal; i++)
	{
		// getting the values stored in a matrix and moving the object to its origin.
		pMatrix[0][0] = obb[i].x - obbPos.x;
		pMatrix[1][0] = obb[i].y - obbPos.y;
		pMatrix[2][0] = obb[i].z - obbPos.z;

		pMatrix = rMatrix * pMatrix; // applying the rotation.

		// moving the vertex back in place.
		obb[i].x = pMatrix[0][0] + obbPos.x;
		obb[i].y = pMatrix[1][0] + obbPos.y;
		obb[i].z = pMatrix[2][0] + obbPos.z;
	}
	
	obbMax = obb[0];
	obbMin = obb[0];

	// finding the maximum and minimum values
	for (int i = 0; i < vertsTotal; i++)
	{
		// finding the maximum values on each axis.
		obbMax.x = obb[i].x > obbMax.x ? obb[i].x : obbMax.x;
		obbMax.y = obb[i].y > obbMax.y ? obb[i].y : obbMax.y;
		obbMax.z = obb[i].z > obbMax.z ? obb[i].z : obbMax.z;

		// finding the maximum values on each axis.
		obbMin.x = obb[i].x < obbMin.x ? obb[i].x : obbMin.x;
		obbMin.y = obb[i].y < obbMin.y ? obb[i].y : obbMin.y;
		obbMin.z = obb[i].z < obbMin.z ? obb[i].z : obbMin.z;
	}


	// calculates the closet point on the box to the sphere's current location.
	// first, it takes the minimum of the largest point for the box on a given axis, and the sphere's position.
	// it then checks against the smallest point on the box and the sphere's position, which gets the closest point to the sphere.
	closest.x = std::fmaxf(obbMin.x, std::fminf(spherePos.x, obbMax.x)); // width (x-axis)
	closest.y = std::fmaxf(obbMin.y, std::fminf(spherePos.y, obbMax.y)); // height (y-axis)
	closest.z = std::fmaxf(obbMin.z, std::fminf(spherePos.z, obbMax.z)); // depth (z-axis)

	
	// calculates the distance between the cloest point and the sphere and sees if there's collision.
	dist = (closest - spherePos).length();

	return dist < sphereRadius; // returns 'true' if there is collision.
}

// sphere-OBB collision using a provided angle and rotation axis.
bool util::math::sphereOBBCollision(const Vec3 spherePos, const float sphereRadius, const Vec3 obbPos, const float obbWidth, const float obbHeight, const float obbDepth, 
	Vec3 angles, bool inDegrees, std::string order)
{
	Mat3 finalMatrix = Mat3::getIdentityMatrix(); // the matrix for rotations
	Mat3 rMatrix = Mat3::getIdentityMatrix(); // matrix for the current rotation.

	// converts ot radians if passed in degrees.
	if (inDegrees)
		angles = Vec3(degreesToRadians(angles.x), degreesToRadians(angles.y), degreesToRadians(angles.z));
	
	if (order != "") // if there are characters
	{
		// gets each rotation.
		for (int i = 0; i < order.length(); i++)
		{
			// gets the matrices
			switch (order.at(i))
			{
				// x-axis
			case 'x':
			case 'X':
				rMatrix = getRotationMatrixX(angles.x, false);
				break;

				// y-axis
			case 'y':
			case 'Y':
				rMatrix = getRotationMatrixY(angles.y, false);
				break;

				// z-axis
			case 'z':
			case 'Z':
				rMatrix = getRotationMatrixZ(angles.z, false);
				break;
			}

			finalMatrix = finalMatrix * rMatrix; // multiplies the matrices
			rMatrix = Mat3::getIdentityMatrix(); // sets back to default
		}
	}

	// calls the other function to do the calculation with the rotation matrix
	return sphereOBBCollision(spherePos, sphereRadius, obbPos, obbWidth, obbHeight, obbDepth, finalMatrix);
}
