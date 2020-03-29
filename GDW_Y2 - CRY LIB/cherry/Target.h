// a target that can be used for a camera or to have an object follow.
#pragma once
#include "animate/Path.h"
// #include "VectorCRY.h"
// a target for an entity to have.

namespace cherry
{
	class Target
	{
	public:
		Target();

		Target(glm::vec3 position);

		Target(cherry::Vec3 position);
		
		// gets the target's position as a glm vector.
		glm::vec3 GetPositionGLM() const;

		// gets the position of the target.
		cherry::Vec3 GetPosition() const;	

		// sets the position of the target.
		void SetPosition(cherry::Vec3 newPos);

		// sets the position of the target.
		void SetPosition(glm::vec3 newPos);

		// sets the position of the target.
		void SetPosition(float x, float y, float z);

		//// gets the offset of the target's position.
		//glm::vec3 GetPositionOffsetGLM() const;

		//// gets the offset of the target's position.
		//cherry::Vec3 GetPositionOffset() const;

		//// sets the target offset.
		//void SetPositionOffset(glm::vec3 newOffset);

		//// sets the target offset.
		//void SetPositionOffset(cherry::Vec3 newOffset);

		//// sets the target offset.
		//void SetPositionOffset(float x, float y, float z);

		//// takes the target's position and applies the offset.
		//cherry::Vec3 ComputeFollowerPositionGLM() const;

		//// takes the target's position and applies the offset.
		//cherry::Vec3 ComputeFollowerPosition() const;

		// translates the target.
		void Translate(glm::vec3 translate);

		// translates the target.
		void Translate(cherry::Vec3 translation);

		// translates the target.
		void Translate(float x, float y, float z);

		// rotates on the z-axis.
		// void RotateZ(float theta, bool inDegrees, cherry::Vec3 origin = cherry::Vec3(0.0F, 0.0F, 0.0F));

		// update loop for the target.
		// void Update(float deltaTime);

		// the target's position
		cherry::Vec3 position{};

		// an offset of the target's position
		cherry::Vec3 offset{};

	private:
		
		
	protected:


	};
}