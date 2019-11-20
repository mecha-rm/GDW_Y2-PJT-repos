#include "PhysicsBody.h"
#include "utils/math/Collision.h"

// body at object origin
// cherry::PhysicsBody::PhysicsBody() : position() {}

// sets position of body
// cherry::PhysicsBody::PhysicsBody(float x, float y, float z) : position(x, y, z) {}

// position of the body (passed as cherry::Vec3)
// cherry::PhysicsBody::PhysicsBody(cherry::Vec3 pos) : position(pos) {}

// sets the physics body ID
cherry::PhysicsBody::PhysicsBody(int id) : id(id), position(cherry::Vec3()) {}

// sets the ID for a specific type of physics body
cherry::PhysicsBody::PhysicsBody(int id, cherry::Vec3 pos) : id(id), position(pos) {}


/*
// gets the ID for the physics body
 *** (0): no specifier
 *** (1): box
 *** (2): sphere
*/
int cherry::PhysicsBody::getId() const { return id; }

// gets the object this physics body is attachted to
cherry::Object* cherry::PhysicsBody::getObject() const { return object; }

// sets the object
void cherry::PhysicsBody::setObject(cherry::Object* obj) { object = obj; }

// attaches to an object
cherry::PhysicsBody* cherry::PhysicsBody::attachToObject(cherry::Object* newObj)
{
	object = newObj;
	// newObj->AddPhysicsBody(this); // adds the physics body if it isn't there already
	return this;
}

// gets the model position (glm)
glm::vec3 cherry::PhysicsBody::getModelPositionGLM() const
{
	Vec3 mpos = getModelPosition();
	return glm::vec3(mpos.v.x, mpos.v.y, mpos.v.z);
}

// gets the model position
cherry::Vec3 cherry::PhysicsBody::getModelPosition() const { return position; }

// sets the model position
void cherry::PhysicsBody::setModelPosition(cherry::Vec3 mpos) { position = mpos; }

// sets the model position
void cherry::PhysicsBody::setModelPosition(glm::vec3 mpos) { position = cherry::Vec3(mpos); }

// gets the world position as a GLM vector
glm::vec3 cherry::PhysicsBody::getWorldPositionGLM() const
{
	Vec3 wpos = getWorldPosition();
	return glm::vec3(wpos.v.x, wpos.v.y, wpos.v.z);
}

// returns the world position; objects save their position in world space
cherry::Vec3 cherry::PhysicsBody::getWorldPosition() const 
{
	// TODO : change this to take a cherry::Vec3
	return (object == nullptr ? position : object->GetPosition() + position);
}

// sets the world position
void cherry::PhysicsBody::setWorldPosition(cherry::Vec3 wpos) { position = wpos; }

// sets the world position
void cherry::PhysicsBody::setWorldPosition(glm::vec3 wpos) { position = cherry::Vec3(wpos); }

