#include "Quaternion.h"

// constructors
util::math::Quat::Quat() : w(0.0F), x(0.0F), y(0.0F), z(0.0F) {}

util::math::Quat::Quat(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) {}

util::math::Quat::Quat(float w, Vec3 vec3) : w(w), x(vec3.x), y(vec3.y), z(vec3.z) {}

util::math::Quat::Quat(Vec4 vec4) : w(vec4.w), x(vec4.x), y(vec4.y), z(vec4.z) {}

// ostream operation
std::ostream & util::math::operator<<(std::ostream & os, const math::Quat & quad) { return os << quad.toString(); }

//istream operator
std::istream & util::math::operator>>(std::istream & in, math::Quat & quad)
{
	in >> quad.w >> quad.x >> quad.y >> quad.z;
	return in;
}

// reading
const float & util::math::Quat::operator[](const int index) const { return(&w)[index]; }

// writing
float & util::math::Quat::operator[](const int index) { return(&w)[index]; }

// Assumes (i, j, k) are all equal to '1'.
util::math::Quat util::math::Quat::operator*(const Quat & quad) const
{
	return multiply(quad, 1.0F, 1.0F, 1.0F);
	/*return Quat(
		(this->w * quad.w - this->x * quad.x - this->y * quad.y - this->z * quad.z),
		(this->w * quad.x + this->x * quad.w + this->y * quad.z - this->z * quad.y),
		(this->w * quad.y - this->x * quad.z + this->y * quad.w + this->z * quad.x),
		(this->w * quad.z + this->x * quad.y - this->y * quad.x + this->z + quad.w)
	);*/
}

// Assumes (i, j, k) are all equal to '1'.
void util::math::Quat::operator*=(const Quat & quad) { *this = *this * quad; }

// gets the conjugate of the quaternion
util::math::Quat util::math::Quat::getConjugate() const { return Quat(w, -x, -y, -z); }

// makes the quaternion its conjugate.
void util::math::Quat::conjugate() { *this = getConjugate(); }

// multiplies the quaternion in the object by the provided quaternion
util::math::Quat util::math::Quat::multiply(const Quat & quad, const float I, const float J, const float K) const { return multiply(*this, quad, I, J, K); }

// multiplies two quaternions
util::math::Quat util::math::Quat::multiply(const Quat & quad1,const Quat & quad2, const float I, const float J, const float K)
{
	return Quat(
		((quad1.w * quad2.w) - (quad1.x * quad2.x) - (quad1.y * quad2.y) - (quad1.z * quad2.z)),
		((quad1.w * quad2.x) + (quad1.x * quad2.w) + (quad1.y * quad2.z) - (quad1.z * quad2.y)) * I,
		((quad1.w * quad2.y) - (quad1.x * quad2.z) + (quad1.y * quad2.w) + (quad1.z * quad2.x)) * J,
		((quad1.w * quad2.z) + (quad1.x * quad2.y) - (quad1.y * quad2.x) + (quad1.z * quad2.w)) * K
	);
}

std::string util::math::Quat::toString() const { return "(" + std::to_string(w) + ", " + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")"; }