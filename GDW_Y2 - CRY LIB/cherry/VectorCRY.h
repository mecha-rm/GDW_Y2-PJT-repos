// Vector Cherry - engine vector classes
#pragma once
#include <vector>
#include <string>

#include <GLM/vec2.hpp>
#include <GLM/vec3.hpp>
#include <GLM/vec4.hpp>
#include "utils/math/Vector.h"
#include "utils/Utils.h"

using namespace std;

// these set of classes reuses the vector classes that are found in the util::math namespace.
// most of these functions (namely the operators) just reuse functions in the util::math::Vector set of classes.
// see src/cherry/utils/math/Vector.h and related .cpp file for how the functions work.
namespace cherry
{
	// VECTOR 2
	typedef struct Vec2
	{
	public:
		Vec2();

		Vec2(float x, float y);

		// converts util::math::Vec2 to cherry::Vec2
		Vec2(util::math::Vec2 vec);

		// converts glm::Vec2 to cherry::Vec2
		Vec2(glm::vec2 vec);

		// copy constructor
		Vec2(const Vec2&);

		// desturctor 
		~Vec2() = default;

		// gets the x-value
		float GetX() const;

		// sets the x-value
		void SetX(float x);

		// gets the y-value
		float GetY() const;

		// sets the y-value
		void SetY(float y);

		// operators
		const float& operator[](const int index) const; // reading
		float& operator[](const int index); // editing

		// equality operator
		Vec2 operator=(const Vec2&);
		bool operator==(const Vec2&) const; // checking if two vectors are equal
		bool operator!=(const Vec2&) const; // checking if two vectors are not equal.

		Vec2 operator*(float) const; // scalar multiplication
		friend Vec2 operator*(float, const Vec2&); // scalar * vector

		Vec2 operator/(float) const; // scalar divison
		Vec2 operator+(Vec2) const; // adding two vectors
		Vec2 operator-(Vec2) const; // subtracting two vectors


		Vec2 operator-() const; // vec2 - vec2
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

		// lerps between two vectors
		static Vec2 Lerp(const cherry::Vec2 & a, const cherry::Vec2 & b, float t);

		// gets the vector as a string.
		std::string ToString() const;

		// vector object from util::math namespace. This is where the information is stored.
		util::math::Vec2 v;

	private:
		float* const x; // pointer to v.x
		float* const y; // pointer to v.y

	protected:
	} Vector2;


	// VECTOR 3 //
	typedef struct Vec3 
	{
	public:
		Vec3();

		Vec3(float x, float y, float z);

		// converts util::math::vec3 to cherry::Vec3
		Vec3(util::math::Vec3 vec);

		// converts glm::vec3 to cherry::Vec3
		Vec3(glm::vec3 vec);

		// copy constructor
		Vec3(const Vec3&);

		~Vec3() = default;

		// gets the x-value
		float GetX() const;

		// sets the x-value
		void SetX(float x);

		// gets the y-value
		float GetY() const;

		// sets the y-value
		void SetY(float y);

		// gets the z-value
		float GetZ() const;

		// sets the z-value
		void SetZ(float z);

		// operators
		// standard operators
		const float& operator[](const int index) const; // reading
		float& operator[](const int index); // editing

		Vec3 operator=(const Vec3&); // equality operator
		bool operator==(const Vec3&) const; // checking if two vectors are equal
		bool operator!=(const Vec3&) const; // checking if two vectors are not equal.


		Vec3 operator*(float) const; // scalar multiplication
		friend Vec3 operator*(float, const Vec3&); // scalar times vec

		Vec3 operator/(float) const; // scalar divison
		Vec3 operator+(Vec3) const; // addition
		Vec3 operator-(Vec3) const; // subtraction


		Vec3 operator-() const; // -vector
		friend Vec3 operator-(const float, const Vec3); // float - vector

		// operation_equals operators
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

		// lerps between two vectors
		static cherry::Vec3 Lerp(const cherry::Vec3 & a, const cherry::Vec3 & b, float t);

		// gets the vector as a string.
		std::string ToString() const;

		// util::math::vector 3 
		util::math::Vec3 v;

	private:
		float* const x; // pointer to v.x
		float* const y; // pointer to v.y
		float* const z; // pointer to v.z

	protected:

	} Vector3;

	// VECTOR 4 //
	typedef struct Vec4
	{
	public:
		Vec4();

		Vec4(float x, float y, float z, float w);

		// util::math::vec4 conversion
		Vec4(util::math::Vec4 vec);

		// glm::vector4 conversion
		Vec4(glm::vec4 vec);

		// copy constructor
		Vec4(const Vec4&);

		~Vec4() = default;

		// gets the x-value
		float GetX() const;

		// sets the x-value
		void SetX(float x);

		// gets the y-value
		float GetY() const;

		// sets the y-value
		void SetY(float y);

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

		Vec4 operator=(const Vec4&); // equality operator
		bool operator==(const Vec4&) const; // checking if two vectors are equal
		bool operator!=(const Vec4&) const; // checking if two vectors are not equal

		Vec4 operator*(float) const; // scalar multiplication
		friend Vec4 operator*(float, const Vec4&); // scalar times vec

		Vec4 operator/(float) const; // scalar divison
		Vec4 operator+(Vec4) const; // addition
		Vec4 operator-(Vec4) const; // subtraction


		Vec4 operator-() const; // vector - vector
		friend Vec4 operator-(const float, const Vec4); // float - vector

		// operator_equals operations
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

		// lerps between two vectors
		static cherry::Vec4 Lerp(const cherry::Vec4& a, const cherry::Vec4& b, float t);

		// gets the vector as a string.
		std::string ToString() const;

		// vec4 object from util::math
		util::math::Vec4 v;

	private:
		float* const x; // pointer to v.x
		float* const y; // pointer to v.y
		float* const z; // pointer to v.z
		float* const w; // pointer to v.w

	protected:
	} Vector4;
}