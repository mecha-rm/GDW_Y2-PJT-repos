#define M_PI 3.14159265358979323846  /* pi */

#include "Rotation.h"
#include "..//Utils.h"
#include "Matrix.h"
#include "Quaternion.h"

// returns the value of pi as a double
double util::math::pi() { return M_PI; }

// returns the value of pi as a double
double util::math::pid() { return pi(); }

// returns the value of pi as a float
float util::math::pif() { return (float)M_PI; }

// converts from degrees to radians
float util::math::degreesToRadians(float degrees) { return degrees * (pif() / 180); }

// converts from radians to degrees
float util::math::radiansToDegrees(float radians) { return radians * (180 / pif()); }

// a rotation function. While it doesn't use a rotation matrix, it's modeled after how one would be used for a rotation.
util::math::Vec2 util::math::rotate(const Vec2& vec, float angle, bool inDegrees)
{
	// rotates the coordinate points using a rotation matrix. Well, it technically ISN'T using a matrix, but it's modeled after how two matrices would be multiplied with one another.
	// This uses a rotation matrix setup, which is modeled below. With matricies, the calculation would be done in the way shown below, which is what was harcoded below.
	// [ cos a , -sin a] [x] = [ xcos a - ysin a ]
	// [ sin a ,  cos a] [y] = [ xsin a + ycos a ]

	if (inDegrees) // if the angle provided is in degrees, it's converted to radians.
		angle = util::math::degreesToRadians(angle);

	return util::math::Vec2(vec.x * (cosf(angle)) - vec.y * (sinf(angle)), vec.x * (sinf(angle)) + vec.y * (cosf(angle)));
}

// rotates a point with a provided angle and axis. 'Axis'can only be 'x', 'y', or 'z'.
// if an unusable point is provided, then it returns the point provided, unrotated.
util::math::Vec3 util::math::rotate(const Vec3& vec, float angle, char axis, bool inDegrees)
{
	util::math::Mat3 rMatrix; // the rotation matrix

	// the point matrix
	util::math::Mat3 pMatrix(vec.x, 0.0F, 0.0F,
		vec.y, 0.0F, 0.0F,
		vec.z, 0.0F, 0.0F);


	util::math::Mat3 tempMatrix; // the matrix that saves the results.

	if (inDegrees) // if the angle is in degrees, it's converted toradians.
		angle = util::math::degreesToRadians(angle);

	switch (axis)
	{
		// rotation on the x-axis
	case 'X':
	case 'x':
		// the rotation matrix for going around the x-axis
		// [ 1, 0, 0 ]
		// [ 0, cos a, -sin a]
		// [ 0, sin a, cos a]
		rMatrix = { 1.0F, 0.0F, 0.0F,
					0.0F, cosf(angle), -sinf(angle),
					0.0F, sinf(angle), cosf(angle)
		};
		break;

		// rotation on the y-axis
	case 'Y':
	case 'y':
		// the rotation matrix for going around the y-axis. The negative sin is at the bottom so that the points are rotated where we think they should be.
		// [ cos  a, 0, sin a]
		// [ 0, 1, 0]
		// [ -sin a, 0, cos a]
		rMatrix = { cosf(angle), 0.0F, sinf(angle),
					0.0F, 1.0F, 0.0F,
					-sinf(angle), 0.0F, cosf(angle)
		};
		break;

		// rotation on the z-axis
	case 'Z':
	case 'z':
		// the rotation matrix for going around the z-axis
		// [ cos a, -sin a, 0]
		// [ sin a, cos a, 0]
		// [ 0, 0, 1]
		rMatrix = Mat3{ cosf(angle), -sinf(angle), 0.0F,
						sinf(angle), cosf(angle), 0.0F,
						0.0F, 0.0F, 1.0F
		};
		break;
	}

	tempMatrix = rMatrix * pMatrix; // applying the rotation.

	return Vec3(tempMatrix[0][0], tempMatrix[1][0], tempMatrix[2][0]);
}

