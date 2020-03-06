// OBJECT CLASS (SOURCE)
#include "Object.h"
#include "..\utils\Utils.h"
#include "..\utils\math\Rotation.h"
#include "..\physics/PhysicsBody.h"
#include "..\WorldTransform.h"

#include "..\scenes/SceneManager.h"
#include "..\MeshRenderer.h"
#include "ObjectManager.h"

#include <iostream>
#include <sstream>
#include <glm/gtc/quaternion.hpp> 
#include <glm/gtx/quaternion.hpp>
#include <GLM/gtc/matrix_transform.hpp>

#include "..\Game.h"

// the maximum amount of vertices; this value isn't used
const unsigned int cherry::Object::VERTICES_MAX = pow(2, 32);

// the maximum amount of indices; this value isn't used
const unsigned int cherry::Object::INDICES_MAX = pow(2, 32);

// constructor - gets the filename and opens it.
cherry::Object::Object(std::string filePath, bool loadMtl, bool dynamicObj) 
	: position(), vertices(nullptr), indices(nullptr), dynamicObject(dynamicObj)
{
	this->filePath = filePath; // saves the file path

	std::ifstream file(filePath, std::ios::in); // opens the file
	// file.open(filePath, std::ios::in); // opens file

	// file access failure check.
	if (!file)
	{
		safe = false; // file cannot be used
		// #ifndef _DEBUG
		throw std::runtime_error("Error opening file. Functions for this object should not be used.");
		// #endif // !DEBUG
		file.close();
		return;
	}
	else // if file opening was successful, it is safe to read from.
	{
		safe = true;
	}

	// closes the file since it was only opened for this check.
	file.close();

	// default world transform
	// worldTransform = TempTransform().GetWorldTransform();

	// loads the object
	LoadObject(loadMtl);
}

// loads an object into the requested scene. The bool loadMtl determines if an mtl file ges loaded.
cherry::Object::Object(std::string filePath, std::string scene, bool loadMtl, bool dynamicObj) :
	Object(filePath, loadMtl, dynamicObj)
{
	CreateEntity(scene, material);
}

// creates an object, puts it in the provided scene, and loads in the mtl file.
cherry::Object::Object(std::string filePath, std::string scene, std::string mtl, bool dynamicObj)
	: Object(filePath, false, dynamicObj)
{
	// creates the entity, and loads in the mtl file.
	CreateEntity(scene, Material::GenerateMtl(mtl));
}

// creates an obj file, puts it in a scene, and then applies the material.
cherry::Object::Object(std::string filePath, std::string scene, Material::Sptr material, bool loadMtl, bool dynamicObj) 
	: Object(filePath, false, dynamicObj)
{
	// gets the .obj file name, but replaces the file extension to find the .mtl file.
	if(loadMtl)
		material->LoadMtl(filePath.substr(0, filePath.find_last_of(".")) + ".mtl");

	CreateEntity(scene, material); 
}

// loads an obj file into the provided scene, gives it the material, and then applies the mtl file.
cherry::Object::Object(std::string filePath, std::string scene, Material::Sptr material, std::string mtl, bool dynamicObj)
	:Object(filePath, false, dynamicObj)
{
	material->LoadMtl(mtl);
	CreateEntity(scene, material);
}

