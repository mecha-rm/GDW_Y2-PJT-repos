#include "PrimitiveSphere.h"
#include "..\utils\math\Rotation.h"
#include <iostream>

pc::PrimitiveSphere::PrimitiveSphere(float radius, unsigned int segRows, unsigned int segCols) : Primitive()
{
	// making sure the minimum amount of values were given
	if (segRows < 3)
		segRows = 3;
	if (segCols < 3)
		segCols = 3;

	// SETUP ANGLES
	// rings are verticle portions (i.e. rows)
	// segments are horizontal (i.e. columns)

	// (2) + segRows * segCols
	// Two is added to account for the vertices at the top and bottom of the sphere respectively.
	
	unsigned int index = 1; // the index of the vector
	float segWidth = radius / (float)segCols; // the width of each segment (should be radius / segCols)
	float segHeight = radius / (float)segRows; // the height of each segment (should be radius / segHeight)

	float rotateX = 0.0F; // rotation on the x-axis
	float rotateZ = 0.0F; // rotation on the z-axis

	float rxInc = glm::radians(180.0F / (float)segRows); // increment for rotations around x-axis
	float rzInc = glm::radians(360.0F / (float)segCols); // crement for rotations around y-axis

	uint32_t** indiArr = nullptr;
	uint32_t val = 0; // a value used to put values into indiArr

	// Position and Color (Default Values)
	// e.g. to get vertices (row * col), the column number determines how much gets subtracted. For example
	// 3 x 3 = -1 from segRows * segCols (3 x 3 - 1) (8 vertices)
	// 3 x 4 = -2 from segRows * segCols (3 x 4 - 2) (10 vertices)
	// 4 x 3 = -1 from segRows * segCols (4 x 3 - 1) (11 vertices)
	// 4 x 4 = -2 from segRows * segCols (4 x 4 - 2) (14 vertices)
	// 4 x 5 = -3 from segRows * segCols (4 x 5 - 3) (17 vertices)
	// 5 x 4 = -2 from segRows * segCols (5 x 4 - 2) (18 vertices)
	// 5 x 5 = -3 from segRows * segCols (5 x 5 - 3) (22 vertices)
	verticesTotal = (segRows * segCols) - ((segCols) - 2); // segRows * segCols - ((segCols) - 2)
	vertices = new Vertex[verticesTotal];

	// e.g. the top and bottom of the sphere don't double the amount of tris used per face. fOR THIS, WE'RE DOING THE SAME THING
	// 3 x 3 = (3 + 1) * 3 (12 indices)
	// 3 x 4 = (3 + 1) * 4 (16 indices)
	// 4 x 3 = (4 + 2) * 3 (18 indices)
	// 4 x 4 = (4 + 2) * 4 (24 indices)
	// 4 x 5 = (4 + 2) * 5 (30 incides)
	// 5 x 4 = (5 + 3) * 4 (32 indices)
	// 5 x 5 = (5 + 3) * 5 (40 indices)
	indicesTotal = (segRows + (segRows - 2)) * segCols; // (segRows + (segRows - 2)) * segCols
	indices = new uint32_t[indicesTotal];
	
	// sets amount of rows, then sets each column
	// the final column is just a repeat of the first column, which exists so the game doesn't have to loop around.
	indiArr = new uint32_t * [segRows];
	for(int i = 0; i < segRows; i++)
		indiArr[i] = new uint32_t[segCols + 1];

	// order (allows for simple addition, e.g. (1, 2, 3), (2, 3, 4), (3, 4, 5)
	// double lines represent a new set of points for dual triangles
	// 5 x 3

	// Ver. 1 (Down -> Up and Right -> Down) - just increases linerarly after
	//	0 (fill)
	//  --------------------------
	//	1 | 3 || 3 | 5 || 5 | 7 ||
	//	2 | 4 || 4 | 6 || 6 | 8 ||
	//	--------------------------
	//	2 | 4 || 4 | 6 || 6 | 8 ||
	//	9 |10 ||11 |12 ||13 |14 ||
	//	--------------------------
	//	9 |10 ||11 |12 ||13 |14 ||
	//	15|16 ||17 |18 ||19 |20 ||
	//  --------------------------
	//	21 (fill)

	// Ver. 2 (Right -> Down + Left -> Right) ~ just increases linerarly after
	// For this version, row 0 is increased by 1
	//	0 (fill)
	//  --------------------------
	//	1 | 2 || 2 | 5 || 5 | 7 ||
	//	3 | 4 || 4 | 6 || 6 | 8 ||
	//	--------------------------
	//	3 | 4 || 4 | 6 || 6 | 8 ||
	//	9 |10 ||11 |12 ||13 |14 ||
	//	--------------------------
	//	9 |10 ||11 |12 ||13 |14 ||
	//	15|16 ||17 |18 ||19 |20 ||
	//	--------------------------
	//	21 (fill)


	// INDICES MUST START FROM 0 AND HAVE ALL VALUE

	// set up array (
	// have a set of three points that are moved along said array so that the triangle is always drawn in the right order)

	vertices[0] = { {0.0F, 0.0F, 0.5F}, {1.0F, 1.0F, 1.0F, 1.0F} }; // top vertex

	int x = 0;
	index = 1;
	rotateX += rxInc; // sets up first set of vertices
	// rotates on the x-axis for the row rotation (z-axis and y-axis positions change)
	for (unsigned int row = 1; row < segRows && index < verticesTotal - 1; row++) // goes through each row
	{
		for (unsigned int col = 0; col < segCols && index < verticesTotal - 1; col++) // goes through each column
		{
			x++;
			util::math::Vec3 tempVec(0.0F, 0.0F, 0.5F);

			// also determines radius at the given point on the sphere.
			tempVec = util::math::rotateX(tempVec, rotateX, false); // rotates around the x-axis so that the z-position is correct

			tempVec = util::math::rotateZ(tempVec, rotateZ, false); // rotates around the z-axis so that the (x, y) positions are  correct

			vertices[index] = { {tempVec.x, tempVec.y, tempVec.z}, {1.0F, 1.0F, 1.0F, 1.0F} };

			// starts with vertices at the top of the sphere.

			// vertices[index] = { {-0.5F + segWidth * edge, 0.5F - segHeight * row, 0.0F}, {1.0F, 1.0F, 1.0F, 1.0F} };
			// vertices[index] = { {-0.5F + segWidth * edge, 0.5F - segHeight * row, 0.0F}, {1.0F, 1.0F, 1.0F, 1.0F} };

			//vertices[index] = { {-0.5F + segWidth, 0.5F - segHeight * row, -0.5F + segWidth}, {1.0F, 1.0F, 1.0F, 1.0F} };
			// vertices[index] = { {-0.5F + segWidth * edge, 0.5F - segHeight * row, -0.5F + segWidth * edge}, {1.0F, 1.0F, 1.0F, 1.0F} };

			rotateZ += rzInc;
			index++;
		}

		rotateZ = 0.0F;
		rotateX += rxInc;
	}

	vertices[index] = { {0.0F, 0.0F, -0.5F}, {1.0F, 1.0F, 1.0F, 1.0F} }; // bottom vertex of the sphere

	val = 0;
	// fills first row with all zeroes; recall that there is one extra column to account for connecting the last set of vertices to the first set
	/*for (int i = 0; i < segCols + 1; i++)
		indiArr[0][i] = val;*/

	// val = 1;
	// adds all the indice values to the array
	for (unsigned int row = 0; row < segRows; row++)
	{
		for (unsigned int col = 0; col < segCols + 1; col++)
		{
			indiArr[row][col] = val;
			if(row > 0 && row < segRows - 1) // first row and final row have the same values
				val++;
		}
	}

	for (int i = 0; i < segCols + 1; i++)
		indiArr[0][i] = val;
	// fills final row with the greatest value. Recall that there's one extra row.
	for (int i = 0; i < segCols + 1; i++)
		indiArr[segRows - 1][i] = val;
	

	// gets values from 2D array and puts them into the indices 1D array
	index = 0;
	int test = 0;
	for (unsigned int row = 1; row < segRows; row++)
	{
		for (unsigned int col = 0; col < segCols; col++)
		{
			if (row == 0) // first row
			{
				// (Top Point) -> (Bottom Left Point) -> (Bottom Right Point)
				indices[index] = indiArr[row][col];
				indices[++index] = indiArr[row + 1][col];
				indices[++index] = indiArr[row + 1][col + 1];

				test += 3;
			}
			else if (row > 0 && row < segRows - 1)
			{
				// (Top Left Corner) -> RIGHT -> (Top Right Corner) -> DOWN + LEFT -> (Bottom Left Corner)
				indices[index] = indiArr[row][col];
				indices[++index] = indiArr[row][col + 1];
				indices[++index] = indiArr[row + 1][col];

				// (Bottom Left Corner) -> LEFT + UP -> (Top Right Corner) -> DOWN -> (Bottom Right Corner)
				indices[++index] = indiArr[row + 1][col];
				indices[++index] = indiArr[row][col + 1];
				indices[++index] = indiArr[row + 1][col + 1];

				test += 6;
			}
			else if (row == segRows - 1) // final row
			{
				// uint32_t x = indiArr[row + 1][col];
				// uint32_t y = indiArr[row][col];
				// uint32_t z = indiArr[row][col + 1];

				// (Bottom Point) -> (Top Left Point) -> (Bottom Right Point)
				indices[index] = indiArr[row][col];
				indices[++index] = indiArr[row - 1][col];
				indices[++index] = indiArr[row - 1][col + 1];

				// x = indices[index];
				// y = indices[index - 1];
				// z = indices[index - 2];

				test += 3;
			}
			// setting up index for next loop
			index++;
			
		}
	}

	// for (int i = 0; i < verticesTotal; i++)
		// std::cout << "vertices [" << i << "]: " << util::math::Vec3(vertices[i].Position.x, vertices[i].Position.y, vertices[i].Position.z) << std::endl;
	
	for (int i = 0; i < indicesTotal; i++)
		std::cout << "indicies [" << i << "]: " << indices[i] << std::endl;

	// Create a new mesh from the data
	mesh = std::make_shared<Mesh>(vertices, verticesTotal, indices, indicesTotal);
}
