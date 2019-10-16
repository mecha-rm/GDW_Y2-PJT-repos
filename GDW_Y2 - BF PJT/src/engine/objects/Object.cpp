// OBJECT CLASS (SOURCE)
#include "Object.h"
#include "..\utils\Utils.h"

#include <iostream>
#include <sstream>

// the maximum amount of vertices; this value isn't used
const unsigned int Object::VERTICES_MAX = pow(2, 32);

// the maximum amount of indices; this value isn't used
const unsigned int Object::INDICES_MAX = pow(2, 32);

// replaces all instances of one substring with another substring
// this function is for replacing slashes ('/') with spaces (' ') since the sstream operation splits up strings based on their spaces.
//std::string replaceSubstring(std::string str, std::string oldSubstr, std::string newSubstr)
//{
//	unsigned int index; // the index of the old substring.
//
//	// while str.find() doesn't return std::string::npos (i.e. instances still exist)
//	while (str.find(oldSubstr) != std::string::npos)
//	{
//		index = str.find(oldSubstr); // finds the old substring.
//		str.replace(index, oldSubstr.length(), newSubstr); // replaces the old substring with the new substring.
//	}
//
//	return str; // returns the string
//}

// constructor - gets the filename and opens it.
Object::Object(std::string filePath) : position(), vertices(nullptr), indices(nullptr), vertexNormals(nullptr)
{
	this->filePath = filePath; // saves the file path
	
	std::ifstream file(filePath, std::ios::in); // opens the file
	// file.open(filePath, std::ios::in); // opens file

	// file access failure check.
	if (!file)
	{
		safe = false; // file cannot be used
		throw std::runtime_error("Error opening file. Functions for this object should not be used.");
		return;
	}
	else // if file opening was successful, it is safe to read from.
	{
		safe = true;
	}

	// closes the file since it was only opened for this check.
	file.close();

	// loads the object
	loadObject();
}

// the protected constructor used for default primitives
Object::Object() : position(), vertices(nullptr), indices(nullptr), vertexNormals(nullptr) { filePath = ""; }

// gets the name of the object.
std::string Object::getName() const { return name; }

// sets the name of the object.
void Object::setName(std::string newName) { name = newName; }

// gets the object description
std::string Object::getDescription() const { return description; }

// sets the object description
void Object::setDescription(std::string newDesc) { description = newDesc; }

// returns true if the file is safe to use, false if not safe to use.
bool Object::getSafe() { return safe; }

// returns a pointer to the mesh.
Mesh::Sptr & Object::getMesh() { return mesh; }

