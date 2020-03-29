// Vertex Structs, and Mesh Class (Header)
// Loads in meshes for rendering to the screen
#pragma once

#include <glad/glad.h>
#include <GLM/glm.hpp> // For vec3 and vec4
#include <cstdint> // Needed for uint32_t
#include <memory> // Needed for smart pointers

#include "UtilsCRY.h"
#include "VectorCRY.h"

namespace cherry
{
	// Vertex Struct - saves vertex position and colour.
	struct Vertex {
		glm::vec3 Position; // vertex position
		glm::vec4 Color; // vertex colour
		glm::vec3 Normal; // vertex normal
		glm::vec2 UV; // texture UV coordinates
	};

	// Morph Vertex Struct - used for morph targets
	// Edit: morph targets don't need colour or UVs. They were added to try and do something else, but that didn't work.
	struct MorphVertex {
		glm::vec3 Position; // vertex position
		glm::vec4 Color; // vertex colour
		glm::vec3 Normal; // vertex normal
		glm::vec2 UV; // texture UV coordinates

		
		glm::vec3 Position1; // position 1 for deformation animation
		glm::vec4 Color1; // colour 1 ()
		glm::vec3 Normal1; // normal 1 for deformation animation
		glm::vec2 UV1; // UV 1 (used for texture animation)

		glm::vec3 Position2; // position 2 for deformation animation
		glm::vec4 Color2; // colour 2 
		glm::vec3 Normal2; // normal 2 for deformation animation
		glm::vec2 UV2; // UV 2 (used for texture animation)
	};

	// Mesh Class - creates meshes so that sceneLists can appear on screen.
	class Mesh {
	public:

		GraphicsClass(Mesh);

		// Shorthand for shared_ptr
		typedef std::shared_ptr<Mesh> Sptr;

		// Creates a new mesh from the given vertices and indices
		Mesh(Vertex* vertices, size_t numVerts, uint32_t* indices, size_t numIndices);

		// 
		Mesh(Vertex* vertices, size_t numVerts, uint32_t* indices, size_t numIndices, bool wireframe);

		// Creates a new mesh for morph target animation.
		// NOTE: this should only be used if morph targets are being used.
		Mesh(MorphVertex* vertices, size_t numVerts, uint32_t* indices, size_t numIndices);

		// destructor
		~Mesh();

		// Draws this mesh
		void Draw();

		// Returns wireframe boolean to tell the caller if the mesh is to be drawn in wireframe mode.
		bool IsWireframe();

		// if 'true' is passed, wireframe is enabled. If false is passed, wireframe is disabled.
		void SetWireframe(bool wf);

		// toggle's the wireframe mode on/off.
		void SetWireframe();

		// Enables wireframe mode on the mesh
		void enableWireframe();

		// Disables wireframe mode on the mesh
		void DisableWireframe();

		// gets whether the faces should be culled or not.
		bool GetCullFaces() const;

		// set to 'true' to cull the faces on the mesh, false to not cull them. It is set to true by default.
		void SetCullFaces(bool cf);

		// toggles culling faces
		void SetCullFaces();

		// returns 'true' if the mesh is visible, false otherwise.
		bool IsVisible();

		// toggle's the visibility of the mesh on/off.
		void SetVisible();

		// sets whether the mesh is visible or not.
		void SetVisible(bool visible);

		// returns 'true' if the mesh is drawn in perspective mode (default mode).
		bool IsPerspectiveMesh() const;

		// sets if the mesh should be drawn in perspective mode.
		void SetPerspectiveMesh(bool perspective);

		// returns 'true' if the mesh is drawn in orthographic mode.
		bool IsOrthographicMesh() const;

		// sets if the mesh should be drawn in orthographic mode.
		void SetOrthographicMesh(bool orthographic);

		// if 'true', the screen position of the mesh is fixed regardless of the camera position or orientation.
		// the mesh itself can still be moved, but it will be uneffected by the movement in the camera.
		bool GetWindowChild() const;

		// if 'true', the mesh will say in the same place on screen.
		void SetWindowChild(bool fixed);


		// morph for regular vertices.
		// note that this does NOT delete the provided vertices array.
		void Morph(Vertex* vertices, size_t numVerts);

		// morphs the mesh using the provided vertices as targets. This only takes the positions and normals from the provided vertices.
		// note that this does NOT delete the provided vertices array.
		void Morph(MorphVertex* vertices, size_t numVerts);

		// converts an array of vertices to a morph vertex. Position1 and Normal1 are made the same as Position1 and Normal.
		// remember to delete the original verts array once this function is finished.
		static MorphVertex* ConvertToMorphVertexArray(const Vertex* const verts, const size_t numVerts);

		// creates an inverted cube; moved from Game.cpp
		static cherry::Mesh::Sptr MakeInvertedCube();

		// creates a subdivided plane; moved from Game.cpp
		static cherry::Mesh::Sptr MakeSubdividedPlane(float size, int numSections, bool worldUvs = true);

		// tells the mesh to cull the faces
		bool cullFaces = true;

		// if 'true', then this mesh gets post processed when frame buffers are enabled.
		// if 'false', this mesh ignores post processing.
		bool postProcess = true;

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

		// if 'true', it is drawn in perspective.
		// if 'false', then the object is drawn in orthgoraphic.
		bool perspectiveMesh = true;

		// if 'true', the screen positon is fixed regardless of the placement of the camera.
		bool windowChild = false;

		// TODO: implement method of redrawing the mesh in multiple locations
		// cherry::Vec3 offset; // offset for each draw call
		// unsigned int arrRows = 1; // y-axis
		// unsigned int arrCols = 1; // x-axis
		// unsigned int planes = 1; // z-axis
	};
}