// copy constructor
cherry::Object::Object(const cherry::Object& obj)
{
	// temporary variables
	const Vertex* tempVerts = obj.GetVertices();
	const uint32_t* tempIndices = obj.GetIndices();

	// total vertices and indices
	verticesTotal = obj.GetVerticesTotal();
	indicesTotal = obj.GetIndicesTotal();

	// vertices and indices
	vertices = new Vertex[verticesTotal];
	indices = new uint32_t[indicesTotal];
	
	// copying vertices
	// for (int i = 0; i < verticesTotal; i++)
	// 	vertices[i] = Vertex(tempVerts[i]);
	// 
	// // copying indices
	// for (int i = 0; i < indicesTotal; i++)
	// 	indices[i] = tempIndices[i];
	

	name = obj.GetName();
	description = obj.GetDescription();

	memcpy(vertices, obj.GetVertices(), sizeof(Vertex) * verticesTotal);
	memcpy(indices, obj.GetIndices(), sizeof(uint32_t) * indicesTotal);

	path = obj.GetPath();
	followPath = obj.followPath;
	dynamicObject = obj.IsDynamicObject();

	meshBodyMax = obj.GetMeshBodyMaximum();
	meshBodyMin = obj.GetMeshBodyMinimum();

	filePath = obj.GetFilePath();

	position = obj.GetPosition();
	scale = obj.GetScale();
	rotation = obj.GetRotationDegrees();
	
	// copying the animation manager.
	animations = obj.animations;

	PhysicsBodyBox* box = nullptr;
	PhysicsBodySphere* sphere = nullptr;

	// copying hte physics body.
	for (PhysicsBody* body : obj.bodies)
	{
		switch (body->GetId())
		{
		case 1: // box		
			box = (PhysicsBodyBox*)body;
			AddPhysicsBody(new PhysicsBodyBox(box->GetLocalPosition(), box->GetLocalWidth(), box->GetLocalHeight(), box->GetLocalDepth()));
			box = nullptr;
			break;

		case 2:// sphere
			sphere = (PhysicsBodySphere*)sphere;
			AddPhysicsBody(new PhysicsBodySphere(sphere->GetLocalPosition(), sphere->GetLocalRadius()));
			sphere = nullptr;
			break;
		}
		
	}
	// TODO: add animation manager
	// TODO: copy physics bodies

	// if the file is an obj file, then the indices shouldn't be used for the mesh.
	if (filePath.substr(filePath.find_last_of(".") + 1) == "obj") // obj file
		mesh = std::make_shared<Mesh>(vertices, verticesTotal, nullptr, 0);
	else // runtime primitive
		mesh = std::make_shared<Mesh>(vertices, verticesTotal, indices, indicesTotal);

	CreateEntity(obj.GetSceneName(), obj.GetMaterial());
}

// the protected constructor used for default primitives
cherry::Object::Object() : position(), vertices(nullptr), indices(nullptr) { filePath = ""; }

cherry::Object::~Object()
{
	// TODO: add back deletions
	delete[] vertices;
	// vertices = nullptr;

	delete[] indices; 
	// indices = nullptr;

	// deleting all of the physics bodies
	for (PhysicsBody* body : bodies)
		delete body;

	bodies.clear();
}


// gets the file path for the object file.
const std::string & cherry::Object::GetFilePath() const { return filePath; }

// returns the scene the object is part of.
std::string cherry::Object::GetSceneName() const { return scene; }

