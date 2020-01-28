// Physics Body - used to add physics related properties to an object.
#include "PhysicsBody.h"
#include "utils/math/Collision.h"
#include "objects/PrimitiveCube.h"
#include "WorldTransform.h"

#include "utils/math/Rotation.h"

// constructors
// sets the physics body ID
cherry::PhysicsBody::PhysicsBody(int id) : PhysicsBody(id, cherry::Vec3()) {}

// sets the ID for a specific type of physics body
cherry::PhysicsBody::PhysicsBody(int id, cherry::Vec3 pos) : id(id), position(pos) 
{
	// creates a material for the physics body. This is used to display the physics body for debugging purposes.
	Shader::Sptr shader = std::make_shared<Shader>(); // creates the shader

	shader->Load("res/lighting.vs.glsl", "res/blinn-phong.fs.glsl");
		
	// creating the material
	material = std::make_shared<Material>(shader);
	material->Set("a_LightCount", 1);
	material->Set("a_LightPos[0]", { 0, 0, 0 });
	material->Set("a_LightColor[0]", { 1.0f, 1.0f, 1.0f });
	material->Set("a_AmbientColor[0]", { 1.0f, 1.0f, 1.0f });
	material->Set("a_AmbientPower[0]", 1.0f); // change this to change the main lighting power (originally value of 0.1F)
	material->Set("a_LightSpecPower[0]", 0.0f);
	material->Set("a_LightShininess[0]", 0.0f); // MUST be a float
	material->Set("a_LightAttenuation[0]", 1.0f);

	material->Set("s_Albedos[0]", Texture2D::LoadFromFile("res/images/default.png"));
	material->Set("s_Albedos[1]", Texture2D::LoadFromFile("res/images/default.png"));
	material->Set("s_Albedos[2]", Texture2D::LoadFromFile("res/images/default.png"));

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
	}
}

// attaches to an object, and reutnrs the body
cherry::PhysicsBody* cherry::PhysicsBody::AttachToObject(cherry::Object* newObj)
{
	SetObject(newObj);
	return this;
}

// gets the model (local) position (glm version)
glm::vec3 cherry::PhysicsBody::GetModelPositionGLM() const
{
	Vec3 mpos = GetModelPosition();
	return glm::vec3(mpos.v.x, mpos.v.y, mpos.v.z);
}

// gets the model (local) position
cherry::Vec3 cherry::PhysicsBody::GetModelPosition() const { return position; }

// sets the model (local) position
void cherry::PhysicsBody::SetModelPosition(cherry::Vec3 mpos) { position = mpos; }

// sets the model (local) position
void cherry::PhysicsBody::SetModelPosition(glm::vec3 mpos) { 
	position = cherry::Vec3(mpos);
}

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
void cherry::PhysicsBody::SetWorldPosition(cherry::Vec3 wpos) 
{
	if (body != nullptr)
		body->SetPosition(wpos);
}

// sets the world position
void cherry::PhysicsBody::SetWorldPosition(glm::vec3 wpos) 
{ 
	if(body != nullptr)
		body->SetPosition(wpos); 
}



// gets rotation in degrees
cherry::Vec3 cherry::PhysicsBody::GetRotationDegrees() const { return rotation; }

// set rotation in degrees
void cherry::PhysicsBody::SetRotationDegrees(Vec3 degrees) { rotation = degrees; }

// get rotation in radians
cherry::Vec3 cherry::PhysicsBody::GetRotationRadians() const 
{ 
	// returns the vec3
	return Vec3(
		util::math::degreesToRadians(rotation.v.x),
		util::math::degreesToRadians(rotation.v.y),
		util::math::degreesToRadians(rotation.v.z)
	);
}

// set rotation in radians
void cherry::PhysicsBody::SetRotationRadians(Vec3 radians)
{
	rotation = Vec3(
		util::math::radiansToDegrees(rotation.v.x),
		util::math::radiansToDegrees(rotation.v.y),
		util::math::radiansToDegrees(rotation.v.z)
	);
}


// gets the scale
cherry::Vec3 cherry::PhysicsBody::GetScale() const { return scale; }

// sets the scale of the object
void cherry::PhysicsBody::SetScale(cherry::Vec3 newScale) { scale = newScale; }


