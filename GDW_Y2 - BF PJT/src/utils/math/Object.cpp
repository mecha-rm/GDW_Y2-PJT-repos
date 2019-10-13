#include "Object.h"

// SHAPES //

util::math::Object::Object() : position() {}

// gets the x position
float util::math::Object::getPositionX() const { return position.x; }

// sets position x
void util::math::Object::setPositionX(float x) { position.x = x; }

// gets the y-position
float util::math::Object::getPositionY() const { return position.y; }

// sets the position on the y-axis
void util::math::Object::setPositionY(float y) { position.y = y; }

// translate on the x-axis
void util::math::Object::translateX(float xt) { position.x += xt; }

// translate on the y-axis
void util::math::Object::translateY(float yt) { position.y += yt; }

// SHAPE (2D) //

// constructor
util::math::Object2D::Object2D(const float SIDES) : SIDES(SIDES) {}

// gets the position
util::math::Vec2 util::math::Object2D::getPosition() const { return util::math::Vec2(position.x, position.y); }

// sets the position 
void util::math::Object2D::setPosition(util::math::Vec2 pos) { position = util::math::Vec3(pos, position.z); }

// set the position (individual)
void util::math::Object2D::setPosition(float x, float y) { setPosition(util::math::Vec2(x, y)); }

// moves the shape using a vector
void util::math::Object2D::translate(util::math::Vec2 translation) { position += translation; }

// moves the shape
void util::math::Object2D::translate(float xt, float yt) { translate(util::math::Vec2(xt, yt)); }

// toString() function
std::string util::math::Object2D::toString()
{
	return util::math::Vec2(position.x, position.y).toString();
}


// 3D Shape //

// constructor
util::math::Object3D::Object3D(const unsigned int FACES, const unsigned int EDGES, const unsigned int VERTICES)
	: FACES(FACES), EDGES(EDGES), VERTICES(VERTICES) {}

// gets te position of the 3D object
util::math::Vec3 util::math::Object3D::getPosition() const { return position; }

// sets the position of the shape
void util::math::Object3D::setPosition(util::math::Vec3 pos) { position = pos; }

// setting the position
void util::math::Object3D::setPosition(float x, float y, float z) { setPosition(util::math::Vec3(x, y, z)); }

// get position z
float util::math::Object3D::getPositionZ() const { return position.z; }

// sets the z-position
void util::math::Object3D::setPositionZ(float z) { position.z = z; }

// moves the shape
void util::math::Object3D::translate(util::math::Vec3 translate) { position += translate; }

// translates the shape
void util::math::Object3D::translate(float xt, float yt, float zt) { translate(util::math::Vec3(xt, yt, zt)); }

// translate along the z-axis
void util::math::Object3D::translateZ(float zt) { position.z += zt; }

// toString() for object.
std::string util::math::Object3D::toString()
{
	return position.toString();
}
