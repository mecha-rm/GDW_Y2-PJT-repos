/*
 * Author: Roderick "R.J." Montague (100701758)
 * Date: 05/01/2019
 * Description: a project that contains a quaternion class
*/
#include "Vector.h"
#include <string>

#ifndef QUATERNION_H
#define QUATERNION_H

namespace util
{
	namespace math
	{
		typedef class Quat
		{
		public:
			Quat();

			// (qw, qx, qy, qz) or alternatively, (q0, q1, q2, q3)
			Quat(float w, float x, float y, float z);

			Quat(float w, Vec3 vec3);

			// Values are set based on the letter, not on the order listed. So the 'w' in the Vec4 will be the 'w' in this quaternion.
			Quat(Vec4 vec4);

			// ostream operation
			friend std::ostream& operator<<(std::ostream& os, const Quat& quat) // << operator
			{
				return os << quat.toString();
			}

			// istream operator
			friend std::istream& operator>>(std::istream& in, math::Quat& quat) // << operator
			{
				in >> quat.w >> quat.x >> quat.y >> quat.z;
				return in;
			}

			const float& operator[](const int index) const; // value reading
			float& operator[](const int index); // value editing

			// Assumes (i, j, k) are all equal to '1'.
			Quat operator*(const Quat& quat) const; // multiplication of quaternion

			// Assumes (i, j, k) are all equal to '1'.
			void operator*=(const Quat& quat);

			// Returns the conjugate of the quaternion, which multiplies (x, y, z) by -1.
			Quat getConjugate() const;

			// makes the quaternion its conjugate.
			void conjugate();

			Quat multiply(const Quat& quat, const float I = 1.0F, const float J = 1.0F, const float K = 1.0F) const; // multiplies a quaternion with the quaternion in the object.

			static Quat multiply(const Quat& quat1, const Quat& quat2, const float I = 1.0F, const float J = 1.0F, const float K = 1.0F); // multiplies two quaternions

			// the toString() function of the quaternion
			std::string toString() const;

			float w = 0.0F, x = 0.0F, y = 0.0F, z = 0.0F;
			// float i = 1.0F, j = 1.0F, k = 1.0F;

		protected:

		private:

		} Quaternion;
	}
}

#endif // !QUATERNION_H