// sets the new scene name.
void cherry::Object::SetScene(std::string newScene)
{
	// removes the object from its object list.
	ObjectManager::RemoveObjectFromSceneObjectList(this);
	
	CreateEntity(newScene, material); // re-creates the entity to switch its scene.

	// adds the object to its scene object list.
	ObjectManager::AddObjectToSceneObjectList(this, true);
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
bool cherry::Object::GetSafe() const { return safe; }

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

// returns the Indices for a given mesh.
const uint32_t* const cherry::Object::GetIndices() const { return indices; }

// returns the total amount of indices
unsigned int cherry::Object::GetIndicesTotal() const { return indicesTotal; }

// returns a pointer to the mesh.
cherry::Mesh::Sptr& cherry::Object::GetMesh() { return mesh; }

// gets the material
const cherry::Material::Sptr& cherry::Object::GetMaterial() const { return material; }

// gets the alpha value of the object.
float cherry::Object::GetAlpha() const { return alpha; }

// sets the alpha value of the object.
void cherry::Object::SetAlpha(float a)
{
	// TODO: this currently doesn't work for primitives.
	// bounds checking
	alpha = (a < 0.0F) ? 0.0F : (a > 1.0F) ? 1.0F : a;

	// if the object doesn't have a 100% alpha value, then it won't need to be sorted for proper transparency.
	if (material != nullptr)
	{
		material->HasTransparency = (alpha < 1.0F) ? true : false;
		material->Set("a_Alpha", alpha);
	}
}

// returns if the object is visible
bool cherry::Object::IsVisible() const { return mesh->IsVisible(); }

// toggle's visibility on/off
void cherry::Object::SetVisible() { mesh->SetVisible(); }

// sets whether the object is visible.
void cherry::Object::SetVisible(bool visible) { mesh->SetVisible(visible); }


// returns 'true' if the object is drawn in perspective.
bool cherry::Object::IsPerspectiveObject() const { return mesh->IsPerspectiveMesh(); }

// sets if the object is drawn in perspective mode.
void cherry::Object::SetPerspectiveObject(bool perspective) { mesh->SetPerspectiveMesh(perspective); }

// returns 'true' if the object is drawn in orthographic mode.
bool cherry::Object::IsOrthographicObject() const { return mesh->IsOrthographicMesh(); }

// sets if the object should be drawn in orthographic perpsective.
void cherry::Object::SetOrthographicObject(bool orthographic) { mesh->SetOrthographicMesh(orthographic); }

// if 'true', the screen position of the object is fixed regardless of the camera.
bool cherry::Object::IsWindowChild() const { return (mesh != nullptr) ? mesh->GetWindowChild() : false; }

// sets if the screen position is fixed.
void cherry::Object::SetWindowChild(bool windowChild) 
{ 
	mesh->SetWindowChild(windowChild); 
	ObjectManager::UpdateWindowChild(this);
}

// returns 'true' if the object gets post processed.
bool cherry::Object::GetPostProcess() const { return (mesh != nullptr) ? mesh->postProcess : false; }

// sets whether this object is post processed or not.
void cherry::Object::SetPostProcess(bool postProcess)
{
	if (mesh != nullptr)
		mesh->postProcess = postProcess;
}


// creates the object.
bool cherry::Object::LoadObject(bool loadMtl)
{
	std::ifstream file; // file
	std::string line = ""; // the current line of the file.
	std::string mtllib = ""; // if 'mtllib' isn't empty, then that means a material was found and can be created.

	std::vector<float> tempVecFlt; // a temporary float vector. Used to save the results of a parsing operation.
	std::vector<uint32_t>tempVecUint; // temporary vector for uin32_t data. Saves information from parsing operation.

	// vertex indices
	std::vector<Vertex> vertVec; // a vector of vertices; gets all vertices from the file before putting them in the array.
	std::vector<uint32_t> vertIndices; // a vector of indices; gets all indices from the file before putting them into the array.
	
	// textures
	std::vector<glm::vec2>vtVec; // temporary vector for vertex vector coordinates; saves values, but doesn't actually get used
	std::vector<unsigned int> textIndices; // a vector of texture indices.

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
		if(line.substr(0, line.find_first_of(" ")) == "o")
		{
			// if the line gotten is the name, it is saved into the name string.
			name = line.substr(2);
		}
		// comment; this is added to the object description
		else if (line.substr(0, line.find_first_of(" ")) == "#")
		{
			description += line.substr(2);
			continue;
		}
		// material template library
		else if (line.substr(0, line.find_first_of(" ")) == "mtllib")
		{
			mtllib = line.substr(line.find_first_of(" ") + 1); // saving the material
		}
		// vertex
		else if (line.substr(0, line.find_first_of(" ")) == "v")
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
				vertVec.push_back(Vertex{ {tempVecFlt[0], tempVecFlt[1], tempVecFlt[2]}, {1.0F, 1.0F, 1.0F, 1.0F}, {0.0F, 0.0F, 0.0F} });
				break;

			case 4: // (x, y, z, w) (n/a) ('w' value is ignored); currently same as case 3.
				vertVec.push_back(Vertex{ {tempVecFlt[0], tempVecFlt[1], tempVecFlt[2]}, {1.0F, 1.0F, 1.0F, 1.0F}, {0.0F, 0.0F, 0.0F} });
				break;

			case 6: // (x, y, z, r, g, b)
				vertVec.push_back(Vertex{ {tempVecFlt[0], tempVecFlt[1], tempVecFlt[2]}, {tempVecFlt[3], tempVecFlt[4], tempVecFlt[5], 1.0F}, {0.0F, 0.0F, 0.0F} });
				break;

			case 7: // (x, y, z, w, r, g, b) (n/a) ('w' value is ignored); currently the same as case 6.
				vertVec.push_back(Vertex{ {tempVecFlt[0], tempVecFlt[1], tempVecFlt[2]}, {tempVecFlt[4], tempVecFlt[5], tempVecFlt[6], 1.0F}, {0.0F, 0.0F, 0.0F} });
				break;
			}
		}
		else if (line.substr(0, line.find_first_of(" ")) == "vt") // Texture UV (u, v); not used for anything
		{
			tempVecFlt = parseStringForTemplate<float>(line); // gets values

			vtVec.push_back(glm::vec2(tempVecFlt[0], tempVecFlt[1])); // saves values
		}
		// TODO: add vertex normals
		else if (line.substr(0, line.find_first_of(" ")) == "vn") // Vertex Normals (x, y, z); not used at this stage
		{
			tempVecFlt = parseStringForTemplate<float>(line); // gets the values from the line

			vnVec.push_back(glm::vec3(tempVecFlt[0], tempVecFlt[1], tempVecFlt[2])); // stores them
		}
		// indices
		else if (line.substr(0, line.find_first_of(" ")) == "f")
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
				textIndices.push_back(tempVecUint[i + 1]);
				normIndices.push_back(tempVecUint[i + 2]);
			}

		}
	}

	// vertices and indices
	{
		verticesTotal = vertIndices.size(); // gets the total amount of vertices, which is currenty based on the total amount of indices.
		vertices = new Vertex[verticesTotal]; // making the dynamic array of vertices

		// if (verticesTotal > VERTICES_MAX) // if it exceeds the limit, it is set at the limit; not used
			// verticesTotal = VERTICES_MAX;

		indicesTotal = vertIndices.size(); // gets the total number of indices.
		indices = new uint32_t[indicesTotal]; // creates the dynamic array

		// if (indicesTotal > INDICES_MAX) // if it exceeds the limit, it is set at the limit; not used
		// indicesTotal > INDICES_MAX;

		// puts the vertices into the dynamic vertex buffer array.
		for (int i = 0; i < vertIndices.size(); i++)
		{
			vertices[i] = vertVec[vertIndices[i] - 1];
			indices[i] = vertIndices[i]; // vector.data() caused issues with deletion, so this version is being used instead.
		}
	}

	// calculating the normals
	{
		// vertex normal Indices and vertex Indices are the same size
		// calculates how many times a given normal is used.
		for (int i = 0; i < normIndices.size(); i++)
		{
			// adding the normal to the vertex
			vertices[i].Normal = vnVec.at(normIndices[i] - 1);
		}
	}

	// calculating the UVs
	{
		// vertex normal Indices and vertex Indices are the same size
		// calculates how many times a given normal is used.
		for (int i = 0; i < textIndices.size(); i++)
		{
			// adding the uvs to the designated vertices
			vertices[i].UV = vtVec.at(textIndices.at(i) - 1);
		}
	}

	CalculateMeshBody(); // calculates the limits of the mesh body.

	// creates the mesh
	// unlike with the default primitives, the amount of vertices corresponds to how many Indices there are, and the values are set accordingly.
	
	// if the object is dynamic, a different set of vertices are used.
	if (dynamicObject)
		mesh = std::make_shared<Mesh>(Mesh::ConvertToMorphVertexArray(vertices, verticesTotal), verticesTotal, nullptr, 0); // deformation
	else
		mesh = std::make_shared<Mesh>(vertices, verticesTotal, nullptr, 0); // no deformation
	
	// the object loader has a material associated with it, and said material should be loaded
	// if the .obj file had a material associated with it.
	if (mtllib != "" && loadMtl)
	{
		// adds the file path to the material
		std::string fpStr = (filePath.find("/") != std::string::npos) ? filePath.substr(0, filePath.find_last_of("/") + 1) : "";
		mtllib = fpStr + mtllib;

		// generates the material. The version generated depends on whether the object uses morph targets or not.
		material = (dynamicObject) ?
			Material::GenerateMtl(mtllib, nullptr, DYNAMIC_VS, DYNAMIC_FS) :
			Material::GenerateMtl(mtllib, nullptr, STATIC_VS, STATIC_FS);
	}

	return (safe = true); // returns whether the object was safely loaded.
}

