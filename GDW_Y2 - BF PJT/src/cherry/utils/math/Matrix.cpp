#include "Matrix.h"
#include "../Array.h"

// ostream operator
// std::ostream & util::math::operator<<(std::ostream & os, const Mat & mat) { return os << mat.toString(); }

// a helper function used to calculate the values for the minor of a matrix.
float *  util::math::Mat::getMinorValues(const float * const matArr, const unsigned int ROW_TOTAL, const unsigned int COL_TOTAL, unsigned const int ROW, unsigned const int COL) const
{
	float * cofactorArr = new float[(ROW_TOTAL - 1) * (COL_TOTAL - 1)]; // holds the array that will
	int index = 0; // index of the cofactor array

	// checking for valid input. If the row or column are out of bounds, then an empty matrix is returned.
	try
	{
		if (ROW >= ROW_TOTAL || COL >= COL_TOTAL)
			throw "Index out of bonds. Returning empty Matrix.";
	}
	catch (const char * ex)
	{
		std::cout << ex << std::endl;
		return cofactorArr;
	}

	// goes through every row and column
	for (int r = 0; r < ROW_TOTAL; r++)
	{
		for (int c = 0; c < COL_TOTAL; c++)
		{
			if (!(r == ROW || c == COL)) // if the value is not along the crossed outlines, then it is added to the cofactor array.
			{
				cofactorArr[index] = matArr[r * COL_TOTAL + c]; // calculates what index of a multi-dimensional matrix would be the equivalent in our 1D matrix, and gets its value.
				index++;
			}
		}
	}

	return cofactorArr;
}


//// MATRIX 2 //////////////////////////////////////////
util::math::Mat2::Mat2() : Mat2(0.0F, 0.0F, 0.0F, 0.0F) {}

util::math::Mat2::Mat2(float f0, float f1, float f2, float f3)
{
	col[0] = util::math::Vec2(f0, f1);
	col[1] = util::math::Vec2(f2, f3);
}


util::math::Mat2::Mat2(const util::math::Vec2 row0, const util::math::Vec2 row1)
{
	col[0] = row0;
	col[1] = row1;
}

// istream operator
// std::istream & util::math::operator>>(std::istream & in, Mat2 & m2)
// {
// 	in >> m2[0][0] >> m2[0][1] 
// 	   >> m2[1][0] >> m2[1][1];
// 	return in;
// }

// reading information
const util::math::Vec2& util::math::Mat2::operator[](const int index) const { return col[index]; }

// editing information
util::math::Vec2 & util::math::Mat2::operator[](const int index) { return col[index]; }

// equality operator
bool util::math::Mat2::operator==(const Mat2 & m2) const
{
	return (
		col[0][0] == m2[0][0] &&
		col[0][1] == m2[0][1] &&

		col[1][0] == m2[1][0] &&
		col[1][1] == m2[1][1]
	);
}

// addition operator
util::math::Mat2 util::math::Mat2::operator+(const Mat2 & mat) const
{
	return Mat2(
		col[0] + mat[0],
		col[1] + mat[1]
	);
}

// minus operator
util::math::Mat2 util::math::Mat2::operator-(const Mat2 & mat) const
{
	return Mat2(
		col[0] - mat[0],
		col[1] - mat[1]
	);
}

// multiplication by scalar
util::math::Mat2 util::math::Mat2::operator*(const float & scalar) const
{
	return Mat2(
		col[0] * scalar,
		col[1] * scalar
	);
}

// multiplication by scalar
util::math::Mat2 util::math::Mat2::operator*(const Mat2 & mat) const
{
	return Mat2(
		col[0][0] * mat[0][0] + col[0][1] * mat[1][0],
		col[0][0] * mat[0][1] + col[0][1] * mat[1][1],

		col[1][0] * mat[0][0] + col[1][1] * mat[1][0],
		col[1][0] * mat[0][1] + col[1][1] * mat[1][1]
	);
}

// division: makes the scalar a fraction, and multiplies by it.
util::math::Mat2 util::math::Mat2::operator/(const float & scalar) const { return *this * std::powf(scalar, -1);  }

