// Camera Class (Source)
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

// constructor
cherry::Camera::Camera() : myPosition(glm::vec3(0)), myView(glm::mat4(1.0f)), Projection(glm::mat4(100.0f))
{

	// mat4 used for perspective and orthographic. They are initialized as being empty matrices (i.e. all values are 0).
	perspective = glm::mat4();
	orthographic = glm::mat4();
}

// gets the perspective mode matrix.
const glm::mat4& cherry::Camera::GetPerspectiveMode() const { return perspective; }

// sets the perspective mode.
void cherry::Camera::SetPerspectiveMode(float fovRadians, float aspect, float zNear, float zFar, bool changeMode)
{
	// saving the values.
	p_fovy = fovRadians;
	p_aspect = aspect;
	p_zNear = zNear;
	p_zFar = zFar;

	perspective = glm::perspective(fovRadians, aspect, zNear, zFar); // changing the values of the perspective matrix

	if (changeMode) // changing mode (if applicable)
	{
		Projection = perspective;
		perspectiveMode = true;
	}
}

// sets perspective mode; if true is passed, the camera is switched to perspective mode. If false, it's set to orthographic mode.
void cherry::Camera::SetPerspectiveMode(bool pspec)
{
	// if already in perspective mode, the settings are checked.
	if (perspectiveMode == pspec)
	{
		// checks to see if the current camera settings match what is stored in the perspective or orthographic matrix (if applicable)
		// if not, it is overwritten with said values
		if((pspec == true && Projection == perspective) ^ (pspec == false && Projection == orthographic))
			return;
	}

	perspectiveMode = pspec;

	// changes the mode
	Projection = (perspectiveMode) ? perspective : orthographic;
}

// gets the field of view
float cherry::Camera::GetFieldOfView() const { return p_fovy; }

// gets the aspect ratio. This is for perspective mode only.
float cherry::Camera::GetAspectRatio() const { return p_aspect; }

// gets the near plane for perspective mode.
float cherry::Camera::GetNearPerspective() const { return p_zNear; }

// gets the far plane for perspective mode.
float cherry::Camera::GetFarPerspective() const { return p_zFar; }



// gets the orthographic mode
const glm::mat4& cherry::Camera::GetOrthographicMode() const { return orthographic; }

// sets to orthographic mode; re-uses other function
void cherry::Camera::SetOrthographicMode(float left, float right, float bottom, float top, float zNear, float zFar, bool changeMode)
{
	// saving the values
	o_left = left;
	o_right = right;
	o_bottom = bottom;
	o_top = top;
	o_zNear = zNear;
	o_zFar = zFar;

	// creating the orthographic matrix.
	orthographic = glm::ortho(left, right, bottom, top, zNear, zFar);

	// changing the mode
	if (changeMode) 
	{
		Projection = orthographic;
		perspectiveMode = false;
	}
}

// changes to orthographic mode if 'true' is passed.
// since it re-uses SetPerspectiveMode(bool), said function needs to be passed the opposite of what the user provided.
void cherry::Camera::SetOrthographicMode(bool ortho) { SetPerspectiveMode(!ortho); }

// switches view mode.
void cherry::Camera::SwitchViewMode() { SetPerspectiveMode(!perspectiveMode); }

// checks if the camera is in perspective mode.
bool cherry::Camera::InPerspectiveMode() { return perspectiveMode; }

// returns 'true if it's a perspective mode camera.
bool cherry::Camera::IsPerspectiveCamera() const { return perspectiveMode; }

// checks if the camera is in orthographic mode
bool cherry::Camera::InOrthographicMode() { return !perspectiveMode; }

// checks to see if this is a orthographic camera.
bool cherry::Camera::IsOrthographicCamera() const { return !perspectiveMode; }

// gets the left bound of the orthographic camera.
float cherry::Camera::GetLeftOrthographic() const { return o_left; }

// gets the right bound of the orthographic camera.
float cherry::Camera::GetRightOrthographic() const { return o_right; }

// gets the bottom bound of the orthographic camera.
float cherry::Camera::GetBottomOrthographic() const { return o_bottom; }

// gets the top bound of the orthographic camera.
float cherry::Camera::GetTopOrthographic() const { return o_top; }

// gets the z-near bound of the orthographic camera
float cherry::Camera::GetNearOrthographic() const { return o_zNear; }

// gets the z-far of the orthographic camera.
float cherry::Camera::GetFarOrthographic() const { return o_zFar; }


// sets the position
void cherry::Camera::SetPosition(float x, float y, float z) { SetPosition(glm::vec3(x, y, z)); }

// sets position of the camera.
// the position in the view matrix is relative to the rotation, so that is accounted for.
void cherry::Camera::SetPosition(const glm::vec3& pos) {
	myView[3] = glm::vec4(-(glm::mat3(myView) * pos), 1.0f);
	myPosition = pos;
}

// sets the position of the camera using a glm vector
void cherry::Camera::SetPosition(const cherry::Vec3& pos) { SetPosition(glm::vec3(pos.v.x, pos.v.y, pos.v.z)); }

// calculates our rotation component of our view matrix.
// camera position, where we want to look, and a up vector.
void cherry::Camera::LookAt(const glm::vec3& target, const glm::vec3& up) 
{
	lookingAt = target;
	this->up = up;

	myView = glm::lookAt(myPosition, target, up);
}

// rotates with quaternions.
// We make sure it's not (0), as we don't want to keep doing math with a 0 value.
// we just want to rotate the 3 X 3 portion, so we have mat4_cast(rot) on the left side.
void cherry::Camera::Rotate(const glm::quat& rot) {
	// Only Update if we have an actual value to rotate by
	if (rot != glm::quat(glm::vec3(0))) {
		myView = glm::mat4_cast(rot) * myView;
	}
}

// rotates the camera using a vector, which is actually converted to a quaternion.
/// this is in radians
void cherry::Camera::Rotate(const glm::vec3& rot) { Rotate(glm::quat(rot)); }

// we need to Update our cache value of our position for world space.
void cherry::Camera::Move(const glm::vec3& local) {
	
	// Only Update if we have actually moved
	if (local != glm::vec3(0)) {
		
		// We only need to subtract since we are already in the camera's local space
		myView[3] -= glm::vec4(local, 0);

		// Recalculate our position in world space and cache it
		// Our rotation is some value in space, so we take the inverse of it, and multiply it by the transformation we just did.
		// We also invert it because GLM works backwards.
		myPosition = -glm::inverse(glm::mat3(myView)) * myView[3];
	}
}

// gets if the camera is following a target or not.
bool cherry::Camera::IsFollowingTarget() const { return followTarget; }

// sets if the camera should follow the target.
void cherry::Camera::SetFollowingTarget(bool follow) { followTarget = follow; }

// if the cmaera shoudl use the camera's offset.
bool cherry::Camera::HasFixedTargetDistance() const { return fixedTargetDistance; }

// sets whether the camera's position should use the target's offset.
void cherry::Camera::SetFixedTargetDistance(bool fixedDist) { fixedTargetDistance = fixedDist; }

// updates the camera
void cherry::Camera::Update(float deltaTime)
{
	// if the camera should be looking at a target, it does so.
	if (followTarget)
	{
		// sets the camera's new position.
		if (fixedTargetDistance)
			SetPosition(target->GetPosition() + targetOffset);

		LookAt(target->GetPositionGLM(), up);
	}
}