// parses a string to get all the values from it as data type (T).
template<typename T>
const std::vector<T> cherry::Object::parseStringForTemplate(std::string str, bool containsSymbol)
{
	// if the string is of length 0, then an empty vector is returned.
	if (str.length() == 0)
		return std::vector<T>();

	if (containsSymbol) // checks if the symbol is still in the string. If so, it is removed.
	{
		str.erase(0, str.find_first_of(" ")); // erases the start of the string, which contains the symbol.
	}

	// returns the string put into a vector
	return util::splitString<T>(str);
}

// creates an entity with the provided m_Scene.
void cherry::Object::CreateEntity(std::string scene, cherry::Material::Sptr material)
{
	this->scene = scene; // saves the scene
	this->material = material; // saves the material.
	
	SetAlpha(alpha); // sets the alpha for the entity, which is by default 1.0 (full opacity).

	// sets up the Update function for the entity. This gets automatically called.
	auto& ecs = GetRegistry(scene);

	// the entity to be set.
	entt::entity entity = ecs.create();


	MeshRenderer& mr = ecs.assign<MeshRenderer>(entity);
	mr.Material = this->material;
	
	mr.Mesh = mesh;

	auto tform = [&](entt::entity e, float dt) 
	{
		auto& transform = CurrentRegistry().get_or_assign<TempTransform>(e);

		transform.Position = glm::vec3(position.v.x, position.v.y, position.v.z); // updates the position
		transform.EulerRotation = glm::vec3(rotation.v.x, rotation.v.y, rotation.v.z); // updates the rotation
		transform.Scale = glm::vec3(scale.v.x, scale.v.y, scale.v.z); // sets the scale
		
		worldTransform = transform.GetWorldTransform();
		// does the same thing, except all in one line (for rotation)
		// CurrentRegistry().get_or_assign<TempTransform>(e).EulerRotation += glm::vec3(0, 0, 90 * dt);
	};

	auto& up = ecs.get_or_assign<UpdateBehaviour>(entity);
	up.Function = tform;
}

