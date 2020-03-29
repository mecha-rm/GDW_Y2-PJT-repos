// Vertex Structs, and Mesh Class (Source)
// Loads in meshes for rendering to the screen
#include "Mesh.h"
#include <toolkit/Logging.h>

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

	/// Second Color
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, false, sizeof(MorphVertex), &(vert->Color1));

	/// Second Normal
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 3, GL_FLOAT, false, sizeof(MorphVertex), &(vert->Normal1));

	// Second UV
	glEnableVertexAttribArray(7);
	glVertexAttribPointer(7, 2, GL_FLOAT, false, sizeof(MorphVertex), &(vert->UV1));

	// Third Position
	glEnableVertexAttribArray(8);
	glVertexAttribPointer(8, 3, GL_FLOAT, false, sizeof(MorphVertex), &(vert->Position2));

	/// Third Color
	glEnableVertexAttribArray(9);
	glVertexAttribPointer(9, 4, GL_FLOAT, false, sizeof(MorphVertex), &(vert->Color2));

	/// Third Normal
	glEnableVertexAttribArray(10);
	glVertexAttribPointer(10, 3, GL_FLOAT, false, sizeof(MorphVertex), &(vert->Normal2));

	// Third UV
	glEnableVertexAttribArray(11);
	glVertexAttribPointer(11, 2, GL_FLOAT, false, sizeof(MorphVertex), &(vert->UV2));

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

// gets whether the faces are culled or not.
bool cherry::Mesh::GetCullFaces() const { return cullFaces; }

// sets whether to cull the faces or not.
void cherry::Mesh::SetCullFaces(bool cf) { cullFaces = cf; }

// toggle's face culling
void cherry::Mesh::SetCullFaces() { cullFaces = !cullFaces; }

// returns 'true' if the mesh is visible, false otherwise.
bool cherry::Mesh::IsVisible() { return visible; }

// toggle's visibility
void cherry::Mesh::SetVisible() { visible = !visible; }

// sets whether the mesh is visible or not.
void cherry::Mesh::SetVisible(bool visible) { this->visible = visible; }

// returns 'true' if the object is drawn in perspective.
bool cherry::Mesh::IsPerspectiveMesh() const { return perspectiveMesh; }

// sets if the mesh is drawn in perspective mode.
void cherry::Mesh::SetPerspectiveMesh(bool perspective) { perspectiveMesh = perspective; }

// returns 'true' if the mesh is drawn in orthographic mode.
bool cherry::Mesh::IsOrthographicMesh() const { return !perspectiveMesh; }

// sets if the mesh should be drawn in orthographic perspective.
void cherry::Mesh::SetOrthographicMesh(bool orthographic) { perspectiveMesh = !orthographic; }

// gets whether the screen position of the mesh is fixed regardless of the orientation or position of the camera. 
bool cherry::Mesh::GetWindowChild() const { return windowChild; }

// sets if the mesh has a fixed screen position or not.
void cherry::Mesh::SetWindowChild(bool windowChild) { this->windowChild = windowChild; }


// morph for regular vertices
void cherry::Mesh::Morph(Vertex* vertices, size_t numVerts)
{
	glNamedBufferData(myBuffers[0], sizeof(Vertex) * numVerts, vertices, GL_DYNAMIC_DRAW);
}

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
cherry::MorphVertex* cherry::Mesh::ConvertToMorphVertexArray(const Vertex* const verts, const size_t numVerts)
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
		morphVerts[i].Color1 = verts[i].Color;
		morphVerts[i].Color2 = verts[i].Color;

		morphVerts[i].Normal = verts[i].Normal;
		morphVerts[i].Normal1 = verts[i].Normal;
		morphVerts[i].Normal2 = verts[i].Normal;

		morphVerts[i].UV = verts[i].UV;
		morphVerts[i].UV1 = verts[i].UV;
		morphVerts[i].UV2 = verts[i].UV;
	}

	return morphVerts;
}

