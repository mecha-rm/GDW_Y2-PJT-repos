// OBJECT CLASS (HEADER)
#pragma once

#include "Shader.h"
#include "Mesh.h"

#include <string>
#include <fstream> // uses the fstream file reading method.
#include <vector>

class Object
{
public:
	// the name and directory of the .obj file
	Object(std::string filePath);

	// gets the name of the object.
	std::string getName() const;

	// sets the name of the object.
	void setName(std::string newName);

	// gets the name of the object.
	std::string getDescription() const;

	// sets the name of the object.
	void setDescription(std::string newDesc);

	// returns 'true' if the file is safe to use, and 'false' if it isn't. If it's false, then something is wrong with the file.
	bool getSafe();

	// returns a reference to the mesh.
	Mesh::Sptr & getMesh();

	// the maximum amount of vertices one object can have. This doesn't get used.
	const static unsigned int VERTICES_MAX;

	// the maximum amount of indices one object can have. This doesn't get used.
	const static unsigned int INDICES_MAX;

private:

	// called to load the object
	bool loadObject();

	// parses the line, gets the values as data type T, and stores them in a vector.
	// containsSymbol: tells the function if the string passed still contains the symbol at the start. If so, it is removed before the parsing begins.
	// *** the symbol at the start is what's used to determine what the values in a given line of a .obj are for.
	template<typename T>
	const std::vector<T> parseStringForTemplate(std::string str, bool containsSymbol = true);

	// the .obj file
	std::ifstream file;

	// true if the file is safe to read from, false otherwise.
	bool safe = false;

	// the position of the object.
	glm::vec3 position;

	// a dynamic array of vertices for the 3D model.
	Vertex * vertices;

	// the number of vertices that exist for the 3D model.
	unsigned int verticesTotal;

	// a dynamic array of indices for the 3D model.
	uint32_t * indices;

	// the total number of indices 
	unsigned int indicesTotal;

	// texture UV array - this doesn't do anything for this assignment
	glm::vec2* vertexTextures;

	// total amount of texture vertices
	unsigned int vertexTexturesTotal;

	// the array for vertex normals - doesn't do anythin for this project
	glm::vec3 * vertexNormals;
	
	// total amount of vertex normals
	unsigned int vertexNormalsTotal;

	// the mesh
	Mesh::Sptr mesh;


protected:
	// object name
	std::string name = "";

	// object description
	std::string description = "";
};