// a rotation function that uses a matrix 3.
util::math::Vec3 util::math::rotateZ(const Vec3& vec, float angle, bool inDegrees) { return rotate(vec, angle, 'z', inDegrees); }

// a rotation function that uses a matrix 3.
util::math::Vec3 util::math::rotateX(const Vec3& vec, float angle, bool inDegrees) { return rotate(vec, angle, 'x', inDegrees); }

// rotates on the y-axis
util::math::Vec3 util::math::rotateY(const Vec3& vec, float angle, bool inDegrees) { return rotate(vec, angle, 'y', inDegrees); }

// quaternion rotation of a 2D vector
util::math::Vec2 util::math::rotateQuat(const Vec2& vec, float angle, char axisOfRotation, bool inDegrees)
{
	Vec3 tempVec = util::math::rotateQuat(util::math::Vec3(vec.x, vec.y, 0.0F), angle, axisOfRotation, inDegrees);
	return util::math::Vec2(tempVec.x, tempVec.y);
}

// rotates a 3D vector via quaternions.
util::math::Vec3 util::math::rotateQuat(const Vec3& vec, float angle, char axisOfRotation, bool inDegrees)
{
	util::math::Vec3 axis; // the 'axis of rotation'

	switch (axisOfRotation)
	{
		// rotation on the x-axis
	case 'X':
	case 'x':
		axis = Vec3(1.0F, 0, 0);
		break;

		// rotation on the y-axis
	case 'Y':
	case 'y':
		axis = Vec3(0, 1.0F, 0);
		break;

		// rotation on the z-axis
	case 'Z':
	case 'z':
		axis = Vec3(0, 0, 1.0F);
		break;
	}

	return util::math::rotateQuat(vec, angle, axis, true, inDegrees);
}

// rotates a 3D vector via quaternions in a provided direction.
// rotates a vector in the direction of 'direcOfRotation', with isNormalized determining whether the direction has been normalized or not.
util::math::Vec3 util::math::rotateQuat(const Vec3& vec, float angle, const Vec3& direcOfRotation, bool isNormalized, bool inDegrees)
{
	util::math::Vec3 axis; // the 'axis of rotation'

	// uses the (q)(p)(q^c) equation
	util::math::Quat q1; // (cos a/2)((sin a/2)v)
	util::math::Quat pv; // (0, vec)
	util::math::Quat qc; // the conjugate 

	util::math::Quat qf; // the final quanternion

	if (inDegrees) // converts the angle to radians
		angle = util::math::degreesToRadians(angle);

	// normalizes the direction of rotation if it hasn't been normalized.
	if (isNormalized)
		axis = direcOfRotation;
	else
		axis = direcOfRotation.getNormalized();

	q1 = Quat(std::cosf(angle / 2.0F), ((std::sinf(angle / 2.0F)) * (axis))); // (cos a/2)((sin a/2)v)
	pv = Quat(0.0F, vec); // the vector as a quaternion (0, vec)
	// qc = Quat(std::cosf(angle / 2.0F), ((std::sinf(angle / 2.0F)) * (-axis))); // (cos a/2)((sin a/2)(-v))
	qc = q1.getConjugate();

	qf = q1 * pv * qc; // calculation

	return util::math::Vec3(qf.x, qf.y, qf.z);
}

// rotate around the z-axis
util::math::Vec3 util::math::rotateQuatZ(const Vec3& vec, float angle, bool inDegrees) { return rotateQuat(vec, angle, 'z', inDegrees); }

// rotate around the x-axis
util::math::Vec3 util::math::rotateQuatX(const Vec3& vec, float angle, bool inDegrees) { return rotateQuat(vec, angle, 'x', inDegrees); }

// rotate around the y-axis
util::math::Vec3 util::math::rotateQuatY(const Vec3& vec, float angle, bool inDegrees) { return rotateQuat(vec, angle, 'y', inDegrees); }