// gets the transformation into world space.
glm::mat4 cherry::Object::GetWorldTransformation() const { return worldTransform; }

// gets the entity of the object
// entt::entity& cherry::Object::getEntity() { return entity; }


cherry::Vec3 cherry::Object::GetPosition() const { return position; }

// gets the object's position as a glm vector
glm::vec3 cherry::Object::GetPositionGLM() const { return glm::vec3(position.v.x, position.v.y, position.v.z); }

// sets the position
void cherry::Object::SetPosition(float x, float y, float z) { SetPosition(glm::vec3(x, y, z)); }

// setting a new position
void cherry::Object::SetPosition(cherry::Vec3 newPos) { position = newPos; }

// sets the position
void cherry::Object::SetPosition(glm::vec3 newPos) { position = cherry::Vec3(newPos); }

// gets the x-position
float cherry::Object::GetPositionX() const { return position.v.x; }

// sets the x-position
void cherry::Object::SetPositionX(float x) { position.v.x = x; }

// gets the y-position
float cherry::Object::GetPositionY() const { return position.v.y; }

// sets the y-position
void cherry::Object::SetPositionY(float y) { position.v.y = y; }

// gets the z-position
float cherry::Object::GetPositionZ() const { return position.v.z; }

// sets the z-position
void cherry::Object::SetPositionZ(float z) { position.v.z = z; }

// sets the position by the screen portion.
void cherry::Object::SetPositionByWindowSize(const cherry::Vec2 windowPos, const cherry::Vec2 camOrigin)
{
	// gets the size of the window.
	glm::vec2 windowSize = Game::GetRunningGame()->GetWindowSize();

	position.v.x = (windowSize.x * windowPos.v.x) - windowSize.x * camOrigin.v.x;
	position.v.y = (windowSize.y * windowPos.v.y) - windowSize.y * camOrigin.v.y;
}

// sets the window size
void cherry::Object::SetPositionByWindowSize(const float x, const float y, const cherry::Vec2 camOrigin)
{
	SetPositionByWindowSize(Vec2(x, y), camOrigin);
}



// ROTATION FUNCTIONS
// gets the rotation in the requested form as a GLM vector.
glm::vec3 cherry::Object::GetRotationGLM(bool inDegrees) const { return inDegrees ? GetRotationDegreesGLM() : GetRotationRadiansGLM(); }

// gets the rotation in the requested form.
cherry::Vec3 cherry::Object::GetRotation(bool inDegrees) const { return inDegrees ? GetRotationDegrees() : GetRotationRadians(); }

// sets the rotation using a glm vector
void cherry::Object::SetRotation(glm::vec3 theta, bool inDegrees) { SetRotation(cherry::Vec3(theta), inDegrees); }

// sets the rotation in the requested form.
void cherry::Object::SetRotation(cherry::Vec3 theta, bool inDegrees) { inDegrees ? SetRotationDegrees(theta) : SetRotationRadians(theta); }


// gets the rotation in degrees as a GLM vector.
glm::vec3 cherry::Object::GetRotationDegreesGLM() const 
{ 
	cherry::Vec3 rot = GetRotationDegrees();
	return glm::vec3(rot.v.x, rot.v.y, rot.v.z); 
}

// returns rotation in degrees, which is the storage default.
cherry::Vec3 cherry::Object::GetRotationDegrees() const { return rotation; }

// sets the rotation in degrees
void cherry::Object::SetRotationDegrees(glm::vec3 theta) { SetRotationDegrees(cherry::Vec3(theta)); }

// sets the rotation in degrees
void cherry::Object::SetRotationDegrees(cherry::Vec3 theta) { rotation = theta; }

// gets the rotation in radians as a GLM vector
glm::vec3 cherry::Object::GetRotationRadiansGLM() const
{
	cherry::Vec3 rot = GetRotationRadians();
	return glm::vec3(rot.v.x, rot.v.y, rot.v.z);
}

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

