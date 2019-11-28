// MESH CLASS (SOURCE)
#include "Mesh.h"

// constructor
cherry::Mesh::Mesh(Vertex* vertices, size_t numVerts, uint32_t* indices, size_t numIndices) {
	myIndexCount = numIndices;
	myVertexCount = numVerts;
	// Create and bind our vertex array
	glCreateVertexArrays(1, &myVao);
	glBindVertexArray(myVao);
	// Create 2 buffers, 1 for vertices and the other for indices
	glCreateBuffers(2, myBuffers); // generates a name interally so you can use direct state access.

	// Bind and buffer our vertex data
	glBindBuffer(GL_ARRAY_BUFFER, myBuffers[0]);
	// upload the number of vertices. We're sticking to static draw for now, which is
	glBufferData(GL_ARRAY_BUFFER, numVerts * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	// Bind and buffer our index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myBuffers[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(uint32_t), indices, GL_STATIC_DRAW);

	// Get a null vertex to get member offsets from
	Vertex* vert = nullptr;

	// Enable vertex attribute 1 (position)
	glEnableVertexAttribArray(0);

	// Our first attribute is 3 floats, the distance between
	// them is the size of our vertex, and they will map to the position in our vertices
	// index, three floats, not normalized, size of data
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), &(vert->Position));

	// Enable vertex attribute 2 (colour)
	glEnableVertexAttribArray(1);

	// Our second attribute is 4 floats, the distance between
	// them is the size of our vertex, and they will map to the color in our vertices
	glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(Vertex), &(vert->Color));

	// Enable vertex attribute 3 (vertex normal)
	glEnableVertexAttribArray(2);

	// Our third attribute is 3 floats, the distance between
	// them is the size of our vertex, and they will map to the color in our vertices
	glVertexAttribPointer(2, 3, GL_FLOAT, false, sizeof(Vertex), &(vert->Normal));

	// UV support
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, false, sizeof(Vertex), &(vert->UV));

	// Unbind our VAO (just ot be safe and make sure we don't modify it by accident)
	glBindVertexArray(0);
}

cherry::Mesh::Mesh(Vertex* vertices, size_t numVerts, uint32_t* indices, size_t numIndices, bool wireframe)
	: Mesh(vertices, numVerts, indices, numIndices)
{
	this->wireframe = wireframe; // wireframe mode
}

// creates a mesh that allows for morph targets
cherry::Mesh::Mesh(MorphVertex* vertices, size_t numVerts, uint32_t* indices, size_t numIndices)
{
	myIndexCount = numIndices;
	myVertexCount = numVerts;
	// Create and bind our vertex array
	glCreateVertexArrays(1, &myVao);
	glBindVertexArray(myVao);
	// Create 2 buffers, 1 for vertices and the other for indices
	glCreateBuffers(2, myBuffers); // generates a name interally so you can use direct state access.

	// Bind and buffer our vertex data
	glBindBuffer(GL_ARRAY_BUFFER, myBuffers[0]);
	// upload the number of vertices. We're sticking to static draw for now, which is
	glBufferData(GL_ARRAY_BUFFER, numVerts * sizeof(MorphVertex), vertices, GL_STATIC_DRAW);

	// Bind and buffer our index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myBuffers[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(uint32_t), indices, GL_STATIC_DRAW);

	// Get a null vertex to get member offsets from
	MorphVertex* vert = nullptr;

	// Enable vertex attribute 1 (position)
	glEnableVertexAttribArray(0);

	// Our first attribute is 3 floats, the distance between
	// them is the size of our vertex, and they will map to the position in our vertices
	// index, three floats, not normalized, size of data
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(MorphVertex), &(vert->Position));

	// Enable vertex attribute 2 (colour)
	glEnableVertexAttribArray(1);

	// Our second attribute is 4 floats, the distance between
	// them is the size of our vertex, and they will map to the color in our vertices
	glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(MorphVertex), &(vert->Color));

	// Enable vertex attribute 3 (vertex normal)
	glEnableVertexAttribArray(2);

	// Our third attribute is 3 floats, the distance between
	// them is the size of our vertex, and they will map to the color in our vertices
	glVertexAttribPointer(2, 3, GL_FLOAT, false, sizeof(MorphVertex), &(vert->Normal));

	// UV support
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, false, sizeof(MorphVertex), &(vert->UV));


	// EXTRAS
	// Second Position
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, false, sizeof(MorphVertex), &(vert->Position1));

	/// Second Normal
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 3, GL_FLOAT, false, sizeof(MorphVertex), &(vert->Normal1));

	// Third Position
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 3, GL_FLOAT, false, sizeof(MorphVertex), &(vert->Position2));

	/// Third Normal
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(7, 3, GL_FLOAT, false, sizeof(MorphVertex), &(vert->Normal2));

	// Unbind our VAO (just ot be safe and make sure we don't modify it by accident)
	glBindVertexArray(0);
}

