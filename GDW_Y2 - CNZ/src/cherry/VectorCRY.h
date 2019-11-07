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
		float GetX() const;
		// sets the x-value
		void SetX(float x);

		// gets the y-value
		float GetY() const;
		// sets the y-value
		void GetY(float y);

		// operators
		// standard operators
		const float& operator[](const int index) const; // reading
		float& operator[](const int index); // editing

		// equality operator
		bool operator==(const Vec2&) const; // checking if two vectors are equal

		Vec2 operator*(float) const; // scalar multiplication
		friend Vec2 operator*(float, const Vec2&); // scalar times vec

		Vec2 operator/(float) const; // scalar divison
		Vec2 operator+(Vec2) const; // adding two vectors
		Vec2 operator-(Vec2) const; // subtracting two vectors


		Vec2 operator-() const; // -vector
		friend Vec2 operator-(const float, const Vec2); // float - vector

		// equals equation operators
		Vec2 operator*=(float); // multiplication equals
		Vec2 operator/=(float); // division equals
		Vec2 operator+=(Vec2); // addition equals
		Vec2 operator-=(Vec2); // subtraction equals

		// gets the length of the vector.
		float GetLength() const;

		// normalizes the vector
		void Normalize();

		// returns a normalized version of the vector
		cherry::Vec2 GetNormalized() const;

		// gets the vector as a string.
		std::string ToString() const;

		// vector
		util::math::Vec2 v;
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
		float GetX() const;
		// sets the x-value
		void SetX(float x);

		// gets the y-value
		float GetY() const;
		// sets the y-value
		void GetY(float y);

		// gets the z-value
		float getZ() const;
		// sets the z-value
		void setZ(float z);

		// operators
		// standard operators
		const float& operator[](const int index) const; // reading
		float& operator[](const int index); // editing

		bool operator==(const Vec3&) const; // checking if two vectors are equal

		Vec3 operator*(float) const; // scalar multiplication
		friend Vec3 operator*(float, const Vec3&); // scalar times vec

		Vec3 operator/(float) const; // scalar divison
		Vec3 operator+(Vec3) const; // addition
		Vec3 operator-(Vec3) const; // subtraction


		Vec3 operator-() const; // -vector
		friend Vec3 operator-(const float, const Vec3); // float - vector

		Vec3 operator*=(float);
		Vec3 operator/=(float);
		Vec3 operator+=(Vec3);
		Vec3 operator-=(Vec3);

		// gets the length of the vector.
		float GetLength() const;

		// normalizes the vector
		void Normalize();

		// returns a normalized version of the vector
		cherry::Vec3 GetNormalized() const;

		// gets the vector as a string.
		std::string ToString() const;

		// vector
		util::math::Vec3 v;

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
		float GetX() const;
		// sets the x-value
		void SetX(float x);

		// gets the y-value
		float GetY() const;
		// sets the y-value
		void GetY(float y);

		// gets the z-value
		float GetZ() const;
		// sets the z-value
		void SetZ(float z);

		// gets the w-value
		float GetW() const;
		// sets the w-value
		void SetW(float w);

		// operators
		// standard operators
		const float& operator[](const int index) const; // reading
		float& operator[](const int index); // editing

		bool operator==(const Vec4&) const; // checking if two vectors are equal

		Vec4 operator*(float) const; // scalar multiplication
		friend Vec4 operator*(float, const Vec4&); // scalar times vec

		Vec4 operator/(float) const; // scalar divison
		Vec4 operator+(Vec4) const; // addition
		Vec4 operator-(Vec4) const; // subtraction


		Vec4 operator-() const; // -vector
		friend Vec4 operator-(const float, const Vec4); // float - vector

		Vec4 operator*=(float);
		Vec4 operator/=(float);
		Vec4 operator+=(Vec4);
		Vec4 operator-=(Vec4);

		// gets the length of the vector.
		float GetLength() const;

		// normalizes the vector
		void Normalize();

		// returns a normalized version of the vector
		cherry::Vec4 GetNormalized() const;

		// gets the vector as a string.
		std::string ToString() const;

		// vector
		util::math::Vec4 v;
	} Vector4;
}