// Physics Body - used to add physics related properties to an object.
#include "PhysicsBody.h"
#include "..\utils/math/Collision.h"
#include "..\objects/PrimitiveCube.h"
#include "..\WorldTransform.h"

#include "..\utils/math/Rotation.h"

float cherry::PhysicsBody::alpha = 0.80F;


// constructors
// sets the physics body ID
cherry::PhysicsBody::PhysicsBody(int id) : PhysicsBody(id, cherry::Vec3()) {}

// sets the ID for a specific type of physics body
cherry::PhysicsBody::PhysicsBody(int id, cherry::Vec3 pos) : id(id), position(pos) 
{
	// creates a material for the physics body. This is used to display the physics body for debugging purposes.
	Shader::Sptr shader = std::make_shared<Shader>(); // creates the shader

	// shader->Load("res/shaders/lighting.vs.glsl", "res/shaders/blinn-phong.fs.glsl");
	shader->Load("res/shaders/shader.vs.glsl", "res/shaders/shader.fs.glsl");

	// creating the material
	material = std::make_shared<Material>(shader);

	// setting transparency since the bodies need to be see through
	material->HasTransparency = true;
}

// deleting the object
cherry::PhysicsBody::~PhysicsBody()
{
	if (body != nullptr)
		delete body;
}


/*
 * gets the ID for the physics body
	*** (0): no type
	*** (1): box
	*** (2): sphere
*/
int cherry::PhysicsBody::GetId() const { return id; }

// gets the object this physics body is attachted to.
cherry::Object* cherry::PhysicsBody::GetObject() const { return object; }

// sets the object for hte physics body
void cherry::PhysicsBody::SetObject(cherry::Object* obj) 
{ 
	object = obj; 

	if (object != nullptr) // adds the body to the scene, and gives it the material
	{
		body->CreateEntity(object->GetSceneName(), material);
		body->SetVisible(false);

		body->SetScale(scale);
		body->SetRotationDegrees(rotation);
		body->SetPosition(position);

		// current values.
		currParentScale = object->GetScale();
		currParentRot = object->GetRotationDegrees();
		currParentPos = object->GetPosition();
	}
}

// attaches to an object, and reutnrs the body
cherry::PhysicsBody* cherry::PhysicsBody::AttachToObject(cherry::Object* newObj)
{
	SetObject(newObj);
	return this;
}

// gets the model (local) position (glm version)
glm::vec3 cherry::PhysicsBody::GetLocalPositionGLM() const
{
	Vec3 mpos = GetLocalPosition();
	return glm::vec3(mpos.v.x, mpos.v.y, mpos.v.z);
}

// gets the model (local) position
cherry::Vec3 cherry::PhysicsBody::GetLocalPosition() const { return position; }

// sets the model (local) position
void cherry::PhysicsBody::SetLocalPosition(cherry::Vec3 lPos)
{
	position = lPos;

	// the collision boxes position relative to its origin (i.e. the model its attachted to).
	// this is a mat4, but it's treated as a mat 3 X 1.
	glm::mat4 child;

	// the parent position
	glm::mat4 parent = glm::mat4(1.0F);

	// orientation of the parent on each axis.
	// glm::mat4 rotX = glm::mat4(1.0F);
	// glm::mat4 rotY = glm::mat4(1.0F);
	// glm::mat4 rotZ = glm::mat4(1.0F);

	// the resulting matrix.
	glm::mat4 result = glm::mat4(1.0F);

	// rotation and scale
	util::math::Mat3 rotScale {
		1.0F, 0.0F, 0.0F,
		0.0F, 1.0F, 0.0F,
		0.0F, 0.0F, 1.0F
	};

	// scale
	util::math::Mat3 scale = rotScale;

	// rotations
	util::math::Mat3 rotX = rotScale;
	util::math::Mat3 rotY = rotScale;
	util::math::Mat3 rotZ = rotScale;
	
	// setting the position in the child's matrix.
	child[0][3] = position.v.x;
	child[1][3] = position.v.y;
	child[2][3] = position.v.z;

	// the child to parent transform.
	if (object != nullptr)
	{
		// translation
		parent[0][3] = object->GetPositionX();
		parent[1][3] = object->GetPositionY();
		parent[2][3] = object->GetPositionZ();
		parent[3][3] = 1.0F;

		// rotation
		rotX = util::math::getRotationMatrixX(object->GetRotationXDegrees(), true);
		rotY = util::math::getRotationMatrixY(object->GetRotationYDegrees(), true);
		rotZ = util::math::getRotationMatrixZ(object->GetRotationZDegrees(), true);

		// scale
		scale[0][0] = object->GetScaleX();
		scale[1][1] = object->GetScaleY();
		scale[2][2] = object->GetScaleZ();

		// rotation and scale.
		rotScale = scale * (rotZ * rotX * rotY);

		// saving the rotation and scale transformations.
		parent[0][0] = rotScale[0][0];
		parent[0][1] = rotScale[0][1];
		parent[0][2] = rotScale[0][2];

		parent[1][0] = rotScale[1][0];
		parent[1][1] = rotScale[1][1];
		parent[1][2] = rotScale[1][2];

		parent[2][0] = rotScale[2][0];
		parent[2][1] = rotScale[2][1];
		parent[2][2] = rotScale[2][2];
	}

	result = parent * child;

	if (body != nullptr)
		body->SetPosition(result[0][3], result[1][3], result[2][3]);
}

