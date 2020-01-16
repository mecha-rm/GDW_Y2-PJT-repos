// PrimitivePlane
#include "PrimitivePlane.h"

// constructor
cherry::PrimitivePlane::PrimitivePlane(float width, float height, bool doubleSided, cherry::Vec4 color) : Primitive(), width(abs(width)), height(abs(height))
{
	// absolute values
	width = abs(width);
	height = abs(height); 

	color.v.x = (color.v.x < 0.0F) ? 0.0F : (color.v.x > 1.0F) ? 1.0F : color.v.x;
	color.v.y = (color.v.y < 0.0F) ? 0.0F : (color.v.y > 1.0F) ? 1.0F : color.v.y;
	color.v.z = (color.v.z < 0.0F) ? 0.0F : (color.v.z > 1.0F) ? 1.0F : color.v.z;
	color.v.w = (color.v.w < 0.0F) ? 0.0F : (color.v.w > 1.0F) ? 1.0F : color.v.w;

	this->color = color; // saving the colour.

	// Position and Colour
	verticesTotal = 4;
	vertices = new Vertex [verticesTotal] 
	{
		//  x			  y				  z		   r	 g	   b	 a		 // normals
		{{ -width / 2.0F, -height / 2.0F, 0.0f }, { color.v.x, color.v.y, color.v.z, color.v.w }, {0.0F, 0.0F, 1.0F}, {0.0F, 0.0F}}, // bottom left
		{{  width / 2.0F, -height / 2.0F, 0.0f }, { color.v.x, color.v.y, color.v.z, color.v.w }, {0.0F, 0.0F, 1.0F}, {1.0F, 0.0F}}, // bottom right
		{{ -width / 2.0F,  height / 2.0F, 0.0f }, { color.v.x, color.v.y, color.v.z, color.v.w }, {0.0F, 0.0F, 1.0F}, {0.0F, 1.0F}}, // top left
		{{  width / 2.0F,  height / 2.0F, 0.0f }, { color.v.x, color.v.y, color.v.z, color.v.w }, {0.0F, 0.0F, 1.0F}, {1.0F, 1.0F}}, // top right
	};

	// indices
	indicesTotal = 6;
	indices = new uint32_t[indicesTotal] {
		0, 1, 2,
		2, 1, 3
	}; 

	CalculateMeshBody(); // calculates the limits of the mesh body.

	// Create a new mesh from the data
	mesh = std::make_shared<Mesh>(vertices, verticesTotal, indices, indicesTotal);
	mesh->cullFaces = !doubleSided;
}

// gets the width of the plane.
float cherry::PrimitivePlane::GetWidth() const { return width; }

// gets the height
float cherry::PrimitivePlane::GetHeight() const { return height; }
