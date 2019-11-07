// OBJECT CLASS (HEADER)
#pragma once

#include <string>
#include <fstream> // uses the fstream file reading method.
#include <vector>
#include <math.h>

#include <entt/entt.hpp>

#include "..\Shader.h"
#include "..\Mesh.h"
#include "..\utils\math\Consts.h"
#include "..\Camera.h"
#include "..\VectorCRY.h"
#include "..\Material.h"
// #include "..\Physics.h"

namespace cherry
{
	class PhysicsBody;

	class Object
	{
	public:
		// the name and directory of the .obj file
		Object(std::string filePath);

		// sets the file path for the object, the m_Scene it's in, and it's material. 
		// If called, CreateEntity() does not need to be called after.
		Object(std::string filePath, std::string scene, cherry::Material::Sptr material);

		// Objects(verts, indices)

		// destructor
		~Object();

		// std::string getFile() const;

		// sets the file. Make sure to include the file path so that
		//void setFile(std::string file);

		// gets the name of the object.
		std::string GetName() const;

		// sets the name of the object.
		void SetName(std::string newName);

		// gets the desc of the object.
		std::string GetDescription() const;

		// sets the desc of the object.
		void SetDescription(std::string newDesc);

		// returns 'true' if the file is safe to use, and 'false' if it isn't. If it's false, then something is wrong with the file.
		bool GetSafe();

		// returns the color as a glm::vec3
		// this returns only the colour of the first vertex, so if other vertices have different colours, they are not set.
		glm::vec4 GetColor() const;

		// sets the colour based on a range of 0 to 255 for the RGB values. Alpha (a) stll ranges from 0.0 to 1.0
		// this overrides ALL RGB values for all vertices
		void SetColor(int r, int g, int b, float a = 1.0F);

		// sets the colour based on a range of 0.0 to 1.0 (OpenGL default)
		// this overrides ALL RGBA values for all vertices
		void SetColor(float r, float g, float b, float a = 1.0F);

		// sets the colour of the mesh. This leaves out the alpha (a) value, which is set to whatever it is for the first vertex.
		void SetColor(glm::vec3 color);

		// sets the colour of the mesh (RGBA [0-1]).
		void SetColor(glm::vec4 color);

		// makes the mesh rainbow
		// setRainbow()

		// Setting a vertex colour
		//// gets the colour of a specific vertex
		//glm::vec4 getVertexColor(unsigned int index) const;

		//void setVertexColor(unsigned int index, int r, int g, int b, float a = 1.0F);

		//void setVertexColor(unsigned int index, float r, float g, float b, float a = 1.0F);

		//void setVertexColor(unsigned int index, glm::vec3 color);

		//void setVertexColor(unsigned int index, glm::vec4 color);

		// object is in wireframe mode.
		bool IsWireframeMode();

		// if 'true' is passed, wireframe is enabled. If false is passed, wireframe is disabled.
		void SetWireframeMode(bool wf);

		// toggles wireframe mode on/off.
		void SetWireframeMode();

		// enable wireframe on the model.
		void EnableWireframeMode();

		// disable wireframe on hte model.
		void DisableWireframeMode();

		// returns the vertices of the mesh in model view.
		const cherry::Vertex * const GetVertices() const;

		// returns the total amount of vertices
		unsigned int GetVerticesTotal() const;

		// returns the indices of the mesh in model view.
		const uint32_t * const GetIndices() const;

		// returns the total amount of indices
		unsigned int GetIndicesTotal() const;

		// returns a reference to the mesh.
		Mesh::Sptr& GetMesh();

		// creates the entity with the provided m_Scene and material.
		void CreateEntity(std::string scene, cherry::Material::Sptr material);

		
		// gets the position as an engine vector
		cherry::Vec3 GetPosition() const;

		// gets the position as a GLM vector
		glm::vec3 GetPositionGLM() const;
		

		// sets the position
		void SetPosition(float x, float y, float z);

		// setting a new position.
		void SetPosition(cherry::Vec3 newPos);

		// sets the position
		void SetPosition(glm::vec3 newPos);


		// returns the rotation of the object. The boolean determines if it's returned in degrees or radians
		cherry::Vec3 GetRotation(bool inDegrees);