// sets the model (local) position
void cherry::PhysicsBody::SetLocalPosition(glm::vec3 lPos) { SetLocalPosition(cherry::Vec3(lPos)); }

// gets the world position as a GLM vector
glm::vec3 cherry::PhysicsBody::GetWorldPositionGLM() const
{
	Vec3 wpos = GetWorldPosition();
	return glm::vec3(wpos.v.x, wpos.v.y, wpos.v.z);
}

// returns the world position; sceneLists save their position in world space
cherry::Vec3 cherry::PhysicsBody::GetWorldPosition() const 
{
	// TODO: reference body object instead?
	return (object == nullptr ? position : object->GetPosition() + position);
}

// sets the world position
// void cherry::PhysicsBody::SetWorldPosition(cherry::Vec3 wpos) 
// {
// 	if (body != nullptr)
// 		body->SetPosition(wpos);
// }

// sets the world position
// void cherry::PhysicsBody::SetWorldPosition(glm::vec3 wpos) 
// { 
// 	if(body != nullptr)
// 		body->SetPosition(wpos); 
// }



// gets the local rotation in degrees
glm::vec3 cherry::PhysicsBody::GetLocalRotationDegreesGLM() const
{
	return glm::vec3(rotation.v.x, rotation.v.y, rotation.v.z);
}

// gets rotation in degrees
cherry::Vec3 cherry::PhysicsBody::GetLocalRotationDegrees() const { return rotation; }

// set rotation in degrees
void cherry::PhysicsBody::SetLocalRotationDegrees(Vec3 degrees) 
{ 
	rotation = degrees;

	// body exists
	if (body != nullptr && object != nullptr)
	{
		body->SetRotationDegrees(object->GetRotationDegrees() + degrees);
	}
}

// gets the local rotation in radians.
glm::vec3 cherry::PhysicsBody::GetLocalRotationRadiansGLM() const
{
	cherry::Vec3 temp = GetLocalRotationRadians();

	return glm::vec3(temp.v.x, temp.v.y, temp.v.z);
}

// get rotation in radians
cherry::Vec3 cherry::PhysicsBody::GetLocalRotationRadians() const 
{ 
	// returns the vec3
	return Vec3(
		util::math::degreesToRadians(rotation.v.x),
		util::math::degreesToRadians(rotation.v.y),
		util::math::degreesToRadians(rotation.v.z)
	);
}

// set rotation in radians
void cherry::PhysicsBody::SetLocalRotationRadians(Vec3 radians)
{
	rotation = Vec3(
		util::math::radiansToDegrees(rotation.v.x),
		util::math::radiansToDegrees(rotation.v.y),
		util::math::radiansToDegrees(rotation.v.z)
	);

	// body exists
	if (body != nullptr)
	{
		body->SetRotationRadians(body->GetRotationRadians() + radians);
	}
}