// += operator
util::math::Mat2 util::math::Mat2::operator+=(const Mat2 & m) { return *this = *this + m; }

// -= operator
util::math::Mat2 util::math::Mat2::operator-=(const Mat2 & m) { return *this = *this - m; }

// *= operator (scalar version)
util::math::Mat2 util::math::Mat2::operator*=(const float & flt) { return *this = *this * flt; }

// *= operator (matrix version)
util::math::Mat2 util::math::Mat2::operator*=(const Mat2 & mat) { return *this = *this * mat; }

// /= oprator
util::math::Mat2 util::math::Mat2::operator/=(const float & flt) { return *this = *this / flt; }

// gets the main diagonal of the matrix
util::math::Vec2 util::math::Mat2::getMainDiagonal() const { return util::math::Vec2(col[0][0], col[1][1]); }

// adds together all values along the main diagonal.
float util::math::Mat2::getTrace() const
{ 
	util::math::Vec2 v2 = getMainDiagonal();
	return v2.x + v2.y;
}

// transposes the matrix
void util::math::Mat2::transpose() { *this = getTranspose(); }

// flips the values along the main diagonal
util::math::Mat2 util::math::Mat2::getTranspose() const
{
	Mat2 tempMatrix;
	
	tempMatrix[0][0] = col[0][0];
	tempMatrix[0][1] = col[1][0];

	tempMatrix[1][0] = col[0][1];
	tempMatrix[1][1] = col[1][1];

	return tempMatrix;
}

// calculates the determinant. Det(A) = ad - bc.
float util::math::Mat2::getDeterminant() const { return col[0][0] * col[1][1] - col[0][1] * col[1][0]; }

// returns the minor at the provided row and column
float util::math::Mat2::getMinor(unsigned int row, unsigned int column) const
{
	// gets the minor, which is a single value.
	return getMinorValues(toArray(), 2, 2, row, column)[0];

	//// checking for valid input.
	//try
	//{
	//	if (row > 1 || column > 1)
	//		throw "Index out of bonds. Returning -1";
	//}
	//catch (const char * ex)
	//{
	//	std::cout << ex << std::endl;
	//	return -1.0F;
	//}

	//for (int r = 0; r < 2; r++)
	//{
	//	for (int c = 0; c < 2; c++)
	//	{
	//		if (r != row && c != column)
	//		{
	//			return col[r][c]; // returns the value.
	//		}
	//	}
	//}

	//return 0.0f;
}

// gets an inverse ofthe matrix.
util::math::Mat2 util::math::Mat2::getInverse() const
{
	util::math::Mat2 mat;

	mat[0][0] = col[1][1]; // d
	mat[0][1] = -col[0][1]; // -b

	mat[1][0] = -col[1][0]; // -c
	mat[1][1] = col[0][0]; // a

	return mat * std::powf(getDeterminant(), -1);
}

// returns an identity matrix
util::math::Mat2 util::math::Mat2::getIdentityMatrix()
{
	return Mat2(
		1.0F, 0.0F,
		0.0F, 1.0F
	);
}

// gets the matrix as a 2D array
float * util::math::Mat2::toArray() const
{
	float * arr = new float[4];

	arr[0] = col[0][0];
	arr[1] = col[0][1];
	arr[2] = col[1][0];
	arr[3] = col[1][1];

	return arr;
}

std::string util::math::Mat2::toString() const
{
	std::string mat = "";

	for (int r = 0; r < 2; r++)
	{
		for (int c = 0; c < 2; c++)
		{
			mat += std::to_string(col[r][c]) + "\t";
		}

		mat += "\n";
	}

	return mat;
}

//// MATRIX 3 //////////////////////////////////////////
util::math::Mat3::Mat3() : Mat3(0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F) {}

util::math::Mat3::Mat3(float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8)
{
	col[0][0] = f0;
	col[0][1] = f1;
	col[0][2] = f2;

	col[1][0] = f3;
	col[1][1] = f4;
	col[1][2] = f5;

	col[2][0] = f6;
	col[2][1] = f7;
	col[2][2] = f8;
}


