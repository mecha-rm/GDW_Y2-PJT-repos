// OBJECT CLASS (HEADER)
#pragma once

#include "..\Shader.h"
#include "..\Mesh.h"

#include <string>
#include <fstream> // uses the fstream file reading method.
#include <vector>

namespace pc
{
	class Object
	{
	public:
		// the name and directory of the .obj file
		Object(std::string filePath);

		// std::string getFile() const;

		// sets the file. Make sure to include the file path so that
		//void setFile(std::string file);

		// gets the name of the object.
		std::string getName() const;

		// sets the name of the object.
		void setName(std::string newName);

		// gets the name of the object.
		std::string getDescription() const;

		// sets the name of the object.
		void setDescription(std::string newDesc);

	// gets the position of the object.
	glm::vec3 getPosition() const;

	// sets the position of the object.
	void setPosition(glm::vec3 newPos);

	// returns 'true' if the file is safe to use, and 'false' if it isn't. If it's false, then something is wrong with the file.
	bool getSafe();

		// returns the color as a glm::vec3
		// this returns only the colour of the first vertex, so if other vertices have different colours, they are not set.
		glm::vec4 getColor() const;

		// sets the colour based on a range of 0 to 255 for the RGB values. Alpha (a) stll ranges from 0.0 to 1.0
		// this overrides ALL RGB values for all vertices
		void setColor(int r, int g, int b, float a = 1.0F);

		// sets the colour based on a range of 0.0 to 1.0 (OpenGL default)
		// this overrides ALL RGBA values for all vertices
		void setColor(float r, float g, float b, float a = 1.0F);

		// sets the colour of the mesh. This leaves out the alpha (a) value, which is set to whatever it is for the first vertex.
		void setColor(glm::vec3 color);

		// sets the colour of the mesh (RGBA [0-1]).
		void setColor(glm::vec4 color);

		// Setting a vertex colour
		//// gets the colour of a specific vertex
		//glm::vec4 getVertexColor(unsigned int index) const;

		//void setVertexColor(unsigned int index, int r, int g, int b, float a = 1.0F);

		//void setVertexColor(unsigned int index, float r, float g, float b, float a = 1.0F);

		//void setVertexColor(unsigned int index, glm::vec3 color);

		//void setVertexColor(unsigned int index, glm::vec4 color);

		// returns a reference to the mesh.
		Mesh::Sptr& getMesh();
		// Mesh::Sptr * getMesh();

		// the maximum amount of vertices one object can have. This doesn't get used.
		const static unsigned int VERTICES_MAX;

		// the maximum amount of indices one object can have. This doesn't get used.
		const static unsigned int INDICES_MAX;

	private:
		// void setMesh(Mesh::sptr);

		// called to load the object
		bool loadObject();

		// parses the line, gets the values as data type T, and stores them in a vector.
		// containsSymbol: tells the function if the string passed still contains the symbol at the start. If so, it is removed before the parsing begins.
		// *** the symbol at the start is what's used to determine what the values in a given line of a .obj are for.
		template<typename T>
		const std::vector<T> parseStringForTemplate(std::string str, bool containsSymbol = true);

		// the string for the file path
		std::string filePath = "";

		// object name
		std::string name = "";

		// object description
		std::string description = "";

		// true if the file is safe to read from, false otherwise.
		bool safe = false;

		// is mesh in wireframe mode
		bool isWireframe();

		// enable wireframe on mesh
		void enableWireframe();

		// disable wireframe on mesh
		void disableWireframe();


	protected:
		// constructor used for default primitives
		Object();

		// the position of the object.
		glm::vec3 position;

		// a dynamic array of vertices for the 3D model.
		Vertex* vertices = nullptr;

		// the number of vertices that exist for the 3D model.
		unsigned int verticesTotal = 0;

		// a dynamic array of indices for the 3D model.
		uint32_t* indices = nullptr;

		// the total number of indices 
		unsigned int indicesTotal = 0;

		// texture UV array - this doesn't do anything for this assignment
		glm::vec2* vertexTextures = nullptr;

		// total amount of texture vertices
		unsigned int vertexTexturesTotal = 0;

		// the array for vertex normals - doesn't do anythin for this project
		glm::vec3* vertexNormals = nullptr;

		// total amount of vertex normals
		unsigned int vertexNormalsTotal = 0;

		// the mesh
		Mesh::Sptr mesh;
	};
}

