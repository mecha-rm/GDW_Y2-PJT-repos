// OBJECT CLASS (SOURCE)
#include "Object.h"
#include "..\utils\Utils.h"
#include "..\utils\math\Rotation.h"
#include "..\PhysicsBody.h"

#include "..\SceneManager.h"
#include "..\MeshRenderer.h"

#include <iostream>
#include <sstream>
#include <glm/gtc/quaternion.hpp> 
#include <glm/gtx/quaternion.hpp>
#include <GLM/gtc/matrix_transform.hpp>

// used for transforming the mesh
struct TempTransform {
	glm::vec3 Position = glm::vec3(0.0f);
	glm::vec3 EulerRotation = glm::vec3(0.0f);
	glm::vec3 Scale = glm::vec3(1.0f);

	// does our TRS for us.
	glm::mat4 GetWorldTransform() const {
		return
			glm::translate(glm::mat4(1.0f), Position) *
			glm::mat4_cast(glm::quat(glm::radians(EulerRotation))) *
			glm::scale(glm::mat4(1.0f), Scale)
			;
	}
};

struct UpdateBehaviour {
	std::function<void(entt::entity e, float dt)> Function;
};


// the maximum amount of vertices; this value isn't used
const unsigned int cherry::Object::VERTICES_MAX = pow(2, 32);

// the maximum amount of indices; this value isn't used
const unsigned int cherry::Object::INDICES_MAX = pow(2, 32);

// constructor - gets the filename and opens it.
cherry::Object::Object(std::string filePath) : position(), vertices(nullptr), indices(nullptr)
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
	LoadObject();
}

// creates an object with a m_Scene and material.
cherry::Object::Object(std::string filePath, std::string scene, cherry::Material::Sptr material) : Object(filePath)
{
	CreateEntity(scene, material);
}

// the protected constructor used for default primitives
cherry::Object::Object() : position(), vertices(nullptr), indices(nullptr) { filePath = ""; }

cherry::Object::~Object()
{
	delete[] vertices;
	delete[] indices;
}

// gets the name of the object.
std::string cherry::Object::GetName() const { return name; }

// sets the name of the object.
void cherry::Object::SetName(std::string newName) { name = newName; }

// gets the object description
std::string cherry::Object::GetDescription() const { return description; }

// sets the object description
void cherry::Object::SetDescription(std::string newDesc) { description = newDesc; }

// returns true if the file is safe to use, false if not safe to use.
bool cherry::Object::GetSafe() { return safe; }

// gets the color of the first vertex
glm::vec4 cherry::Object::GetColor() const { return vertices->Color; }

// sets colour based on range of 0 to 255. Alpha (a) still goes from 0 to 1.
void cherry::Object::SetColor(int r, int g, int b, float a) { SetColor((float)r / 255.0F, (float)g / 255.0F, (float)b / 255.0F, a); }

// sets the color for all vertices
void cherry::Object::SetColor(float r, float g, float b, float a)
{
	// bounds checking for RGBA
	r = (r < 0.0F) ? 0.0F : (r > 1.0F) ? 1.0F : r;
	g = (g < 0.0F) ? 0.0F : (g > 1.0F) ? 1.0F : g;
	b = (b < 0.0F) ? 0.0F : (b > 1.0F) ? 1.0F : b;
	a = (a < 0.0F) ? 0.0F : (a > 1.0F) ? 1.0F : a;

	for (int i = 0; i < verticesTotal; i++)
		vertices[i].Color = glm::vec4(r, g, b, a);

	// TODO: doing this causes the mesh to screw up for some reason.
	bool wf = mesh->IsWireframe(); // copying over values
	bool vis = mesh->IsVisible(); // copying over values

	mesh = std::make_shared<Mesh>(vertices, verticesTotal, indices, indicesTotal); // creates the mesh
	mesh->SetWireframe(wf);
	mesh->SetVisible(vis);
}

// sets the color, keeping the alpha (a) value from the first vertex.
void cherry::Object::SetColor(glm::vec3 color) { SetColor(color.x, color.y, color.z, vertices[0].Color.w); }

// sets the color (RGBA [0-1])
void cherry::Object::SetColor(glm::vec4 color) { SetColor(color.x, color.y, color.z, color.w); }