util::math::Mat3::Mat3(const util::math::Vec3 row0, const util::math::Vec3 row1, const util::math::Vec3 row2)
{
	col[0] = row0;
	col[1] = row1;
	col[2] = row2;
}

// istream operator
// std::istream & util::math::operator>>(std::istream & in, Mat3 & m2)
// {
// 	in >> m2[0][0] >> m2[0][1] >> m2[0][2]
// 		>> m2[1][0] >> m2[1][1] >> m2[1][2]
// 		>> m2[2][0] >> m2[2][1] >> m2[2][2];
// 
// 	return in;
// }

// reading values
const util::math::Vec3& util::math::Mat3::operator[](const int index) const { return col[index]; }

// editing values
util::math::Vec3 & util::math::Mat3::operator[](const int index) { return col[index]; }

// equality operator
bool util::math::Mat3::operator==(const Mat3 & m2) const
{
	return (
		col[0][0] == m2[0][0] &&
		col[0][1] == m2[0][1] &&
		col[0][2] == m2[0][2] &&

		col[1][0] == m2[1][0] &&
		col[1][1] == m2[1][1] &&
		col[1][2] == m2[1][2] &&

		col[2][0] == m2[2][0] &&
		col[2][1] == m2[2][1] &&
		col[2][2] == m2[2][2]
	);
}

// addition operator
util::math::Mat3 util::math::Mat3::operator+(const Mat3 & mat) const
{
	return Mat3(
		col[0] + mat[0],
		col[1] + mat[1],
		col[2] + mat[2]
	);
}

// minus operator
util::math::Mat3 util::math::Mat3::operator-(const Mat3 & mat) const
{
	return Mat3(
		col[0] - mat[0],
		col[1] - mat[1],
		col[2] - mat[2]
	);
}

// multiplies a mat3 by a scalar
util::math::Mat3 util::math::Mat3::operator*(const float & scalar) const
{
	return Mat3(
		col[0] * scalar,
		col[1] * scalar,
		col[2] * scalar
	);
}


// multiplies a mat3 by a mat3
util::math::Mat3 util::math::Mat3::operator*(const Mat3 & mat) const
{
	// 'col' is the first matrix, and 'mat' the second matrix.
	return Mat3(
		col[0][0] * mat[0][0] + col[0][1] * mat[1][0] + col[0][2] * mat[2][0],
		col[0][0] * mat[0][1] + col[0][1] * mat[1][1] + col[0][2] * mat[2][1],
		col[0][0] * mat[0][2] + col[0][1] * mat[1][2] + col[0][2] * mat[2][2],

		col[1][0] * mat[0][0] + col[1][1] * mat[1][0] + col[1][2] * mat[2][0],
		col[1][0] * mat[0][1] + col[1][1] * mat[1][1] + col[1][2] * mat[2][1],
		col[1][0] * mat[0][2] + col[1][1] * mat[1][2] + col[1][2] * mat[2][2],

		col[2][0] * mat[0][0] + col[2][1] * mat[1][0] + col[2][2] * mat[2][0],
		col[2][0] * mat[0][1] + col[2][1] * mat[1][1] + col[2][2] * mat[2][1],
		col[2][0] * mat[0][2] + col[2][1] * mat[1][2] + col[2][2] * mat[2][2]
	);
}

// division operator by a scalar
util::math::Mat3 util::math::Mat3::operator/(const float & scalar) const { return *this * std::powf(scalar, -1); }

// += operator
util::math::Mat3 util::math::Mat3::operator+=(const Mat3 & m) { return *this = *this + m; }

// -= operator
util::math::Mat3 util::math::Mat3::operator-=(const Mat3 & m) { return *this = *this - m; }

// *= operator (scalar version)
util::math::Mat3 util::math::Mat3::operator*=(const float & flt) { return *this = *this * flt; }

// *= operator (matrix version)
util::math::Mat3 util::math::Mat3::operator*=(const Mat3 & mat) { return *this = *this * mat; }

// /= oprator
util::math::Mat3 util::math::Mat3::operator/=(const float & flt) { return *this = *this / flt; }

