#include "PrimitivePlane.h"

// constructor
cherry::PrimitivePlane::PrimitivePlane() : Primitive()
{
	// Position and Colour
	verticesTotal = 4;
	vertices = new Vertex [verticesTotal] 
	{
		//  x      y	 z		   r	 g	   b	 a
		{{ -0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, {0.0F, 0.0F, 1.0F}}, // bottom left
		{{  0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, {0.0F, 0.0F, 1.0F}}, // bottom right
		{{ -0.5f,  0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, {0.0F, 0.0F, 1.0F}}, // top left
		{{  0.5f,  0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, {0.0F, 0.0F, 1.0F}}, // top right
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