// gets the world rotation in degrees.
glm::vec3 cherry::PhysicsBody::GetWorldRotationDegreesGLM() const
{
	if (body != nullptr)
		return body->GetRotationDegreesGLM();
	else
		return GetLocalRotationDegreesGLM();
}

// gets the world rotation of the body.
cherry::Vec3 cherry::PhysicsBody::GetWorldRotationDegrees() const 
{ 
	if (body != nullptr)
		return body->GetRotationDegrees();
	else
		return rotation;
}

// gets the world rotation in radians.
cherry::Vec3 cherry::PhysicsBody::GetWorldRotationRadiansGLM() const
{
	if (body != nullptr)
		return body->GetRotationRadiansGLM();
	else
		return GetLocalRotationRadiansGLM();
}

// gets the world rotation in radians.
cherry::Vec3 cherry::PhysicsBody::GetWorldRotationRadians() const
{
	if (body != nullptr)
		return body->GetRotationRadians();
	else
		return GetLocalRotationRadians();
}




// gets the local scale of the physics body.
glm::vec3 cherry::PhysicsBody::GetLocalScaleGLM() const { return glm::vec3(scale.v.x, scale.v.y, scale.v.z); }

// gets the scale
cherry::Vec3 cherry::PhysicsBody::GetLocalScale() const { return scale; }

// sets the scale of the object
void cherry::PhysicsBody::SetLocalScale(cherry::Vec3 newScale) 
{ 
	scale = newScale; 

	// scale of the physics body.
	cherry::Vec3 bodyScale = newScale;

	// setting the scale proportional to the body.
	if (object != nullptr && body != nullptr)
	{
		bodyScale.v.x = object->GetScaleX() * scale.v.x;
		bodyScale.v.y = object->GetScaleY() * scale.v.y;
		bodyScale.v.z = object->GetScaleZ() * scale.v.z;
	}
	
	body->SetScale(bodyScale);
}

// gets the world scale.
glm::vec3 cherry::PhysicsBody::GetWorldScaleGLM() const
{
	if (body != nullptr)
		return body->GetScaleGLM();
	else
		return glm::vec3(scale.v.x, scale.v.y, scale.v.z);
}

// gets the world scale of the physics body.
cherry::Vec3 cherry::PhysicsBody::GetWorldScale() const
{
	if (body != nullptr)
		return body->GetScale();
	else
		return scale;
}