// gets the main diagonal of the matrix
util::math::Vec3 util::math::Mat3::getMainDiagonal() const { return util::math::Vec3(col[0][0], col[1][1], col[2][2]); }

// adds together all values along the main diagonal
float util::math::Mat3::getTrace() const
{
	Vec3 v3 = getMainDiagonal();
	return (v3.x + v3.y + v3.z);
}

// transposes the matrix
void util::math::Mat3::transpose() { *this = getTranspose(); }

// flips the values along the main diagonal
util::math::Mat3 util::math::Mat3::getTranspose() const
{
	Mat3 tempMatrix;

	// You can just cut out the column and paste it as the row for col to make it easier
	tempMatrix[0][0] = col[0][0];
	tempMatrix[1][0] = col[0][1];
	tempMatrix[2][0] = col[0][2];
		
	tempMatrix[0][1] = col[1][0];
	tempMatrix[1][1] = col[1][1];
	tempMatrix[2][1] = col[1][2];
		
	tempMatrix[0][2] = col[2][0];
	tempMatrix[1][2] = col[2][1];
	tempMatrix[2][2] = col[2][2];

	return tempMatrix;
}

// calculates the determinant.
float util::math::Mat3::getDeterminant() const
{
	// a, b, c
	// d, e, f
	// g, h, i
	
	// det(A) = aei - afh + bfg - bdi + cdh - ceg
	// det(A) = a(ei - fh) - b(di - fg) + c(dh - eg)
	// det(A) = a(ei - fh) + b(fg - di) + c(dh - eg)

	return col[0][0] * getMinor(0, 0).getDeterminant() - col[0][1] * getMinor(0, 1).getDeterminant() + col[0][2] * getMinor(0, 2).getDeterminant();
}

// gets the minor of the matrix
util::math::Mat2 util::math::Mat3::getMinor(unsigned int row, unsigned int column) const
{
	// gets the minor, which is a 2 X 2 matrix.
	float * tempArr = getMinorValues(toArray(), 3, 3, row, column);

	return Mat2(tempArr[0], tempArr[1], tempArr[2], tempArr[3]);

	//Mat2 tempMatrix; // temporary matrix. If the row or column are out of bounds, then an empty matrix is returned.
	//float arr[4]; // gets the values that will be saved to the matrix3.
	//int index = 0; // index of the array

	//// checking for valid input.
	//try
	//{
	//	if (row > 2 || column > 2)
	//		throw "Index out of bonds. Returning empty matrix.";
	//}
	//catch (const char * ex)
	//{
	//	std::cout << ex << std::endl;
	//	return Mat2();
	//}

	//// gets the values
	//for (int r = 0; r < 3; r++)
	//{
	//	for (int c = 0; c < 3; c++)
	//	{
	//		if (!(r == row || c == column))
	//		{
	//			arr[index] = col[r][c];
	//			index++;
	//		}
	//	}
	//}

	//tempMatrix = Mat2(arr[0], arr[1], arr[2], arr[3]); // creates the matrix.

	//return tempMatrix;
}

// returns an identity matrix
util::math::Mat3 util::math::Mat3::getIdentityMatrix()
{
	return Mat3(
		1.0F, 0.0F, 0.0F,
		0.0F, 1.0F, 0.0F,
		0.0F, 0.0F, 1.0F
	);
}

// gets the matrix as an array of length 9.
float * util::math::Mat3::toArray() const
{
	float * arr = new float[9]; // gets the matrix as 1D array.

	arr[0] = col[0][0];
	arr[1] = col[0][1];
	arr[2] = col[0][2];

	arr[3] = col[1][0];
	arr[4] = col[1][1];
	arr[5] = col[1][2];

	arr[6] = col[2][0];
	arr[7] = col[2][1];
	arr[8] = col[2][2];

	return arr;
}

std::string util::math::Mat3::toString() const
{
	std::string mat = "";

	for (int r = 0; r < 3; r++)
	{
		for (int c = 0; c < 3; c++)
		{
			mat += std::to_string(col[r][c]) + "\t";
		}

		mat += "\n";
	}

	return mat;
}



