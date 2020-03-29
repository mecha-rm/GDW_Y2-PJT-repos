#include "../Utils.h"
#include "Collision.h"
#include "Rotation.h"

#include <iostream>
#include <math.h>
#include <algorithm>

// SPHERE/CIRCLE COLLISION
bool util::math::circleCollision(Vec2 circ1, float radius1, Vec2 circ2, float radius2)
{
	return sphereCollision(Vec3(circ1), radius1, Vec3(circ2), radius2);
}

// circle collision
bool util::math::circleCollision(Circle circ1, Circle circ2)
{
	return circleCollision(circ1.position, circ1.radius, circ2.position, circ2.radius);
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

// calculates sphere collision
bool util::math::sphereCollision(Sphere sphr1, Sphere sphr2)
{
	return sphereCollision(sphr1.position, sphr1.radius, sphr2.position, sphr2.radius);
}

// AABB Colliion
// calculates AABB collision by reusing the 3D calculation
bool util::math::aabbCollision(const Vec2 posA, float widthA, float heightA, const Vec2 posB, float widthB, float heightB)
{
	return aabbCollision(Vec3(posA, 0), widthA, heightA, 0, Vec3(posB, 0), widthB, heightB, 0);
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

	// // If the projection on the x-axis shows collision.
	// if ((minB.x > minA.x&& minB.x < maxA.x) ^ (minA.x > minB.x&& minA.x < maxB.x))
	// 	xCol = true;
	// 
	// // If the projection on the y-axis shows collision.
	// if ((minB.y > minA.y&& minB.y < maxA.y) ^ (minA.y > minB.y&& minA.y < maxB.y))
	// 	yCol = true;
	// 
	// // If the projection on the z-axis shows collision, or if there is no z-value (i.e. a square is being used and not a cube)
	// if ((minB.z > minA.z&& minB.z < maxA.z) ^ (minA.z > minB.z&& minA.z < maxB.z) ^ (minA.z == 0.0F && maxA.z == 0.0F && minB.z == 0.0F && maxB.z == 0.0F))
	// 	zCol = true;

	// If the projection on the x-axis shows collision.
	if ((minB.x >= minA.x && minB.x <= maxA.x) || (minA.x >= minB.x && minA.x <= maxB.x))
		xCol = true;

	// If the projection on the y-axis shows collision.
	if ((minB.y >= minA.y && minB.y <= maxA.y) || (minA.y >= minB.y && minA.y <= maxB.y))
		yCol = true;

	// If the projection on the z-axis shows collision, or if there is no z-value (i.e. a square is being used and not a cube)
	if ((minB.z >= minA.z && minB.z <= maxA.z) || (minA.z >= minB.z && minA.z <= maxB.z) || (minA.z == 0.0F && maxA.z == 0.0F && minB.z == 0.0F && maxB.z == 0.0F))
		zCol = true;

	// returns if all booleans have been set to 'true'. If so, then there is collision.
	return (xCol && yCol && zCol);
}

// aabb collision
bool util::math::aabbCollision(const Box2D box1, const Box2D box2)
{
	return aabbCollision(box1.position, box1.width, box1.height, box2.position, box2.width, box2.height);
}

// aabb collision with two boxes
// bool util::math::aabbCollision(const Box2D box1, const bool inDegrees1, const Box2D box2, const bool inDegrees2)
// {
// 	// the two rotation factors
// 	float theta1 = (inDegrees1) ? degreesToRadians(box1.rotation) : box1.rotation;
// 	float theta2 = (inDegrees1) ? degreesToRadians(box1.rotation) ? box1.rotation;
// 
// 	return obbCollision(;
// }

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

// AABB collision between two boxes.
bool util::math::aabbCollision(const Box3D& boxA, const Box3D& boxB)
{
	return aabbCollision(boxA.position, boxA.width, boxA.height, boxA.depth,
		boxB.position, boxB.width, boxB.height, boxB.depth);
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

// // obb collision between two boxes.
bool util::math::obbCollision(const Box3D& boxA, bool inDegreesA, const Box3D& boxB, bool inDegreesB)
{
	const int ARR_SIZE = 8;

	// the centre position that the two rectangles are rotated around.
	Vec3 rPos = (boxA.position + boxB.position) / 2.0F;
	// rotation position as a Matrix
	Mat3 rPosMat(
		rPos.x, 0, 0,
		rPos.y, 0, 0,
		rPos.z, 0, 0
	);

	// the vertices of box A
	Vec3 vertsA[ARR_SIZE] {
		Vec3(boxA.position + Vec3(-boxA.width, -boxA.height, -boxA.depth) / 2.0F),
		Vec3(boxA.position + Vec3(boxA.width, -boxA.height, -boxA.depth) / 2.0F),
		Vec3(boxA.position + Vec3(-boxA.width, boxA.height, -boxA.depth) / 2.0F),
		Vec3(boxA.position + Vec3(boxA.width, boxA.height, -boxA.depth) / 2.0F),

		Vec3(boxA.position + Vec3(-boxA.width, -boxA.height, boxA.depth) / 2.0F),
		Vec3(boxA.position + Vec3(boxA.width, -boxA.height, boxA.depth) / 2.0F),
		Vec3(boxA.position + Vec3(-boxA.width, boxA.height, boxA.depth) / 2.0F),
		Vec3(boxA.position + Vec3(boxA.width, boxA.height, boxA.depth) / 2.0F)
	};

	// transformed verts A
	Vec3 t_vertsA[ARR_SIZE];

	// the vertices of box B
	Vec3 vertsB[ARR_SIZE]{
		Vec3(boxB.position + Vec3(-boxB.width, -boxB.height, -boxB.depth) / 2.0F),
		Vec3(boxB.position + Vec3(boxB.width, -boxB.height, -boxB.depth) / 2.0F),
		Vec3(boxB.position + Vec3(-boxB.width, boxB.height, -boxB.depth) / 2.0F),
		Vec3(boxB.position + Vec3(boxB.width, boxB.height, -boxB.depth) / 2.0F),

		Vec3(boxB.position + Vec3(-boxB.width, -boxB.height, boxB.depth) / 2.0F),
		Vec3(boxB.position + Vec3(boxB.width, -boxB.height, boxB.depth) / 2.0F),
		Vec3(boxB.position + Vec3(-boxB.width, boxB.height, boxB.depth) / 2.0F),
		Vec3(boxB.position + Vec3(boxB.width, boxB.height, boxB.depth) / 2.0F)
	};

	// transformed verts B
	Vec3 t_vertsB[ARR_SIZE];

	bool intersects = false; // saves the results of intersection checks.
	char axisA = ' '; // the axis of rotation for box A.
	char axisB = ' '; // the axis of rotation for box B.

	// maximum and minimum values for A
	Vec3 minA{}, maxA{};

	// maximum and minimum values for B
	Vec3 minB{}, maxB{};

	// rotation matrix for A, and the reverse of it.
	Mat3 rotMatA, rotMatA_R;

	// rotation matrix for B, and the reverse of it.
	Mat3 rotMatB, rotMatB_R;

	// matrices for minimum and maximums
	Mat3 minMatA, minMatB, maxMatA, maxMatB;
	Mat3 minMatA_T, minMatB_T, maxMatA_T, maxMatB_T;

	// copying the arrays
	memcpy(t_vertsA, vertsA, sizeof(Vec3) * ARR_SIZE);
	memcpy(t_vertsB, vertsB, sizeof(Vec3) * ARR_SIZE);

	// bool thetaA = box1

	// transforms the boxes around their centre.
	for (int i = 0; i < 3; i++)
	{

		// box A - getting the axis of rotation
		switch (boxA.rotationOrder[i])
		{
		case 'X':
		case 'x':
			axisA = 'X';
			break;

		case 'Y':
		case 'y':
			axisA = 'Y';
			break;

		case 'Z':
		case 'z':
			axisA = 'Z';
			break;

		default:
			axisA = ' ';
			break;
		}

		// box B - getting the axis of rotation
		switch (boxB.rotationOrder[i])
		{
		case 'X':
		case 'x':
			axisB = 'X';
			break;

		case 'Y':
		case 'y':
			axisB = 'Y';
			break;

		case 'Z':
		case 'z':
			axisB = 'Z';
			break;

		default:
			axisB = ' ';
			break;
		}


		// goes through the arrays and transforms the points for the axes
		for (int j = 0; j < ARR_SIZE; j++)
		{
			if(axisA != ' ')
				t_vertsA[j] = util::math::rotate(t_vertsA[j] - boxA.position, boxA.rotation[i], axisA, inDegreesA) + boxA.position;

			if (axisB != ' ')
				t_vertsB[j] = util::math::rotate(t_vertsB[j] - boxB.position, boxB.rotation[i], axisB, inDegreesB) + boxB.position;
		}	
	}

	// now to find the maximum and minimum positions.
	// initial maximum and minimum.
	minA = t_vertsA[0];
	maxA = minA;

	minB = t_vertsB[0];
	maxB = minB;

	// finding the maximum and minimum positions
	for (int i = 0; i < ARR_SIZE; i++)
	{
		// minimum values for A
		minA.x = std::fmin(t_vertsA[i].x, minA.x);
		minA.y = std::fmin(t_vertsA[i].y, minA.y);
		minA.z = std::fmin(t_vertsA[i].z, minA.z);

		// maximum values for A
		maxA.x = std::fmax(t_vertsA[i].x, maxA.x);
		maxA.y = std::fmax(t_vertsA[i].y, maxA.y);
		maxA.z = std::fmax(t_vertsA[i].z, maxA.z);


		// minimum values for B
		minB.x = std::fmin(t_vertsB[i].x, minB.x);
		minB.y = std::fmin(t_vertsB[i].y, minB.y);
		minB.z = std::fmin(t_vertsB[i].z, minB.z);

		// maximum values for B
		maxB.x = std::fmax(t_vertsB[i].x, maxB.x);
		maxB.y = std::fmax(t_vertsB[i].y, maxB.y);
		maxB.z = std::fmax(t_vertsB[i].z, maxB.z);
	}

	// min matrix for A
	minMatA = Mat3(
		minA.x, 0, 0,
		minA.y, 0, 0,
		minA.z, 0, 0
	);

	// max matrix for A
	maxMatA = Mat3(
		maxA.x, 0, 0,
		maxA.y, 0, 0,
		maxA.z, 0, 0
	);

	// min matrix for B
	minMatB = Mat3(
		minB.x, 0, 0,
		minB.y, 0, 0,
		minB.z, 0, 0
	);

	// max matrix for B
	maxMatB = Mat3(
		maxB.x, 0, 0,
		maxB.y, 0, 0,
		maxB.z, 0, 0
	);

	// Rotation Matrices - getting the rotation matrices for going forward and backwards
	// regular
	rotMatA = 
		getRotationMatrix(boxA.rotation.x, inDegreesA, boxA.rotationOrder[0]) *
		getRotationMatrix(boxA.rotation.y, inDegreesA, boxA.rotationOrder[1]) * 
		getRotationMatrix(boxA.rotation.z, inDegreesA, boxA.rotationOrder[2]);

	// reverse
	rotMatA_R = 
		getRotationMatrix(-boxA.rotation.z, inDegreesA, boxA.rotationOrder[2]) *
		getRotationMatrix(-boxA.rotation.y, inDegreesA, boxA.rotationOrder[1]) *
		getRotationMatrix(-boxA.rotation.x, inDegreesA, boxA.rotationOrder[0]);


	// regular
	rotMatB =
		getRotationMatrix(boxB.rotation.x, inDegreesB, boxB.rotationOrder[0]) *
		getRotationMatrix(boxB.rotation.y, inDegreesB, boxB.rotationOrder[1]) *
		getRotationMatrix(boxB.rotation.z, inDegreesB, boxB.rotationOrder[2]);

	// reverse
	rotMatB_R =
		getRotationMatrix(-boxB.rotation.z, inDegreesB, boxB.rotationOrder[2]) *
		getRotationMatrix(-boxB.rotation.y, inDegreesB, boxB.rotationOrder[1]) *
		getRotationMatrix(-boxB.rotation.x, inDegreesB, boxB.rotationOrder[0]);
	
	// getting the minimum and maximums now that the items have been rotated.
	for (int i = 1; i <= 2; i++)
	{
		if (i == 1) // Check 1 (A Check)
		{
			// rotating the values for A's rotation
			// minimum a and maximum a
			minMatA_T = rotMatA_R * (minMatA - rPosMat) + rPosMat;
			maxMatA_T = rotMatA_R * (maxMatA - rPosMat) + rPosMat;

			// minimum b and maximum b
			minMatB_T = rotMatA_R * (minMatB - rPosMat) + rPosMat;
			maxMatB_T = rotMatA_R * (maxMatB - rPosMat) + rPosMat;
		}
		else if (i == 2) // Check 2 (B Check)
		{
			// rotating the values for B's rotation
			// minimum a and maximum a
			minMatA_T = rotMatB_R * (minMatA - rPosMat) + rPosMat;
			maxMatA_T = rotMatB_R * (maxMatA - rPosMat) + rPosMat;

			// minimum b and maximum b
			minMatB_T = rotMatB_R * (minMatB - rPosMat) + rPosMat;
			maxMatB_T = rotMatB_R * (maxMatB - rPosMat) + rPosMat;
		}

		// rotated minimums and maximums
		Vec3 rMinA{}, rMaxA{}, rMinB{}, rMaxB{};

		// getting the minimums for A
		rMinA.x = std::fmin(minMatA_T[0][0], maxMatA_T[0][0]);
		rMinA.y = std::fmin(minMatA_T[1][0], maxMatA_T[1][0]);
		rMinA.z = std::fmin(minMatA_T[2][0], maxMatA_T[2][0]);

		// getting the maximums for A
		rMaxA.x = std::fmax(minMatA_T[0][0], maxMatA_T[0][0]);
		rMaxA.y = std::fmax(minMatA_T[1][0], maxMatA_T[1][0]);
		rMaxA.z = std::fmax(minMatA_T[2][0], maxMatA_T[2][0]);

		// getting the minimums for B
		rMinB.x = std::fmin(minMatB_T[0][0], maxMatB_T[0][0]);
		rMinB.y = std::fmin(minMatB_T[1][0], maxMatB_T[1][0]);
		rMinB.z = std::fmin(minMatB_T[2][0], maxMatB_T[2][0]);

		// getting the maximums for B
		rMaxB.x = std::fmax(minMatB_T[0][0], maxMatB_T[0][0]);
		rMaxB.y = std::fmax(minMatB_T[1][0], maxMatB_T[1][0]);
		rMaxB.z = std::fmax(minMatB_T[2][0], maxMatB_T[2][0]);

		// intersection check
		intersects = aabbCollision(rMinA, rMaxA, rMinB, rMaxB);

		// if no intersection
		if (intersects == false)
			return false;
	}

	// if 'intersects' is true, then there is collision. If not, then there is no intersection.
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

	// if the distance between the centre of the circle and the centre of the AABB is less than the circle's radius...
	// the centre of the aabb is inside the circle, and there is collision.
	if ((circPos - aabbPos).length() <= circRadius)
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

// sphere-AABB collision.
bool util::math::circleAABBCollision(const Circle& circle, const Box2D box)
{
	return circleAABBCollision(circle.position, circle.radius, 
		box.position + Vec2(-box.width / 2.0F, box.height / 2.0F), // top left
		box.position + Vec2(box.width / 2.0F, box.height / 2.0F), // top right
		box.position + Vec2(-box.width / 2.0F, -box.height / 2.0F), // bottom left
		box.position + Vec2(box.width / 2.0F, -box.height / 2.0F) // bottom right
	);
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

// sphere-aabb collision.
bool util::math::sphereAABBCollision(const Sphere& sphere, const Box3D& box)
{
	return sphereAABBCollision(sphere.position, sphere.radius, box.position, box.width, box.height, box.depth);
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
	Vec3 angles, bool inDegrees, const char order[3])
{
	Mat3 finalMatrix = Mat3::getIdentityMatrix(); // the matrix for rotations
	Mat3 rMatrix = Mat3::getIdentityMatrix(); // matrix for the current rotation.

	// converts ot radians if passed in degrees.
	if (inDegrees)
		angles = Vec3(degreesToRadians(angles.x), degreesToRadians(angles.y), degreesToRadians(angles.z));
	
	// gets each rotation.
	for (int i = 0; i < 3; i++)
	{
		// gets the matrices
		switch (order[i])
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

	// calls the other function to do the calculation with the rotation matrix
	return sphereOBBCollision(spherePos, sphereRadius, obbPos, obbWidth, obbHeight, obbDepth, finalMatrix);
}

// sphere OBB collision with the structs
bool util::math::sphereOBBCollision(const Sphere& sphere, const Box3D& box, const bool inDegrees)
{
	return sphereOBBCollision(sphere.position, sphere.radius,
		box.position, box.width, box.height, box.depth, box.rotation, inDegrees, box.rotationOrder);
}