// calculates collision between sceneLists
bool cherry::PhysicsBody::Collision(PhysicsBody* p1, PhysicsBody* p2)
{
	// if either object is null.
	if (p1 == nullptr || p2 == nullptr)
		return false;

	// no physics body type attachted to either
	if (p1->GetId() == 0 || p2->GetId() == 0)
		return false;

	// OBB Collision
	if (p1->GetId() == 1 && p2->GetId() == 1)
	{
		float val1 = 0, val2 = 0; // used to fast track collisio

		// downcasts the sceneLists
		cherry::PhysicsBodyBox * temp1 = (cherry::PhysicsBodyBox*)p1;
		cherry::PhysicsBodyBox * temp2 = (cherry::PhysicsBodyBox*)p2;

		{
			util::math::Box3D b1; // A
			util::math::Box3D b2; // B

			// box 1 (start)
			b1.position = temp1->GetWorldPosition().v;
			b1.width = temp1->GetWorldWidth();
			b1.height = temp1->GetWorldHeight();
			b1.depth = temp1->GetWorldDepth();

			// box 2 (start)
			b2.position = temp2->GetWorldPosition().v;
			b2.width = temp2->GetWorldWidth();
			b2.height = temp2->GetWorldHeight();
			b2.depth = temp2->GetWorldDepth();

			// checks if collision is possible.
			val1 = max(b1.width, max(b1.height, b1.depth));
			val2 = max(b2.width, max(b2.height, b2.depth));

			// creates a sphere to encompass each box.
			// since the sphere is the smallest possible will containing the box...
			// if the distance between the boxes is larger than it, there can't possibly be collision.
			if ((b1.position - b2.position).length() >= val1 / 2.0F + val2 / 2.0F)
				return false;


			// box 1 (end)
			b1.rotation = temp1->GetWorldRotationDegrees().v;
			b1.rotationOrder[0] = 'x';
			b1.rotationOrder[1] = 'y';
			b1.rotationOrder[2] = 'z';

			// box 2(end)
			b2.rotation = temp2->GetWorldRotationDegrees().v;
			b2.rotationOrder[0] = 'x';
			b2.rotationOrder[1] = 'y';
			b2.rotationOrder[2] = 'z';

			// if the objects haven't been rotated, a regular aabb check is done.
			if (b1.rotation == Vec3(0, 0, 0).v && b2.rotation == Vec3(0, 0, 0).v)
				return util::math::aabbCollision(b1, b2); // aabb
			else
				return util::math::obbCollision(b1, true, b2, true); // obb
		}
	}
	// Sphere Collision
	else if (p1->GetId() == 2 && p2->GetId() == 2)
	{
		// downcasts to shere bodies
		cherry::PhysicsBodySphere* temp1 = (cherry::PhysicsBodySphere*)p1;
		cherry::PhysicsBodySphere* temp2 = (cherry::PhysicsBodySphere*)p2;

		// calculation
		return util::math::sphereCollision(temp1->GetWorldPosition().v, temp1->GetLocalRadius(), 
			temp2->GetWorldPosition().v, temp2->GetLocalRadius());
	}
	// AABB - Sphere Collision
	else if ((p1->GetId() == 1 && p2->GetId() == 2) || (p1->GetId() == 2 && p2->GetId() == 1))
	{
		// if the first object is an aabb, meaning that the other object is a sphere
		if (p1->GetId() == 1) // calls the collision function again, but swaps the sceneLists around
		{
			return Collision(p2, p1);
		}

		// gets the maixmum bounds of the box.
		float val = 0;

		// p1 is a sphere, p2 is an aabb
		cherry::PhysicsBodySphere* temp1 = (cherry::PhysicsBodySphere*)p1;
		cherry::PhysicsBodyBox* temp2 = (cherry::PhysicsBodyBox*)p2;

		// sphere
		util::math::Sphere sphere;
		sphere.position = temp1->GetWorldPosition().v;
		sphere.radius = temp1->GetWorldRadius();

		// box (start)
		util::math::Box3D box;
		box.position = temp2->GetWorldPosition().v;
		box.width = temp2->GetWorldWidth();
		box.height = temp2->GetWorldHeight();
		box.depth = temp2->GetWorldDepth();

		// gets the maximum axis length for the box.
		val = max(box.width, max(box.height, box.depth));

		// checks to see if the objects are close enough for collision to be possible.
		if ((sphere.position - box.position).length() >= sphere.radius + val / 2.0F)
			return false;

		// box (end)
		// since there could be collision, the rest of the values are received.
		box.rotation = temp2->GetWorldRotationDegrees().v;
		box.rotationOrder[0] = 'x';
		box.rotationOrder[1] = 'y';
		box.rotationOrder[2] = 'z';

		// if the box hasn't been rotated, a regular AABB check is done.
		if (box.rotation == Vec3(0, 0, 0).v)
			return util::math::sphereAABBCollision(sphere, box);
		else // rotated box
			return util::math::sphereOBBCollision(sphere, box, true);
	}

	return false;
}

// states whether the body is visible.
bool cherry::PhysicsBody::IsVisible() const
{
	// if a body hasn't been set, then it must be invisible.
	if (body == nullptr)
		return false;
	else
		return body->IsVisible();
}

// toggle visibility
void cherry::PhysicsBody::SetVisible()
{
	if (body != nullptr)
		body->SetVisible();
}

// sets visibility
void cherry::PhysicsBody::SetVisible(bool visible)
{
	if (body != nullptr)
		body->SetVisible(visible);
}

