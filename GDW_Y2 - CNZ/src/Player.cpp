#include "Player.h"
#include "cherry/objects/Primitive.h"
#include "cherry/PhysicsBody.h"

// creates a player with 
cnz::Player::Player(std::string modelFile) : Player(modelFile, cherry::Vec3()) {}

// sets the player file, and position
cnz::Player::Player(std::string modelPath, float posX, float posY, float posZ)
	: Player(modelPath, cherry::Vec3(posX, posY, posZ)) {}

// sets the player file, and position
cnz::Player::Player(std::string modelFile, cherry::Vec3 pos) : Object(modelFile)
{
	position = pos;
	// TODO: replace with a more accurate hitbox
	AddPhysicsBody(new cherry::PhysicsBodyBox(3.5F, 3.5F, 3.5F));
}

// creates the player at the world's origin
cnz::Player::Player(std::string modelPath, std::string scene, cherry::Material::Sptr material)
	: Player(modelPath, scene, material, cherry::Vec3())
{}

// creates the player in hte provided scene
cnz::Player::Player(std::string modelPath, std::string scene, cherry::Material::Sptr material, cherry::Vec3 pos)
	:Object(modelPath, scene, material)
{
	position = pos;
}

// TODO: either fix this, or remove it.
// copies a primitive (doesn't work)
//cnz::Player::Player(const cherry::Primitive * model)
//{
//	if (model == nullptr)
//	{
//		safe = false;
//		return;
//	}
//	// if(model->getSafe)
//
//	model->getSafe();
//
//	// copies the memory for vertices and indices
//	memcpy(vertices, model->GetVertices(), sizeof(cherry::Vertex) * model->getVerticesTotal());
//	memcpy(indices, model->GetIndices(), sizeof(uint32_t) * model->GetIndicesTotal());
//
//
//	mesh = std::make_shared<cherry::Mesh>(vertices, verticesTotal, indices, indicesTotal); // creates the mesh
//	position = model->GetPosition();
//}


// sets dash for the player
void cnz::Player::SetDash(bool dash) { this->dash = dash; }

// gets whether the player is currently dashing.
bool cnz::Player::IsDashing() const { return GetDash(); }

bool cnz::Player::GetDash() const { return dash; }

// gets the dash time.
float cnz::Player::GetDashTime() const { return dashTime; }

// sets dash time for the player
void cnz::Player::SetDashTime(float dashTime) { this->dashTime = dashTime; }


// TODO: holdovers from object class. May not be needed?
float cnz::Player::GetDegreeAngle() { return degreeAngle; }

float cnz::Player::GetRadianAngle() { return radianAngle; }

glm::vec3 cnz::Player::GetVec3Angle() { return this->worldAngle; }

void cnz::Player::UpdateAngle(cherry::Camera::Sptr camera, double xpos, double ypos, unsigned int width, unsigned int height) {

	float a = atanf((float)ypos / (float)xpos);

	if (ypos <= 0 && xpos <= 0) {
		a = 90 * (M_PI / 180.0f) - a;
		a += 90.0f * (M_PI / 180.0f);
	}

	if (ypos > 0 && xpos <= 0) {
		a = 90 * (M_PI / 180.0f) - a;
		a += 90.0f * (M_PI / 180.0f);
	}

	if (ypos >= 0 && xpos >= 0) {
		a = 90 * (M_PI / 180.0f) - a;
		a += 270.0f * (M_PI / 180.0f);
	}

	if (ypos < 0 && xpos >= 0) {
		a = 90 * (M_PI / 180.0f) - a;
		a += 270.0f * (M_PI / 180.0f);
	}

	this->radianAngle = a;
	this->degreeAngle = a * (180.0f / M_PI);

	// Update world angle
	glm::mat4 pv = camera->GetViewProjection();
	pv = glm::inverse(pv);
	glm::vec4 pos = glm::vec4(xpos / width, ypos / height, 0.0f, 1.0f);
	pv *= pos;


	pos.w = 1.0 / pos.w;
	pos.x *= pos.w;
	pos.y *= pos.w;
	pos.z *= pos.w;

	this->worldAngle = glm::vec3(pos.x, pos.y, pos.z);
	glm::quat rotation = glm::quat(this->worldAngle);
}

// sets the angle
void cnz::Player::SetAngle(float angle, bool isDegrees) {
	if (isDegrees) {
		this->degreeAngle = angle;
		this->radianAngle = (this->degreeAngle) * (M_PI / 180.0f);
	}
	else if (!isDegrees) {
		this->radianAngle = angle;
		this->degreeAngle = (this->radianAngle) * (180.0f / M_PI);
	}

	// add code here to convert from screenspace to world space
}

void cnz::Player::SetAngle(glm::vec3 angle) { this->worldAngle = angle; }

glm::vec3 cnz::Player::GetDash(float dist) {
	glm::vec3 dash;
	dash.x = dist * glm::degrees(sinf(this->GetRadianAngle()));
	dash.y = -(dist * glm::degrees(cosf(this->GetRadianAngle())));
	dash.z = 0.0f;
	return dash;
}
