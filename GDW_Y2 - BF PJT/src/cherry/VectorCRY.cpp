// Vector Cherry - engine vector classes
#include "VectorCRY.h"
#include "utils/math/Interpolation.h"

// VECTOR 2 //
// constructor
cherry::Vec2::Vec2() : v(util::math::Vec2()), x(&v.x), y(&v.y) {}

// constructor
cherry::Vec2::Vec2(float x, float y) : v(util::math::Vec2(x, y)), x(&v.x), y(&v.y) {}

// converts from util::math::vec2 to cherry::vec2
cherry::Vec2::Vec2(util::math::Vec2 vec) : v(util::math::Vec2(vec)), x(&v.x), y(&v.y) {}

// converts a glm vector to a cherry::vector
cherry::Vec2::Vec2(glm::vec2 vec) : v(util::math::Vec2(vec.x, vec.y)), x(&v.x), y(&v.y) {}

// copy constructor
cherry::Vec2::Vec2(const Vec2& v2) : v(util::math::Vec2()), x(&v.x), y(&v.y) { v = v2.v; }

// gets the x-value
float cherry::Vec2::GetX() const { return v.x; }

// sets the x-value
void cherry::Vec2::SetX(float x) { v.x = x; }

// gets the y-value
float cherry::Vec2::GetY() const { return v.y; }

// sets the y-value
void cherry::Vec2::SetY(float y) { v.y = y; }


// operators
// operator [] (reading)
const float& cherry::Vec2::operator[](const int index) const { return v[index]; }

// operator [] (writing)
float& cherry::Vec2::operator[](const int index) { return v[index]; }

// equals operator
cherry::Vec2 cherry::Vec2::operator=(const Vec2& v2) { return (v = v2.v); }

// checking if two vectors are equal
bool cherry::Vec2::operator==(const Vec2& v2) const { return v == v2.v; }

// checking if two vectors are not equal
bool cherry::Vec2::operator!=(const Vec2& v2) const { return v != v2.v; }

// multiplication
cherry::Vec2 cherry::Vec2::operator*(float scalar) const { return cherry::Vec2(v * scalar); }

// scalar times vector
cherry::Vec2 cherry::operator*(float flt, const Vec2& vec) { return vec * flt; }

// division
cherry::Vec2 cherry::Vec2::operator/(float scalar) const { return cherry::Vec2(v / scalar); }

// addition
cherry::Vec2 cherry::Vec2::operator+(Vec2 vec) const { return cherry::Vec2(v + vec.v); }

// subtraction of vectors
cherry::Vec2 cherry::Vec2::operator-(Vec2 vec) const { return cherry::Vec2(v - vec.v); }

// negation of vector
cherry::Vec2 cherry::Vec2::operator-() const { return 0.0f - *this; }

// float minus vector
cherry::Vec2 cherry::operator-(const float a, const cherry::Vec2 b) { return a - b; }

// equals operators
// addition
cherry::Vec2 cherry::Vec2::operator+=(cherry::Vec2 v3) { return *this = *this + v3; }

// subtraction
cherry::Vec2 cherry::Vec2::operator-=(cherry::Vec2 v3) { return *this = *this - v3; }

// multiplication
cherry::Vec2 cherry::Vec2::operator*=(float scalar) { return *this = *this * scalar; }

// division
cherry::Vec2 cherry::Vec2::operator/=(float scalar) { return *this = *this / scalar; }

// gets the length of the vector.
float cherry::Vec2::GetLength() const { return v.length(); }

// normalizes the vector
void cherry::Vec2::Normalize() { v.normalize(); }

// gets a normalized version of the vector
cherry::Vec2 cherry::Vec2::GetNormalized() const { return v.getNormalized(); }

// lerp for vector 2
cherry::Vec2 cherry::Vec2::Lerp(const cherry::Vec2 & a, const cherry::Vec2 & b, float t) { return Vec2(util::math::lerp(a.v, b.v, t)); }

// toString
std::string cherry::Vec2::ToString() const { return v.toString(); }


// VECTOR 3 //
// constructor
cherry::Vec3::Vec3() : v(util::math::Vec3()), x(&v.x), y(&v.y), z(&v.z) {}

