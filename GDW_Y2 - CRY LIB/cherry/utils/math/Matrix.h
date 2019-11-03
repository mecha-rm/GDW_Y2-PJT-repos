#ifndef MATRIX_H
#define MATRIX_H

#include "Vector.h"

#include <ostream> // needed for '<<' operator overloading
#include <iostream> // iostream contains istream, and istream contains ostream
#include <string>

namespace util
{
	namespace math
	{
		// abstract matrix class
		typedef class Mat
		{
		public:
			// istream
			friend std::ostream& operator<<(std::ostream& os, const Mat& mat);

			// adds together all values along the main diagonal.
			virtual float getTrace() const = 0;

			// calculates the determinant.
			virtual float getDeterminant() const = 0;

			// gets the matrix as an array. The length of the array will be equal to the total amount of rows times the total amount of columns.
			virtual float* toArray() const = 0;

			// gets the matrix as a string
			virtual std::string toString() const = 0;

		private:

		protected:
			/*
			 * a helper function used to calculate the values for the minor of a matrix.
				* matArr: the matrix that was passed, in array form.
				* ROW_MAX: the total amount of rows in the matrix.
				* COL_MAX: the total amount of columns in the matrix.
				* ROW: the row to be crossed out.
				* COL: the column to be crossed out
			*/
			float* getMinorValues(const float* const matArr, const unsigned int ROW_TOTAL, const unsigned int COL_TOTAL, unsigned const int ROW, unsigned const int COL) const;

		} Matrix;

		// Matrix 2
		typedef class Mat2 : public Mat
		{
		public:

			Mat2();

			Mat2(float f0, float f1,
				float f2, float f3);

			Mat2(const util::math::Vec2 row0, const util::math::Vec2 row1);

			// istream
			friend std::istream& operator>>(std::istream&, Mat2&);

			// accessor and modifier
			const util::math::Vec2& operator[](const int) const;
			util::math::Vec2& operator[](const int);

			// equality operator
			bool operator==(const Mat2&) const;

			Mat2 operator+(const Mat2&) const;
			Mat2 operator-(const Mat2&) const;

			Mat2 operator*(const float&) const; // apply scalar
			Mat2 operator*(const Mat2&) const; // applying two rotations to one another to create a new matrix

			Mat2 operator/(const float&) const; // apply scalar; re-uses operator*

			Mat2 operator+=(const Mat2&); // += operator
			Mat2 operator-=(const Mat2&); // -= operator
			Mat2 operator*=(const float&); // *= operator (scalar version)
			Mat2 operator*=(const Mat2&); // *= operator (matrix version)
			Mat2 operator/=(const float&); // /= operator


			// gets the main diagonal of the matrix
			util::math::Vec2 getMainDiagonal() const;

			// gets together the values along hte main diagonal
			float getTrace() const;

			// transposes the matrix
			void transpose();

			// flips the values along the main diagonal
			Mat2 getTranspose() const;

			// calculates the determinant.
			float getDeterminant() const;

			// returns a minor. If the row or column are out of bounds, a float of '0' is returned.
			float getMinor(unsigned int row, unsigned int column) const;

			// gets an inverse ofthe matrix.
			Mat2 getInverse() const;

			// gets an identity matrix
			static Mat2 getIdentityMatrix();

			// gets the matrix as an array of length 4.
			float* toArray() const override;

			std::string toString() const override;

		private:
			util::math::Vec2 col[2];

		protected:


		} Matrix2;

		// Matrix 3
		typedef class Mat3 : public Mat
		{
		public:

			Mat3();

			Mat3(float f0, float f1, float f2,
				float f3, float f4, float f5,
				float f6, float f7, float f8);

			Mat3(const util::math::Vec3 row0, const util::math::Vec3 row1, const util::math::Vec3 row2);

			// istream
			friend std::istream& operator>>(std::istream&, Mat3&);

			const util::math::Vec3& operator[](const int) const;
			util::math::Vec3& operator[](const int);

			// equality operator
			bool operator==(const Mat3&) const;

			Mat3 operator+(const Mat3&) const;
			Mat3 operator-(const Mat3&) const;

			Mat3 operator*(const float&) const; // apply scalar
			Mat3 operator*(const Mat3&) const; // applying two rotations to one another to create a new matrix

			Mat3 operator/(const float&) const; // apply scalar; re-uses operator*

			Mat3 operator+=(const Mat3&); // += operator
			Mat3 operator-=(const Mat3&); // -= operator
			Mat3 operator*=(const float&); // *= operator (scalar version)
			Mat3 operator*=(const Mat3&); // *= operator (matrix version)
			Mat3 operator/=(const float&); // /= operator

			// gets the main diagonal of the matrix
			util::math::Vec3 getMainDiagonal() const;

			// flips the values along the main diagonal
			float getTrace() const;

			// transposes the matrix
			void transpose();

			// flips the values along the main diagonal
			Mat3 getTranspose() const;

			// calculates the determinant.
			float getDeterminant() const;

			// returns a minor. If the row or column are out of bounds, an empty matrix is returned.
			Mat2 getMinor(unsigned int row, unsigned int column) const;

			// gets an identity matrix
			static Mat3 getIdentityMatrix();

			// gets the matrix as an array of length 9.
			float* toArray() const override;

			std::string toString() const override;


		private:
			util::math::Vec3 col[3];

		protected:

		} Matrix3;

		// Matrix 4
		typedef class Mat4 : public Mat
		{
		public:
			Mat4();
			// right normalized vector (x), up normalized vector (y), direct, position
			Mat4(const util::math::Vec4 right, const util::math::Vec4 up, const util::math::Vec4 dir, const util::math::Vec4 pos);

			// Creates a matrix
			Mat4(float f0, float f1, float f2, float f3,
				float f4, float f5, float f6, float f7,
				float f8, float f9, float f10, float f11,
				float f12, float f13, float f14, float f15);

			// istream
			friend std::istream& operator>>(std::istream&, Mat4&);

			// accessors and readers
			const util::math::Vec4& operator[](const int) const;
			util::math::Vec4& operator[](const int);

			// eqaulity
			bool operator==(const Mat4&);

			Mat4 operator+(const Mat4&) const;
			Mat4 operator-(const Mat4&) const;

			Mat4 operator*(const float&) const; // apply scalar
			Mat4 operator*(const Mat4&) const; // applying two rotations to one another to create a new matrix

			Mat4 operator/(const float&) const; // apply scalar; re-uses operator*

			Mat4 operator+=(const Mat4&); // += operator
			Mat4 operator-=(const Mat4&); // -= operator
			Mat4 operator*=(const float&); // *= operator (scalar version)
			Mat4 operator*=(const Mat4&); // *= operator (matrix version)
			Mat4 operator/=(const float&); // /= operator

			// gets the main diagonal
			util::math::Vec4 getMainDiagonal() const;

			// adds together the values on the main diagonal
			float getTrace() const;

			// transposes the matrix
			void transpose();

			// flips the values along the main diagonal
			Mat4 getTranspose() const;

			// calculates the determinant.
			float getDeterminant() const;

			// returns a minor. If the row or column are out of bounds, an empty matrix is returned.
			Mat3 getMinor(unsigned int row, unsigned int column) const;

			// gets an identity matrix
			static Mat4 getIdentityMatrix();

			// gets the matrix as an array of length 16.
			float* toArray() const override;

			// gets the array as a string
			std::string toString() const override;

		private:
			util::math::Vec4 col[4];

		protected:

		} Matrix4;
	}
}
#endif // !MATRIX_H