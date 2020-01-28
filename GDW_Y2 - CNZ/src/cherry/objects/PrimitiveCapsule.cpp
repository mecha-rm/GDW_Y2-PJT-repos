// PrimitiveCapsule
#include "PrimitiveCapsule.h"
#include "..\VectorCRY.h"
#include "..\utils\math\Rotation.h"

cherry::PrimitiveCapsule::PrimitiveCapsule(float radius, float height, unsigned int cylSegments, unsigned int capSegments, cherry::Vec4 color)
	: radius(abs(radius)), height(abs(height))
{
	radius = abs(radius);

	height = abs(height);

	// todo: account for height
	// PrimitiveUVSphere tempSphere(cylRadius, (capSegments + 1) * 2, cylSegments);

		// making sure the minimum amount of values were given. This only works if the object is greater than t
	if (cylSegments < 3) // columns
		cylSegments = 3;
	if (capSegments < 1) // rows
		capSegments = 1;
	
	// accounts for half-circle and middle
	capSegments = (capSegments * 2 + 1);

	// bounds checking for the colour
	color.v.x = (color.v.x < 0.0F) ? 0.0F : (color.v.x > 1.0F) ? 1.0F : color.v.x;
	color.v.y = (color.v.y < 0.0F) ? 0.0F : (color.v.y > 1.0F) ? 1.0F : color.v.y;
	color.v.z = (color.v.z < 0.0F) ? 0.0F : (color.v.z > 1.0F) ? 1.0F : color.v.z;
	color.v.w = (color.v.w < 0.0F) ? 0.0F : (color.v.w > 1.0F) ? 1.0F : color.v.w;
	
	this->color = color; // saving the colour.

	// Polygon Setup
	// rings are verticle portions (i.e. rows)
	// segments are horizontal portions (i.e. columns)

	unsigned int index = 1; // the index of a vector. This is used for both setting up vertices and indices.

	float rotateX = 0.0F; // rotation on the x-axis; used for vertex positions
	float rotateZ = 0.0F; // rotation on the z-axis; used for vertex positions

	float rxInc = glm::radians(180.0F / (float)capSegments); // increment for rotations around x-axis
	float rzInc = glm::radians(360.0F / (float)cylSegments); // increment for rotations around z-axis

	// the three indices being used.
	uint32_t ind0 = 0; // used for the triangles at the top and bottom of the sphere.
	uint32_t ind1 = 0; // used for drawing indices for all triangles.
	uint32_t ind2 = 0; // used for drawing indices for all triangles.

	/*
	 * e.g. to get vertices (row * col), the column number determines how much gets subtracted. For example
	 ***	3 x 3 = -1 from capSegments * cylSegments (3 x 3 - 1) (8 vertices)
	 ***	3 x 4 = -2 from capSegments * cylSegments (3 x 4 - 2) (10 vertices)
	 ***	4 x 3 = -1 from capSegments * cylSegments (4 x 3 - 1) (11 vertices)
	 ***	4 x 4 = -2 from capSegments * cylSegments (4 x 4 - 2) (14 vertices)
	 ***	4 x 5 = -3 from capSegments * cylSegments (4 x 5 - 3) (17 vertices)
	 ***	5 x 4 = -2 from capSegments * cylSegments (5 x 4 - 2) (18 vertices)
	 ***	5 x 5 = -3 from capSegments * cylSegments (5 x 5 - 3) (22 vertices)
	*/


	// total amount of vertices
	// (capSegments * cylSegments): calculates the base amount of vertices, which assumes the vertex amount is the same for all rows.
	// (cylSegments - 2): accounts for the single vertices at the top and bottom of the sphere, as those "rows" have a different amount from the rest.

	verticesTotal = (capSegments * cylSegments) - ((cylSegments)-2); // capSegments * cylSegments - ((cylSegments) - 2)
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

	// cylSegments * 2: accounts for the triangles at the top and bottom "rows", which all come to a single point on the top and bottom respectively. 
	// ((capSegments - 2) * cylSegments * 2): gets the triangle total for the rest of the rows. Multiplies by (2) since two tris make a quad.
	// (...) * 3: multiplies by 3 because three points are used to draw an individual triangle.
	indicesTotal = (cylSegments * 2 + ((capSegments - 2) * cylSegments * 2)) * 3; // (cylSegments * 2 + (capSegments - 2) * cylSegments * 2) * 3 
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

	// ALSO DOES NOT WORK
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
	vertices[0] = { {0.0F, 0.0F, height / 2.0F}, {color.v.x, color.v.y, color.v.z, color.v.w}, {0.0F, 0.0F, 0.0F} }; // top vertex

	index = 1;
	rotateX += rxInc; // sets up first set of vertices
	// rotates on the x-axis for the row rotation (z-axis and y-axis positions change)
	for (unsigned int row = 1; row < capSegments && index < verticesTotal - 1; row++) // goes through each row
	{
		for (unsigned int col = 0; col < cylSegments && index < verticesTotal - 1; col++) // goes through each column
		{
			// vector for getting position
			util::math::Vec3 posVec(0.0F, 0.0F, radius);

			// vector for getting normals
			util::math::Vec3 normVec(0.0F, 0.0F, 1.0F);

			// also determines radius at the given point on the sphere.
			posVec = util::math::rotateX(posVec, rotateX, false); // rotates around the x-axis so that the z-position is correct
			posVec = util::math::rotateZ(posVec, rotateZ, false); // rotates around the z-axis so that the (x, y) positions are correct

			// determines the normal position
			normVec = util::math::rotateX(normVec, rotateX, false); // rotates around the x-axis so that the z-position is correct
			normVec = util::math::rotateZ(normVec, rotateZ, false); // rotates around the z-axis so that the (x, y) positions are correct

			// adjusts the height on the z-value for the capsule
			// radius * 2 to account for the top and bottom cap
			posVec.z += (row < round(capSegments / 2.0F)) ? (height - radius * 2) / 2.0F : -(height - radius * 2)/ 2.0F;

			vertices[index] = { {posVec.x, posVec.y, posVec.z}, {color.v.x, color.v.y, color.v.z, color.v.w}, {0.0F, 0.0F, 0.0F} };

			rotateZ += rzInc; // adding to the z-rotation
			index++;
		}

		rotateZ = 0.0F; // resetting the z-rotation
		rotateX += rxInc;
	}

	vertices[index] = { {0.0F, 0.0F, -height / 2.0F}, {color.v.x, color.v.y, color.v.z, color.v.w}, {0.0F, 0.0F, 0.0F} }; // bottom vertex of the sphere

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
	for (int row = 0; row < capSegments && index < indicesTotal; row++)
	{
		for (int col = 0; col < cylSegments && index < indicesTotal; col++)
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
				if (col == cylSegments - 1)
					indices[++index] = ind0 + 1;
				else
					indices[++index] = ind2;

				ind1++;
				ind2++;
				index++;
			}
			// final row
			else if (row == capSegments - 1)
			{
				// top left -> bottom point -> top right
				indices[index] = ind1 - cylSegments;
				indices[++index] = ind0;

				if (col == cylSegments - 1) // if on the final column of the final row
					indices[++index] = ind2 - cylSegments * 2;
				else
					indices[++index] = ind2 - cylSegments;

				ind1++;
				ind2++;
				index++;
			}
			else // other rows
			{
				if (col == cylSegments - 1)
				{
					// triangle 1 (top left -> top right -> bottom left)
					indices[index] = ind1 - cylSegments; // ind1 - cylSegments;
					indices[++index] = ind1; // ind2 - cylSegments;
					indices[++index] = ind2 - cylSegments * 2; // ind1;

					// triangle 2 bottom left -> top right -> bottom right)
					indices[++index] = ind2 - cylSegments * 2; // ind1;
					indices[++index] = ind1; // ind2 - cylSegments;
					indices[++index] = ind2 - cylSegments; // ind2;
				}
				else
				{
					// triangle 1 (top left -> top right -> bottom left)
					indices[index] = ind1 - cylSegments; // ind1 - cylSegments;
					indices[++index] = ind1; // ind2 - cylSegments;
					indices[++index] = ind2 - cylSegments; // ind1;

					// triangle 2 bottom left -> top right -> bottom right)
					indices[++index] = ind1; // ind1;
					indices[++index] = ind2; // ind2 - cylSegments;
					indices[++index] = ind2 - cylSegments; // ind2;
				}
				

				ind1++;
				ind2++;
				index++;

				if (row == capSegments - 2 && col == cylSegments - 1)
					ind0 = ind1; // will be used for setting final vertex value.					
			}
		}
	}

	// calculating the normals
	CalculateNormals();
	InvertNormals();

	CalculateMeshBody(); // calculates the limits of the mesh body.

	// Create a new mesh from the data
	mesh = std::make_shared<Mesh>(vertices, verticesTotal, indices, indicesTotal);
}

// gets the radius
float cherry::PrimitiveCapsule::GetRadius() const { return radius; }

// gets the height
float cherry::PrimitiveCapsule::GetHeight() const { return height; }
