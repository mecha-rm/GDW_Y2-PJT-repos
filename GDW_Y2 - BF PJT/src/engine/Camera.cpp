// CAMERA CLASS (SOURCE)
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

// constructor
Camera::Camera() : myPosition(glm::vec3(0)), myView(glm::mat4(1.0f)), Projection(glm::mat4(1.0f))
{
	// mat4 used for perspective and orthographic. They are initialized as being empty matrices (i.e. all values are 0).
	perspective = glm::mat4();
	orthographic = glm::mat4();
}

// gets the perspective mode matrix.
glm::mat4 Camera::getPerspectiveMode() const { return perspective; }

// sets perspective mode
void Camera::SetPerspectiveMode(glm::mat4 pspec, bool changeMode)
{
	perspective = pspec; // changing the values of the perspective matrix

	if (changeMode) // changing mode (if applicable)
	{
		Projection = perspective;
		perspectiveMode = true;
	}
}

// sets the perspective mode.
void Camera::SetPerspectiveMode(float fovRadians, float aspect, float zNear, float zFar, bool changeMode)
{ 
	SetPerspectiveMode(glm::perspective(fovRadians, aspect, zNear, zFar), changeMode); // calls other function
}

// sets perspective mode; if true is passed, the camera is switched to perspective mode. If false, it's set to orthographic mode.
void Camera::SetPerspectiveMode(bool pspec)
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

// gets the orthographic mode
glm::mat4 Camera::GetOrthographicMode() const { return orthographic; }

// sets the orthographic mode using a matrix.
void Camera::SetOrthographicMode(glm::mat4 ortho, bool changeMode)
{
	orthographic = ortho; // overriding the old matrix

	if (changeMode) // changing the mode
	{
		Projection = orthographic;
		perspectiveMode = false;
	}
}

// sets to orthographic mode; re-uses other function
void Camera::SetOrthographicMode(float left, float right, float bottom, float top, float zNear, float zFar, bool changeMode)
{
	SetOrthographicMode(glm::ortho(left, right, bottom, top, zNear, zFar), changeMode);
}

// changes to orthographic mode if 'true' is passed.
// since it re-uses SetPerspectiveMode(bool), said function needs to be passed the opposite of what the user provided.
void Camera::SetOrthographicMode(bool ortho) { SetPerspectiveMode(!ortho); }

// switches view mode.
void Camera::SwitchViewMode() { SetPerspectiveMode(!perspectiveMode); }

// checks if the camera is in perspective mode.
bool Camera::InPerspectiveMode() { return perspectiveMode; }

// checks if the camera is in orthographic mode
bool Camera::InOrthographicMode() { return !perspectiveMode; }



// sets position of the camera.
// the position in the view matrix is relative to the rotation, so that is accounted for.
void Camera::SetPosition(const glm::vec3& pos) {
	myView[3] = glm::vec4(-(glm::mat3(myView) * pos), 1.0f);
	myPosition = pos;
}

// calculates our rotation component of our view matrix.
// camera position, where we want to look, and a up vector.
void Camera::LookAt(const glm::vec3& target, const glm::vec3& up) {
	myView = glm::lookAt(myPosition, target, up);
}

// rotates with quaternions.
// We make sure it's not (0), as we don't want to keep doing math with a 0 value.
// we just want to rotate the 3 X 3 portion, so we have mat4_cast(rot) on the left side.
void Camera::Rotate(const glm::quat& rot) {
	// Only update if we have an actual value to rotate by
	if (rot != glm::quat(glm::vec3(0))) {
		myView = glm::mat4_cast(rot) * myView;
	}
}

// rotates the camera using a vector, which is actually converted to a quaternion.
void Camera::Rotate(const glm::vec3& rot) { Rotate(glm::quat(rot)); }

// we need to update our cache value of our position for world space.
void Camera::Move(const glm::vec3& local) {

	// Only update if we have actually moved
	if (local != glm::vec3(0)) {
		
		// We only need to subtract since we are already in the camera's local space
		myView[3] -= glm::vec4(local, 0);

		// Recalculate our position in world space and cache it
		// Our rotation is some value in space, so we take the inverse of it, and multiply it by the transformation we just did.
		// We also invert it because GLM works backwards.
		myPosition = -glm::inverse(glm::mat3(myView)) * myView[3];
	}
}
