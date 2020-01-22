#pragma once
#include "VectorCRY.h"
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

		// void SetPosition(cherry::Vec3 newPos);

		// void SetPosition

		// update loop for the target.
		// void Update(float deltaTime);

	private:
		cherry::Vec3 position;
		
	protected:


	};
}