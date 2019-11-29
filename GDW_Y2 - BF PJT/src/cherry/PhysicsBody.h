// Physics Body - used to add physics related properties to an object.
#pragma once
#include "VectorCRY.h"
#include "objects/Primitives.h"

namespace cherry
{
	// abstract physics body class
	class PhysicsBody
	{
	public:
		// constructors
		// sets the id for a specific physics body.
		PhysicsBody(int id);

		// sets the id for a specific physics body, and its position.
		PhysicsBody(int id, cherry::Vec3 pos);

		~PhysicsBody();

		/*
		 * gets the identifier for the physics body
		 *** (0): no type
		 *** (1): box
		 *** (2): sphere
		*/
		int GetId() const;

		// gets the object this physics body is attachted to.
		cherry::Object* GetObject() const;

		// sets the object the physics body is attachted to.
		void SetObject(cherry::Object * obj);

		// sets the object for the physics body, and returns the physics body so that it can be added to the object's physics body list.
		// NOTE: this does not save the body to the object's list. Call AddPhysicsBody() on the object to add it to the object's list.
		cherry::PhysicsBody* AttachToObject(cherry::Object * newObj);

		// gets the model (local space) position as a GLM vector
		glm::vec3 GetModelPositionGLM() const;

		// gets the model (local space) position as a GLM vector
		cherry::Vec3 GetModelPosition() const;

		// sets the model (local space) position
		void SetModelPosition(cherry::Vec3 mpos);

		// set model (local space) position (GLM version)
		void SetModelPosition(glm::vec3 mpos);


		// gets the world position as a glm vector
		glm::vec3 GetWorldPositionGLM() const;

		// gets the world position as a cherry Vector
		// if no object is attached, then the model position is returned.
		cherry::Vec3 GetWorldPosition() const;

		// sets the world position of the body
		void SetWorldPosition(cherry::Vec3 wpos);

		// sets the world position of the body
		void SetWorldPosition(glm::vec3 wpos);


		// gets the rotation of the body (in degrees)
		cherry::Vec3 GetRotationDegrees() const;

		// sets the rotation in degrees
		void SetRotationDegrees(Vec3 degrees);
		
		// gets rotation of the physics body in radians
		cherry::Vec3 GetRotationRadians() const;

		// sets rotation in radians
		void SetRotationRadians(Vec3 radians);


		// gets the scale of the body
		cherry::Vec3 GetScale() const;

		// sets the scale of the body.
		void SetScale(cherry::Vec3 newScale);

		
		// calculates collision between two physics bodies if an equation for it is available.
		static bool Collision(PhysicsBody* p1, PhysicsBody* p2);

		// states whether the body is visible.
		bool IsVisible() const;

		// toggles visibility
		void SetVisible();

		// sets visibility of the body
		void SetVisible(bool visible);

		// update loop for physics bodies
		virtual void Update(float deltaTime);

		// the toString function for physics bodies. This is basically here just so that this is an abstract class.
		virtual std::string ToString() const = 0;

	private:
		int id = 0; // identifier; check this value for downcasting

		// rotation
		cherry::Vec3 rotation;

	protected:
		// the colour of the physics bodies (RGBA)
		const Vec4 COLOUR{ 0.9F, 0.1F, 0.1F, 0.45F };

		// the object the body is attachted to.
		cherry::Object* object = nullptr;

		// position
		cherry::Vec3 position;

		// used to draw the body to the scene.
		cherry::Primitive* body = nullptr;

		// body scale
		cherry::Vec3 scale{ 1.0F, 1.0F, 1.0F };

		// material for body when drawn to hte screen
		Material::Sptr material;
	};

	// the object for a rectangular physics body
	typedef class PhysicsBodyBox : public PhysicsBody
	{
	public:
		// position of (0, 0, 0), with provided dimensions of (width, height, depth) for (x, y, z)
		PhysicsBodyBox(float width, float height, float depth);

		// location of the physics body (relative to object origin)
		// (x, y, z) = position
		// (width, height, depth) = size on (x, y, z) axes respectively
		PhysicsBodyBox(float x, float y, float z, float width, float height, float depth);

		// location of the physics body (relative to the object's origin), and its dimensions.
		PhysicsBodyBox(cherry::Vec3 position, float width, float height, float depth);

		// local position and dimensions (x, y, z) = (width, height, depth)
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

		// update for bounding box
		void Update(float deltaTime);

		// toString
		virtual std::string ToString() const;

	private:
		// dimensions
		float width = 0, height = 0, depth = 0;

	protected:


	} BoxBody;

	// the class for the sphere physics body
	typedef class PhysicsBodySphere : public PhysicsBody
	{
	public:
		// radius of the sphere. The sphere location is the object's origin.
		PhysicsBodySphere(float radius);

		// position of the sphere (relative to the object's origin), and its radius.
		PhysicsBodySphere(cherry::Vec3 position, float radius);

		// gets the radius
		float GetRadius() const;

		// sets the radius; if a negative is passed, the absolute value is received.
		void SetRadius(float r);

		// update loop
		void Update(float deltaTime);

		// ToString function
		std::string ToString() const;

	private:
		float radius = 0.0F; // radius of the sphere.

	protected:
	} SphereBody;
}