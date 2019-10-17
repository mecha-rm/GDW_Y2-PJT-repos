#include "PrimitivePlane.h"

// constructor
PrimitivePlane::PrimitivePlane() : Primitive()
{
	// Position and Colour
	verticesTotal = 4;
	vertices = new Vertex [verticesTotal] 
	{
		//  x      y	 z		   r	 g	   b	 a
		{{ -0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }}, // bottom left
		{{  0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }}, // bottom right
		{{ -0.5f,  0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }}, // top left
		{{  0.5f,  0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }}, // top right
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
