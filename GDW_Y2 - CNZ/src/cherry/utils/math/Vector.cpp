#include "Vector.h"
#include "Rotation.h"
#include "../Utils.h"

#include <math.h>

//// VECTOR //////////////////////////////////////////////////////////////////////////////////////////
// ostream operator
std::ostream& util::math::operator<<(std::ostream& os, const Vec& vec) { return os << vec.toString(); }

// calculates the length of the dot product
template<typename T>
float util::math::Vec::calculateDotLength(const T& v1, const T& v2, const bool projV2)
{
	return v1.dot(v2) / ((projV2) ? v2.length() : v1.length());
}

// calculates the angle of the dot product
template<typename T>
float util::math::Vec::calculateDotAngle(const T& v1, const T& v2, const bool inDegrees)
{
	float theta = std::acosf(v1.dot(v2) / (v1.length() * v2.length()));

	if (inDegrees) // if degrees was requested, a conversion occurs.
		theta *= (180 / 3.14159265358979323846);

	return theta;
}

//// VECTOR 2 ////////////////////////////////////////////////////////////////////////////////////////
util::math::Vec2::Vec2(float x, float y) : x(x), y(y) {}

// istream
std::istream& util::math::operator>>(std::istream& in, Vec2& vec)
{
	// in.ignore(); // We use 'ignore' because if the user follows the proper format, we get a bunch of characters we don't want to use.

	in >> vec.x >> vec.y;
	return in;
}


// Operators
const float& util::math::Vec2::operator[](const int index) const // reading
{
	// Since all of the variables are sequential in memory, offsetting the location of 'x' by the index will provide the other variables
	return(&x)[index]; // returning the location of 'x', offset by our index.
}

float& util::math::Vec2::operator[](const int index) // editing
{
	// Since all of the variables are sequential in memory, offsetting the location of 'x' by the index will provide the other variables
	return(&x)[index]; // returning the location of 'x', offset by our index.
}

// checking if two vectors are equal
bool util::math::Vec2::operator==(const Vec2& v2) const { return (x == v2.x && y == v2.y); }


// multiplication
util::math::Vec2 util::math::Vec2::operator*(float scalar) const { return util::math::Vec2(x * scalar, y * scalar); }

// scalar times vector
util::math::Vec2 util::math::operator*(float flt, const Vec2& vec) { return vec * flt; }

// division
util::math::Vec2 util::math::Vec2::operator/(float scalar) const { return *this * std::powf(scalar, -1); }

// addition
util::math::Vec2 util::math::Vec2::operator+(Vec2 vec) const { return util::math::Vec2(x + vec.x, y + vec.y); }

// subtraction of vectors
util::math::Vec2 util::math::Vec2::operator-(Vec2 vec) const { return util::math::Vec2(x - vec.x, y - vec.y); }

// negation of vector
util::math::Vec2 util::math::Vec2::operator-() const { return 0.0f - *this; }

// float minus vector
util::math::Vec2 util::math::operator-(const float a, const util::math::Vec2 b) { return Vec2(a - b.x, a - b.y); }

// equals operators
// addition
util::math::Vec2 util::math::Vec2::operator+=(util::math::Vec2 v3) { return *this = *this + v3; }

// subtraction
util::math::Vec2 util::math::Vec2::operator-=(util::math::Vec2 v3) { return *this = *this - v3; }

// multiplication
util::math::Vec2 util::math::Vec2::operator*=(float scalar) { return *this = *this * scalar; }

// division
util::math::Vec2 util::math::Vec2::operator/=(float scalar) { return *this = *this / scalar; }

// returns the vector the provided exponent
util::math::Vec2 util::math::Vec2::pow(float scl) const { return Vec2::pow(*this, scl); }

// returns vector to an exponent
util::math::Vec2 util::math::Vec2::pow(Vec2 vec, float scl) { return Vec2(powf(vec.x, scl), powf(vec.y, scl)); }

float util::math::Vec2::length() const
{
	return std::sqrt(std::powf(x, 2) + std::powf(y, 2));
}

// gets the vector, normalized
util::math::Vec2 util::math::Vec2::getNormalized() const { return *this / length(); }

// normalizes the vector, replacing its current values.
void util::math::Vec2::normalize() { *this = getNormalized(); }

// dot product
float util::math::Vec2::dot(Vec2 v2) const
{
	return x * v2.x + y * v2.y;
}

// calculates the length of the dot product. The value of 'projV2' determines what vector is being projected upon (true = v2, false = v1 (i.e. the object))
float util::math::Vec2::dotLength(const Vec2& v2, bool projV2)
{
	return Vec::calculateDotLength(*this, v2, projV2);
	//return dot(v2) / ((projV2) ? v2.length() : length());
}