//// MATRIX 4 //////////////////////////////////////////
util::math::Mat4::Mat4() : Mat4(0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F) // just making an identy matrix
{
	// creates an identity matrix.
	//col[0][0] = 1.0f;
	//col[0][1] = 0.0f;
	//col[0][2] = 0.0f;
	//col[0][3] = 0.0f;

	//col[1][0] = 0.0f;
	//col[1][1] = 1.0f;
	//col[1][2] = 0.0f;
	//col[1][3] = 0.0f;

	//col[2][0] = 0.0f;
	//col[2][1] = 0.0f;
	//col[2][2] = 1.0f;
	//col[2][3] = 0.0f;

	//col[3][0] = 0.0f;
	//col[3][1] = 0.0f;
	//col[3][2] = 0.0f;
	//col[3][3] = 1.0f;
}

util::math::Mat4::Mat4(const Vec4 right, const Vec4 up, const Vec4 dir, const Vec4 pos)
{
	col[0] = right;
	col[1] = up;
	col[2] = dir;
	col[3] = pos;
}

// Initializing all values for a matrix for your game
util::math::Mat4::Mat4(float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, float f10, float f11, float f12, float f13, float f14, float f15)
{
	col[0][0] = f0;
	col[0][1] = f1;
	col[0][2] = f2;
	col[0][3] = f3;

	col[1][0] = f4;
	col[1][1] = f5;
	col[1][2] = f6;
	col[1][3] = f7;

	col[2][0] = f8;
	col[2][1] = f9;
	col[2][2] = f10;
	col[2][3] = f11;

	col[3][0] = f12;
	col[3][1] = f13;
	col[3][2] = f14;
	col[3][3] = f15;
}

// istream operator
// std::istream & util::math::operator>>(std::istream & in, Mat4 & m2)
// {
// 	in >> m2[0][0] >> m2[0][1] >> m2[0][2] >> m2[0][3]
// 		>> m2[1][0] >> m2[1][1] >> m2[1][2] >> m2[1][3]
// 		>> m2[2][0] >> m2[2][1] >> m2[2][2] >> m2[2][3]
// 		>> m2[3][0] >> m2[3][1] >> m2[3][2] >> m2[3][3];
// 
// 	return in;
// }

// reading
const util::math::Vec4& util::math::Mat4::operator[](const int index) const { return col[index]; }

// writing
util::math::Vec4 & util::math::Mat4::operator[](const int index) { return col[index]; }

// eqaulity
bool util::math::Mat4::operator==(const Mat4 & m2)
{
	return (
		col[0][0] == m2[0][0] &&
		col[0][1] == m2[0][1] &&
		col[0][2] == m2[0][2] &&
		col[0][3] == m2[0][3] &&

		col[1][0] == m2[1][0] &&
		col[1][1] == m2[1][1] &&
		col[1][2] == m2[1][2] &&
		col[1][3] == m2[1][3] &&

		col[2][0] == m2[2][0] &&
		col[2][1] == m2[2][1] &&
		col[2][2] == m2[2][2] &&
		col[2][3] == m2[2][3] &&

		col[3][0] == m2[3][0] &&
		col[3][1] == m2[3][1] &&
		col[3][2] == m2[3][2] &&
		col[3][3] == m2[3][3]
	);
}

// addition operator
util::math::Mat4 util::math::Mat4::operator+(const Mat4 & m) const // this const means everything in the matrix class will not be changed
{
	return Mat4(
		col[0] + m[0],
		col[1] + m[1],
		col[2] + m[2],
		col[3] + m[3]
	);
}

// minus operator
util::math::Mat4 util::math::Mat4::operator-(const Mat4 & m) const
{
	return Mat4(
		col[0] - m[0],
		col[1] - m[1],
		col[2] - m[2],
		col[3] - m[3]
	);
}

// multiplication operator for scalars
util::math::Mat4 util::math::Mat4::operator*(const float & scalar) const
{
	return Mat4(
		col[0] * scalar,
		col[1] * scalar,
		col[2] * scalar,
		col[3] * scalar
	);
}

