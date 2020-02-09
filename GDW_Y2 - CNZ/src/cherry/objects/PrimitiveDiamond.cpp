// PrimitiveDiamond
#include "PrimitiveDiamond.h"
#include "..\VectorCRY.h"
#include "..\utils\math\Rotation.h"

// creates a diamond; same calculation as capsules.
cherry::PrimitiveDiamond::PrimitiveDiamond(float radius, float height, unsigned int segments, float origin, cherry::Vec4 color)
	: radius(abs(radius)), height(abs(height))
{
	// absolute values
	radius = abs(radius);
	height = abs(height);

	// minimum
	if (segments < 3)
		segments = 3;

	// cannot be less than 0 or greater than 1
	if (origin < 0.0F)
		origin = 0.0F;
	else if (origin > 1.0F)
		origin = 1.0F;

	// colour bounds checking
	color.v.x = (color.v.x < 0.0F) ? 0.0F : (color.v.x > 1.0F) ? 1.0F : color.v.x;
	color.v.y = (color.v.y < 0.0F) ? 0.0F : (color.v.y > 1.0F) ? 1.0F : color.v.y;
	color.v.z = (color.v.z < 0.0F) ? 0.0F : (color.v.z > 1.0F) ? 1.0F : color.v.z;
	color.v.w = (color.v.w < 0.0F) ? 0.0F : (color.v.w > 1.0F) ? 1.0F : color.v.w;

	this->color = color; // saving the colour.

	float rFactor = 0; // the rotation factor
	float rInc = glm::radians(360.0F / (float)segments);

	// gets the placement of the origin.
	float originLoc = (1 - origin) * (-height / 2.0F) + (origin) * (height/2.0F); // p = (1 - t)(p0) + (t)(p1)

	int indiIndex = 0; // index used for indices
	uint32_t ind0; // indice 0 (top/bottom point)
	uint32_t ind1; // indice 1
	uint32_t ind2; // indice 2

	// vertices
	verticesTotal = segments + 2; // centre vertex, and highest vertex
	vertices = new Vertex[verticesTotal];

	// indices
	indicesTotal = segments * 2 * 3; // three indices per triangle, needed for the base and height of the cone.
	indices = new uint32_t[indicesTotal];

	// top vertex
	vertices[0] = { { 0.0F, 0.0F, height / 2.0F}, {color.v.x, color.v.y, color.v.z, color.v.w}, {0.0F, 0.0F, 0.0F} };

	// adds in all the vertices
	for (int i = 1; i < verticesTotal - 1; i++)
	{

		// rotates the position vector
		// NOTE: as long as radius is set for (x) or (y), it doesn't matter.
		cherry::Vec3 posVec = util::math::rotateZ(util::math::Vec3(radius, 0.0F, originLoc), rFactor, false);
		
		// rotates the normal vector
		cherry::Vec3 normVec = util::math::rotateZ(util::math::Vec3(1.0F, 0.0F, 0.0F), rFactor, false);

		vertices[i] = { {posVec.v.x, posVec.v.y, posVec.v.z}, {color.v.x, color.v.y, color.v.z, color.v.w}, {0.0F, 0.0F, 0.0F} };

		rFactor += rInc; // increases the rotation factor
	}

	// bottom, centre vertex.
	vertices[verticesTotal - 1] = { { 0.0F, 0.0F, -height / 2.0F}, {color.v.x, color.v.y, color.v.z, color.v.w}, {0.0F, 0.0F, 0.0F} };

	ind0 = 0;
	ind1 = 1;
	ind2 = 2;
	indiIndex = 0;
	// adding in the indices
	for (int i = 0; i < 2; i++) // point and base
	{
		for (int j = 0; j < verticesTotal - 2; j++) // point or base
		{
			// elevated portion connected to the top of the cone.
			if (i <= 0)
			{
				// draws the triangle from the highest point down.
				indices[indiIndex] = ind0; // 0
				indices[++indiIndex] = ind1;

				// final triangle
				if (j == verticesTotal - 3)
				{
					indices[++indiIndex] = 1;
					ind0 = ind1 + 1;
				}
				else // other trangles
				{
					indices[++indiIndex] = ind2;
				}
			}
			else if (i >= 1) // base
			{
				// draws the triangle from the highest point down.
				indices[indiIndex] = ind1 - segments; // ind0
				indices[++indiIndex] = ind0; // ind1 - segments;

				// final triangle
				if (j == verticesTotal - 3)
				{
					indices[++indiIndex] = 1;
					ind0 = ind1 + 1;
				}
				else // other trangles
				{
					indices[++indiIndex] = ind2 - segments;
				}
			}

			ind1++;
			ind2++;
			indiIndex++;
		}
	}

	CalculateNormals();
	InvertNormals();

	CalculateMeshBody(); // calculates the limits of the mesh body.

	// creating the mesh
	mesh = std::make_shared<Mesh>(vertices, verticesTotal, indices, indicesTotal);

	// setting the position
	// setPosition(pos.v->x, pos.v->y, pos.v->z);
}

// gets the radius
float cherry::PrimitiveDiamond::GetRadius() const { return radius; }

// gets the height
float cherry::PrimitiveDiamond::GetHeight() const { return height; }
