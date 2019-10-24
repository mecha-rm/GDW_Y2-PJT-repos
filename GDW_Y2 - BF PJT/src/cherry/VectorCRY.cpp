// file for general physics calculation. WIP.
#include "VectorCRY.h"

// VECTOR 2 //
// constructor
cherry::Vec2::Vec2() : v(new util::math::Vec2()) {}

// constructor
cherry::Vec2::Vec2(float x, float y) : v(new util::math::Vec2(x, y)) {}

// converts from util::math::vec2 to cherry::vec2
cherry::Vec2::Vec2(util::math::Vec2 vec) : v(new util::math::Vec2(vec)) {}

// converts a glm vector to a cherry::vector
cherry::Vec2::Vec2(glm::vec2 vec) : v(new util::math::Vec2(vec.x, vec.y)) {}

// destructor - deletes pointer
cherry::Vec2::~Vec2() { delete v; }

// gets the x-value
float cherry::Vec2::getX() const { return v->x; }

// sets the x-value
void cherry::Vec2::setX(float x) { v->x = x; }

// gets the y-value
float cherry::Vec2::getY() const { return v->y; }

// sets the y-value
void cherry::Vec2::setY(float y) { v->y = y; }

// operator +
//cherry::Vec2 cherry::Vec2::operator+(Vec2& vec)  { return cherry::Vec2(*v + *vec.v); }

// operator -
//cherry::Vec2 cherry::Vec2::operator-(Vec2& vec) { return cherry::Vec2(*v - *vec.v); }


// VECTOR 3 //
// constructor
cherry::Vec3::Vec3() : v(new util::math::Vec3()) {}

// constructor
cherry::Vec3::Vec3(float x, float y, float z) : v(new util::math::Vec3(x, y, z)) {}

// converts from util::math::vec3 to cherry::vec3
cherry::Vec3::Vec3(util::math::Vec3 vec) : v(new util::math::Vec3(vec)) {}

// converts a glm vector to a cherry::vector
cherry::Vec3::Vec3(glm::vec3 vec) : v(new util::math::Vec3(vec.x, vec.y, vec.z)) {}

// destructor - deletes pointer (already deleted?)
cherry::Vec3::~Vec3() { /*delete v;*/ }

// gets the x-value
float cherry::Vec3::getX() const { return v->x; }

// sets the x-value
void cherry::Vec3::setX(float x) { v->x = x; }

// gets the y-value
float cherry::Vec3::getY() const { return v->y; }

// sets the y-value
void cherry::Vec3::setY(float y) { v->y = y; }

// gets the z-value
float cherry::Vec3::getZ() const { return v->z; }

// sets the z-value
void cherry::Vec3::setZ(float z) { v->z = z; }

// operator +
// cherry::Vec3 cherry::Vec3::operator+(Vec3& vec)  { return cherry::Vec3(*v + *vec.v); }

// operator -
// cherry::Vec3 cherry::Vec3::operator-(Vec3& vec) { return cherry::Vec3(*v - *vec.v); }


// VECTOR 4 //
// constructor
cherry::Vec4::Vec4() : v(new util::math::Vec4()) {}

// constructor
cherry::Vec4::Vec4(float x, float y, float z, float w) : v(new util::math::Vec4(x, y, z, w)) {}

// converts from util::math::vec4 to cherry::vec4
cherry::Vec4::Vec4(util::math::Vec4 vec) : v(new util::math::Vec4(vec)) {}

// converts from util::glm::vec4 to cherry::vec4
cherry::Vec4::Vec4(glm::vec4 vec) : v(new util::math::Vec4(vec.x, vec.y, vec.z, vec.w)) {}

// gets the x-value
float cherry::Vec4::getX() const { return v->x; }

// sets the x-value
void cherry::Vec4::setX(float x) { v->x = x; }

// gets the y-value
float cherry::Vec4::getY() const { return v->y; }

// sets the y-value
void cherry::Vec4::setY(float y) { v->y = y; }

// gets the z-value
float cherry::Vec4::getZ() const { return v->z; }

// sets the z-value
void cherry::Vec4::setZ(float z) { v->z = z; }

// gets the w-value
float cherry::Vec4::getW() const { return v->w; }

// sets the w-value
void cherry::Vec4::setW(float w) { v->w = w; }

// operator +
// cherry::Vec4 cherry::Vec4::operator+(Vec4& vec)  { return cherry::Vec4(*v + *vec.v); }

// operator -
// cherry::Vec4 cherry::Vec4::operator-(Vec4& vec) { return cherry::Vec4(*v - *vec.v); }