#include "PrimitivePlane.h"

// constructor
cherry::PrimitivePlane::PrimitivePlane(float width, float height) : Primitive(), width(abs(width)), height(abs(height))
{
	// absolute values
	width = abs(width);
	height = abs(height); 

	// Position and Colour
	verticesTotal = 4;
	vertices = new Vertex [verticesTotal] 
	{
		//  x			  y				  z		   r	 g	   b	 a		 // normals
		{{ -width / 2.0F, -height / 2.0F, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, {0.0F, 0.0F, 1.0F}}, // bottom left
		{{  width / 2.0F, -height / 2.0F, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, {0.0F, 0.0F, 1.0F}}, // bottom right
		{{ -width / 2.0F,  height / 2.0F, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, {0.0F, 0.0F, 1.0F}}, // top left
		{{  width / 2.0F,  height / 2.0F, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, {0.0F, 0.0F, 1.0F}}, // top right
	};

	// indices
	indicesTotal = 6;
	indices = new uint32_t[indicesTotal] {
		0, 1, 2,
		2, 1, 3
	};

	// Create a new mesh from the data
	mesh = std::make_shared<Mesh>(vertices, verticesTotal, indices, indicesTotal);

}

// gets the width of the plane.
float cherry::PrimitivePlane::GetWidth() const { return width; }

// gets the height
float cherry::PrimitivePlane::GetHeight() const { return height; }