// checks to see if the object is in wireframe mode.
bool cherry::Object::IsWireframeMode() { return mesh->IsWireframe(); }

// enables or disables wireframe, based on 'bool' passed
void cherry::Object::SetWireframeMode(bool wf) { mesh->SetWireframe(wf); }

// toggle's wireframe mode on/off.
void cherry::Object::SetWireframeMode() { mesh->SetWireframe(); }

// enables the wireframe
void cherry::Object::EnableWireframeMode() { mesh->enableWireframe(); }

// disables the wireframe
void cherry::Object::DisableWireframeMode() { mesh->DisableWireframe(); }

// returns the vertices
const cherry::Vertex* const cherry::Object::GetVertices() const { return vertices; }

// returns the total amount of vertices
unsigned int cherry::Object::GetVerticesTotal() const { return verticesTotal; }

// returns the indicies for a given mesh.
const uint32_t* const cherry::Object::GetIndices() const { return indices; }

// returns the total amount of indices
unsigned int cherry::Object::GetIndicesTotal() const { return indicesTotal; }

// returns a pointer to the mesh.
cherry::Mesh::Sptr& cherry::Object::GetMesh() { return mesh; }

// creates the object.
bool cherry::Object::LoadObject()
{
	std::ifstream file; // file
	std::string line = ""; // the current line of the file.

	std::vector<float> tempVecFlt; // a temporary float vector. Used to save the results of a parsing operation.
	std::vector<uint32_t>tempVecUint; // temporary vector for uin32_t data. Saves information from parsing operation.

	// vertex indices
	std::vector<Vertex> vertVec; // a vector of vertices; gets all vertices from the file before putting them in the array.
	std::vector<uint32_t> vertIndices; // a vector of indices; gets all indices from the file before putting them into the array.
	
	// textures
	std::vector<glm::vec2>vtVec; // temporary vector for vertex vector coordinates; saves values, but doesn't actually get used
	std::vector<unsigned int> textIndicies; // a vector of texture indices.

	// normals
	std::vector<glm::vec3>vnVec; // temporary vector for vertex normals; saves values, but doesn't actually get used
	std::vector<unsigned int> normIndices; // vector of vertex normal indices

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
				vertVec.push_back(Vertex{ {tempVecFlt[0], tempVecFlt[1], tempVecFlt[2]}, {1.0F, 1.0F, 1.0F, 1.0F}, {0.0F, 0.0F, 0.0F} });
				break;

			case 6: // (x, y, z, r, g, b)
				vertVec.push_back(Vertex{ {tempVecFlt[0], tempVecFlt[1], tempVecFlt[2]}, {tempVecFlt[3], tempVecFlt[4], tempVecFlt[5], 1.0F}, {0.0F, 0.0F, 0.0F} });
			case 7: // (x, y, z, w, r, g, b) (n/a) ('w' value is ignored)
				vertVec.push_back(Vertex{ {tempVecFlt[0], tempVecFlt[1], tempVecFlt[2]}, {tempVecFlt[4], tempVecFlt[5], tempVecFlt[6], 1.0F}, {0.0F, 0.0F, 0.0F} });
			}
		}
		else if (line.substr(0, 2) == "vt") // Texture UV (u, v); not used for anything
		{
			tempVecFlt = parseStringForTemplate<float>(line); // gets values

			vtVec.push_back(glm::vec2(tempVecFlt[0], tempVecFlt[1])); // saves values
		}
		// TODO: add vertex normals
		else if (line.substr(0, 2) == "vn") // Vertex Normals (x, y, z); not used at this stage
		{
			tempVecFlt = parseStringForTemplate<float>(line); // gets the values from the line

			vnVec.push_back(glm::vec3(tempVecFlt[0], tempVecFlt[1], tempVecFlt[2])); // stores them
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
				// vertex indice/vertex texture indice/vertex normal indice
				// v1/vt1/vn1
				vertIndices.push_back(tempVecUint[i]);
				textIndicies.push_back(tempVecUint[i + 1]);
				normIndices.push_back(tempVecUint[i + 2]);
			}

		}
	}

	verticesTotal = vertIndices.size(); // gets the total amount of vertices, which is currenty based on the total amount of indices.
	vertices = new Vertex[verticesTotal]; // making the dynamic array of vertices

	// if (verticesTotal > VERTICES_MAX) // if it exceeds the limit, it is set at the limit; not used
		// verticesTotal = VERTICES_MAX;

	// puts the vertices into the dynamic vertex buffer array.
	for (int i = 0; i < vertIndices.size(); i++)
		vertices[i] = vertVec[vertIndices[i] - 1];

	indicesTotal = vertIndices.size(); // gets the total number of indices.
	indices = new uint32_t[indicesTotal]; // creates the dynamic array

	// if (indicesTotal > INDICES_MAX) // if it exceeds the limit, it is set at the limit; not used
		// indicesTotal > INDICES_MAX;

	indices = vertIndices.data(); // gets the indices as an array; not being used at this time.

	// calculating the normals
	{
		// vertex normal indicies and vertex indicies are the same size
		// calculates how many times a given normal is used.
		for (int i = 0; i < normIndices.size(); i++)
		{
			// adding the normal to the vertex
			vertices[i].Normal = vnVec.at(normIndices[i] - 1);
		}
	}

	// calculating the UVs
	{
		// vertex normal indicies and vertex indicies are the same size
		// calculates how many times a given normal is used.
		for (int i = 0; i < textIndicies.size(); i++)
		{
			// adding the uvs to the designated vertices
			vertices[i].UV = vtVec.at(textIndicies.at(i) - 1);
		}
	}

	// creates the mesh
	// unlike with the default primitives, the amount of vertices corresponds to how many indicies there are, and the values are set accordingly.
	mesh = std::make_shared<Mesh>(vertices, verticesTotal, nullptr, 0);

	return (safe = true); // returns whether the object was safely loaded.
}

