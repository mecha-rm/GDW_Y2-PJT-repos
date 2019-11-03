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
		glm::vec3 Normal = { 0, 0, 1 }; // vertex normal
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
		bool IsWireframe();

		// if 'true' is passed, wireframe is enabled. If false is passed, wireframe is disabled.
		void SetWireframe(bool wf);

		// toggle's the wireframe mode on/off.
		void SetWireframe();

		// Enables wireframe mode on the mesh
		void enableWireframe();

		// Disables wireframe mode on the mesh
		void DisableWireframe();

		// returns 'true' if the mesh is visible, false otherwise.
		bool IsVisible();

		// toggle's the visibility of the mesh on/off.
		void SetVisible();

		// sets whether the mesh is visible or not.
		void SetVisible(bool visible);

		

	private:
		// Our GL handle for the Vertex Array Object
		GLuint myVao;

		// 0 is vertices, 1 is indices
		GLuint myBuffers[2];

		// The number of vertices and indices in this mesh
		size_t myVertexCount, myIndexCount;

		// Whether or not the mesh should be drawn in wireframe mode
		bool wireframe = false;

		// boolan for visibility
		bool visible = true;

		// TODO: implement method of redrawing the mesh in multiple locations
		// cherry::Vec3 offset; // offset for each draw call
		// unsigned int arrRows = 1; // y-axis
		// unsigned int arrCols = 1; // x-axis
		// unsigned int planes = 1; // z-axis
	};
}