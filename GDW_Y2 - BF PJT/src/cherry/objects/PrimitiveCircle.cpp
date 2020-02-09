// PrimitiveCircle - used for making circles
#include "PrimitiveCircle.h"
#include "..\utils\math\Rotation.h"

// creates a circle
cherry::PrimitiveCircle::PrimitiveCircle(float radius, unsigned int edges, bool doubleSided, cherry::Vec4 color) : radius(abs(radius))
{
	radius = abs(radius);

	if (edges < 3)
		edges = 3;

	// bounds checking for the colour
	color.v.x = (color.v.x < 0.0F) ? 0.0F : (color.v.x > 1.0F) ? 1.0F : color.v.x;
	color.v.y = (color.v.y < 0.0F) ? 0.0F : (color.v.y > 1.0F) ? 1.0F : color.v.y;
	color.v.z = (color.v.z < 0.0F) ? 0.0F : (color.v.z > 1.0F) ? 1.0F : color.v.z;
	color.v.w = (color.v.w < 0.0F) ? 0.0F : (color.v.w > 1.0F) ? 1.0F : color.v.w;

	this->color = color; // saving the colour.

	float rFactor = 0; // the rotation factor
	float rInc = glm::radians(360.0F / (float)edges);

	unsigned int indiIndex = 0; // the index used for the indices

	// vertices
	verticesTotal = edges + 1; // centre vertex, plush a certain amount of edges
	vertices = new Vertex[verticesTotal];

	// indices
	indicesTotal = edges * 3; // three indices per triangle
	indices = new uint32_t[indicesTotal];
	
	// centre vertex (position, colour, and normal)
	vertices[0] = { { 0.0F, 0.0F, 0.0F}, {color.v.x, color.v.y, color.v.z, color.v.w}, {0.0F, 0.0F, 1.0F} };

	// adds in all the vertices
	for (int i = 1; i < verticesTotal && indiIndex < indicesTotal; i++)
	{
		// rotates the vector
		// NOTE: as long as radius is set for (x) or (y), it doesn't matter.
		cherry::Vec3 tempVec = util::math::rotateZ(util::math::Vec3(radius, 0.0F, 0.0F), rFactor, false);

		// vertex (position, colour, and normal)
		vertices[i] = { {tempVec.v.x, tempVec.v.y, tempVec.v.z}, {color.v.x, color.v.y, color.v.z, color.v.w}, {0.0F, 0.0F, 1.0F} };
		
		// centre -> top left -> top right
		indices[indiIndex] = 0;
		indices[++indiIndex] = i;

		// final triangle
		if (i == verticesTotal - 1)
			indices[++indiIndex] = 1;
		else // other trangles
			indices[++indiIndex] = i + 1;
		
		rFactor += rInc; // increases the rotation factor
		indiIndex++; // increases the index for indices
	}

	CalculateMeshBody(); // calculates the limits of the mesh body.

	// creating the mesh
	mesh = std::make_shared<Mesh>(vertices, verticesTotal, indices, indicesTotal);
	mesh->cullFaces = !doubleSided;
	// setting the position
	// setPosition(pos.v->x, pos.v->y, pos.v->z);
}

// gets the radius
float cherry::PrimitiveCircle::GetRadius() const { return radius; }
