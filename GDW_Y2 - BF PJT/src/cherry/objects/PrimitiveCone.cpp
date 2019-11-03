#include "PrimitiveCone.h"
#include "..\VectorCRY.h"
#include "..\utils\math\Rotation.h"

cherry::PrimitiveCone::PrimitiveCone(float radius, float height, unsigned int segments)
{
	if (segments < 3)
		segments = 3;

	float rFactor = 0; // the rotation factor
	float rInc = glm::radians(360.0F / (float)segments);

	int indiIndex = 0; // index used for indices
	uint32_t ind0; // indice 0
	uint32_t ind1;
	uint32_t ind2;

	// vertices
	verticesTotal = segments + 2; // centre vertex, and highest vertex
	vertices = new Vertex[verticesTotal];

	// indices
	indicesTotal = segments * 2 * 3; // three indices per triangle, needed for the base and height of the cone.
	indices = new uint32_t[indicesTotal];

	// top vertex. The normal points directly upwards
	vertices[0] = { { 0.0F, 0.0F, height / 2.0F}, {1.0F, 1.0F, 1.0F, 1.0F}, {0.0F, 0.0F, 1.0F} };

	// adds in all the vertices
	for (int i = 1; i < verticesTotal - 1; i++)
	{
		// rotates the position vector
		// NOTE: as long as radius is set for (x) or (y), it doesn't matter.
		cherry::Vec3 posVec = util::math::rotateZ(util::math::Vec3(radius, 0.0F, -height / 2.0F), rFactor);

		// calculates the normal by rotating it to account for the vertex position, and the angle of the normal itself.
		cherry::Vec3 normVec = util::math::rotateZ(util::math::Vec3(0.0F, 0.0F, -1.0F), rFactor);
		normVec = util::math::rotateX(util::math::Vec3(normVec.getX(), normVec.getY(), normVec.getZ()), glm::radians(45.0F));

		vertices[i] = { {posVec.v->x, posVec.v->y, posVec.v->z}, {1.0F, 1.0F, 1.0F, 1.0F}, {normVec.getX(), normVec.getY(), normVec.getZ()} };

		rFactor += rInc; // increases the rotation factor
	}

	// bottom, centre vertex.
	vertices[verticesTotal - 1] = { { 0.0F, 0.0F, -height / 2.0F}, {1.0F, 1.0F, 1.0F, 1.0F}, {0.0F, 0.0F, -1.0F} };

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
				indices[indiIndex] = ind0; // 0
				indices[++indiIndex] = ind1 - segments;
				
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

	// creating the mesh
	mesh = std::make_shared<Mesh>(vertices, verticesTotal, indices, indicesTotal);

	// setting the position
	// setPosition(pos.v->x, pos.v->y, pos.v->z);
}