// creates an entity with the provided m_Scene.
void cherry::Object::CreateEntity(std::string scene, cherry::Material::Sptr material)
{
	this->material = material; // saves the material.

	// sets up the Update function for the entity. This gets automatically called.
	auto& ecs = GetRegistry(scene);
	entt::entity entity = ecs.create();

	MeshRenderer& mr = ecs.assign<MeshRenderer>(entity);
	mr.Material = material;
	mr.Mesh = mesh;

	auto tform = [&](entt::entity e, float dt) 
	{
		auto& transform = CurrentRegistry().get_or_assign<TempTransform>(e);

		transform.Position = glm::vec3(position.GetX(), position.GetY(), position.getZ()); // udpates the position
		transform.EulerRotation = glm::vec3(rotation.GetX(), rotation.GetY(), rotation.getZ()); // UPDATES THE ROTATION


		// does the same thing, except all in one line (for rotation)
		// CurrentRegistry().get_or_assign<TempTransform>(e).EulerRotation += glm::vec3(0, 0, 90 * dt);
	};

	auto& up = ecs.get_or_assign<UpdateBehaviour>(entity);
	up.Function = tform;
}

// gets the entity of the object
// entt::entity& cherry::Object::getEntity() { return entity; }


cherry::Vec3 cherry::Object::GetPosition() const { return position; }

// gets the object's position as a glm vector
glm::vec3 cherry::Object::GetPositionGLM() const { return glm::vec3(position.v.x, position.v.y, position.v.z); }

void cherry::Object::SetPosition(float x, float y, float z) { SetPosition(glm::vec3(x, y, z)); }

// sets the position
void cherry::Object::SetPosition(glm::vec3 newPos) { position = cherry::Vec3(newPos); }

// setting a new position
void cherry::Object::SetPosition(cherry::Vec3 newPos) { position = newPos; }



// ROTATION FUNCTIONS

// gets the rotation in the requested form.
cherry::Vec3 cherry::Object::GetRotation(bool inDegrees) { return inDegrees ? GetRotationDegrees() : GetRotationRadians(); }

// sets the rotation in the requested form.
void cherry::Object::SetRotation(cherry::Vec3 theta, bool inDegrees) { inDegrees ? SetRotationDegrees(theta) : SetRotationRadians(theta); }

// returns rotation in degrees, which is the storage default.
cherry::Vec3 cherry::Object::GetRotationDegrees() const { return rotation; }

// sets the rotation in degrees
void cherry::Object::SetRotationDegrees(cherry::Vec3 theta) { rotation = theta; }

