// file for general physics calculation. WIP.
#pragma once
#include <vector>
#include <string>

#include <GLM/vec2.hpp>
#include <GLM/vec3.hpp>
#include <GLM/vec4.hpp>
#include "utils/math/Vector.h"

using namespace std;

namespace cherry
{
	typedef struct Vec2
	{
		Vec2();

		Vec2(float x, float y);

		// converts util::math::vector to cherry::vector
		Vec2(util::math::Vec2 vec);

		// converts glm::vector to cherry::vector
		Vec2(glm::vec2 vec);

		// desturctor 
		~Vec2();

		// gets the x-value
		float getX() const;
		// sets the x-value
		void setX(float x);

		// gets the y-value
		float getY() const;
		// sets the y-value
		void setY(float y);

		// operators
		// Vec2 operator+ (Vec2& vec);
		// Vec2 operator- (Vec2& vec);

		// vector
		util::math::Vec2* v;
	} Vector2;

	typedef struct Vec3 {

		Vec3();

		Vec3(float x, float y, float z);

		// converts util::math::vector to cherry::vector
		Vec3(util::math::Vec3 vec);

		// converts glm::vector to cherry::vector
		Vec3(glm::vec3 vec);

		~Vec3();

		// gets the x-value
		float getX() const;
		// sets the x-value
		void setX(float x);

		// gets the y-value
		float getY() const;
		// sets the y-value
		void setY(float y);

		// gets the z-value
		float getZ() const;
		// sets the z-value
		void setZ(float z);

		// operators
		// Vec3 operator+ (Vec3& vec);
		// Vec3 operator- (Vec3& vec);

		// vector
		util::math::Vec3* v;

	} Vector3;

	typedef struct Vec4
	{
		Vec4();

		Vec4(float x, float y, float z, float w);

		// util::math::vec4 conversion
		Vec4(util::math::Vec4 vec);

		// glm::vector4 conversion
		Vec4(glm::vec4 vec);

		// gets the x-value
		float getX() const;
		// sets the x-value
		void setX(float x);

		// gets the y-value
		float getY() const;
		// sets the y-value
		void setY(float y);

		// gets the z-value
		float getZ() const;
		// sets the z-value
		void setZ(float z);

		// gets the w-value
		float getW() const;
		// sets the w-value
		void setW(float w);

		// operators
		// Vec4 operator+ (Vec4& vec);
		// Vec4 operator- (Vec4& vec);

		// vector
		util::math::Vec4* v;
	} Vector4;
}