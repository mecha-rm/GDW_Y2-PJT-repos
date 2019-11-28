#pragma once
#include "VectorCRY.h"
#include "objects/Primitives.h"
// #include "objects/Object.h"

namespace cherry
{
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

		~PhysicsBody();

		/*
		 * gets the identifier for the physics body
		 *** (0): no specifier
		 *** (1): box
		 *** (2): sphere
		*/
		int GetId() const;

		// gets the object this physics body is attachted to
		cherry::Object* GetObject() const;

		// sets the object the physics body is attachted to
		void SetObject(cherry::Object * obj);

		// attachs the physics body to an object, returning itself.
		// NOTE: this does not save the body to the object's list.
		cherry::PhysicsBody* AttachToObject(cherry::Object * newObj);

		// gets the model position
		glm::vec3 GetModelPositionGLM() const;

		// gets the model position
		cherry::Vec3 GetModelPosition() const;

		// sets the model position
		void SetModelPosition(cherry::Vec3 mpos);

		// set model position (GLM version)
		void SetModelPosition(glm::vec3 mpos);


		// get the world position as a glm vector
		glm::vec3 GetWorldPositionGLM() const;

		// gets the world position
		// if no object is attachted, then the model position is returned.
		cherry::Vec3 GetWorldPosition() const;

		// sets the world position
		// if no object is attachted, then the model position is returned.
		void SetWorldPosition(cherry::Vec3 wpos);

		// sets the world position
		void SetWorldPosition(glm::vec3 wpos);


		// gets the rotation of the body
		cherry::Vec3 GetRotationDegrees() const;

		// sets the rotation in degrees
		void SetRotationDegrees(Vec3 degrees);
		
		// gets rotation in radians
		cherry::Vec3 GetRotationRadians() const;

		// sets rotation in radians
		void SetRotationRadians(Vec3 radians);


		// gets the scale of the body
		cherry::Vec3 GetScale() const;

		// sets the scale of the body.
		void SetScale(cherry::Vec3 newScale);

		// virtual void GetMesh() = 0;

		// bool getVisible();
		
		// void setVisible();
		
		// calculates collision between two physics bodies if is available.
		static bool Collision(PhysicsBody* p1, PhysicsBody* p2);

		// states whether the body is visible.
		bool IsVisible() const;

		// toggle visibility
		void SetVisible();

		// sets visibility
		void SetVisible(bool visible);

		// update loop for physics bodies
		virtual void Update(float deltaTime);

		// the toString function for physics bodies. This is basically here just so that this is an abstract class.
		virtual std::string ToString() const = 0;

	private:
		int id = 0; // identifier

		// bool visible = false; // checks for visibility in reference to if the object is visible.

		// rotation
		cherry::Vec3 rotation;

	protected:
		// the colour of the physics bodies (RGBA)
		const Vec4 COLOUR{ 0.9F, 0.1F, 0.1F, 0.45F };

		// doesn't get deleted since the object it's attachted to isn't necessarily unused.
		cherry::Object* object = nullptr; // the object the body is attachted to.

		// a matrix that holds the node's transformation to the parent.
		// glm::mat4 nodeParentTransform;

		cherry::Vec3 position; // position

		// used to draw the body
		cherry::Primitive* body = nullptr;

		cherry::Vec3 scale{ 1.0F, 1.0F, 1.0F };

		// material for primitives
		Material::Sptr material;
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
		float GetWidth() const;

		// sets width
		void SetWidth(float newWidth);

		// gets the height (size on y-axis)
		float GetHeight() const;

		// sets height (size on y-axis)
		void SetHeight(float newHeight);

		// gets the depth (size on z-axis)
		float GetDepth() const;

		// sets depth (size on z-axis)
		void SetDepth(float newDepth);

		void Update(float deltaTime);

		// toString
		virtual std::string ToString() const;

	private:
		float width = 0, height = 0, depth = 0;

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
		float GetRadius() const;

		// sets the radius; if negative is passed, the absolute value is received.
		void SetRadius(float r);

		void Update(float deltaTime);

		// ToString function
		std::string ToString() const;

	private:
		float radius = 0.0F; // radius of the sphere.

	protected:
	} SphereBody;
}