// calculates the angle of the dot product
float util::math::Vec2::dotAngle(const Vec2& v2, bool inDegrees) const
{
	return Vec::calculateDotAngle(*this, v2, inDegrees);

	//float theta = acosf(dot(v2) / (length() * v2.length()));

	//if (inDegrees) // if degrees was requested, a conversion occurs.
	//	theta = util::math::radiansToDegrees(theta);

	//return theta;
}

// gets the vector as a string
std::string util::math::Vec2::toString() const { return "(" + std::to_string(x) + ", " + std::to_string(y) + ")"; }



//// VECTOR 3 ////////////////////////////////////////////////////////////////////////////////////////
// Consturctors

util::math::Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

// z is set to 0.0F by default.
util::math::Vec3::Vec3(Vec2 vec, float z) : x(vec.x), y(vec.y), z(z) {}

// istream
std::istream& util::math::operator>>(std::istream& in, Vec3& vec)
{
	in >> vec.x >> vec.y >> vec.z;
	return in;
}

// Operators
const float& util::math::Vec3::operator[](const int index) const // reading
{
	// Since all of the variables are sequential in memory, offsetting the location of 'x' by the index will provide the other variables
	return(&x)[index]; // returning the location of 'x', offset by our index.
}

float& util::math::Vec3::operator[](const int index) // editing
{
	// Since all of the variables are sequential in memory, offsetting the location of 'x' by the index will provide the other variables
	return(&x)[index]; // returning the location of 'x', offset by our index.
}

// vector equality
bool util::math::Vec3::operator==(const Vec3& v2) const { return (x == v2.x && y == v2.y && z == v2.z); }


// vector times scalar
util::math::Vec3 util::math::Vec3::operator*(float scalar) const { return Vec3(x * scalar, y * scalar, z * scalar); }

// scalar times vector
util::math::Vec3 util::math::operator*(float flt, const Vec3& vec) { return vec * flt; }

util::math::Vec3 util::math::Vec3::operator/(float scalar) const { return *this * std::powf(scalar, -1); }


util::math::Vec3 util::math::Vec3::operator+(Vec3 vec) const { return Vec3(x + vec.x, y + vec.y, z + vec.z); }

// subtraction of vectors
util::math::Vec3 util::math::Vec3::operator-(Vec3 vec) const { return Vec3(x - vec.x, y - vec.y, z - vec.z); }

// negation of vector
util::math::Vec3 util::math::Vec3::operator-() const { return 0.0f - *this; }

// float minus vector
util::math::Vec3 util::math::operator-(const float a, const util::math::Vec3 b) { return Vec3(a - b.x, a - b.y, a - b.z); }

util::math::Vec3 util::math::Vec3::operator*=(float scalar) { return *this = *this * scalar; }

util::math::Vec3 util::math::Vec3::operator/=(float scalar) { return *this = *this / scalar; }

util::math::Vec3 util::math::Vec3::operator+=(Vec3 v3) { return *this = *this + v3; }

util::math::Vec3 util::math::Vec3::operator-=(Vec3 v3) { return *this = *this - v3; }

// returns this vector to the exponent.
util::math::Vec3 util::math::Vec3::pow(float scl) { return pow(*this, scl); }

// returns vector to an exponent
util::math::Vec3 util::math::Vec3::pow(Vec3 vec, float scl) { return Vec3(powf(vec.x, scl), powf(vec.y, scl), powf(vec.z, scl)); }

// returns the length of a vec3
float util::math::Vec3::length() const { return std::sqrt(std::powf(x, 2) + std::powf(y, 2) + std::powf(z, 2)); }

// gets the vector in a normalized form
util::math::Vec3 util::math::Vec3::getNormalized() const { return *this / length(); }

// normalizes the vector
void util::math::Vec3::normalize() { *this = getNormalized(); }

// calculates the dot product of the vector
float util::math::Vec3::dot(util::math::Vec3 v3) const { return x * v3.x + y * v3.y + z * v3.z; }

// calculates the length of the dot product. The value of 'projV2' determines what vector is being projected upon (true = v2, false = v1 (i.e. the object))
float util::math::Vec3::dotLength(const Vec3& v2, bool projV2)
{
	return Vec::calculateDotLength(*this, v2, projV2);
	//return dot(v2) / ((projV2) ? v2.length() : length());
}

// calculates the angle of the dot product
float util::math::Vec3::dotAngle(const Vec3& v2, bool inDegrees) const
{
	return Vec::calculateDotAngle(*this, v2, inDegrees);

	//float theta = acosf(dot(v2) / (length() * v2.length()));

	//if (inDegrees) // if degrees was requested, a conversion occurs.
	//	theta = util::math::radiansToDegrees(theta);

	//return theta;
}

// cross product (re-uses static cross)
util::math::Vec3 util::math::Vec3::cross(const Vec3 v2, float i, float j, float k) const
{
	return util::math::Vec3::cross(*this, v2, i, j, k);
}