// sets the rotation in radians
void cherry::Object::SetRotationRadians(glm::vec3 theta) { SetRotationRadians(cherry::Vec3(theta)); }

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
float cherry::Object::GetRotationXDegrees() const { return rotation.v.x; }

// sets the rotation on the x-axis in degrees.
void cherry::Object::SetRotationXDegrees(float degrees) { rotation.v.x = degrees; }

// gets the rotation on the x-axis in radians
float cherry::Object::GetRotationXRadians() const { return util::math::degreesToRadians(rotation.v.x); }

// sets the rotation on the x-axis in radians.
void cherry::Object::SetRotationXRadians(float radians) { rotation.v.x = util::math::radiansToDegrees(radians); }


// gets the rotation on the y-axis in degrees
float cherry::Object::GetRotationYDegrees() const { return rotation.v.y; }

// sets the rotation on the y-axis in degrees.
void cherry::Object::SetRotationYDegrees(float degrees) { rotation.v.y = degrees; }

// gets the rotation on the y-axis in radians
float cherry::Object::GetRotationYRadians() const { return util::math::degreesToRadians(rotation.v.y); }

// sets the rotation on the y-axis in radians.
void cherry::Object::SetRotationYRadians(float radians) { rotation.v.y = util::math::radiansToDegrees(radians); }


// gets the rotation on the z-axis in degrees
float cherry::Object::GetRotationZDegrees() const { return rotation.v.z; }

// sets the rotation on the z-axis in degrees.
void cherry::Object::SetRotationZDegrees(float degrees) { rotation.v.z = degrees; }

// gets the rotation on the z-axis in radians
float cherry::Object::GetRotationZRadians() const { return util::math::degreesToRadians(rotation.v.z); }

// sets the rotation on the z-axis in radians.
void cherry::Object::SetRotationZRadians(float radians) { rotation.v.y = util::math::radiansToDegrees(radians); }



// gets the scale as a glm vector
glm::vec3 cherry::Object::GetScaleGLM() const { return glm::vec3(scale.v.x, scale.v.y, scale.v.z); }

// gets the scale of the object
cherry::Vec3 cherry::Object::GetScale() const { return scale; }

// sets the scale
void cherry::Object::SetScale(float scl) { scale = Vec3(scl, scl, scl); }

// sets scale
void cherry::Object::SetScale(float scaleX, float scaleY, float scaleZ) { scale = { scaleX, scaleY, scaleZ }; }

// sets the scale
void cherry::Object::SetScale(glm::vec3 newScale) { scale = newScale; }

// sets the scale
void cherry::Object::SetScale(cherry::Vec3 newScale) { scale = newScale; }

// gets the scale on the x-axis
float cherry::Object::GetScaleX() const { return scale.v.x; }

// sets the scale on the x-axis
void cherry::Object::SetScaleX(float scaleX) { scale.v.x = scaleX; }

// gets the y-axis scale
float cherry::Object::GetScaleY() const { return scale.v.y; }

// sets the scale on the y-axis
void cherry::Object::SetScaleY(float scaleY) { scale.v.y = scaleY; }

// gets the z-axis scale
float cherry::Object::GetScaleZ() const { return scale.v.z; }

// sets the z-axis scale
void cherry::Object::SetScaleZ(float scaleZ) { scale.v.z = scaleZ; }



// translates the object
void cherry::Object::Translate(cherry::Vec3 translation) { position += translation; }

// translates the object
void cherry::Object::Translate(float x, float y, float z) { Translate(Vec3(x, y, z)); }

// rotates in the order of x-y-z
void cherry::Object::Rotate(cherry::Vec3 theta, bool inDegrees) 
{ 
	if (inDegrees) // in degrees
	{
		rotation += theta; // rotation is stored in radians.
	}
	else // in radians
	{
		rotation.v.x += util::math::radiansToDegrees(theta.v.x);
		rotation.v.y += util::math::radiansToDegrees(theta.v.y);
		rotation.v.z += util::math::radiansToDegrees(theta.v.z);
	}	
}

// rotates in the order of x-y-z
void cherry::Object::Rotate(float x, float y, float z, bool inDegrees) { Rotate(cherry::Vec3(x, y, z), inDegrees); }

// rotate x-axis
void cherry::Object::RotateX(float x, bool inDegrees) { Rotate(x, 0, 0, inDegrees); }

