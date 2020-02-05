// Physics Body - used to add physics related properties to an object.
#include "PhysicsBody.h"
#include "utils/math/Collision.h"
#include "objects/PrimitiveCube.h"
#include "WorldTransform.h"

#include "utils/math/Rotation.h"

float cherry::PhysicsBody::alpha = 0.80F;


// constructors
// sets the physics body ID
cherry::PhysicsBody::PhysicsBody(int id) : PhysicsBody(id, cherry::Vec3()) {}

// sets the ID for a specific type of physics body
cherry::PhysicsBody::PhysicsBody(int id, cherry::Vec3 pos) : id(id), position(pos) 
{
	// creates a material for the physics body. This is used to display the physics body for debugging purposes.
	Shader::Sptr shader = std::make_shared<Shader>(); // creates the shader

	// shader->Load("res/lighting.vs.glsl", "res/blinn-phong.fs.glsl");
	shader->Load("res/shader.vs.glsl", "res/shader.fs.glsl");

	// creating the material
	material = std::make_shared<Material>(shader);

	// now using a shader without lighting.
	// material->Set("a_LightCount", 1);
	// material->Set("a_LightPos[0]", { 0, 0, 0 });
	// material->Set("a_LightColor[0]", { 1.0f, 1.0f, 1.0f });
	// material->Set("a_AmbientColor[0]", { 1.0f, 1.0f, 1.0f });
	// material->Set("a_AmbientPower[0]", 1.0f); // change this to change the main lighting power (originally value of 0.1F)
	// material->Set("a_LightSpecPower[0]", 0.0f);
	// material->Set("a_LightShininess[0]", 0.0f); // MUST be a float
	// material->Set("a_LightAttenuation[0]", 1.0f);
	// 
	// material->Set("s_Albedos[0]", Texture2D::LoadFromFile("res/images/default.png"));
	// material->Set("s_Albedos[1]", Texture2D::LoadFromFile("res/images/default.png"));
	// material->Set("s_Albedos[2]", Texture2D::LoadFromFile("res/images/default.png"));

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

		body->SetRotationDegrees(rotation);
		body->SetScale(scale);
		body->SetPosition(position);
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
		// downcasts the sceneLists
		cherry::PhysicsBodyBox * temp1 = (cherry::PhysicsBodyBox*)p1;
		cherry::PhysicsBodyBox * temp2 = (cherry::PhysicsBodyBox*)p2;

		// calculation (aabb)
		// return util::math::aabbCollision(temp1->GetWorldPosition().v, temp1->GetWidth(), temp1->GetHeight(), temp1->GetDepth(),
		// 								 temp2->GetWorldPosition().v, temp2->GetWidth(), temp2->GetHeight(), temp2->GetDepth());

		// calculation (obb)
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
	// body->SetPosition(object->GetPosition().v.x + position.v.x,
	// 	object->GetPosition().v.y + position.v.y,
	// 	object->GetPosition().v.z + position.v.z);
	// 
	// // sets the rotation in degrees
	// body->SetRotationDegrees(Vec3(
	// 	object->GetRotationXDegrees() + rotation.v.x,
	// 	object->GetRotationYDegrees() + rotation.v.y,
	// 	object->GetRotationZDegrees() + rotation.v.z));
	// 
	// // sets the scale
	// body->SetScale(Vec3(
	// 	object->GetScaleX() + scale.v.x,
	// 	object->GetScaleY() + scale.v.y,
	// 	object->GetScaleZ() + scale.v.z));
	

	// updating the physics body.
	SetLocalScale(scale);
	SetLocalRotationDegrees(rotation);
	SetLocalPosition(position);
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
	return "Model Position: " + GetLocalPosition().ToString() + " | Radius: " + std::to_string(radius);
}