// cross product between two vectors.
util::math::Vec3 util::math::Vec3::cross(const Vec3 v1, const Vec3 v2, float i, float j, float k)
{
	// calculation
	// A X B
	// [AyBz - AzBy] * i
	// [AzBx - AxBz] * j
	// [AxBy - AyBx] * k

	return Vec3(
		(v1.y * v2.z - v1.z * v2.y) * i,
		(v1.z * v2.x - v1.x * v2.z) * j,
		(v1.x * v2.y - v1.y * v2.x) * k
	);
}

// Returned in the order of (x, y, z)
std::string util::math::Vec3::toString() const { return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")"; }



//// VECTOR 4 ////////////////////////////////////////////////////////////////////////////////////////
util::math::Vec4::Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

util::math::Vec4::Vec4(Vec3 vec, float w) : x(vec.x), y(vec.y), z(vec.z), w(w) {}

// istream
std::istream& util::math::operator>>(std::istream& in, Vec4& vec)
{
	in >> vec.x >> vec.y >> vec.z >> vec.w;
	return in;
}

const float& util::math::Vec4::operator[](const int index) const // reading
{
	// Since all of the variables are sequential in memory, offsetting the location of 'x' by the index will provide the other variables
	return(&x)[index]; // returning the location of 'x', offset by our index.
}

float& util::math::Vec4::operator[](const int index) // editing
{
	// Since all of the variables are sequential in memory, offsetting the location of 'x' by the index will provide the other variables
	return(&x)[index]; // returning the location of 'x', offset by our index.
}

// compatibility operator
bool util::math::Vec4::operator==(const Vec4& v2) const { return ((x == v2.x) && (y == v2.y) && (z == v2.z) && (w == v2.w)); }


// operations
// vector times scalar
util::math::Vec4 util::math::Vec4::operator*(float scalar) const { return Vec4(x * scalar, y * scalar, z * scalar, w * scalar); }

// scalar times vector
util::math::Vec4 util::math::operator*(float flt, const Vec4& vec) { return vec * flt; }

util::math::Vec4 util::math::Vec4::operator/(const float scalar) const { return *this * (std::powf(scalar, -1)); }

util::math::Vec4 util::math::Vec4::operator+(const Vec4& vec) const { return Vec4(x + vec.x, y + vec.y, z + vec.z, w + vec.w); }

// vector - vector
util::math::Vec4 util::math::Vec4::operator-(const Vec4& vec) const { return Vec4(x - vec.x, y - vec.y, z - vec.z, w - vec.w); }

// negation of vector
util::math::Vec4 util::math::Vec4::operator-() const { return 0.0f - *this; }

// float minus vector
util::math::Vec4 util::math::operator-(const float a, const util::math::Vec4 b) { return Vec4(a - b.x, a - b.y, a - b.z, a - b.w); }


util::math::Vec4 util::math::Vec4::operator+=(util::math::Vec4 vec4) { return *this = *this + vec4; }

util::math::Vec4 util::math::Vec4::operator-=(Vec4 vec4) { return *this = *this - vec4; }

util::math::Vec4 util::math::Vec4::operator*=(float scalar) { return *this = *this * scalar; }

util::math::Vec4 util::math::Vec4::operator/=(float scalar) { return *this = *this * scalar; }

// returns the vector to the provided exponet
util::math::Vec4 util::math::Vec4::pow(float scl) { return Vec4::pow(*this, scl); }

util::math::Vec4 util::math::Vec4::pow(Vec4 vec, float scl) { return Vec4(std::powf(vec.x, scl), std::powf(vec.y, scl), std::powf(vec.z, scl), std::powf(vec.w, scl)); }


float util::math::Vec4::length() const { return std::sqrt(powf(x, 2) + powf(y, 2) + powf(z, 2) + powf(w, 2)); }

util::math::Vec4 util::math::Vec4::getNormalized() const { return *this / length(); }

void util::math::Vec4::normalize() { *this = getNormalized(); }


// dot product
float util::math::Vec4::dot(Vec4 v2) const { return this->x * v2.x + this->y * v2.y + this->z * v2.z + this->w * v2.w; }

// calculates the length of the dot product. The value of 'projV2' determines what vector is being projected upon (true = v2, false = v1 (i.e. the object))
float util::math::Vec4::dotLength(const Vec4& v2, bool projV2)
{
	return Vec::calculateDotLength(*this, v2, projV2);
	// return dot(v2) / ((projV2) ? v2.length() : length());
}

// calculates the angle of the dot product
float util::math::Vec4::dotAngle(const Vec4& v2, bool inDegrees) const
{
	return Vec::calculateDotAngle(*this, v2, inDegrees);

	//float theta = acosf(dot(v2) / (length() * v2.length()));

	//if (inDegrees) // if degrees was requested, a conversion occurs.
	//	theta = util::math::radiansToDegrees(theta);

	//return theta;
}


// Returns values in the order of (x, y, z, w)
std::string util::math::Vec4::toString() const { return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ", " + std::to_string(w) + ")"; }