// rotate y-axis
void cherry::Object::RotateY(float y, bool inDegrees) { Rotate(0, y, 0, inDegrees); }

// rotate z-axis
void cherry::Object::RotateZ(float z, bool inDegrees) { Rotate(0, 0, z, inDegrees); }

// pushes forward in the direction of the x-axis roation.
void cherry::Object::ForwardX(float scalar, bool fromY)
{
	cherry::Vec3 temp{};

	// from the y-axis
	if (fromY)
		temp = util::math::rotateX(util::math::Vec3(0, scalar, 0), GetRotationDegrees().v.x, true);
	// from the z-axis
	else
		temp = util::math::rotateX(util::math::Vec3(0, 0, scalar), GetRotationDegrees().v.x, true);

	Translate(temp);
}

// pushes forward in the direction of the y-axis roation.
void cherry::Object::ForwardY(float scalar, bool fromX)
{
	cherry::Vec3 temp{};

	// from the x-axis
	if (fromX)
		temp = util::math::rotateY(util::math::Vec3(scalar, 0, 0), GetRotationDegrees().v.y, true);
	// from the z-axis
	else
		temp = util::math::rotateY(util::math::Vec3(0, 0, scalar), GetRotationDegrees().v.y, true);

	Translate(temp);
}

// pushes forward in the direction of the z-axis roation.
void cherry::Object::ForwardZ(float scalar, bool fromX)
{
	cherry::Vec3 temp{};

	// from the x-axis
	if(fromX)
		util::math::rotateZ(util::math::Vec3(scalar, 0, 0), GetRotationDegrees().v.z, true);
	// from the y-axis
	else
		util::math::rotateZ(util::math::Vec3(0, scalar, 0), GetRotationDegrees().v.z, true);

	Translate(temp);
}




// gets the parent of the object
// const cherry::Object* cherry::Object::GetParent() const { return parent; }

// sets the parent for an object.
// void cherry::Object::SetParent(const cherry::Object* newParent) 
// { 
// 	// // removing from the old parent.
// 	// if (parent != nullptr)
// 	// 	parent->RemoveChild(this);
// 	// 
// 	// // adding the new parent.
// 	// if (newParent != nullptr)
// 	// 	newParent->AddChild(this);
// 
// 	parent = newParent; 
// }

// removes the parent
// void cherry::Object::RemoveParent() { parent = nullptr; }

// adds a child.
// bool cherry::Object::AddChild(cherry::Object* child)
// {
// 	return util::addToVector(children, child);
// }