// calculates collision between sceneLists
bool cherry::PhysicsBody::Collision(PhysicsBody* p1, PhysicsBody* p2)
{
	// if either object is null.
	if (p1 == nullptr || p2 == nullptr)
		return false;

	// no physics body type attachted to either
	if (p1->GetId() == 0 || p2->GetId() == 0)
		return false;

	// AABB Collision
	if (p1->GetId() == 1 && p2->GetId() == 1)
	{
		// downcasts the sceneLists
		cherry::PhysicsBodyBox * temp1 = (cherry::PhysicsBodyBox*)p1;
		cherry::PhysicsBodyBox * temp2 = (cherry::PhysicsBodyBox*)p2;

		// calculation
		return util::math::aabbCollision(temp1->GetWorldPosition().v, temp1->GetWidth(), temp1->GetHeight(), temp1->GetDepth(),
										 temp2->GetWorldPosition().v, temp2->GetWidth(), temp2->GetHeight(), temp2->GetDepth());
	}
	// Sphere Collision
	else if (p1->GetId() == 2 && p2->GetId() == 2)
	{
		// downcasts to shere bodies
		cherry::PhysicsBodySphere* temp1 = (cherry::PhysicsBodySphere*)p1;
		cherry::PhysicsBodySphere* temp2 = (cherry::PhysicsBodySphere*)p2;

		// calculation
		return util::math::sphereCollision(temp1->GetWorldPosition().v, temp1->GetRadius(), temp2->GetWorldPosition().v, temp2->GetRadius());
	}

	// AABB - Sphere Collision
	else if ((p1->GetId() == 1 && p2->GetId() == 2) || (p1->GetId() == 2 && p2->GetId() == 1))
	{
		// if the first object is an aabb, meaning that the other object is a sphere
		if (p1->GetId() == 1) // calls the collision function again, but swaps the sceneLists around
		{
			return Collision(p2, p1);
		}

		// p1 is a sphere, p2 is an aabb
		cherry::PhysicsBodySphere* temp1 = (cherry::PhysicsBodySphere*)p1;
		cherry::PhysicsBodyBox* temp2 = (cherry::PhysicsBodyBox*)p2;

		// calculation
		return util::math::sphereAABBCollision(temp1->GetWorldPosition().v, temp1->GetRadius(),
			temp2->GetWorldPosition().v, temp2->GetWidth(), temp2->GetHeight(), temp2->GetDepth());
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
	// ORIGINAL ATTEMPT USING MAT4 (DID NOT WORK)
	//cherry::TempTransform temp; // used for generating matrices

	//// Equation: T_nodeToWorld = T_parentToWorld * T_nodeToParent
	//glm::mat4 nodeParentTransform; // node to parent transformation
	//glm::mat4 parentWorldTransform; // parent to world transformation
	//glm::mat4 nodeWorldTransform; // node to world transform
	//glm::mat4 valMat; // holds values for a given type
	//
	//// body doesn't exist, or parent doesn't exist.
	//if (object == nullptr || body == nullptr)
	//	return;


	//// the values for the node.
	//temp.Position = glm::vec3(position.v.x, position.v.y, position.v.z);
	//temp.EulerRotation = glm::vec3(rotation.v.x, rotation.v.y, rotation.v.z);
	//temp.Scale = glm::vec3(scale.v.x, scale.v.y, scale.v.z);
	//
	//// getting the mat4s
	//nodeParentTransform = temp.GetWorldTransform();
	//parentWorldTransform = object->GetWorldTransformation();
	//nodeWorldTransform = parentWorldTransform * nodeParentTransform;

	//// tempMat4 
	//valMat[0][0] = position.v.x;
	//valMat[1][0] = position.v.y;
	//valMat[2][0] = position.v.z;
	//valMat[3][0] = 0;

	//valMat = valMat * nodeWorldTransform;
	//body->SetPosition(Vec3(valMat[0][0], valMat[1][0], valMat[2][0]));
	// std::cout << "Parent: " + object->GetPosition().ToString() << std::endl;

	// temp.EulerRotation = object->GetRotationDegreesGLM(); // gets the parent's rotation
	// temp.Scale = object->GetScaleGLM();
	// // position
	// parentTransform[0][3] = object->GetPosition().v.x;
	// parentTransform[1][3] = object->GetPosition().v.x;
	// parentTransform[2][3] = object->GetPosition().v.x;
	// parentTransform[3][3] = 1.0F;
	
	// Current Method (offset from object values)
	// setting the world space values
	body->SetPosition(object->GetPosition().v.x + position.v.x,
		object->GetPosition().v.y + position.v.y,
		object->GetPosition().v.z + position.v.z);

	// sets the rotation in degrees
	body->SetRotationDegrees(Vec3(
		object->GetRotationXDegrees() + rotation.v.x,
		object->GetRotationYDegrees() + rotation.v.y,
		object->GetRotationZDegrees() + rotation.v.z));

	// sets the scale
	body->SetScale(Vec3(
		object->GetScaleX() + scale.v.x,
		object->GetScaleY() + scale.v.y,
		object->GetScaleZ() + scale.v.z));
	
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
}

// location of the physics body and its dimensions
cherry::PhysicsBodyBox::PhysicsBodyBox(cherry::Vec3 position, cherry::Vec3 dimensions)
	: PhysicsBodyBox(position, dimensions.v.x, dimensions.v.y, dimensions.v.z)
{}

// gets the width
float cherry::PhysicsBodyBox::GetWidth() const { return width; }

// sets the width
void cherry::PhysicsBodyBox::SetWidth(float newWidth) { width = newWidth; }

// returns the height
float cherry::PhysicsBodyBox::GetHeight() const { return height; }

// sets the height
void cherry::PhysicsBodyBox::SetHeight(float newHeight) { height = newHeight; }

// returns depth
float cherry::PhysicsBodyBox::GetDepth() const { return depth; }

// sets the depth
void cherry::PhysicsBodyBox::SetDepth(float newDepth) { depth = newDepth; }

// update
void cherry::PhysicsBodyBox::Update(float deltaTime)
{
	PhysicsBody::Update(deltaTime); // no unique behaviour at this time
}

// toString
std::string cherry::PhysicsBodyBox::ToString() const
{
	return "Model Position: " + GetModelPosition().v.toString() +
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
	
}

// gets the radius
float cherry::PhysicsBodySphere::GetRadius() const { return radius; }

// sets the radius
void cherry::PhysicsBodySphere::SetRadius(float r) { radius = abs(r); }

// returns the diameter.
float cherry::PhysicsBodySphere::GetDiameter() const { return radius * 2; }

// sets the diameter.
void cherry::PhysicsBodySphere::SetDiameter(float diameter) { radius = abs(diameter/ 2.0F); }

// update
void cherry::PhysicsBodySphere::Update(float deltaTime)
{
	PhysicsBody::Update(deltaTime); // no unique behaviour
}

// toString function
std::string cherry::PhysicsBodySphere::ToString() const
{
	return "Model Position: " + GetModelPosition().ToString() + " | Radius: " + std::to_string(radius);
}