// multiplication operator for matrices
util::math::Mat4 util::math::Mat4::operator*(const Mat4 & m) const // dot product. rotate on the 'x' axis, then rotate on the 'y' axis
{
	// 'col' is the first matrix, and 'mat' the second matrix.
	return Mat4(
		col[0][0] * m[0][0] + col[0][1] * m[1][0] + col[0][2] * m[2][0] + col[0][3] * m[3][0],
		col[0][0] * m[0][1] + col[0][1] * m[1][1] + col[0][2] * m[2][1] + col[0][3] * m[3][1],
		col[0][0] * m[0][2] + col[0][1] * m[1][2] + col[0][2] * m[2][2] + col[0][3] * m[3][2],
		col[0][0] * m[0][3] + col[0][1] * m[1][3] + col[0][2] * m[2][3] + col[0][3] * m[3][3],

		col[1][0] * m[0][0] + col[1][1] * m[1][0] + col[1][2] * m[2][0] + col[1][3] * m[3][0],
		col[1][0] * m[0][1] + col[1][1] * m[1][1] + col[1][2] * m[2][1] + col[1][3] * m[3][1],
		col[1][0] * m[0][2] + col[1][1] * m[1][2] + col[1][2] * m[2][2] + col[1][3] * m[3][2],
		col[1][0] * m[0][3] + col[1][1] * m[1][3] + col[1][2] * m[2][3] + col[1][3] * m[3][3],

		col[2][0] * m[0][0] + col[2][1] * m[1][0] + col[2][2] * m[2][0] + col[2][3] * m[3][0],
		col[2][0] * m[0][1] + col[2][1] * m[1][1] + col[2][2] * m[2][1] + col[2][3] * m[3][1],
		col[2][0] * m[0][2] + col[2][1] * m[1][2] + col[2][2] * m[2][2] + col[2][3] * m[3][2],
		col[2][0] * m[0][3] + col[2][1] * m[1][3] + col[2][2] * m[2][3] + col[2][3] * m[3][3],

		col[3][0] * m[0][0] + col[3][1] * m[1][0] + col[3][2] * m[2][0] + col[3][3] * m[3][0],
		col[3][0] * m[0][1] + col[3][1] * m[1][1] + col[3][2] * m[2][1] + col[3][3] * m[3][1],
		col[3][0] * m[0][2] + col[3][1] * m[1][2] + col[3][2] * m[2][2] + col[3][3] * m[3][2],
		col[3][0] * m[0][3] + col[3][1] * m[1][3] + col[3][2] * m[2][3] + col[3][3] * m[3][3]
	);
}

// divides the mat4 by a scalar
util::math::Mat4 util::math::Mat4::operator/(const float & scalar) const { return *this * std::powf(scalar, -1); }

// += operator
util::math::Mat4 util::math::Mat4::operator+=(const Mat4 & m) { return *this = *this + m; }

// -= operator
util::math::Mat4 util::math::Mat4::operator-=(const Mat4 & m) { return *this = *this - m; }

// *= operator (scalar version)
util::math::Mat4 util::math::Mat4::operator*=(const float & flt) { return *this = *this * flt; }

// *= operator (matrix version)
util::math::Mat4 util::math::Mat4::operator*=(const Mat4 & mat) { return *this = *this * mat; }

// /= oprator
util::math::Mat4 util::math::Mat4::operator/=(const float & flt) { return *this = *this / flt; }


// gets the main diagonal
util::math::Vec4 util::math::Mat4::getMainDiagonal() const { return Vec4(col[0][0], col[1][1], col[2][2], col[3][3]); }

// adds together all values along the main diagonal
float util::math::Mat4::getTrace() const
{ 
	Vec4 v4 = getMainDiagonal();
	return (v4.x + v4.y + v4.z + v4.w);
}

// transposes the matrix
void util::math::Mat4::transpose() { *this = getTranspose(); }

