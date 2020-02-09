// PrimitiveSphere
#include "PrimitiveSphere.h"
#include "..\utils\math\Rotation.h"
#include "..\physics/PhysicsBody.h"

#include <iostream>

cherry::PrimitiveUVSphere::PrimitiveUVSphere(float radius, unsigned int segRows, unsigned int segCols, Vec4 color) : Primitive(), radius(abs(radius))
{
	// making sure the minimum amount of values were given. This only works if the object is greater than t
	if (segRows < 3)
		segRows = 3;
	if (segCols < 3)
		segCols = 3;

	// makes sure the radius has its absolute value.
	radius = abs(radius);

	// colour of the sphere
	glm::vec4 clr = glm::vec4(
		(color.v.x < 0.0F) ? 0.0F : (color.v.x > 1.0F) ? 1.0F : color.v.x,
		(color.v.y < 0.0F) ? 0.0F : (color.v.y > 1.0F) ? 1.0F : color.v.y,
		(color.v.z < 0.0F) ? 0.0F : (color.v.z > 1.0F) ? 1.0F : color.v.z,
		(color.v.w < 0.0F) ? 0.0F : (color.v.w > 1.0F) ? 1.0F : color.v.w
	);

	color = clr; // saving the base colour

	// Polygon Setup
	// rings are verticle portions (i.e. rows)
	// segments are horizontal portions (i.e. columns)
	
	unsigned int index = 1; // the index of a vector. This is used for both setting up vertices and indices.

	float rotateX = 0.0F; // rotation on the x-axis; used for vertex positions
	float rotateZ = 0.0F; // rotation on the z-axis; used for vertex positions

	float rxInc = glm::radians(180.0F / (float)segRows); // increment for rotations around x-axis
	float rzInc = glm::radians(360.0F / (float)segCols); // increment for rotations around z-axis

	// the three indices being used.
	uint32_t ind0 = 0; // used for the triangles at the top and bottom of the sphere.
	uint32_t ind1 = 0; // used for drawing indices for all triangles.
	uint32_t ind2 = 0; // used for drawing indices for all triangles.

	/*
	 * e.g. to get vertices (row * col), the column number determines how much gets subtracted. For example
	 ***	3 x 3 = -1 from segRows * segCols (3 x 3 - 1) (8 vertices)
	 ***	3 x 4 = -2 from segRows * segCols (3 x 4 - 2) (10 vertices)
	 ***	4 x 3 = -1 from segRows * segCols (4 x 3 - 1) (11 vertices)
	 ***	4 x 4 = -2 from segRows * segCols (4 x 4 - 2) (14 vertices)
	 ***	4 x 5 = -3 from segRows * segCols (4 x 5 - 3) (17 vertices)
	 ***	5 x 4 = -2 from segRows * segCols (5 x 4 - 2) (18 vertices)
	 ***	5 x 5 = -3 from segRows * segCols (5 x 5 - 3) (22 vertices)
	*/
	

	// total amount of vertices
	// (segRows * segCols): calculates the base amount of vertices, which assumes the vertex amount is the same for all rows.
	// (segCols - 2): accounts for the single vertices at the top and bottom of the sphere, as those "rows" have a different amount from the rest.

	verticesTotal = (segRows * segCols) - ((segCols) - 2); // segRows * segCols - ((segCols) - 2)
	vertices = new Vertex[verticesTotal];

	/*
	 * e.g. the top and bottom of the sphere don't double the amount of tris used per face.
	 ***	3 x 3 = (3 * 2 + (3 - 2) * 3 * 2) * 3 = 12 triangles and 36 indices
	 ***	3 x 4 = (4 * 2 + (3 - 2) * 4 * 2) * 3 = 16 triangles and 48 indices
	 ***	4 x 3 = (3 * 2 + (4 - 2) * 3 * 2) * 3 = 18 triangles and 54 indices
	 ***	4 x 4 = (4 * 2 + (4 - 2) * 4 * 2) * 3 = 24 triangles and 72 indices
	 ***	4 x 5 = (5 * 2 + (4 - 2) * 5 * 2) * 3 = 30 triangles and 90 indices
	 ***	5 x 4 = (4 * 2 + (5 - 2) * 4 * 2) * 3 = 32 triangles and 96 indices
	 ***	5 x 5 = (5 * 2 + (5 - 2) * 5 * 2) * 3 = 40 triangles and 120 indices
	*/

	// segCols * 2: accounts for the triangles at the top and bottom "rows", which all come to a single point on the top and bottom respectively. 
	// ((segRows - 2) * segCols * 2): gets the triangle total for the rest of the rows. Multiplies by (2) since two tris make a quad.
	// (...) * 3: multiplies by 3 because three points are used to draw an individual triangle.
	indicesTotal = (segCols * 2 + ((segRows - 2) * segCols * 2)) * 3; // (segCols * 2 + (segRows - 2) * segCols * 2) * 3 
	indices = new uint32_t[indicesTotal];


	// order (allows for simple addition, e.g. (1, 2, 3), (2, 3, 4), (3, 4, 5)
	// double lines represent a new set of points for dual triangles
	// 5 x 3

	// DOES NOT WORK
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
	// ALSO DOESN'T WORK
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

	// top vertex
	vertices[0] = { {0.0F, 0.0F, radius}, {clr}, {0.0F, 0.0F, 0.0F} }; // top vertex

	index = 1;
	rotateX += rxInc; // sets up first set of vertices
	// rotates on the x-axis for the row rotation (z-axis and y-axis positions change)
	for (unsigned int row = 1; row < segRows && index < verticesTotal - 1; row++) // goes through each row
	{
		for (unsigned int col = 0; col < segCols && index < verticesTotal - 1; col++) // goes through each column
		{
			util::math::Vec3 posVec(0.0F, 0.0F, radius);

			util::math::Vec3 normVec(0.0F, 0.0F, 1.0F); // vector uused for normals



			// also determines radius at the given point on the sphere.
			posVec = util::math::rotateX(posVec, rotateX, false); // rotates around the x-axis so that the z-position is correct
			posVec = util::math::rotateZ(posVec, rotateZ, false); // rotates around the z-axis so that the (x, y) positions are correct

			// rotates the normals in the same fashion.
			normVec = util::math::rotateX(normVec, rotateX, false);
			normVec = util::math::rotateZ(normVec, rotateZ, false);

			vertices[index] = { {posVec.x, posVec.y, posVec.z}, {clr}, {0.0F, 0.0F, 0.0F} };

			rotateZ += rzInc; // adding to the z-rotation
			index++;
		}

		rotateZ = 0.0F; // resetting the z-rotation
		rotateX += rxInc;
	}

	vertices[index] = { {0.0F, 0.0F, -radius}, {clr}, {0.0F, 0.0F, 0.0F} }; // bottom vertex of the sphere
	
	// starting values for the indice drawing.
	

	// For this version, row 0 is increased by 1

	// Drawing Order
	//	- the indice values increase by 1 per column
	//	- the indices in the following row are the previous row values plus the amount of columns
	/*
	 * e.g. a 4 X 4 sphere
	 *			  0
	 *		1	2	3	4
	 *		5	6	7	8
	 *			  9
	*/

	index = 0;
	ind0 = 0;
	ind1 = 1;
	ind2 = 2;
	// drawing the indices.
	for (int row = 0; row < segRows && index < indicesTotal; row++)
	{
		for (int col = 0; col < segCols && index < indicesTotal; col++)
		{
			// first row
			if (row == 0)
			{
				// top point -> bottom left -> bottom right
				indices[index] = ind0;
				indices[++index] = ind1;

				// because of the drawing order, an edge ends up not getting drawn, leaving a face missing.
				// this is just to fix that problem by making sure the indices are all correct for the final top face.
				// this only happens with the top of the sphere. The bottom of the sphere works fine.
				if (col == segCols - 1)
					indices[++index] = ind0 + 1;
				else
					indices[++index] = ind2;

				ind1++;
				ind2++;
				index++;
			}
			// final row
			else if (row == segRows - 1)
			{
				// top left -> bottom point -> top right
				indices[index] = ind1 - segCols;
				indices[++index] = ind0;

				if (col == segCols - 1) // if on the final column of the final row
					indices[++index] = ind2 - segCols * 2;
				else
					indices[++index] = ind2 - segCols;

				ind1++;
				ind2++;
				index++;
			}
			else // other rows
			{
				if (col == segCols - 1)
				{
					// triangle 1 (top left -> top right -> bottom left)
					indices[index] = ind1 - segCols; // ind1 - segCols;
					indices[++index] = ind1; // ind2 - segCols;
					indices[++index] = ind2 - segCols * 2; // ind1;

					// triangle 2 bottom left -> top right -> bottom right)
					indices[++index] = ind2 - segCols * 2; // ind1;
					indices[++index] = ind1; // ind2 - segCols;
					indices[++index] = ind2 - segCols; // ind2;
				}
				else
				{
					// triangle 1 (top left -> top right -> bottom left)
					indices[index] = ind1 - segCols; // ind1 - segCols;
					indices[++index] = ind1; // ind2 - segCols;
					indices[++index] = ind2 - segCols; // ind1;

					// triangle 2 bottom left -> top right -> bottom right)
					indices[++index] = ind1; // ind1;
					indices[++index] = ind2; // ind2 - segCols;
					indices[++index] = ind2 - segCols; // ind2;
				}


				ind1++;
				ind2++;
				index++;

				if (row == segRows - 2 && col == segCols - 1)
					ind0 = ind1; // will be used for setting final vertex value.					
			}
		}
	}

	CalculateNormals();
	InvertNormals();

	CalculateMeshBody(); // calculates the limits of the mesh body.

	// Create a new mesh from the data
	mesh = std::make_shared<Mesh>(vertices, verticesTotal, indices, indicesTotal);
	// AddPhysicsBody(new cherry::PhysicsBodySphere(radius));
}

// gets the radius
float cherry::PrimitiveUVSphere::GetRadius() const { return radius; }
