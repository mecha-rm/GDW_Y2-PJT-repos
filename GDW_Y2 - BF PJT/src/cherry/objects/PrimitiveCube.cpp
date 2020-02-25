// PrimitiveCube (Source)
#include "PrimitiveCube.h"
#include "..\physics/PhysicsBody.h"

cherry::PrimitiveCube::PrimitiveCube() : PrimitiveCube(1.0F, 1.0F, 1.0F, cherry::Vec4(1.0F, 1.0F, 1.0F, 1.0F)) {}

// sets a single side length
cherry::PrimitiveCube::PrimitiveCube(float sideLength, cherry::Vec4 color) : PrimitiveCube(sideLength, sideLength, sideLength, color) {}

cherry::PrimitiveCube::PrimitiveCube(float width, float height, float depth, cherry::Vec4 color) : 
	cherry::Primitive(), width(abs(width)), height(abs(height)), depth(abs(depth))
{
	// making sure both are positive.
	width = abs(width);
	height = abs(height);
	depth = abs(depth);
	
	// colour of the primitive cube.
	glm::vec4 clr = glm::vec4(
		(color.v.x < 0.0F) ? 0.0F : (color.v.x > 1.0F) ? 1.0F : color.v.x,
		(color.v.y < 0.0F) ? 0.0F : (color.v.y > 1.0F) ? 1.0F : color.v.y,
		(color.v.z < 0.0F) ? 0.0F : (color.v.z > 1.0F) ? 1.0F : color.v.z,
		(color.v.w < 0.0F) ? 0.0F : (color.v.w > 1.0F) ? 1.0F : color.v.w
	);

	color = clr;
	this->color = color;

	// Position and Color (Default Values) // NORMALS ARE WRONG
	verticesTotal = 8;

	// Original; kept here since the faces are being drawn wrong
	//{ { -width / 2.0F, -height / 2.0F, depth / 2.0F }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0F, 0.0F, 0.0f }}, // bottom left, front corner
	//{ { -width / 2.0F,  height / 2.0F,  depth / 2.0F }, { 1.0f, 1.0f, 1.0f, 1.0f }, {0.0F, 0.0F, 0.0f} }, // top left, front corner
	//{ { -width / 2.0F, -height / 2.0F, -depth / 2.0F }, { 1.0f, 1.0f, 1.0f, 1.0f }, {0.0F, 0.0F, 0.0f} }, // bottom left, back corner
	//{ { -width / 2.0F,  height / 2.0F, -depth / 2.0F }, { 1.0f, 1.0f, 1.0f, 1.0f }, {0.0F, 0.0F, 0.0f} }, // top left, back corner

	//{ { width / 2.0F, -height / 2.0F,  depth / 2.0F }, { 1.0f, 1.0f, 1.0f, 1.0f }, {0.0F, 0.0F, 0.0f} }, // bottom right, front corner
	//{ { width / 2.0F,  height / 2.0F,  depth / 2.0F }, { 1.0f, 1.0f, 1.0f, 1.0f }, {0.0F, 0.0F, 0.0f} }, // top right, front corner
	//{ { width / 2.0F, -height / 2.0F, -depth / 2.0F }, { 1.0f, 1.0f, 1.0f, 1.0f }, {0.0F, 0.0F, 0.0f} }, // bottom right, back corner
	//{ { width / 2.0F,  height / 2.0F, -depth / 2.0F }, { 1.0f, 1.0f, 1.0f, 1.0f }, {0.0F, 0.0F, 0.0f} } // top right, back corner

	vertices = new Vertex[verticesTotal]
	{
		//  x      y	 z									rgba
		
		{{ -width / 2.0F, -height / 2.0F,  depth / 2.0F }, { clr }, {0.0F, 0.0F, 0.0f}, {1, 0}}, // bottom left, front corner
		{{ -width / 2.0F,  height / 2.0F,  depth / 2.0F }, { clr }, {0.0F, 0.0F, 0.0f}, {1, 1}}, // top left, front corner
		{{ -width / 2.0F, -height / 2.0F, -depth / 2.0F }, { clr }, {0.0F, 0.0F, 0.0f}, {0, 0}}, // bottom left, back corner
		{{ -width / 2.0F,  height / 2.0F, -depth / 2.0F }, { clr }, {0.0F, 0.0F, 0.0f}, {0, 1}}, // top left, back corner
																									
		{{ width / 2.0F, -height / 2.0F,  depth / 2.0F }, { clr }, {0.0F, 0.0F, 0.0f}, {1, 1}}, // bottom right, front corner
		{{ width / 2.0F,  height / 2.0F,  depth / 2.0F }, { clr }, {0.0F, 0.0F, 0.0f}, {1, 0}}, // top right, front corner
		{{ width / 2.0F, -height / 2.0F, -depth / 2.0F }, { clr }, {0.0F, 0.0F, 0.0f}, {0, 1}}, // bottom right, back corner
		{{ width / 2.0F,  height / 2.0F, -depth / 2.0F }, { clr }, {0.0F, 0.0F, 0.0f}, {0, 0}} // top right, back corner

	};


	// (0/4) (1/5)
	// (2/6) (3/7)

	// indices (drawn as triangles) (descriptions are wrong)
	indicesTotal = 36; // 8 * 4
	indices = new uint32_t[indicesTotal] 
	{
		0, 1, 2, // right-side face - top half (correct)
		2, 1, 3, // right-side face - bottom half (correct)
		0, 2, 4, // right side-face (box's perspective) - top half
		2, 6, 4, // right side-face (box's perspectice) - bottom half
		1, 5, 3, // left side-face - top half
		3, 5, 7, // left side-face, bottom half
		4, 6, 5, // back face - top half
		6, 7, 5, // back face - bottom half
		0, 4, 5, // top tri 1
		5, 1, 0, //  top tri 2
		2, 7, 6, // bottom tri 1
		7, 2, 3 // bottom tri 2
	};

	// calculates the normals
	CalculateNormals();
	InvertNormals();

	CalculateMeshBody(); // calculates the limits of the mesh body.

	// Create a new mesh from the data
	mesh = std::make_shared<Mesh>(vertices, verticesTotal, indices, indicesTotal);
	
	// Maybe don't have primitives automatically get physics bodies (causes infinite loop)
	// PhysicsBody* temp = new cherry::PhysicsBodyBox(width, height, depth);
	// AddPhysicsBody(new cherry::PhysicsBodyBox(width, height, depth));
}

// gets the width
float cherry::PrimitiveCube::GetWidth() const { return width; }

// gets the height
float cherry::PrimitiveCube::GetHeight() const { return height; }

// gets the depth
float cherry::PrimitiveCube::GetDepth() const { return depth; }