// flips elements along the main diagonal
util::math::Mat4 util::math::Mat4::getTranspose() const
{
	Mat4 temp;
	// You can just cut out the column and paste it as the row for col to make it easier
	temp[0][0] = col[0][0];
	temp[1][0] = col[0][1];
	temp[2][0] = col[0][2];
	temp[3][0] = col[0][3];

	temp[0][1] = col[1][0];
	temp[1][1] = col[1][1];
	temp[2][1] = col[1][2];
	temp[3][1] = col[1][3];

	temp[0][2] = col[2][0];
	temp[1][2] = col[2][1];
	temp[2][2] = col[2][2];
	temp[3][2] = col[2][3];

	temp[0][3] = col[3][0];
	temp[1][3] = col[3][1];
	temp[2][3] = col[3][2];
	temp[3][3] = col[3][3];

	return temp;
}

// gets the determinant
float util::math::Mat4::getDeterminant() const
{
	// a, b, c, d
	// e, f, g, h
	// i, j, k, l
	// m, n, o, p

	// a [mat3] - b[mat3] + c[mat3] - d[mat3]

	return getMinor(0, 0).getDeterminant() * col[0][0] - getMinor(0, 1).getDeterminant() * col[0][1] + getMinor(0, 2).getDeterminant() * col[0][2] + getMinor(0, 3).getDeterminant() * col[0][3];
}

// returns a minor. If the row or column are out of bounds, an empty matrix is returned.
util::math::Mat3 util::math::Mat4::getMinor(unsigned int row, unsigned int column) const
{
	// gets the minor as a 4 X 4 matrix, returning a 3 X 3
	float * tempArr = getMinorValues(toArray(), 4, 4, row, column);

	return Mat3(tempArr[0], tempArr[1], tempArr[2], tempArr[3], tempArr[4], tempArr[5], tempArr[6], tempArr[7], tempArr[8]);

	//Mat3 tempMatrix; // temporary matrix
	//float arr[9]; // gets the values that will be saved to the matrix3.
	//int index = 0; // index of the array

	//// checking for valid input. If the row or column are out of bounds, thenan empty matrix is returned.
	//try
	//{
	//	if (row > 3 || column > 3)
	//		throw "Index out of bonds. Returning empty Matrix.";
	//}
	//catch (const char * ex)
	//{
	//	std::cout << ex << std::endl;
	//	return Mat3();
	//}

	//for (int r = 0; r < 4; r++)
	//{
	//	for (int c = 0; c < 4; c++)
	//	{
	//		if (!(r == row || c == column))
	//		{
	//			arr[index] = col[r][c];
	//			index++;
	//		}
	//	}
	//}

	//tempMatrix = Mat3(arr[0], arr[1], arr[2], arr[3], arr[4], arr[5], arr[6], arr[7], arr[8]);

	//return tempMatrix;
}

// gets an identity matrix
util::math::Mat4 util::math::Mat4::getIdentityMatrix()
{
	return Mat4(
		1.0F, 0.0F, 0.0F, 0.0F,
		0.0F, 1.0F, 0.0F, 0.0F,
		0.0F, 0.0F, 1.0F, 0.0F,
		0.0F, 0.0F, 0.0F, 1.0F
	);
}

// gets the matrix as an array of length 16.
float * util::math::Mat4::toArray() const
{
	float * arr = new float[16]; // gets the matrix as 1D array.

	arr[0] = col[0][0];
	arr[1] = col[0][1];
	arr[2] = col[0][2];
	arr[3] = col[0][3];

	arr[4] = col[1][0];
	arr[5] = col[1][1];
	arr[6] = col[1][2];
	arr[7] = col[1][3];

	arr[8] = col[2][0];
	arr[9] = col[2][1];
	arr[10] = col[2][2];
	arr[11] = col[2][3];

	arr[12] = col[3][0];
	arr[13] = col[3][1];
	arr[14] = col[3][2];
	arr[15] = col[3][3];

	return arr;
}

// gets the matrix was a string
std::string util::math::Mat4::toString() const
{
	std::string mat = "";

	for (int r = 0; r < 4; r++)
	{
		for (int c = 0; c < 4; c++)
		{
			mat += std::to_string(col[r][c]) + "\t";
		}

		mat += "\n";
	}

	return mat;
}