// makes the faces face outward.
cherry::Mesh::Sptr cherry::Mesh::MakeInvertedCube() {
	// Create our 4 vertices
	cherry::Vertex verts[8] = {
		// Position
		// x y z
		{{ -1.0f, -1.0f, -1.0f }}, {{ 1.0f, -1.0f, -1.0f }}, {{ -1.0f, 1.0f, -1.0f }}, {{ 1.0f, 1.0f, -1.0f }},
		{{ -1.0f, -1.0f, 1.0f }}, {{ 1.0f, -1.0f, 1.0f }}, {{ -1.0f, 1.0f, 1.0f }}, {{ 1.0f, 1.0f, 1.0f }}
	};
	// Create our 6 indices
	uint32_t indices[36] = {
	0, 1, 2, 2, 1, 3, 4, 6, 5, 6, 7, 5, // bottom / top
	0, 1, 4, 4, 1, 5, 2, 3, 6, 6, 3, 7, // front /back
	2, 4, 0, 2, 6, 4, 3, 5, 1, 3, 7, 5 // left / right
	};
	// Create a new mesh from the data
	return std::make_shared<cherry::Mesh>(verts, 8, indices, 36);
}

// chopping up the plane.
cherry::Mesh::Sptr cherry::Mesh::MakeSubdividedPlane(float size, int numSections, bool worldUvs) {
	LOG_ASSERT(numSections > 0, "Number of sections must be greater than 0!");
	LOG_ASSERT(size != 0, "Size cannot be zero!");

	// Determine the number of edge vertices, and the number of vertices and indices we'll need
	int numEdgeVerts = numSections + 1;
	size_t vertexCount = numEdgeVerts * numEdgeVerts;
	size_t indexCount = numSections * numSections * 6;

	// Allocate some memory for our vertices and indices
	cherry::Vertex* vertices = new cherry::Vertex[vertexCount];
	uint32_t* indices = new uint32_t[indexCount];

	// Determine where to start vertices from, and the step pre grid square
	float start = -size / 2.0f;
	float step = size / numSections;

	// vertices
	// Iterate over the grid's edge vertices
	for (int ix = 0; ix <= numSections; ix++) {
		for (int iy = 0; iy <= numSections; iy++) {
			// Get a reference to the vertex so we can modify it
			cherry::Vertex& vert = vertices[ix * numEdgeVerts + iy];
			// Set its position
			vert.Position.x = start + ix * step;
			vert.Position.y = start + iy * step;
			vert.Position.z = 0.0f;
			// Set its normal
			vert.Normal = glm::vec3(0, 0, 1);
			// The UV will go from [0, 1] across the entire plane (can change this later)
			if (worldUvs) {
				vert.UV.x = vert.Position.x;
				vert.UV.y = vert.Position.y;
			}
			else {
				vert.UV.x = vert.Position.x / size;
				vert.UV.y = vert.Position.y / size;
			}
			// Flat white color
			vert.Color = glm::vec4(1.0f);
		}
	}

	// indices
	// We'll just increment an index instead of calculating it
	uint32_t index = 0;
	// Iterate over the quads that make up the grid
	for (int ix = 0; ix < numSections; ix++) {
		for (int iy = 0; iy < numSections; iy++) {
			// Determine the indices for the points on this quad
			uint32_t p1 = (ix + 0) * numEdgeVerts + (iy + 0);
			uint32_t p2 = (ix + 1) * numEdgeVerts + (iy + 0);
			uint32_t p3 = (ix + 0) * numEdgeVerts + (iy + 1);
			uint32_t p4 = (ix + 1) * numEdgeVerts + (iy + 1);
			// Append the quad to the index list
			indices[index++] = p1;
			indices[index++] = p2;
			indices[index++] = p3;
			indices[index++] = p3;
			indices[index++] = p2;
			indices[index++] = p4;
		}
	}

	// returning the mesh
	// Create the result, then clean up the arrays we used
	cherry::Mesh::Sptr result = std::make_shared<cherry::Mesh>(vertices, vertexCount, indices, indexCount);
	delete[] vertices;
	delete[] indices;
	// Return the result
	return result;
}
