#include "PrimitivePlane.h"

// constructor
PrimitivePlane::PrimitivePlane() : Primitive()
{
	// Position and Colour
	Vertex vertices[4] = {
		//  x      y	 z		   r	 g	   b	 a
		{{ -0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }}, // bottom left
		{{  0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }}, // bottom right
		{{ -0.5f,  0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }}, // top left
		{{  0.5f,  0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }}, // top right
	};

	// indices
	uint32_t indices[6] = {
		0, 1, 2,
		2, 1, 3
	};

	// Create a new mesh from the data
	mesh = std::make_shared<Mesh>(vertices, 4, indices, 6);

}
