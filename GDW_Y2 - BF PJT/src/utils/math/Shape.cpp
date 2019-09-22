#include "Shape.h"

// SHAPES //

util::math::Shape::Shape() : position() {}

// gets the x position
float util::math::Shape::getPositionX() const { return position.x; }

// sets position x
void util::math::Shape::setPositionX(float x) { position.x = x; }

// gets the y-position
float util::math::Shape::getPositionY() const { return position.y; }

// sets the position on the y-axis
void util::math::Shape::setPositionY(float y) { position.y = y; }

// translate on the x-axis
void util::math::Shape::translateX(float xt) { position.x += xt; }

// translate on the y-axis
void util::math::Shape::translateY(float yt) { position.y += yt; }

// SHAPE (2D) //

// constructor
util::math::Shape2D::Shape2D(const float SIDES) : SIDES(SIDES) {}

// gets the position
util::math::Vec2 util::math::Shape2D::getPosition() const { return util::math::Vec2(position.x, position.y); }

// sets the position 
void util::math::Shape2D::setPosition(util::math::Vec2 pos) { position = util::math::Vec3(pos, position.z); }

// set the position (individual)
void util::math::Shape2D::setPosition(float x, float y) { setPosition(util::math::Vec2(x, y)); }

// moves the shape using a vector
void util::math::Shape2D::translate(util::math::Vec2 translation) { position += translation; }

// moves the shape
void util::math::Shape2D::translate(float xt, float yt) { translate(util::math::Vec2(xt, yt)); }


// 3D Shape //

// constructor
util::math::Shape3D::Shape3D(const unsigned int FACES, const unsigned int EDGES, const unsigned int VERTICES)
	: FACES(FACES), EDGES(EDGES), VERTICES(VERTICES) {}

// gets te position of the 3D object
util::math::Vec3 util::math::Shape3D::getPosition() const { return position; }

// sets the position of the shape
void util::math::Shape3D::setPosition(util::math::Vec3 pos) { position = pos; }

// setting the position
void util::math::Shape3D::setPosition(float x, float y, float z) { setPosition(util::math::Vec3(x, y, z)); }

// get position z
float util::math::Shape3D::getPositionZ() const { return position.z; }

// sets the z-position
void util::math::Shape3D::setPositionZ(float z) { position.z = z; }

// moves the shape
void util::math::Shape3D::translate(util::math::Vec3 translate) { position += translate; }

// translates the shape
void util::math::Shape3D::translate(float xt, float yt, float zt) { translate(util::math::Vec3(xt, yt, zt)); }

// translate along the z-axis
void util::math::Shape3D::translateZ(float zt) { position.z += zt; }
