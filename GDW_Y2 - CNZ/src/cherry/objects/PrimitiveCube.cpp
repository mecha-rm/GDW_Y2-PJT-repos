#include "PrimitiveCube.h"
#include "..\PhysicsBody.h"

cherry::PrimitiveCube::PrimitiveCube() : PrimitiveCube(1.0F, 1.0F, 1.0F) {}

// sets a single side length
cherry::PrimitiveCube::PrimitiveCube(float sideLength) : PrimitiveCube(sideLength, sideLength, sideLength) {}

cherry::PrimitiveCube::PrimitiveCube(float width, float height, float depth) : 
	cherry::Primitive(), width(abs(width)), height(abs(height)), depth(abs(depth))
{
	// making sure both are positive.
	width = abs(width);
	height = abs(height);
	depth = abs(depth);
	

	// Position and Color (Default Values) // NORMALS ARE WRONG
	verticesTotal = 8;
	vertices = new Vertex[verticesTotal]
	{
		//  x      y	 z									r	 g	   b	 a
		
		{{ -width / 2.0F, -height / 2.0F,  depth / 2.0F }, { 1.0f, 1.0f, 1.0f, 1.0f }, {0.0F, 0.0F, 0.0f}}, // bottom left, front corner
		{{ -width / 2.0F,  height / 2.0F,  depth / 2.0F }, { 1.0f, 1.0f, 1.0f, 1.0f }, {0.0F, 0.0F, 0.0f}}, // top left, front corner
		{{ -width / 2.0F, -height / 2.0F, -depth / 2.0F }, { 1.0f, 1.0f, 1.0f, 1.0f }, {0.0F, 0.0F, 0.0f}}, // bottom left, back corner
		{{ -width / 2.0F,  height / 2.0F, -depth / 2.0F }, { 1.0f, 1.0f, 1.0f, 1.0f }, {0.0F, 0.0F, 0.0f}}, // top left, back corner
																									
		{{ width / 2.0F, -height / 2.0F,  depth / 2.0F }, { 1.0f, 1.0f, 1.0f, 1.0f }, {0.0F, 0.0F, 0.0f}}, // bottom right, front corner
		{{ width / 2.0F,  height / 2.0F,  depth / 2.0F }, { 1.0f, 1.0f, 1.0f, 1.0f }, {0.0F, 0.0F, 0.0f}}, // top right, front corner
		{{ width / 2.0F, -height / 2.0F, -depth / 2.0F }, { 1.0f, 1.0f, 1.0f, 1.0f }, {0.0F, 0.0F, 0.0f}}, // bottom right, back corner
		{{ width / 2.0F,  height / 2.0F, -depth / 2.0F }, { 1.0f, 1.0f, 1.0f, 1.0f }, {0.0F, 0.0F, 0.0f}} // top right, back corner

	};


	// (0/4) (1/5)
	// (2/6) (3/7)

	// indices (drawn as triangles) (descriptions are wrong)
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

	// calculates the normals
	calculateNormals();

	// Create a new mesh from the data
	mesh = std::make_shared<Mesh>(vertices, verticesTotal, indices, indicesTotal);
	
	// TODO: LOOPS INFINITELY
	// PhysicsBody* temp = new cherry::PhysicsBodyBox(width, height, depth);
	AddPhysicsBody(new cherry::PhysicsBodyBox(width, height, depth));
}

// gets the width
float cherry::PrimitiveCube::GetWidth() const { return width; }

// gets the height
float cherry::PrimitiveCube::GetHeight() const { return height; }

// gets the depth
float cherry::PrimitiveCube::GetDepth() const { return depth; }
