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
		
		// gets the position of the target.
		cherry::Vec3 GetPosition() const;

		void SetPosition(cherry::Vec3 newPos);

		// translates the target.
		void Translate(cherry::Vec3 translation);

		// rotates on the z-axis.
		// void RotateZ(float theta, bool inDegrees, cherry::Vec3 origin = cherry::Vec3(0.0F, 0.0F, 0.0F));

		// update loop for the target.
		// void Update(float deltaTime);

		cherry::Vec3 position;

	private:
		
		
	protected:


	};
}