// creates the object.
bool Object::loadObject()
{
	std::ifstream file;
	std::string line = ""; // the current line of the file.
	std::vector<float> tempVecFlt; // a temporary float vector. Used to save the results of a parsing operation.
	std::vector<Vertex> vertVec; // a vector of vertices; gets all vertices from the file before putting them in the array.
	
	std::vector<uint32_t>tempVecUint; // temporary vector for uin32_t data. Saves information from parsing operation.
	std::vector<uint32_t> indiVec; // a vector of indices; gets all indices from the file before putting them into the array.

	// Unused
	std::vector<glm::vec2>VtVec; // temporary vector for vertex vector coordinates; saves values, but doesn't actually get used
	std::vector<glm::vec3>VnVec; // temporary vector for vertex normals; saves values, but doesn't actually get used

	file.open(filePath, std::ios::in); // opens file

	// if the file is closed.
	if (!file || !file.is_open())
	{
		std::cout << "File not open. Error encountered." << std::endl;
		safe = false;
		return safe;
	}

	// while there are still lines to receive from the file.
	while (std::getline(file, line))
	{
		if (line.length() == 0) // if there was nothing on the line, then it is skipped.
			continue;

		// object name
		if (line.substr(0, 2) == "o " || line.at(0) == 'o')
		{
			// if the line gotten is the name, it is saved into the name string.
			name = line.substr(2);
		}
		// comment; this is added to the object description
		else if (line.substr(0, 2) == "# " || line[0] == '#')
		{
			description += line.substr(2);
			continue;
		}
		// vertex
		else if (line.substr(0, 2) == "v ")
		{
			/*
			 * Versions:
			 *** (x, y, z) (version used by Blender)
			 *** (x, y, z, r, g, b)
			 *** (x, y, z, w)
			 *** (x, y, z, w, r, g, b)
			*/
			tempVecFlt = parseStringForTemplate<float>(line); // gets the values from the line

			// checks what version was used.
			switch (tempVecFlt.size())
			{
			case 3: // (x, y, z)
			case 4: // (x, y, z, w) (n/a) ('w' value is ignored)
				vertVec.push_back(Vertex{ {tempVecFlt[0], tempVecFlt[1], tempVecFlt[2]}, {1.0F, 1.0F, 1.0F, 1.0F} });
				break;

			case 6: // (x, y, z, r, g, b)
				vertVec.push_back(Vertex{ {tempVecFlt[0], tempVecFlt[1], tempVecFlt[2]}, {tempVecFlt[3], tempVecFlt[4], tempVecFlt[5], 1.0F} });
			case 7: // (x, y, z, w, r, g, b) (n/a) ('w' value is ignored)
				vertVec.push_back(Vertex{ {tempVecFlt[0], tempVecFlt[1], tempVecFlt[2]}, {tempVecFlt[4], tempVecFlt[5], tempVecFlt[6], 1.0F} });
			}
		}
		else if (line.substr(0, 2) == "vt") // Texture UV (u, v); not used for anything
		{
			tempVecFlt = parseStringForTemplate<float>(line); // gets values

			VtVec.push_back(glm::vec2(tempVecFlt[0], tempVecFlt[1])); // saves values
		}
		else if (line.substr(0, 2) == "vn") // Vertex Normals (x, y, z); not used at this stage
		{
			tempVecFlt = parseStringForTemplate<float>(line); // gets the values from the line

			VnVec.push_back(glm::vec3(tempVecFlt[0], tempVecFlt[1], tempVecFlt[2])); // stores them
		}
		// indices
		else if (line.substr(0, 2) == "f ")
		{
			// passes the line and replaces all '/' with ' ' so that the string parser can work.
			// format: (face/texture/normal) (shortened to (v1/vt/vn).
			tempVecUint = parseStringForTemplate<uint32_t>(util::replaceSubstring(line, "/", " "));

			// We only need every 1st value in a set, which this loop accounts for.
			for (int i = 0; i < tempVecUint.size(); i += 3)
			{
				indiVec.push_back(tempVecUint[i]);
			}
			
		}
	}

	verticesTotal = indiVec.size(); // gets the total amount of vertices, which is currenty based on the total amount of indices.
	vertices = new Vertex[verticesTotal]; // making the dynamic array of vertices

	// if (verticesTotal > VERTICES_MAX) // if it exceeds the limit, it is set at the limit; not used
		// verticesTotal = VERTICES_MAX;

	// puts the vertices into the dynamic vertex buffer array.
	for (int i = 0; i < indiVec.size(); i++)
		vertices[i] = vertVec[indiVec[i] - 1];
	
	indicesTotal = indiVec.size(); // gets the total number of indices.
	indices = new uint32_t[indicesTotal]; // creates the dynamic array

	// if (indicesTotal > INDICES_MAX) // if it exceeds the limit, it is set at the limit; not used
		// indicesTotal > INDICES_MAX;

	indices = indiVec.data(); // gets the indices as an array; not being used at this time.

	// saving the vertex textures; not being used at this time
	vertexTextures = VtVec.data();
	vertexTexturesTotal = VtVec.size();

	// saving the vertex normals; not used for this project
	vertexNormals = VnVec.data();
	vertexNormalsTotal = VnVec.size();

	mesh = std::make_shared<Mesh>(vertices, verticesTotal, nullptr, 0); // creates the mesh
	return (safe = true); // returns whether the object was safely loaded.
}

// parses a string to get all the values from it as data type (T).
template<typename T>
const std::vector<T> Object::parseStringForTemplate(std::string str, bool containsSymbol)
{
	std::stringstream ss; // the string stream.
	std::vector<T> vec; // the vector used for the vertex.
	T var; // used to store the item from the string.

	// if the string is of length 0, then an empty vector is returned.
	if (str.length() == 0)
		return std::vector<T>();

	if (containsSymbol) // checks if the symbol is still in the string. If so, it is removed.
	{
		str.erase(0, str.find_first_of(" ")); // erases the start of the string, which contains the symbol.
	}

	ss.str(str); // stores the string in the stream

	while (ss >> var) // while the string stream isn't empty
	{
		// if the conversion failed, the string stream moves onto the next item.
		if (ss.bad())
			continue;

		vec.push_back(var); // saves in the vector
	}

	return vec; // returns the vector
}