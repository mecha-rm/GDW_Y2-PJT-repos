// VERTEX STRUCT AND MESH CLASS (SOURCE)
#pragma once
#include <glad/glad.h>
#include <GLM/glm.hpp> // For vec3 and vec4
#include <cstdint> // Needed for uint32_t
#include <memory> // Needed for smart pointers

namespace cherry
{
	// Vertex Struct - saves vertex position and colour.
	struct Vertex {
		glm::vec3 Position; // vertex position
		glm::vec4 Color; // vertex colour
	};

	// Mesh Class - creates meshes so that objects can appear on screen.
	class Mesh {
	public:
		// Shorthand for shared_ptr
		typedef std::shared_ptr<Mesh> Sptr;

		// Creates a new mesh from the given vertices and indices
		Mesh(Vertex* vertices, size_t numVerts, uint32_t* indices, size_t numIndices);
		Mesh(Vertex* vertices, size_t numVerts, uint32_t* indices, size_t numIndices, bool wireframe);

		// destructor
		~Mesh();

		// Draws this mesh
		void Draw();

		// Returns wireframe boolean to tell the caller if the mesh is to be drawn in wirframe
		bool isWireframeMode();

		// if 'true' is passed, wireframe is enabled. If false is passed, wireframe is disabled.
		void setWireframeMode(bool wf);

		// Enables wireframe mode on the mesh
		void enableWireframeMode();

		// Disables wireframe mode on the mesh
		void disableWireframeMode();

	private:
		// Our GL handle for the Vertex Array Object
		GLuint myVao;

		// 0 is vertices, 1 is indices
		GLuint myBuffers[2];

		// The number of vertices and indices in this mesh
		size_t myVertexCount, myIndexCount;

		// Whether or not the mesh should be drawn in wireframe mode
		bool wireframe;
	};
}