#include "Primitive.h"

// constructor
cherry::Primitive::Primitive() : cherry::Object()
{
	
}

// copy constructor.
// cherry::Primitive::Primitive(const cherry::Primitive& prim) : Object(prim)
// {
// 	baseColor = prim.GetColor();
// }

// destructor
cherry::Primitive::~Primitive()
{
	// delete[] indices; // TODO: fix deletions
}

// gets the base colour of the primitive.
cherry::Vec4 cherry::Primitive::GetColor() const { return color; }

// calculates the normals of the primitive.
void cherry::Primitive::CalculateNormals()
{
	// dynamic array of surface normals.
	// float* surfaceNormals = new float[round(indicesTotal / 3.0F)];
	unsigned int* vertexNormals = new unsigned int[verticesTotal] {0}; // gets the vertex normals


	// setting up the normals
	glm::vec3 uVec;
	glm::vec3 vVec;
	glm::vec3 uvVec;

	// goes through each triangle and calculates the normal
	for (int i = 0; i < indicesTotal; i += 3)
	{
		glm::vec3 tempVec;
		// u = p2 - p1
		uVec = vertices[indices[i + 1]].Position - vertices[indices[i]].Position;
		
		// v = p3 - p1
		vVec = vertices[indices[i + 2]].Position - vertices[indices[i]].Position;
		

		// gets the normal vector, which uses cross product.
		// uvVec = uVec.cross(vVec);
		uvVec.x = uVec.y * vVec.z - uVec.z * vVec.y;
		uvVec.y = uVec.z * vVec.x - uVec.x * vVec.z;
		uvVec.z = uVec.x * vVec.y - uVec.y * vVec.x;

		// saves the normals, and how many faces connect to the given vertex
		vertices[indices[i]].Normal += uvVec;
		vertexNormals[indices[i]]++;

		vertices[indices[i + 1]].Normal += uvVec;
		vertexNormals[indices[i + 1]]++;

		vertices[indices[i + 2]].Normal += uvVec;
		vertexNormals[indices[i + 2]]++;
	}

	// averages and normalizes each vertex normal.
	for (int i = 0; i < verticesTotal; i++)
	{
		glm::vec3 x = vertices[i].Normal;
		int y = vertexNormals[i];

		// gets the average of the normals
		vertices[i].Normal = glm::vec3(
			vertices[i].Normal.x / vertexNormals[i], 
			vertices[i].Normal.y / vertexNormals[i], 
			vertices[i].Normal.z / vertexNormals[i]);

		x = vertices[i].Normal;

		// normalizes the normal (v / ||v||)
		vertices[i].Normal = glm::vec3(
			vertices[i].Normal.x / vertices[i].Normal.length(), 
			vertices[i].Normal.y / vertices[i].Normal.length(), 
			vertices[i].Normal.z / vertices[i].Normal.length());

		x = vertices[i].Normal;
		y = vertexNormals[i];
	}
}

// flip the normals
void cherry::Primitive::InvertNormals()
{
	if (vertices == nullptr)
		return;

	for (int i = 0; i < verticesTotal; i++)
		vertices[i].Normal *= -1;
}
