#pragma once

#include "Vector.h"

namespace util
{
	namespace math
	{
		// Circle
		struct Circle
		{

			util::math::Vec2 position{};

			float radius = 0;
		};

		// Sphere
		struct Sphere
		{
			util::math::Vec3 position{};

			float radius = 0;
		};

		
		// Square
		struct Box2D
		{
			// position
			util::math::Vec2 position{};

			// width
			float width = 0;

			// height
			float height = 0;

			// rotation
			// make sure to note if the rotation is in degrees or radians when rotating.
			float rotation = 0;
		};

		// Cube
		struct Box3D
		{
			util::math::Vec3 position;

			// dimensions
			float width = 0;
			float height = 0;
			float depth = 0;

			// rotation on x, y, and z axis.
			// make sure to note if they are in degrees or radians when doing collision.
			util::math::Vec3 rotation{};

			// rotation order.
			char rotationOrder[3]{ ' ', ' ', ' ' };
		};
	}
}

