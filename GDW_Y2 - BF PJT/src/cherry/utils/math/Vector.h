#ifndef VECTOR_H
#define VECTOR_H

#include <string>
#include <vector>
#include <iostream>

namespace util
{
	namespace math
	{
		// abstract Vec class
		class Vec
		{
		public:

			friend std::ostream& operator<<(std::ostream&, const Vec&); // << operator

			virtual float length() const = 0;

			// normalizes the vector, replacing its current values.
			virtual void normalize() = 0;

			virtual std::string toString() const = 0;

		protected:
			// Optimizes functions, but ultimately is really messy and fragile if someone inherits this class and uses it. So it has been commented out.
			// HELPER FUNCTION: calculates the length of the dot product. The value of 'projV2' determines what vector is being projected upon.
			// projV2: if true, then the dot product is divided by v2's length. If false, then the dot product is divided by the length of v1 (the object being used to call this function).
			// NOTE: ONLY SEND VECTORS TO THIS TEMPLATE FUNCTION
			template<typename T>
			static float calculateDotLength(const T& v1, const T& v2, const bool projV2);

			// HELPER FUNCTION: calculates the angle of the dot product between two vectors.
			// v1 is the first vector, v2 is the second vector, and inDegrees determines whether the angle is returned in degrees, or radians.
			// NOTE: ONLY SEND VECTORS OF THE SAME TYPE TO THIS TEMPLATE
			template<typename T>
			static float calculateDotAngle(const T& v1, const T& v2, const bool inDegrees);

		private:
		};

		// Vector 2
		typedef class Vec2 : public Vec
		{
		public:
			Vec2(float x = 0.0F, float y = 0.0F);

			// istream
			friend std::istream& operator>>(std::istream&, Vec2&);

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

			Vec2 pow(float) const; // returns the vector the provided exponent
			static Vec2 pow(Vec2, float); // returns vector to an exponent

			// calculates and returns the length of the vector
			float length() const override;

			// gets the vector's normalized form.
			Vec2 getNormalized() const;

			// normalizes the vector, replacing its current values.
			void normalize();

			// calculates the dot product
			float dot(Vec2) const;

			// calculates the length of the dot product. The value of 'projV2' determines what vector is being projected upon.
			// projV2: if true, then the dot product is divided by v2's length. If false, then the dot product is divided by the length of v1 (the object being used to call this function).
			float dotLength(const Vec2& v2, bool projV2);

			// calculates the angle of the dot product. The value of 'inDegress' determines whether it's converted to degrees or not
			float dotAngle(const Vec2& v2, bool inDegrees = false) const;

			// gets the vector as a string.
			std::string toString() const override;

			float x = 0.0F, y = 0.0F;
		} Vector2;

		// Vector 3
		typedef class Vec3 : public Vec
		{
		public:
			Vec3(float x = 0.0F, float y = 0.0F, float z = 0.0F);

			Vec3(Vec2 vec, float z = 0.0F);

			// istream
			friend std::istream& operator>>(std::istream&, Vec3&);

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

			Vec3 pow(float); // returns this vector to the exponent.
			static Vec3 pow(Vec3, float); // returns vector to an exponent

			// returns the length of the vector
			float length() const;

			// gets the vector, noramlized
			Vec3 getNormalized() const;

			// normalizes the vector
			void normalize() override;

			// dot product
			float dot(Vec3) const;

			// calculates the length of the dot product. The value of 'projV2' determines what vector is being projected upon.
			// projV2: if true, then the dot product is divided by v2's length. If false, then the dot product is divided by the length of v1 (the object being used to call this function).
			float dotLength(const Vec3& v2, bool projV2);

			// angle of dot product. 'inDegrees' determines whether the angle is returned in degrees or radians
			float dotAngle(const Vec3& v2, bool inDegrees = false) const;

			// cross product with this and another vector.(i, j, k) is multiplied by (x, y, z)
			Vec3 cross(const Vec3 v2, float i = 1.0F, float j = 1.0F, float k = 1.0F) const;

			// cross product between two vectors. (i, j, k) is multiplied by (x, y, z).
			static Vec3 cross(const Vec3 v1, const Vec3 v2, float i = 1.0F, float j = 1.0F, float k = 1.0F);

			// toString function.
			std::string toString() const;

			float x = 0.0F, y = 0.0F, z = 0.0F;
		} Vector3;

		// Vector 4
		typedef class Vec4 : public Vec
		{
		public:
			Vec4(float x = 0.0F, float y = 0.0F, float z = 0.0F, float w = 0.0F);

			Vec4(Vec3 vec, float w = 0.0F);

			// istream
			friend std::istream& operator>>(std::istream&, Vec4&);

			// one is for reading the value, the other is for editing
			const float& operator[](const int index) const;
			float& operator[](const int index);

			bool operator==(const Vec4&) const; // checking if two vectors are equal

			Vec4 operator*(float) const;
			friend Vec4 operator*(float, const Vec4&); // scalar times vec

			Vec4 operator/(float) const;
			Vec4 operator+(const Vec4& vec) const;
			Vec4 operator-(const Vec4& vec) const;

			Vec4 operator-() const;
			friend Vec4 operator-(const float, const Vec4);

			Vec4 operator*=(float);
			Vec4 operator/=(float);
			Vec4 operator+=(Vec4);
			Vec4 operator-=(Vec4);

			Vec4 pow(float); // returns the vector to the provided exponet
			static Vec4 pow(Vec4, float); // returns vector to an exponent

			// gets the length of the vector.
			float length() const;

			// gets the vector, normalized.
			Vec4 getNormalized() const;

			// normalizes the vector
			void normalize() override;

			// gets the dot product of the vector
			float dot(Vec4) const;

			// calculates the length of the dot product. The value of 'projV2' determines what vector is being projected upon.
			// projV2: if true, then the dot product is divided by v2's length. If false, then the dot product is divided by the length of v1 (the object being used to call this function).
			float dotLength(const Vec4& v2, bool projV2);

			// angle of dot product. 'inDegrees' determines whether the angle is returned in degrees or radians
			float dotAngle(const Vec4& v2, bool inDegrees = false) const;

			// toString function
			std::string toString() const;

			float x = 0.0F, y = 0.0F, z = 0.0F, w = 0.0F;
		} Vector4;
	}
}

#endif // !VECTOR_H