// gets the rotation in radians
cherry::Vec3 cherry::Object::GetRotationRadians() const
{
	// returns the rotation in radians
	return cherry::Vec3(
		util::math::degreesToRadians(rotation.v.x),
		util::math::degreesToRadians(rotation.v.y),
		util::math::degreesToRadians(rotation.v.z)
	);
}

// sets the rotation in degrees
void cherry::Object::SetRotationRadians(cherry::Vec3 theta)
{
	// converts to degrees due to being stored as degrees.
	rotation = cherry::Vec3(
		util::math::radiansToDegrees(theta.v.x),
		util::math::radiansToDegrees(theta.v.y),
		util::math::radiansToDegrees(theta.v.z)
	);
}

// gets the rotation on the x-axis in degrees
float cherry::Object::SetRotationXDegrees() { return rotation.v.x; }

// sets the rotation on the x-axis in degrees.
void cherry::Object::SetRotationXDegrees(float degrees) { rotation.v.x = degrees; }

// gets the rotation on the x-axis in radians
float cherry::Object::GetRotationXRadians() { return util::math::degreesToRadians(rotation.v.x); }

// sets the rotation on the x-axis in radians.
void cherry::Object::SetRotationXRadians(float radians) { rotation.v.x = util::math::radiansToDegrees(radians); }


// gets the rotation on the y-axis in degrees
float cherry::Object::GetRotationYDegrees() { return rotation.v.y; }

// sets the rotation on the y-axis in degrees.
void cherry::Object::SetRotationYDegrees(float degrees) { rotation.v.y = degrees; }

// gets the rotation on the y-axis in radians
float cherry::Object::GetRotationYRadians() { return util::math::degreesToRadians(rotation.v.y); }

// sets the rotation on the y-axis in radians.
void cherry::Object::SetRotationYRadians(float radians) { rotation.v.y = util::math::radiansToDegrees(radians); }


// gets the rotation on the z-axis in degrees
float cherry::Object::GetRotationZDegrees() { return rotation.v.z; }

// sets the rotation on the z-axis in degrees.
void cherry::Object::SetRotationZDegrees(float degrees) { rotation.v.z = degrees; }

// gets the rotation on the z-axis in radians
float cherry::Object::GetRotationZRadians() { return util::math::degreesToRadians(rotation.v.z); }

// sets the rotation on the z-axis in radians.
void cherry::Object::SetRotationZRadians(float radians) { rotation.v.y = util::math::radiansToDegrees(radians); }




// returns true if added successfully.
bool cherry::Object::AddPhysicsBody(cherry::PhysicsBody* body) 
{ 
	if (body == nullptr)
		return false;

	return util::addToVector(bodies, body->attachToObject(this));
	// body->attachToObject(this);
}

// returns 'true' if removed, false if not.
bool cherry::Object::RemovePhysicsBody(cherry::PhysicsBody* body) 
{
	if (body == nullptr)
		return false;

	return util::removeFromVector(bodies, body); 
}

// removes physics body based on index
bool cherry::Object::RemovePhysicsBody(unsigned int index)
{
	if (index >= 0 && index < bodies.size()) // erases the body
	{
		bodies.erase(bodies.begin() + index);
		return true;
	}

	return false;
}

// gets the amount of physics bodies
unsigned int cherry::Object::GetPhysicsBodyCount() const { return bodies.size(); }

// returns hte physics bodies
std::vector<cherry::PhysicsBody*> cherry::Object::GetPhysicsBodies() const { return bodies; }

// gets if intersection is happening.
bool cherry::Object::GetIntersection() const { return intersection; }

// sets whether the object is interecting with something or not.
void cherry::Object::SetIntersection(bool inter) { intersection = inter; }

// updates the object
void cherry::Object::Update(float deltaTime)
{
	// TODO: remove this for the final version.
	rotation.SetX(rotation.GetX() + 15.0F * deltaTime);
	rotation.setZ(rotation.getZ() + 90.0F * deltaTime);
}

// returns a string representing the object
std::string cherry::Object::ToString() const
{
	return "Name: " + name + " | Description: " + description + " | Position: " + position.ToString();
}

// parses a string to get all the values from it as data type (T).
template<typename T>
const std::vector<T> cherry::Object::parseStringForTemplate(std::string str, bool containsSymbol)
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