// constructor
cherry::Vec3::Vec3(float x, float y, float z) : v(util::math::Vec3(x, y, z)), x(&v.x), y(&v.y), z(&v.z) {}

// converts from util::math::vec3 to cherry::vec3
cherry::Vec3::Vec3(util::math::Vec3 vec) : v(util::math::Vec3(vec)), x(&v.x), y(&v.y), z(&v.z) {}

// converts a glm vector to a cherry::vector
cherry::Vec3::Vec3(glm::vec3 vec) : v(util::math::Vec3(vec.x, vec.y, vec.z)), x(&v.x), y(&v.y), z(&v.z) {}

// copy constructor
cherry::Vec3::Vec3(const Vec3& vec) : v(util::math::Vec3()), x(&v.x), y(&v.y), z(&v.z) { v = vec.v; }

// gets the x-value
float cherry::Vec3::GetX() const { return v.x; }

// sets the x-value
void cherry::Vec3::SetX(float x) { v.x = x; }

// gets the y-value
float cherry::Vec3::GetY() const { return v.y; }

// sets the y-value
void cherry::Vec3::SetY(float y) { v.y = y; }

// gets the z-value
float cherry::Vec3::GetZ() const { return v.z; }

// sets the z-value
void cherry::Vec3::SetZ(float z) { v.z = z; }

// operators
// operator [] (reading)
const float& cherry::Vec3::operator[](const int index) const { return v[index]; }

// operator [] (editing)
float& cherry::Vec3::operator[](const int index) { return v[index]; }

// equality operator
cherry::Vec3 cherry::Vec3::operator=(const Vec3& vec) { return v = vec.v; }

// checking if two vectors are equal
bool cherry::Vec3::operator==(const Vec3& v2) const { return v == v2.v; }

// checking if two vectors are not equal
bool cherry::Vec3::operator!=(const Vec3& v2) const { return v != v2.v; }


// multiplication
cherry::Vec3 cherry::Vec3::operator*(float scalar) const { return cherry::Vec3(v * scalar); }

// scalar times vector
cherry::Vec3 cherry::operator*(float flt, const Vec3& vec) { return vec * flt; }

// division
cherry::Vec3 cherry::Vec3::operator/(float scalar) const { return v / scalar; }

// addition
cherry::Vec3 cherry::Vec3::operator+(Vec3 vec) const { return cherry::Vec3(v + vec.v); }

// subtraction of vectors
cherry::Vec3 cherry::Vec3::operator-(Vec3 vec) const { return cherry::Vec3(v - vec.v); }

// negation of vector
cherry::Vec3 cherry::Vec3::operator-() const { return 0.0f - *this; }

// float minus vector
cherry::Vec3 cherry::operator-(const float a, const cherry::Vec3 b) { return cherry::Vec3(a - b.v); }

// equals operators
// addition
cherry::Vec3 cherry::Vec3::operator+=(cherry::Vec3 v3) { return *this = *this + v3; }

// subtraction
cherry::Vec3 cherry::Vec3::operator-=(cherry::Vec3 v3) { return *this = *this - v3; }

// multiplication
cherry::Vec3 cherry::Vec3::operator*=(float scalar) { return *this = *this * scalar; }

// division
cherry::Vec3 cherry::Vec3::operator/=(float scalar) { return *this = *this / scalar; }

// gets the length of the vector.
float cherry::Vec3::GetLength() const { return v.length(); }

// normalizes the vector
void cherry::Vec3::Normalize() { v.normalize(); }

// gets a normalized verison of the vector
cherry::Vec3 cherry::Vec3::GetNormalized() const { return v.getNormalized(); }

// lerp for a vector 3.
cherry::Vec3 cherry::Vec3::Lerp(const Vec3 & a, const Vec3 & b, float t) { return Vec3(util::math::lerp(a.v, b.v, t)); }


// toString
std::string cherry::Vec3::ToString() const { return v.toString(); }


// VECTOR 4 //
// constructor
cherry::Vec4::Vec4() : v(util::math::Vec4()), x(&v.x), y(&v.y), z(&v.z), w(&v.w) {}

// constructor
cherry::Vec4::Vec4(float x, float y, float z, float w) : v(util::math::Vec4(x, y, z, w)), x(&v.x), y(&v.y), z(&v.z), w(&v.w) {}

