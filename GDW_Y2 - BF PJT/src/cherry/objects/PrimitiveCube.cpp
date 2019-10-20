#include "PrimitiveCube.h"

cherry::PrimitiveCube::PrimitiveCube() : cherry::Primitive()
{
	// Position and Color (Default Values)
	verticesTotal = 8;
	vertices = new Vertex[verticesTotal]
	{
		//  x      y	 z		   r	 g	   b	 a
		
		{{ -0.5f, -0.5f,  0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f }}, // bottom left, front corner
		{{ -0.5f,  0.5f,  0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f }}, // top left, front corner
		{{ -0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f }}, // bottom left, back corner
		{{ -0.5f,  0.5f, -0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f }}, // top left, back corner

		{{ 0.5f, -0.5f,  0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f }}, // bottom right, front corner
		{{ 0.5f,  0.5f,  0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f }}, // top right, front corner
		{{ 0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f }}, // bottom right, back corner
		{{ 0.5f,  0.5f, -0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f }} // top right, back corner

	};


	// (0/4) (1/5)
	// (2/6) (3/7)

	// indices (drawn as triangles)
	indicesTotal = 24; // 8 * 3
	indices = new uint32_t[indicesTotal] 
	{
		0, 1, 2, // front face - top half
		2, 1, 3, // front face - bottom half
		0, 4, 2, // right side-face (box's perspective) - top half
		2, 4, 6, // right side-face (box's perspectice) - bottom half
		1, 5, 3, // left side-face - top half
		3, 5, 7, // left side-face, bottom half
		4, 5, 6, // back face - top half
		6, 5, 7 // back face - bottom half
	};

	// Create a new mesh from the data
	mesh = std::make_shared<Mesh>(vertices, verticesTotal, indices, indicesTotal);
}
