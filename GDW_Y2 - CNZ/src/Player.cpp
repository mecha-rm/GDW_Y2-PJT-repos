#include "Player.h"
#include "cherry/objects/Primitive.h"
#include "cherry/physics/PhysicsBody.h"
#include "cherry/animate/AnimationManager.h"

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
	//AddPhysicsBody(new cherry::PhysicsBodyBox(3.5F, 3.5F, 3.5F));
}

// creates the player at the world's origin
cnz::Player::Player(std::string modelPath, std::string scene)
	: Player(modelPath, scene, cherry::Vec3())
{}

// creates the player in hte provided scene
cnz::Player::Player(std::string modelPath, std::string scene, cherry::Vec3 pos)
	:Object(modelPath, scene, true, true)
{
	position = pos;
}

cnz::Player::Player(cherry::Object obj) : Object(obj) {
	position = obj.GetPosition();
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

// generates the default character.
cnz::Player* cnz::Player::GenerateDefault(std::string scene, cherry::Vec3 position)
{
	// TODO: the player object gets remade in CNZ_GameplayScene, so this doesn't work.
	// that will need to be fixed.
	using namespace cherry;

	Player* plyr = new Player("res/objects/hero_ver.2/One_T.obj", scene, position);

	// animation 1
	if(false) // commenting out
	{
		MorphAnimation* mph = new MorphAnimation(plyr);

		// first 10 frames
		mph->AddFrame(new MorphAnimationFrame("res/objects/hero_ver.2/OneAnimationTest_000000.obj"));
		mph->AddFrame(new MorphAnimationFrame("res/objects/hero_ver.2/OneAnimationTest_000001.obj"));
		mph->AddFrame(new MorphAnimationFrame("res/objects/hero_ver.2/OneAnimationTest_000002.obj"));
		mph->AddFrame(new MorphAnimationFrame("res/objects/hero_ver.2/OneAnimationTest_000003.obj"));
		mph->AddFrame(new MorphAnimationFrame("res/objects/hero_ver.2/OneAnimationTest_000004.obj"));
		mph->AddFrame(new MorphAnimationFrame("res/objects/hero_ver.2/OneAnimationTest_000005.obj"));
		mph->AddFrame(new MorphAnimationFrame("res/objects/hero_ver.2/OneAnimationTest_000006.obj"));
		mph->AddFrame(new MorphAnimationFrame("res/objects/hero_ver.2/OneAnimationTest_000007.obj"));
		mph->AddFrame(new MorphAnimationFrame("res/objects/hero_ver.2/OneAnimationTest_000008.obj"));
		mph->AddFrame(new MorphAnimationFrame("res/objects/hero_ver.2/OneAnimationTest_000009.obj"));
		mph->AddFrame(new MorphAnimationFrame("res/objects/hero_ver.2/OneAnimationTest_000010.obj"));

		// second 10 frames
		mph->AddFrame(new MorphAnimationFrame("res/objects/hero_ver.2/OneAnimationTest_000011.obj"));
		mph->AddFrame(new MorphAnimationFrame("res/objects/hero_ver.2/OneAnimationTest_000012.obj"));
		mph->AddFrame(new MorphAnimationFrame("res/objects/hero_ver.2/OneAnimationTest_000013.obj"));
		mph->AddFrame(new MorphAnimationFrame("res/objects/hero_ver.2/OneAnimationTest_000014.obj"));
		mph->AddFrame(new MorphAnimationFrame("res/objects/hero_ver.2/OneAnimationTest_000015.obj"));
		mph->AddFrame(new MorphAnimationFrame("res/objects/hero_ver.2/OneAnimationTest_000016.obj"));
		mph->AddFrame(new MorphAnimationFrame("res/objects/hero_ver.2/OneAnimationTest_000017.obj"));
		mph->AddFrame(new MorphAnimationFrame("res/objects/hero_ver.2/OneAnimationTest_000018.obj"));
		mph->AddFrame(new MorphAnimationFrame("res/objects/hero_ver.2/OneAnimationTest_000019.obj"));
		mph->AddFrame(new MorphAnimationFrame("res/objects/hero_ver.2/OneAnimationTest_000020.obj"));

		// final frames
		mph->AddFrame(new MorphAnimationFrame("res/objects/hero_ver.2/OneAnimationTest_000021.obj"));
		mph->AddFrame(new MorphAnimationFrame("res/objects/hero_ver.2/OneAnimationTest_000022.obj"));
		mph->AddFrame(new MorphAnimationFrame("res/objects/hero_ver.2/OneAnimationTest_000023.obj"));
		mph->AddFrame(new MorphAnimationFrame("res/objects/hero_ver.2/OneAnimationTest_000024.obj"));

		plyr->AddAnimation(mph, true);
		mph->Play();
	}

	return plyr;
}

// gets whether the player is currently dashing.
bool cnz::Player::IsDashing() const { return dash; }

// gets the dash time.
float cnz::Player::GetDashTime() const { return dashTime; }

// sets dash time for the player
void cnz::Player::SetDashTime(float dashTime) { this->dashTime = dashTime; }


// TODO: holdovers from object class. May not be needed?
float cnz::Player::GetDegreeAngle() const { return degreeAngle; }

float cnz::Player::GetRadianAngle() const { return radianAngle; }

glm::vec3 cnz::Player::GetVec3Angle() const { return this->worldAngle; }

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

void cnz::Player::Update(float deltaTime) {
	// Object::Update(deltaTime);
	if (this == nullptr) {
		return;
	}

	// TODO: remove this for the final version.
	// rotation.SetX(rotation.GetX() + 15.0F * deltaTime);
	// rotation.SetZ(rotation.GetZ() + 90.0F * deltaTime);
	
	// already in Object update.
	// runs the path and sets the new position
	//if (followPath)
	//	position = path.Run(deltaTime);

	//// if the animation is playing
	//if (animations.GetCurrentAnimation() != nullptr) {
	//	animations.GetCurrentAnimation()->isPlaying();
	//	animations.GetCurrentAnimation()->Update(deltaTime);
	//}

	//// updating the physics bodies
	//for (cherry::PhysicsBody* body : bodies)
	//	body->Update(deltaTime);

	Object::Update(deltaTime);
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

glm::vec3 cnz::Player::GetDash(float dist) const {
	glm::vec3 dash;
	// glm is backwards so both need to be negative to dash in the correct direction.
	// afaik GetDegreeAngle() is not working correctly so we use Radians converted to degrees instead.
	dash.x = -(dist * glm::degrees(cosf(this->GetRadianAngle())));
	dash.y = -(dist * glm::degrees(sinf(this->GetRadianAngle())));
	dash.z = 0.0f;

	return dash;
}

bool cnz::Player::SetDrawPBody(bool draw)
{
	if (this->GetPhysicsBodyCount() == 0) {
		this->drawPBody = false;
		return false;
	}
	else {
		this->drawPBody = true;
		return true;
	}
}

bool cnz::Player::GetDrawPBody() const
{
	return this->drawPBody;
}

cherry::Vec3 cnz::Player::GetPBodySize() const
{
	return this->pBodySize;
}

float cnz::Player::GetPBodyWidth() const
{
	return this->GetPBodySize().GetX() / 2;
}

float cnz::Player::GetPBodyHeight() const
{
	return this->GetPBodySize().GetY() / 2;
}

float cnz::Player::GetPBodyDepth() const
{
	return this->GetPBodySize().GetZ() / 2;
}