// updates a physics body
void cherry::PhysicsBody::Update(float deltaTime)
{
	if (object != nullptr)
	{
		// if the scale, rotation, or position has been changed.
		// the values are updated accordingly.

		// updates the physics body.
		// scale has changed.
		if (currParentScale != object->GetScale())
		{
			SetLocalScale(scale);
			currParentScale = object->GetScale();
		}

		// rotation has changed.
		if (currParentRot != object->GetRotationDegrees())
		{
			SetLocalRotationDegrees(rotation);
			currParentRot = object->GetRotationDegrees();
		}

		// position has cahnged.
		if (currParentPos != object->GetPosition())
		{
			SetLocalPosition(position);
			currParentPos = object->GetPosition();
		}
	}

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
	body = new cherry::PrimitiveCube(width, height, depth, COLOUR);
	// body->SetPosition(position);
	if(object != nullptr)
		body->CreateEntity(object->GetSceneName(), material);

	SetLocalPosition(position);
	body->SetAlpha(alpha);
}

// location of the physics body and its dimensions
cherry::PhysicsBodyBox::PhysicsBodyBox(cherry::Vec3 position, cherry::Vec3 dimensions)
	: PhysicsBodyBox(position, dimensions.v.x, dimensions.v.y, dimensions.v.z)
{}

// gets the width
float cherry::PhysicsBodyBox::GetLocalWidth() const { return width; }

// sets the width
// void cherry::PhysicsBodyBox::SetWidth(float newWidth) { width = newWidth; }

// gets the world width of the physics body.
float cherry::PhysicsBodyBox::GetWorldWidth() const { return width * GetWorldScale().v.x; }

// returns the height
float cherry::PhysicsBodyBox::GetLocalHeight() const { return height; }

// sets the height
// void cherry::PhysicsBodyBox::SetHeight(float newHeight) { height = newHeight; }

// gets the world height for the phyics body.
float cherry::PhysicsBodyBox::GetWorldHeight() const { return height * GetWorldScale().v.y; }

// returns depth
float cherry::PhysicsBodyBox::GetLocalDepth() const { return depth; }

// sets the depth
// void cherry::PhysicsBodyBox::SetDepth(float newDepth) { depth = newDepth; }

// gets the world depth
float cherry::PhysicsBodyBox::GetWorldDepth() const { return depth * GetWorldScale().v.z; }

// update
void cherry::PhysicsBodyBox::Update(float deltaTime)
{
	PhysicsBody::Update(deltaTime); // no unique behaviour at this time
}

// toString
std::string cherry::PhysicsBodyBox::ToString() const
{
	return "Model Position: " + GetLocalPosition().v.toString() +
		" | Width: " + std::to_string(width) +
		" | Height: " + std::to_string(height) +
		" | Depth: " + std::to_string(depth);
}

// PHYSICS BODY SPHERE //
// constructor
cherry::PhysicsBodySphere::PhysicsBodySphere(float radius) : PhysicsBodySphere(Vec3(), radius)
{}

// constructor (with position)
cherry::PhysicsBodySphere::PhysicsBodySphere(cherry::Vec3 position, float radius)
	: PhysicsBody(2, position), radius(radius) 
{
	body = new cherry::PrimitiveUVSphere(radius, 15, 15, COLOUR);

	if(object != nullptr)
		body->CreateEntity(object->GetSceneName(), object->GetMaterial());

	SetLocalPosition(position);
	body->SetAlpha(alpha);
	
}

// gets the radius
float cherry::PhysicsBodySphere::GetLocalRadius() const { return radius; }

// sets the radius
// void cherry::PhysicsBodySphere::SetRadius(float r) { radius = abs(r); }

// gets the world radius
float cherry::PhysicsBodySphere::GetWorldRadius() const
{
	cherry::Vec3 ws = GetWorldScale(); // gets the world scale.

	return radius * max(ws.v.x, max(ws.v.y, ws.v.z));
}

// returns the diameter.
float cherry::PhysicsBodySphere::GetLocalDiameter() const { return radius * 2; }

// sets the diameter.
// void cherry::PhysicsBodySphere::SetDiameter(float diameter) { radius = abs(diameter/ 2.0F); }

// gets teh world diameter
float cherry::PhysicsBodySphere::GetWorldDiameter() const { return GetWorldRadius() * 2; }

// update
void cherry::PhysicsBodySphere::Update(float deltaTime)
{
	PhysicsBody::Update(deltaTime); // no unique behaviour
}

// toString function
std::string cherry::PhysicsBodySphere::ToString() const
{
	return "Model Position: " + GetLocalPosition().ToString() + " | Radius: " + std::to_string(radius);
}