// converts from util::math::vec4 to cherry::vec4
cherry::Vec4::Vec4(util::math::Vec4 vec) : v(util::math::Vec4(vec)), x(&v.x), y(&v.y), z(&v.z), w(&v.w) {}

// converts from util::glm::vec4 to cherry::vec4
cherry::Vec4::Vec4(glm::vec4 vec) : v(util::math::Vec4(vec.x, vec.y, vec.z, vec.w)), x(&v.x), y(&v.y), z(&v.z), w(&v.w) {}

// copy constructor
cherry::Vec4::Vec4(const Vec4& vec) : v(util::math::Vec4()), x(&v.x), y(&v.y), z(&v.z), w(&v.w) { v = vec.v; }

// gets the x-value
float cherry::Vec4::GetX() const { return v.x; }

// sets the x-value
void cherry::Vec4::SetX(float x) { v.x = x; }

// gets the y-value
float cherry::Vec4::GetY() const { return v.y; }

// sets the y-value
void cherry::Vec4::SetY(float y) { v.y = y; }

// gets the z-value
float cherry::Vec4::GetZ() const { return v.z; }

// sets the z-value
void cherry::Vec4::SetZ(float z) { v.z = z; }

// gets the w-value
float cherry::Vec4::GetW() const { return v.w; }

// sets the w-value
void cherry::Vec4::SetW(float w) { v.w = w; }

// operators
// operator [] (reading)
const float& cherry::Vec4::operator[](const int index) const { return v[index]; }

// operator [] (editing)
float& cherry::Vec4::operator[](const int index) { return v[index]; }

// =operator
cherry::Vec4 cherry::Vec4::operator=(const Vec4& vec) { return (v = vec.v); }

// checking if two vectors are equal
bool cherry::Vec4::operator==(const Vec4& v2) const { return v == v2.v; }

// checking if two vectors are not equal
bool cherry::Vec4::operator!=(const Vec4& v2) const { return v != v2.v; }

// multiplication
cherry::Vec4 cherry::Vec4::operator*(float scalar) const { return v * scalar; }

// scalar times vector
cherry::Vec4 cherry::operator*(float flt, const Vec4& vec) { return vec * flt; }

// division
cherry::Vec4 cherry::Vec4::operator/(float scalar) const { return cherry::Vec4(v / scalar); }

// addition
cherry::Vec4 cherry::Vec4::operator+(Vec4 vec) const { return cherry::Vec4(v + vec.v); }

// subtraction of vectors
cherry::Vec4 cherry::Vec4::operator-(Vec4 vec) const { return cherry::Vec4(v - vec.v); }

// negation of vector
cherry::Vec4 cherry::Vec4::operator-() const { return 0.0f - *this; }

// float minus vector
cherry::Vec4 cherry::operator-(const float a, const cherry::Vec4 b) { return Vec4(a - b.v); }

// equals operators
// addition
cherry::Vec4 cherry::Vec4::operator+=(cherry::Vec4 v3) { return *this = *this + v3; }

// subtraction
cherry::Vec4 cherry::Vec4::operator-=(cherry::Vec4 v3) { return *this = *this - v3; }

// multiplication
cherry::Vec4 cherry::Vec4::operator*=(float scalar) { return *this = *this * scalar; }

// division
cherry::Vec4 cherry::Vec4::operator/=(float scalar) { return *this = *this / scalar; }

// gets the length of the vector.
float cherry::Vec4::GetLength() const { return v.length(); }

// normalizes the vector
void cherry::Vec4::Normalize() { v.normalize(); }

// gets a normalized verison of the vector
cherry::Vec4 cherry::Vec4::GetNormalized() const { return v.getNormalized(); }

// lerp for a vector 4.
cherry::Vec4 cherry::Vec4::Lerp(const cherry::Vec4& a, const cherry::Vec4& b, float t)
{
	// using the util::math version caused linker errors, so it's done manually instead.
	t = (t > 1.0F) ? 1.0F : (t < 0.0F) ? 0.0F : t;

	return ((1.0F - t) * a + t * b);
}

// toString
std::string cherry::Vec4::ToString() const { return v.toString(); }