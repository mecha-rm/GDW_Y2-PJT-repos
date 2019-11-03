#pragma once
#include "VectorCRY.h"
#include "objects/Primitives.h"
// #include "objects/Object.h"

namespace cherry
{
	static float gravity = -9.8F;

	// the object for the physics body
	class PhysicsBody
	{
	public:
		// constructor
		// PhysicsBody();
		
		// location of the physics body (relative to object origin)
		// PhysicsBody(float x, float y, float z);

		// location of the physics body (relative to object origin)
		// PhysicsBody(cherry::Vec3 pos);

		// location of the physics body (relative to object origin)
		// PhysicsBody(glm::vec3 position);

		// sets the id for a specific physics body.
		PhysicsBody(int id);

		// sets the id for a specific physics body, and its position.
		PhysicsBody(int id, cherry::Vec3 pos);

		// ~PhysicsBody();

		/*
		 * gets the identifier for the physics body
		 *** (0): no specifier
		 *** (1): box
		 *** (2): sphere
		*/
		int getId() const;

		// gets the object this physics body is attachted to
		cherry::Object* getObject() const;

		// sets the object the physics body is attachted to
		void setObject(cherry::Object * obj);

		// attachs the physics body to an object, returning itself.
		// NOTE: this does not save the body to the object's list.
		cherry::PhysicsBody* attachToObject(cherry::Object * newObj);

		// gets the model position
		glm::vec3 getModelPositionGLM() const;

		// gets the model position
		cherry::Vec3 getModelPosition() const;

		// sets the model position
		void setModelPosition(cherry::Vec3 mpos);

		// set model position (GLM version)
		void setModelPosition(glm::vec3 mpos);


		// get the world position as a glm vector
		glm::vec3 getWorldPositionGLM() const;

		// gets the world position
		// if no object is attachted, then the model position is returned.
		cherry::Vec3 getWorldPosition() const;

		// sets the world position
		// if no object is attachted, then the model position is returned.
		void setWorldPosition(cherry::Vec3 wpos);

		// sets the world position
		void setWorldPosition(glm::vec3 wpos);

		// virtual void GetMesh() = 0;

		// bool getVisible();
		
		// void setVisible();
		
		// calculates collision between two physics bodies if is available.
		static bool Collision(PhysicsBody* p1, PhysicsBody* p2);

		// the toString function for physics bodies. This is basically here just so that this is an abstract class.
		virtual std::string toString() const = 0;

	private:
		cherry::Vec3 position; // position
		
		int id = 0; // identifier

		// doesn't get deleted since the object it's attachted to isn't necessarily unused.
		cherry::Object * object = nullptr; // the object the body is attachted to.

	protected:
	};

	// the object for a rectange physics body
	typedef class PhysicsBodyBox : public PhysicsBody
	{
	public:
		// position of (0, 0, 0), with dimensions
		PhysicsBodyBox(float width, float height, float depth);

		// location of the physics body (relative to object origin)
		// (x, y, z) = position
		// (width, height, depth) = size on (x, y, z)
		PhysicsBodyBox(float x, float y, float z, float width, float height, float depth);

		// location of the physics body (relative to object origin), and dimensions 
		PhysicsBodyBox(cherry::Vec3 position, float width, float height, float depth);

		// position and dimensions (width, height, depth)
		PhysicsBodyBox(cherry::Vec3 position, cherry::Vec3 dimensions);

		// gets the width (size on x-axis)
		float getWidth() const;

		// sets width
		void setWidth(float newWidth);

		// gets the height (size on y-axis)
		float getHeight() const;

		// sets height (size on y-axis)
		void setHeight(float newHeight);

		// gets the depth (size on z-axis)
		float getDepth() const;

		// sets depth (size on z-axis)
		void setDepth(float newDepth);

		// toString
		virtual std::string toString() const;

	private:
		float width = 0, height = 0, depth = 0;
		// PrimitiveCube box;

	protected:


	} BoxBody;

	// the class for the sphere physics body
	typedef class PhysicsBodySphere : public PhysicsBody
	{
	public:
		// radius of the sphere. The location is the object's origin.
		PhysicsBodySphere(float radius);

		// position of the sphere (relative to the object), and its radius.
		PhysicsBodySphere(cherry::Vec3 position, float radius);

		// gets the radius
		float getRadius() const;

		// sets the radius; if negative is passed, the absolute value is received.
		void setRadius(float r);

		// toString function
		std::string toString() const;

	private:
		float radius = 0.0F; // radius of the sphere.

	protected:
	} SphereBody;
}