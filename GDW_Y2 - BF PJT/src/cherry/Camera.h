// Camera Class (Header)
#pragma once
#include <GLM/glm.hpp>
#include <GLM/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>

#include "Target.h"

namespace cherry
{
	// camera class
	class Camera {
	public:
		Camera();

		virtual ~Camera() = default;

		// gets the perspective mode matrix set by the user.
		const glm::mat4& GetPerspectiveMode() const;

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

		// gets the field of view, which is used in perspective mode.
		float GetFieldOfView() const;

		// gets the aspect ratio. This is for perspective mode only.
		float GetAspectRatio() const;

		// gets the z-near bound of the camera. This version is for perspective mode.
		float GetNearPerspective() const;

		// gets the z-far bound of the camera. This is for perspective mode.
		float GetFarPerspective() const;



		// gets the orthographic mode matrix
		const glm::mat4& GetOrthographicMode() const;

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

		// returns 'true', if this is a perpective camera. This is the same as calling InPerspectiveMode().
		bool IsPerspectiveCamera() const;

		// returns true if in orthographic mode. False if not in orthographic mode.
		bool InOrthographicMode();

		// returns 'true' if this is a orthographic camera. This is the same as calling 'InOrthographic Mode'
		bool IsOrthographicCamera() const;

		// gets the left bound of the camera. This is exclusively for orthographic mode.
		float GetLeftOrthographic() const;

		// gets the right bound of the camera. This is exclusively for orthographic mode.
		float GetRightOrthographic() const;

		// gets the bottom bound of the camera. This is exclusively for orthographic mode.
		float GetBottomOrthographic() const;

		// gets the top bound of the camera. This is exclusively for orthographic mode.
		float GetTopOrthographic() const;

		// gets the z-near bound of the camera. This is exclusively for orthographic mode.
		float GetNearOrthographic() const;

		// gets the z-far bound of the camera. This is exclusively for orthographic mode.
		float GetFarOrthographic() const;

		// gets the camera view
		const glm::mat4& GetView() const { return myView; }

		// returns the projection.
		const glm::mat4& GetProjection() const { return Projection; }

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

		// gets where the camera is looking.
		glm::vec3 LookingAt() const { return lookingAt; };

		// looks at a given location
		void LookAt(const glm::vec3& target, const glm::vec3& up = glm::vec3(0, 0, 1));

		// rotates the camera using a quaternion
		void Rotate(const glm::quat& rot);

		// rotates the camera using a vec3
		void Rotate(const glm::vec3& rot);

		// moves the camera
		void Move(const glm::vec3& local);

		// if 'true', the camera is following a target.
		bool IsFollowingTarget() const;

		// set if the cmaera should be following its target.
		void SetFollowingTarget(bool follow);

		// if 'true', then the camera stays a fixed position away from the target if it follows the taregt.
		// if 'false', then the camera's position stays the same.
		bool HasFixedTargetDistance() const;

		// sets whether the camera's position should use the target's offset.
		void SetFixedTargetDistance(bool fixedDist);

		// updates the camera. If the camera has a target it should be looking at, it looks at that target.
		void Update(float deltaTime);

		// camera pointer.
		typedef std::shared_ptr<Camera> Sptr;

		// the projection (i.e. space that the camera sees)
		glm::mat4 Projection;

		// a target that the camera can lock onto. Use 'followTarget' to have the camera use the target.
		// if you want the camera to be a fixed distance away, change the position offset.
		std::shared_ptr<cherry::Target> target = std::make_shared<cherry::Target>();

		// an offset of the target's position
		cherry::Vec3 targetOffset{};

		// if 'true', then the camera will follow the target.
		bool followTarget = false;

		// if 'true', the camera stays a fixed distance from the target 'target offset'
		bool fixedTargetDistance = false;

		// variables for rendering the camera

		// the size of the viewport; make sure to update this if the window size changes.
		glm::ivec4 viewport;

		// if 'true', the camera draws the skybox when rendered.
		// the skybox must be visisble for this to happen
		bool drawSkybox = true;

		// the size of the camera's border
		int borderSize = 0;

		// the colour of the border
		glm::vec4 borderColor{ 1.0F, 1.0F, 1.0F, 1.0F };

		// // the clear colour of the camera (black by default)
		glm::vec4 clearColor{ 0.0F, 0.0F, 0.0F, 1.0F };
		
		// if 'true', the camera clears the previously rendered colour_buffer_bits and depth_buffer_bits
		// a standard camera should keep this enabled.
		bool clearBuffers = true;


	private:
		
		glm::vec3 lookingAt{}; // the location being looked at.
		glm::vec3 up{ 0, 0, 1 }; // the up parameter for the camera.

		// the perspective bool
		bool perspectiveMode = true;

		// perspective mat4
		glm::mat4 perspective;

		// variables for the perspective matrix.
		float p_fovy = 0.0F; // orientation (rotation) of the camera.
		float p_aspect = 0.0F; // aspect ratio  
		float p_zNear = 0.0F; // near plane (distance)
		float p_zFar = 0.0F; // far plane (distance)

		// orthogrphic mat4
		glm::mat4 orthographic;

		// variables for the orthographic matrix.
		float o_left = 0.0F;
		float o_right = 0.0F;
		float o_bottom = 0.0F;
		float o_top = 0.0F;
		float o_zNear = 0.0F;
		float o_zFar = 0.0F;

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