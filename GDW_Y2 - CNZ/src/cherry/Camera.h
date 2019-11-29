// Camera Class (Header)
#pragma once
#include <GLM/glm.hpp>
#include <GLM/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <memory>

#include "VectorCRY.h"

namespace cherry
{
	// camera class
	class Camera {
	public:
		Camera();

		virtual ~Camera() = default;

		// gets the perspective mode matrix set by the user.
		glm::mat4 GetPerspectiveMode() const;

		// sets the values for perspective mode. Create a mat4 using glm::perspective(...) and pass it here.
		// if 'changeMode' is true, the mode is changed to perspective once the perspective matrix is altered.
		void SetPerspectiveMode(glm::mat4 pspec, bool changeMode = true);

		// sets the values for perspective mode.
		/*
		 * fovRadians: the angle of the camera, which must be providied in radians. Use glm::radians(float) to convert from degrees to radians.
		 * aspect: the width/height of the viewpoint/window. Recommened to be 1.0, or window_width / window_height.
		 * zNear: the near plane of the camera. Recommended value: 0.1
		 * zFar: the far plane of the camera.
		 * changeMode: changes the camera to perspective mode. If false, the values are saved, but the camera mode isn't switched.
		*/
		void SetPerspectiveMode(float fovRadians, float aspect, float zNear, float zFar, bool changeMode = true);

		/*
		 * sets the view mode for the camera.
		 *** if 'true' is passed, the camera is set to perspective mode.
		 *** if 'false' is passed, the camera is set to orthographic mode.
		 *** if the camera is already in perspective mode with the current settings, nothing happens.
		*/
		void SetPerspectiveMode(bool pspec);

		// gets the orthographic mode matrix
		glm::mat4 GetOrthographicMode() const;

		// sets the values for orthographic mode. Use glm::ortho(...) and pass it to this function.
		// if changeMode is true, the mode is changed to orthographic when the ortho mat4 is altered. If false, the current camera mode is kept.
		void SetOrthographicMode(glm::mat4 ortho, bool changeMode = true);

		// sets the values for orthographic mode.
		/*
		 * left: the left side of the camera's clipping plane. Recommended to be -4 or -5.
		 * right: the right side of the camera's clipping plane. Recommended to be -4 or -5.
		 * bottom: the bottom of the camera's clipping plane. Recommended to be -4 or -5.
		 * top: the top of the camera's clipping plane. Recommended to be -4 or -5.
		 * zNear: the near plane/front of the clipping plane. Recommended value is 0 since the clipping plane is just a box.
		 * zFar: the far plane/back of the clipping plane. No recommended value here.
		 * changeMode: changes the camera mode to orthographic if true. If false, the values are saved, but the camera doesn't change.
		*/
		void SetOrthographicMode(float left, float right, float bottom, float top, float zNear, float zFar, bool changeMode = true);

		/*
		 * sets the view mode for the camera.
		 *** if 'true' is passed, the camera is set to orthographic mode.
		 *** if 'false' is passed, it is set to perspective mode.
		 *** if the camera is already in orthographic mode with the current orthographic settings, nothing happens.
		*/
		void SetOrthographicMode(bool ortho);

		// call this function to switch the view mode.
		void SwitchViewMode();

		// returns 'true' if in perspective mode. Returns false if not in perpsective mode.
		bool InPerspectiveMode();

		// returns true if in orthographic mode. False if not in orthographic mode.
		bool InOrthographicMode();

		// gets the camera view
		const glm::mat4& GetView() const { return myView; }

		// Gets the camera's view projection
		inline glm::mat4 GetViewProjection() const { return Projection * myView; }

		// gets position
		const glm::vec3& GetPosition() const { return myPosition; }

		// sets hte position using individual values
		void SetPosition(float x, float y, float z);

		// sets position
		void SetPosition(const glm::vec3& pos);

		// sets position using a cherry::Vector 3
		void SetPosition(const cherry::Vec3& pos);

		// Gets the front facing vector of this camera
		inline glm::vec3 GetForward() const { return glm::vec3(-BackX, -BackY, -BackZ); }

		// Gets the up vector of this camera
		inline glm::vec3 GetUp() const { return glm::vec3(UpX, UpY, UpZ); }

		// Gets the right hand vector of this camera
		inline glm::vec3 GetRight() const { return glm::vec3(-LeftX, -LeftY, -LeftZ); }

		// looks at a given location
		void LookAt(const glm::vec3& target, const glm::vec3& up = glm::vec3(0, 0, 1));

		// rotates the camera using a quaternion
		void Rotate(const glm::quat& rot);

		// rotates the camera using a vec3
		void Rotate(const glm::vec3& rot);

		// moves the camera
		void Move(const glm::vec3& local);

		typedef std::shared_ptr<Camera> Sptr;

		glm::mat4 Projection; // the projection (i.e. space that the camera sees)

	private:

		// the perspective bool
		bool perspectiveMode = true;

		// perspective mat4
		glm::mat4 perspective;

		// orthogrphic mat4
		glm::mat4 orthographic;

	protected:
		glm::vec3 myPosition; // camera position

	// Declaring a union between multiple data members
	// These will exist in the same spot in memory, but can be accessed with different names
		union {
			// The base type is our view matrix
			mutable glm::mat4 myView;
			// The next types are all grouped together
			struct {
				float
					LeftX, UpX, BackX, M03,
					LeftY, UpY, BackY, M13,
					LeftZ, UpZ, BackZ, M33,
					TransX, TransY, TransZ, M43;
			};
		};
	};
}