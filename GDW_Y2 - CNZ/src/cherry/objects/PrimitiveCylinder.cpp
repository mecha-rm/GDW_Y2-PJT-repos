// PrimitiveCylinder
#include "PrimitiveCylinder.h"
#include "..\utils\math\Rotation.h"

// creates a cylinder
cherry::PrimitiveCylinder::PrimitiveCylinder(float radius, float height, unsigned int segments, cherry::Vec4 color)
	: radius(abs(radius)), height(abs(height))
{
	radius = abs(radius);
	height = abs(height);

	// minimum amount of segments
	if (segments < 3)
		segments = 3;

	// bounds checking for the colour
	color.v.x = (color.v.x < 0.0F) ? 0.0F : (color.v.x > 1.0F) ? 1.0F : color.v.x;
	color.v.y = (color.v.y < 0.0F) ? 0.0F : (color.v.y > 1.0F) ? 1.0F : color.v.y;
	color.v.z = (color.v.z < 0.0F) ? 0.0F : (color.v.z > 1.0F) ? 1.0F : color.v.z;
	color.v.w = (color.v.w < 0.0F) ? 0.0F : (color.v.w > 1.0F) ? 1.0F : color.v.w;

	this->color = color; // saving the colour.

	float rFactor = 0; // the rotation factor
	float rInc = glm::radians(360.0F / (float)segments); // increment for rotation
	unsigned int index = 0;
	
	// the three indices being used.
	uint32_t ind0 = 0; 
	uint32_t ind1 = 0; 
	uint32_t ind2 = 0;

	// 2: the top and bottom vertex
	// sides * 2: the edges on the top and bottom of the object
	verticesTotal = 2 + segments * 2;
	vertices = new Vertex[verticesTotal];

	// (segments * 4) - x4 to account for the top, bottom, and double tris that make up each face on the side of the cylinder
	indicesTotal = (segments * 4) * 3; // segments * 4 * 3
	indices = new uint32_t[indicesTotal];

	// centre starting vertex
	vertices[0] = { {0.0F, 0.0F, height / 2.0F}, {color.v.x, color.v.y, color.v.z, color.v.w}, {0.0F, 0.0F, 0.0F} };

	index = 1;

	// goes through the top and bottom of cylinder vertices
	for (int i = 0; i < 2; i++)
	{
		// goes through each segment
		for (int j = 0; j < segments; j++)
		{
			cherry::Vec3 posVec; // position vector
			cherry::Vec3 normVec; // normal vector

			if (i <= 0) // top
			{
				posVec = util::math::rotateZ(util::math::Vec3(radius, 0.0F, height / 2.0F), rFactor, false);

				// rotating the normal vector so that it's in the right place and angled properly.
				normVec = util::math::rotateZ(util::math::Vec3(0.0F, 0.0F, 1.0F), rFactor, false);
				normVec = util::math::rotateX(util::math::Vec3(normVec.GetX(), normVec.GetY(), normVec.GetZ()), glm::radians(45.0F), false);


			}
			else if (i >= 1) // bottom
			{
				posVec = util::math::rotateZ(util::math::Vec3(radius, 0.0F, -height / 2.0F), rFactor, false);
				
				// rotating the normal vector so that it's in the right place and angled properly.
				normVec = util::math::rotateZ(util::math::Vec3(0.0F, 0.0F, -1.0F), rFactor, false);
				normVec = util::math::rotateX(util::math::Vec3(normVec.GetX(), normVec.GetY(), normVec.GetZ()), glm::radians(45.0F), false);
			}
			
			vertices[index] = { {posVec.v.x, posVec.v.y, posVec.v.z}, {color.v.x, color.v.y, color.v.z, color.v.w}, {0.0F, 0.0F, 0.0F} };
			
			rFactor += rInc; // adds to the rotation factor.
			index++; // increaes the index.
		}

		rFactor = 0.0F;
	}

	// final centre vertex
	vertices[index] = { {0.0F, 0.0F, -height / 2.0F}, {color.v.x, color.v.y, color.v.z, color.v.w}, {0.0F, 0.0F, 0.0F} };

	index = 0;
	ind0 = 0;
	ind1 = 1;
	ind2 = 2;

	// drawing the indices.
	for (int i = 0; i < 3 && index < indicesTotal; i++)
	{
		for (int j = 0; j < segments && index < indicesTotal; j++)
		{
			// first draw (top of cylinder)
			if (i == 0)
			{
				// top point -> bottom left -> bottom right
				indices[index] = ind0;
				indices[++index] = ind1;

				// because of the drawing order, an edge ends up not getting drawn, leaving a face missing.
				// this is just to fix that problem by making sure the indices are all correct for the final top face.
				// this only happens with the top of the sphere. The bottom of the sphere works fine.
				if (j == segments - 1)
					indices[++index] = ind0 + 1;
				else
					indices[++index] = ind2;

				ind1++;
				ind2++;
				index++;
			}
			// final draw (bottom of cylinder)
			else if (i == 2)
			{
				// top left -> bottom point -> top right
				indices[index] = ind1 - segments;
				indices[++index] = ind0;
				
				if (j == segments - 1) // if on the final column of the final row
					indices[++index] = ind2 - segments * 2;
				else
					indices[++index] = ind2 - segments;

				ind1++;
				ind2++;
				index++;
			}
			else // other rows
			{
				if (j == segments - 1)
				{
					// triangle 1 (top left -> top right -> bottom left)
					indices[index] = ind1 - segments; // ind1 - segments;
					indices[++index] = ind1; // ind2 - segments;
					indices[++index] = ind2 - segments * 2; // ind1;

					// triangle 2 bottom left -> top right -> bottom right)
					indices[++index] = ind2 - segments * 2; // ind1;
					indices[++index] = ind1; // ind2 - segments;
					indices[++index] = ind2 - segments; // ind2;
				}
				else
				{
					// triangle 1 (top left -> top right -> bottom left)
					indices[index] = ind1 - segments; // ind1 - segments;
					indices[++index] = ind1; // ind2 - segments;
					indices[++index] = ind2 - segments; // ind1;

					// triangle 2 bottom left -> top right -> bottom right)
					indices[++index] = ind1; // ind1;
					indices[++index] = ind2; // ind2 - segments;
					indices[++index] = ind2 - segments; // ind2;
				}

				ind1++;
				ind2++;
				index++;

				if (i == 1 && j == segments - 1)
					ind0 = ind1; // will be used for setting final vertex value.					
			}
		}
	}

	CalculateNormals();
	InvertNormals();

	CalculateMeshBody(); // calculates the limits of the mesh body.

	// Create a new mesh from the data
	mesh = std::make_shared<Mesh>(vertices, verticesTotal, indices, indicesTotal);
}

// returns the radius
float cherry::PrimitiveCylinder::GetRadius() const { return radius; }

// gets the height
float cherry::PrimitiveCylinder::GetHeight() const { return height; }
