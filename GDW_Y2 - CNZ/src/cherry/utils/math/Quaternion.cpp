#include "Quaternion.h"

// constructors
util::math::Quat::Quat() : w(0.0F), x(0.0F), y(0.0F), z(0.0F) {}

util::math::Quat::Quat(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) {}

util::math::Quat::Quat(float w, Vec3 vec3) : w(w), x(vec3.x), y(vec3.y), z(vec3.z) {}

util::math::Quat::Quat(Vec4 vec4) : w(vec4.w), x(vec4.x), y(vec4.y), z(vec4.z) {}

// ostream operation
// std::ostream & util::math::operator<<(std::ostream & os, const math::Quat & quat) { return os << quat.toString(); }

//istream operator
// std::istream & util::math::operator>>(std::istream & in, math::Quat & quat)
// {
// 	in >> quat.w >> quat.x >> quat.y >> quat.z;
// 	return in;
// }

// reading
const float & util::math::Quat::operator[](const int index) const { return(&w)[index]; }

// writing
float & util::math::Quat::operator[](const int index) { return(&w)[index]; }

// Assumes (i, j, k) are all equal to '1'.
util::math::Quat util::math::Quat::operator*(const Quat & quat) const
{
	return multiply(quat, 1.0F, 1.0F, 1.0F);
	/*return Quat(
		(this->w * quat.w - this->x * quat.x - this->y * quat.y - this->z * quat.z),
		(this->w * quat.x + this->x * quat.w + this->y * quat.z - this->z * quat.y),
		(this->w * quat.y - this->x * quat.z + this->y * quat.w + this->z * quat.x),
		(this->w * quat.z + this->x * quat.y - this->y * quat.x + this->z + quat.w)
	);*/
}

// Assumes (i, j, k) are all equal to '1'.
void util::math::Quat::operator*=(const Quat & quat) { *this = *this * quat; }

// gets the conjugate of the quaternion
util::math::Quat util::math::Quat::getConjugate() const { return Quat(w, -x, -y, -z); }

// makes the quaternion its conjugate.
void util::math::Quat::conjugate() { *this = getConjugate(); }

// multiplies the quaternion in the object by the provided quaternion
util::math::Quat util::math::Quat::multiply(const Quat & quat, const float I, const float J, const float K) const { return multiply(*this, quat, I, J, K); }

// multiplies two quaternions
util::math::Quat util::math::Quat::multiply(const Quat & quat1,const Quat & quat2, const float I, const float J, const float K)
{
	return Quat(
		((quat1.w * quat2.w) - (quat1.x * quat2.x) - (quat1.y * quat2.y) - (quat1.z * quat2.z)),
		((quat1.w * quat2.x) + (quat1.x * quat2.w) + (quat1.y * quat2.z) - (quat1.z * quat2.y)) * I,
		((quat1.w * quat2.y) - (quat1.x * quat2.z) + (quat1.y * quat2.w) + (quat1.z * quat2.x)) * J,
		((quat1.w * quat2.z) + (quat1.x * quat2.y) - (quat1.y * quat2.x) + (quat1.z * quat2.w)) * K
	);
}

std::string util::math::Quat::toString() const { return "(" + std::to_string(w) + ", " + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")"; }