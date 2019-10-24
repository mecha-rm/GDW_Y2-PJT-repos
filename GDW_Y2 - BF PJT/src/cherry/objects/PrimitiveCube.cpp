#include "PrimitiveCube.h"
#include "..\Physics.h"

cherry::PrimitiveCube::PrimitiveCube() : PrimitiveCube(1.0F, 1.0F, 1.0F) {}

// sets a single side length
cherry::PrimitiveCube::PrimitiveCube(float sideLength) : PrimitiveCube(sideLength, sideLength, sideLength) {}

cherry::PrimitiveCube::PrimitiveCube(float width, float height, float depth) : 
	cherry::Primitive(), width(abs(width)), height(abs(height)), depth(abs(depth))
{
	// Position and Color (Default Values)
	verticesTotal = 8;
	vertices = new Vertex[verticesTotal]
	{
		//  x      y	 z									r	 g	   b	 a
		
		{{ -width / 2.0F, -height / 2.0F,  depth / 2.0F }, { 1.0f, 1.0f, 1.0f, 1.0f }, {1.0F, 1.0F, 1.0F}}, // bottom left, front corner
		{{ -width / 2.0F,  height / 2.0F,  depth / 2.0F }, { 1.0f, 1.0f, 1.0f, 1.0f }, {1.0F, 1.0F, 1.0F}}, // top left, front corner
		{{ -width / 2.0F, -height / 2.0F, -depth / 2.0F }, { 1.0f, 1.0f, 1.0f, 1.0f }, {1.0F, 1.0F, 1.0F}}, // bottom left, back corner
		{{ -width / 2.0F,  height / 2.0F, -depth / 2.0F }, { 1.0f, 1.0f, 1.0f, 1.0f }, {1.0F, 1.0F, 1.0F}}, // top left, back corner

		{{ width / 2.0F, -height / 2.0F,  depth / 2.0F }, { 1.0f, 1.0f, 1.0f, 1.0f }, {1.0F, 1.0F, 1.0F}}, // bottom right, front corner
		{{ width / 2.0F,  height / 2.0F,  depth / 2.0F }, { 1.0f, 1.0f, 1.0f, 1.0f }, {1.0F, 1.0F, 1.0F}}, // top right, front corner
		{{ width / 2.0F, -height / 2.0F, -depth / 2.0F }, { 1.0f, 1.0f, 1.0f, 1.0f }, {1.0F, 1.0F, 1.0F}}, // bottom right, back corner
		{{ width / 2.0F,  height / 2.0F, -depth / 2.0F }, { 1.0f, 1.0f, 1.0f, 1.0f }, {1.0F, 1.0F, 1.0F}} // top right, back corner

	};


	// (0/4) (1/5)
	// (2/6) (3/7)

	// indices (drawn as triangles)
	indicesTotal = 36; // 8 * 4
	indices = new uint32_t[indicesTotal] 
	{
		0, 1, 2, // front face - top half
		2, 1, 3, // front face - bottom half
		0, 4, 2, // right side-face (box's perspective) - top half
		2, 4, 6, // right side-face (box's perspectice) - bottom half
		1, 5, 3, // left side-face - top half
		3, 5, 7, // left side-face, bottom half
		4, 5, 6, // back face - top half
		6, 5, 7, // back face - bottom half
		0, 4, 5, // top tri 1
		5, 0, 1, //  top tri 2
		2, 6, 7, // bottom tri 1
		7, 2, 3 // bottom tri 2
	};

	// Create a new mesh from the data
	mesh = std::make_shared<Mesh>(vertices, verticesTotal, indices, indicesTotal);
	
	// TODO: LOOPS INFINITELY
	// PhysicsBody* temp = new cherry::PhysicsBodyBox(width, height, depth);
	addPhysicsBody(new cherry::PhysicsBodyBox(width, height, depth));
}