		// sets the rotation for the object. Bool'InDegrees' determines if the provided values are in degrees or radians.
		void SetRotation(cherry::Vec3 theta, bool inDegrees);

		// gets the rotation in degrees, which is the storage default.
		cherry::Vec3 GetRotationDegrees() const;

		// sets the rotation in degrees
		void SetRotationDegrees(cherry::Vec3 theta);

		// gets the rotation in radians
		cherry::Vec3 GetRotationRadians() const;


		// sets the rotation in degrees
		void SetRotationRadians(cherry::Vec3 theta);

		// gets the x-axis rotation in degrees
		float SetRotationXDegrees();

		// sets the x-axis rotation in degrees
		void SetRotationXDegrees(float degrees);

		// gets the x-axis rotation in radians
		float GetRotationXRadians();

		// gets the x-axis rotation in radians
		void SetRotationXRadians(float radians);


		// gets the y-axis rotation in degrees
		float GetRotationYDegrees();

		// sets the y-axis rotation in degrees
		void SetRotationYDegrees(float degrees);

		// gets the y-axis rotation in radians
		float GetRotationYRadians();

		// gets the y-axis rotation in radians
		void SetRotationYRadians(float radians);


		// gets the z-axis rotation in degrees
		float GetRotationZDegrees();

		// sets the z-axis rotation in degrees
		void SetRotationZDegrees(float degrees);

		// gets the z-axis rotation in radians
		float GetRotationZRadians();

		// gets the z-axis rotation in radians
		void SetRotationZRadians(float radians);


		// adds a physics body; returns true if added. The same physics body can't be added twice.
		bool AddPhysicsBody(cherry::PhysicsBody * body);

		// removes a physics body; returns 'true' if successful.
		bool RemovePhysicsBody(cherry::PhysicsBody * body);

		// removes a physics body based on its index.
		bool RemovePhysicsBody(unsigned int index);

		// gets the amount of physics bodies
		unsigned int GetPhysicsBodyCount() const;

		// gets the physics bodies
		std::vector<cherry::PhysicsBody *> GetPhysicsBodies() const;
		
		// gets whether the object intersects with another object.
		bool GetIntersection() const;

		// sets whether the object is currently intersecting with another object.
		void SetIntersection(bool inter);

		// updates the object
		void Update(float deltaTime);

		virtual std::string ToString() const;

		// the maximum amount of vertices one object can have. This doesn't get used.
		const static unsigned int VERTICES_MAX;

		// the maximum amount of indices one object can have. This doesn't get used.
		const static unsigned int INDICES_MAX;

	private:
		// void setMesh(Mesh::sptr);

		// called to load the object
		bool LoadObject();

		// parses the line, gets the values as data type T, and stores them in a vector.
		// containsSymbol: tells the function if the string passed still contains the symbol at the start. If so, it is removed before the parsing begins.
		// *** the symbol at the start is what's used to determine what the values in a given line of a .obj are for.
		template<typename T>
		const std::vector<T> parseStringForTemplate(std::string str, bool containsSymbol = true);

		// template<typename T>
		// void calculateNormals(std::vector<);

		// the string for the file path
		std::string filePath = "";

		// a vector of physics bodies
		std::vector<cherry::PhysicsBody*> bodies;

		// becomes 'true' when an object intersects something.
		bool intersection = false;

		// used for object transformations
		// entt::registry ecs;
		// the entity for the object
		// entt::entity entity;

		// saves the rotation on the x, y, and z axis in DEGREES.
		cherry::Vec3 rotation;

	protected:
		// constructor used for default primitives
		Object();
		

		// object name
		std::string name = "";

		// object description
		std::string description = "";

		// true if the file is safe to read from, false otherwise.
		bool safe = false;

		// the position of the object.
		cherry::Vec3 position;

		// a dynamic array of vertices for the 3D model.
		Vertex* vertices = nullptr;

		// the number of vertices that exist for the 3D model.
		unsigned int verticesTotal = 0;

		// a dynamic array of indices for the 3D model.
		uint32_t* indices = nullptr;

		// the total number of indices 
		unsigned int indicesTotal = 0;

		// the mesh
		Mesh::Sptr mesh;

		// the color of the model.
		// cherry::Vec4 color;

		// the material of the object.
		Material::Sptr material;
	};
}

