#ifndef ROTATION_H
#define ROTATION_H

// #include "Vector.h"
#include "Matrix.h"

namespace util
{
	namespace math
	{
		// returns the value of pi() as a double
		double pi();

		// returns pi as a double.
		double pid();

		// returns pi as a float.
		float pif();


		// conversion from degrees to radians. 1 degree = pi/180 radians. 
		float degreesToRadians(float degrees);

		// conversion from radians to degrees. 1 radian = 180/pi degrees.
		float radiansToDegrees(float radians);

		// gets a rotation matrix using variable 'theta'. 'InDegrees' tells the program if the angle passed is in degrees or radians.
		// axis is used to determine what axis to rotate on (x, y, z). If any other symbol is passed, an identity matrix is returned.
		Mat3 getRotationMatrix(float angle, bool inDegrees, char axis);

		// gets a z-axis rotation matrix using variable 'theta'. 'InDegrees' tells the program if the angle passed is in degrees or radians.
		Mat3 getRotationMatrixZ(float angle, bool inDegrees);

		// gets an x-axis rotation matrix using variable 'theta'. 'InDegrees' tells the program if the angle passed is in degrees or radians.
		Mat3 getRotationMatrixX(float angle, bool inDegrees);

		// gets a y-axis rotation matrix using variable 'theta'. 'InDegrees' tells the program if the angle passed is in degrees or radians.
		Mat3 getRotationMatrixY(float angle, bool inDegrees);

		// rotates the provided point. The value of 'inDegrees' determines whether the provided angle is in degrees, or in radians.
		// the angle needs to be in radians for the rotation to work, so if it is provided in degrees, a conversion is necessary.
		Vec2 rotate(const Vec2& vec, float angle, bool inDegrees = false);

		// rotates a point with a provided angle and axis. 'Axis'can only be 'x', 'y', or 'z'.
		// if an unusable point is provided, then it returns the point provided, unrotated.
		Vec3 rotate(const Vec3& vec, float angle, char axisOfRotation, bool inDegrees);

		// rotates on the z-axis using matrices
		Vec3 rotateZ(const Vec3& vec, float angle, bool inDegrees);

		// rotates on the x-axis using matrices
		Vec3 rotateX(const Vec3&, float angle, bool inDegrees);

		// rotates on the y-axis using matrices
		Vec3 rotateY(const Vec3&, float angle, bool inDegrees);

		// quaternion rotation of a 2D vector
		Vec2 rotateQuat(const Vec2& vec, float angle, char axisOfRotation, bool inDegrees);

		// rotates a 3D vector via quaternions. If the axis of rotation is invalid, then the vector is returned with no changes.
		Vec3 rotateQuat(const Vec3& vec, float angle, char axisOfRotation, bool inDegrees);

		// rotates a 3D vector via quaternions in a provided direction.
		// rotates a vector in the direction of 'direcOfRotation'. The boolean 'isNormalized' is used to determine whether the provided direction has already been normalized. If not, it is normalized.
		Vec3 rotateQuat(const Vec3& vec, float angle, const Vec3& direcOfRotation, bool isNormalized, bool inDegrees);

		// rotate around the z-axis
		Vec3 rotateQuatZ(const Vec3& vec, float angle, bool inDegrees);

		// rotate around the x-axis
		Vec3 rotateQuatX(const Vec3& vec, float angle, bool inDegrees);

		// rotate around the y-axis
		Vec3 rotateQuatY(const Vec3& vec, float angle, bool inDegrees);
	}
}

#endif // !ROTATION_H
