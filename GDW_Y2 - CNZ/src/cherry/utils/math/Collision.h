/*
 * Author: Roderick "R.J." Montague (100701758)
 * Date: 05/01/2019
 * Description: holds various calculations for collisions
*/

#ifndef COLLISIONS_H
#define COLLISIONS_H

#include "Matrix.h" // includes the vector
#include "Shape.h"

namespace util
{
	namespace math
	{
		// circle collision
		bool circleCollision(Vec2 circ1, float radius1, Vec2 circ2, float radius2);

		// circle collision
		bool circleCollision(Circle circ1, Circle circ2);

		// calculates sphere collision. Set the 'z' value as '0' for both spheres if you want to use it for circle collision.
		bool sphereCollision(Vec3 sphr1, float radius1, Vec3 sphr2, float radius2);

		// calculates sphere collision. Set the 'z' values to 0 if you want to use it as circle-circle-collision.
		bool sphereCollision(float x1, float y1, float z1, float r1, float x2, float y2, float z2, float r2);

		// calculates sphere collision
		bool sphereCollision(Sphere sphr1, Sphere sphr2);

		// AABB Collision
		// calculates aabb collision. The positions are considered the centre of the bounding boxes.
		bool aabbCollision(const Vec2 posA, float widthA, float heightA, const Vec2 posB, float widthB, float heightB);

		// calculates aabb collision. The positions are considered the centre of the bounding boxes.
		bool aabbCollision(const Vec3 posA, float widthA, float heightA, float depthA, const Vec3 posB, float widthB, float heightB, float depthB);

		// calculates cube-cube collision with the provided minimum (x, y) and maximum (x, y) of cube A and cubeB.
		bool aabbCollision(const Vec2 minA, const Vec2 maxA, const Vec2 minB, const Vec2 maxB);

		// calculates cube-cube collision with the provided minimum (x, y, z) and maximum (x, y, z) of cube A and cubeB.
		bool aabbCollision(const Vec3 minA, const Vec3 maxA, const Vec3 minB, const Vec3 maxB);

		// aabb collision between two boxes and two rotation factors.
		bool aabbCollision(const Box2D box1, const Box2D box2);

		// calculates axis-aligned bounding box collision. The 'w' parameter isn't used, and set the 'z' parameter to 0.
		// row 0 = top left corner, row 1 = top right corner, row 2 = bottom left corner, row 3 = bottom right corner
		bool aabbCollision(Mat4 squareA, Mat4 squareB);

		// calculates axis-aligned bounding box collision.
		// row 0 = top left corner, row 1 = top right corner, row 2 = bottom left corner, row 3 = bottom right corner
		// this is from the perspective of the front of the cube. So top left for the back face is based on if you were looking at the cube from its front.
		bool aabbCollision(Mat4 aFrontFace, Mat4 aBackFace, Mat4 bFrontFace, Mat4 bBackFace);

		// calculates axis-aligned bounding box collision. You input the four corner points.
		bool aabbCollision(Vec2 topLeftA, Vec2 topRightA, Vec2 bottomLeftA, Vec2 bottomRightA, Vec2 topLeftB, Vec2 topRightB, Vec2 bottomLeftB, Vec2 bottomRightB);

		// calculates axis-aligned bounding box collision. You input the eight corner points.
		bool aabbCollision(Vec3 topLeftFrontA, Vec3 topRightFrontA, Vec3 bottomLeftFrontA, Vec3 bottomRightFrontA, Vec3 topLeftBackA, Vec3 topRightBackA, Vec3 bottomLeftBackA, Vec3 bottomRightBackA,
			Vec3 topLeftFrontB, Vec3 topRightFrontB, Vec3 bottomLeftFrontB, Vec3 bottomRightFrontB, Vec3 topLeftBackB, Vec3 topRightBackB, Vec3 bottomLeftBackB, Vec3 bottomRightBackB);

		// AABB collision between two boxes.
		bool aabbCollision(const Box3D& boxA, const Box3D& boxB);

		// calculates oriented bounding box collision. Rotation factor is in RADIANS.
		// if IS_ROTATED is true, the points passed have already had their rotation factor applied to them.
		// if IS_ROTATED is false, the points passed have not been rotated, and the function will automatically rotate them.
		// Either both obbs are rotated already, or not.
		bool obbCollision(const Vec2 topLeftA, const Vec2 topRightA, const Vec2 bottomLeftA, const Vec2 bottomRightA, const float thetaA, const Vec2 topLeftB, const Vec2 topRightB, const Vec2 bottomLeftB, const Vec2 bottomRightB, const float thetaB, const bool IS_ROTATED = false);
		
		// obb collision between two boxes.
		bool obbCollision(const Box3D& boxA, bool inDegrees1, const Box3D& boxB, bool inDegrees2);

		// Mixed collisions
		// checks for collision between a circle and a point.
		bool circlePointCollision(const Vec2 circPos, const float circRadius, const Vec2 point);

		// collision between the sphere and a provided point.
		bool spherePointCollision(const Vec3 spherePos, const float sphereRadius, const Vec3 point);

		// checks collision between a circle and a line.
		// requires the circle's position (i.e. its centre), the circle's radius, the starting point of the line, and the ending point of the line line.
		bool circleLineCollision(const Vec2 circPos, const float circRadius, const Vec2 lineStart, const Vec2 lineEnd);

		// checks collision between a sphere and a line.
		// requires the sphere's position (based on its centre), the sphere's radius, the start point of hte line, and the end point of the line.
		bool sphereLineCollision(const Vec3 spherePos, const float sphereRadius, const Vec3 lineStart, const Vec3 lineEnd);

		// circle collision with AABB.
		bool circleAABBCollision(const Vec2 circPos, const float circRadius, Vec2 topLeftCorner, Vec2 topRightCorner, Vec2 bottomLeftCorner, Vec2 bottomRightCorner);

		// circle-AABB collision.
		bool circleAABBCollision(const Circle& circle, const Box2D box);

		// sphere-AABB collision.
		// the position for the sphere and the AABB are both considered to be the centre of their respective shapes.
		bool sphereAABBCollision(const Vec3 spherePos, const float sphereRadius, const Vec3 aabbPos, const float aabbWidth, const float aabbHeight, const float aabbDepth);

		// sphere-aabb collision
		bool sphereAABBCollision(const Sphere& sphere, const Box3D& box);

		// sphere-OBB collision.
		// the position for the sphere and the AABB are both considered to be the centre of their respective shapes.
		// rMatrix represents the rotations applied to the obb at its origin. 
		// If rotating on multiple axes, make sure to multiply said matrices beforehand.
		bool sphereOBBCollision(const Vec3 spherePos, const float sphereRadius, 
			const Vec3 obbPos, const float obbWidth, const float obbHeight, const float obbDepth, const Mat3 rMatrix);

		// sphere-OBB collision
		// rotations are applied in the order specified in the string 'order', which can contain the letters 'x', 'y', and 'z'.
		// if an unusable symbol is found, it is skipped. InDegrees determines whether it is in degrees or radians.
		// (x, y, z) in angles corresponds to 'x', 'y', and 'z' axis respectively.
		bool sphereOBBCollision(const Vec3 spherePos, const float sphereRadius,
			const Vec3 obbPos, const float obbWidth, const float obbHeight, const float obbDepth, Vec3 angles, bool inDegrees, const char order[3]);

		// sphere and OBB collision with sphere and box.
		bool sphereOBBCollision(const Sphere& sphere, const Box3D& box, const bool inDegrees);

		// calculates seperating axis theorem collision.
		// float satCollision();
	}
};
#endif // !COLLISIONS_H