// destructor
cherry::Mesh::~Mesh()
{
	// Clean up our buffers
	glDeleteBuffers(2, myBuffers);

	// Clean up our VAO
	glDeleteVertexArrays(1, &myVao);
}

// drawing function for meshes
void cherry::Mesh::Draw() {
	// Bind the mesh
	glBindVertexArray(myVao);

	// if we have indices, we enter this statement.
	if (myIndexCount > 0)
	{
		// Draw all of our vertices as triangles, our indexes are unsigned ints (uint32_t)
		glDrawElements(GL_TRIANGLES, myIndexCount, GL_UNSIGNED_INT, nullptr);
	}
	// if we don't have indices (i.e. we just have vertices), we enter this statement.
	else
	{
		// draws all our vertices as triangles, without indexes
		glDrawArrays(GL_TRIANGLES, 0, myVertexCount);
	}
}

// returns 'true' if the mesh is in wireframe mode. False otherwise.
bool cherry::Mesh::IsWireframe() { return wireframe; }

// if 'true' is passed, wireframe is enabled. If false is passed, wireframe is disabled.
void cherry::Mesh::SetWireframe(bool wf) { wireframe = wf; }

// if 'true' is passed, wireframe is enabled. If false is passed, wireframe is disabled.
void cherry::Mesh::SetWireframe() { wireframe = !wireframe; }

// enables wireframe mode.
void cherry::Mesh::enableWireframe() { this->wireframe = true; }

// disables wireframe mode.
void cherry::Mesh::DisableWireframe() { this->wireframe = false; }

// returns 'true' if the mesh is visible, false otherwise.
bool cherry::Mesh::IsVisible() { return visible; }

// toggle's visibility
void cherry::Mesh::SetVisible() { visible = !visible; }

// sets whether the object is visible or not.
void cherry::Mesh::SetVisible(bool visible) { this->visible = visible; }

// gets the vertices and takes out the positions and normals for morphing the mesh
void cherry::Mesh::Morph(MorphVertex* vertices, size_t numVerts)
{
	//glm::vec3* positions = new glm::vec3[numVerts];
	//glm::vec3* normals = new glm::vec3[numVerts];

	//for (unsigned int i = 0; i < numVerts; i++)
	//{
	//	positions[i] = vertices->Position;
	//	normals[i] = vertices->Normal;
	//}

	//Morph(positions, normals, numVerts);

	glNamedBufferData(myBuffers[0], sizeof(MorphVertex) * numVerts, vertices, GL_DYNAMIC_DRAW);

}

// converts from a regular vertex to a morph vertex
cherry::MorphVertex* cherry::Mesh::ConvertToMorphVertexArray(const Vertex* verts, const size_t numVerts)
{
	// no data given
	if (verts == nullptr || numVerts == 0)
		return nullptr;

	MorphVertex* morphVerts = new MorphVertex[numVerts];

	// copies over the values
	for (int i = 0; i < numVerts; i++)
	{
		morphVerts[i].Position = verts[i].Position;
		morphVerts[i].Position1 = verts[i].Position;
		morphVerts[i].Position2 = verts[i].Position;

		morphVerts[i].Color = verts[i].Color;

		morphVerts[i].Normal = verts[i].Normal;
		morphVerts[i].Normal1 = verts[i].Normal;
		morphVerts[i].Normal2 = verts[i].Normal;

		morphVerts[i].UV = verts[i].UV;
	}

	return morphVerts;
}