// returns true if added successfully.
bool cherry::Object::AddPhysicsBody(cherry::PhysicsBody* body) 
{ 
	if (body == nullptr)
		return false;

	return util::addToVector(bodies, body->AttachToObject(this));
	// body->AttachToObject(this);
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



// ANIMATION
// object is dynamic.
bool cherry::Object::IsDynamicObject() const { return dynamicObject; }

// object is static
bool cherry::Object::IsStaticObject() const { return !dynamicObject; }

// returns the animation manager for the object
cherry::AnimationManager& cherry::Object::GetAnimationManager() { return animations; }

// adds an animation
bool cherry::Object::AddAnimation(Animation * anime, bool current)
{
	if (anime == nullptr)
		return false;

	// sets the object.
	if (anime->GetObject() != this)
		anime->SetObject(this);

	// TODO: set up MorphVertex so that it can use vertex shaders as well.
	// Use this for replacing shaders for given animations
	// if using morph targets
	if (anime->GetId() == 1 && dynamicObject == true)
	{
		// checking for proper shaders
		std::string dvs = DYNAMIC_VS;
		std::string dfs = DYNAMIC_FS;
		if (std::string(material->GetShader()->GetVertexShader()) != dvs || std::string(material->GetShader()->GetFragmentShader()) != dfs)
		{
			// TODO: runtime error?
			// ERROR: cannot run with set shaders
			return false;
		}
	}
	// deformation is not available, so animation addition has failed.
	else if (anime->GetId() == 1 && !dynamicObject == true)
	{
		// #ifndef _DEBUG
		// 
		// #endif // !_DEBUG

		// std::runtime_error("Error. Static object cannot utilize deformation animation.");
		return false;
	}

	animations.AddAnimation(anime, current);
	return true;
}

// gets an animation
cherry::Animation * cherry::Object::GetAnimation(unsigned int index) { return animations.GetAnimation(index); }

// gets the current animation
cherry::Animation * cherry::Object::GetCurrentAnimation() { return animations.GetCurrentAnimation(); }

// sets the current animation
void cherry::Object::SetCurrentAnimation(unsigned int index) { animations.SetCurrentAnimation(index); }


// gets the path
cherry::Path cherry::Object::GetPath() const { return path; }

// sets the path the object follows.
void cherry::Object::SetPath(cherry::Path newPath) { path = newPath; }

// attaching a path.
void cherry::Object::SetPath(cherry::Path newPath, bool attachPath)
{
	SetPath(newPath);

	followPath = attachPath;
}

// removes the path from the object. It still exists in memory.
void cherry::Object::ClearPath() { path = Path(); }

// determines whether the object should use the path.
void cherry::Object::UsePath(bool follow) { followPath = follow; }

// gets the object as a target. The target is updated each frame to match up with the current position.
const std::shared_ptr<cherry::Target>& cherry::Object::GetObjectAsTarget() const { return leaderTarget; }

// gets the mesh body maximum.
const cherry::Vec3 & cherry::Object::GetMeshBodyMaximum() const { return meshBodyMax; }

// gets the maximum mesh body values.
cherry::Vec3 cherry::Object::CalculateMeshBodyMaximum(const Vertex* vertices, const unsigned int VERTEX_COUNT)
{
	if (vertices == nullptr || VERTEX_COUNT == 0) // no vertices were passed.
		return Vec3();

	cherry::Vec3 maxVerts{}; // maximum verts

	// getting the maximum mesh body values.
	for (int i = 0; i < VERTEX_COUNT; i++)
	{
		if (maxVerts.v.x < vertices[i].Position.x) // x-position
			maxVerts.v.x = vertices[i].Position.x;
		
		if (maxVerts.v.y < vertices[i].Position.y) // y-position
			maxVerts.v.y = vertices[i].Position.y;

		if (maxVerts.v.z < vertices[i].Position.z) // z-position
			maxVerts.v.z = vertices[i].Position.z;
	}

	return maxVerts; // maximum vertex values
}

// gets the mesh body minimum.
const cherry::Vec3& cherry::Object::GetMeshBodyMinimum() const { return meshBodyMin; }

// gets the mesh body minimum values
cherry::Vec3 cherry::Object::CalculateMeshBodyMinimum(const Vertex* vertices, const unsigned int VERTEX_COUNT)
{
	if (vertices == nullptr || VERTEX_COUNT == 0) // no vertices were passed.
		return Vec3();

	cherry::Vec3 minVerts{}; // maximum verts

	// getting the maximum mesh body values.
	for (int i = 0; i < VERTEX_COUNT; i++)
	{
		if (minVerts.v.x > vertices[i].Position.x) // x-position
			minVerts.v.x = vertices[i].Position.x;

		if (minVerts.v.y > vertices[i].Position.y) // y-position
			minVerts.v.y = vertices[i].Position.y;

		if (minVerts.v.z > vertices[i].Position.z) // z-position
			minVerts.v.z = vertices[i].Position.z;
	}

	return minVerts; // minimum vertex values
}

// calculates the mesh body, which is the limits of the mesh.
void cherry::Object::CalculateMeshBody()
{
	meshBodyMax = CalculateMeshBodyMaximum(vertices, verticesTotal); // maximum values
	meshBodyMin = CalculateMeshBodyMinimum(vertices, verticesTotal); // minimum values
}

// updates the object
void cherry::Object::Update(float deltaTime)
{
	// TODO: remove this for the final version.
	// rotation.SetX(rotation.GetX() + 15.0F * deltaTime);
	// rotation.SetZ(rotation.GetZ() + 90.0F * deltaTime);

	// runs the path and sets the new position
	if (followPath)
		position = path.Run(deltaTime);

	// if the animation is playing
	if (animations.GetCurrentAnimation() != nullptr)
	{
		if(animations.GetCurrentAnimation()->isPlaying())
			animations.GetCurrentAnimation()->Update(deltaTime);
	}	

	// if the object is meant to follow a target.
	if (followTarget)
		position = target->GetPosition() + targetOffset;

	// updating the physics bodies
	for (cherry::PhysicsBody* body : bodies)
		body->Update(deltaTime);

	// updating the leader target.
	leaderTarget->SetPosition(position);
	// SetRotationDegrees(GetRotationDegrees() + Vec3(30.0F, 10.0F, 5.0F) * deltaTime);
}

// returns a string representing the object
std::string cherry::Object::ToString() const
{
	return "Name: " + name + " | Description: " + description + " | Position: " + position.ToString();
}