// calculations collision between objects
bool cherry::PhysicsBody::Collision(PhysicsBody* p1, PhysicsBody* p2)
{
	// if either object is null.
	if (p1 == nullptr || p2 == nullptr)
		return false;

	// no physics body type attachted
	if (p1->getId() == 0 || p2->getId() == 0)
		return false;

	// AABB Collision
	if (p1->getId() == 1 && p2->getId() == 1)
	{
		cherry::PhysicsBodyBox * temp1 = (cherry::PhysicsBodyBox*)p1;
		cherry::PhysicsBodyBox * temp2 = (cherry::PhysicsBodyBox*)p2;

		//// origin is the centre of the 
		//// minimum values of A
		//util::math::Vec3 minA (
		//	temp1->getWorldPosition().GetX() - temp1->getWidth() / 2.0F, 
		//	temp1->getWorldPosition().GetY() - temp1->getHeight() / 2.0F, 
		//	temp1->getWorldPosition().GetZ() - temp1->getDepth() / 2.0F
		//);

		//// maximum values of A
		//util::math::Vec3 maxA(
		//	temp1->getWorldPosition().GetX() + temp1->getWidth() / 2.0F,
		//	temp1->getWorldPosition().GetY() + temp1->getHeight() / 2.0F,
		//	temp1->getWorldPosition().GetZ() + temp1->getDepth() / 2.0F
		//);

		//// minimum values of B
		//util::math::Vec3 minB(
		//	temp2->getWorldPosition().GetX() - temp2->getWidth() / 2.0F,
		//	temp2->getWorldPosition().GetY() - temp2->getHeight() / 2.0F,
		//	temp2->getWorldPosition().GetZ() - temp2->getDepth() / 2.0F
		//);

		//// maximum values of B
		//util::math::Vec3 maxB(
		//	temp2->getWorldPosition().GetX() + temp2->getWidth() / 2.0F,
		//	temp2->getWorldPosition().GetY() + temp2->getHeight() / 2.0F,
		//	temp2->getWorldPosition().GetZ() + temp2->getDepth() / 2.0F
		//);

		//return util::math::aabbCollision(minA, maxA, minB, maxB);

		return util::math::aabbCollision(temp1->getWorldPosition().v, temp1->getWidth(), temp1->getHeight(), temp1->getDepth(),
										 temp2->getWorldPosition().v, temp2->getWidth(), temp2->getHeight(), temp2->getDepth());
	}
	// Sphere Collision
	else if (p1->getId() == 2 && p2->getId() == 2)
	{
		// converts to spehre
		cherry::PhysicsBodySphere* temp1 = (cherry::PhysicsBodySphere*)p1;
		cherry::PhysicsBodySphere* temp2 = (cherry::PhysicsBodySphere*)p2;

		return util::math::sphereCollision(temp1->getWorldPosition().v, temp1->getRadius(), temp2->getWorldPosition().v, temp2->getRadius());
	}

	// AABB - Sphere Collision
	else if ((p1->getId() == 1 && p2->getId() == 2) || (p1->getId() == 2 && p2->getId() == 1))
	{
		// if the first object is an aabb,  meaning that the other object is a sphere
		if (p1->getId() == 1) // calls the collision function again, but swaps the variables
		{
			return Collision(p2, p1);
		}

		// p1 is a sphere, p2 is an aabb
		cherry::PhysicsBodySphere* temp1 = (cherry::PhysicsBodySphere*)p1;
		cherry::PhysicsBodyBox* temp2 = (cherry::PhysicsBodyBox*)p2;

		return util::math::sphereAABBCollision(temp1->getWorldPosition().v, temp1->getRadius(),
			temp2->getWorldPosition().v, temp2->getWidth(), temp2->getHeight(), temp2->getDepth());
	}

	return false;
}

// PHYSICS BODY BOX
// constructor
cherry::PhysicsBodyBox::PhysicsBodyBox(float width, float height, float depth) : 
	PhysicsBodyBox(cherry::Vec3(), width, height, depth)
{}

// sets position
cherry::PhysicsBodyBox::PhysicsBodyBox(float x, float y, float z, float width, float height, float depth) 
	: PhysicsBodyBox(cherry::Vec3(x, y, z), width, height, depth)
{}

// location of the physics body (relative to object origin), and dimensions
cherry::PhysicsBodyBox::PhysicsBodyBox(cherry::Vec3 position, float width, float height, float depth)
	: PhysicsBody(1, position), width(abs(width)), height(abs(height)), depth(abs(depth))// , box(width, height, depth)
{
	// box.setColor(255, 0, 0); // RED
}

// location of the physics body and its dimensions
cherry::PhysicsBodyBox::PhysicsBodyBox(cherry::Vec3 position, cherry::Vec3 dimensions)
	: PhysicsBodyBox(position, dimensions.v.x, dimensions.v.y, dimensions.v.z)
{}

// gets the width
float cherry::PhysicsBodyBox::getWidth() const { return width; }

// sets the width
void cherry::PhysicsBodyBox::setWidth(float newWidth) { width = newWidth; }

// returns the height
float cherry::PhysicsBodyBox::getHeight() const { return height; }

// sets the height
void cherry::PhysicsBodyBox::setHeight(float newHeight) { height = newHeight; }

// returns depth
float cherry::PhysicsBodyBox::getDepth() const { return depth; }

// sets the depth
void cherry::PhysicsBodyBox::setDepth(float newDepth) { depth = newDepth; }

// toString
std::string cherry::PhysicsBodyBox::toString() const
{
	return "Model Position: " + getModelPosition().v.toString() +
		" | Width: " + std::to_string(width) +
		" | Height: " + std::to_string(height) +
		" | Depth: " + std::to_string(depth);
}

// Physics Body Sphere

// constructor
cherry::PhysicsBodySphere::PhysicsBodySphere(float radius) : PhysicsBody(2), radius(radius) {}

// constructor (with position)
cherry::PhysicsBodySphere::PhysicsBodySphere(cherry::Vec3 position, float radius)
	: PhysicsBody(2, position), radius(radius) {}

// gets the radius
float cherry::PhysicsBodySphere::getRadius() const { return radius; }

void cherry::PhysicsBodySphere::setRadius(float r) { radius = r; }

// toString function
std::string cherry::PhysicsBodySphere::toString() const
{
	return "Model Position: " + getModelPosition().ToString() + " | Radius: " + std::to_string(radius);
}
