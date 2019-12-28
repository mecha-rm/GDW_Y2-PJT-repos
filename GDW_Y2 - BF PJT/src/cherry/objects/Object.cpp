// OBJECT CLASS (SOURCE)
#include "Object.h"
#include "..\utils\Utils.h"
#include "..\utils\math\Rotation.h"
#include "..\PhysicsBody.h"
#include "..\WorldTransform.h"

#include "..\SceneManager.h"
#include "..\MeshRenderer.h"

#include <iostream>
#include <sstream>
#include <glm/gtc/quaternion.hpp> 
#include <glm/gtx/quaternion.hpp>
#include <GLM/gtc/matrix_transform.hpp>


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
		throw std::runtime_error("Error opening file. Functions for this object should not be used.");
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
	Object(filePath, loadMtl)
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

// the protected constructor used for default primitives
cherry::Object::Object() : position(), vertices(nullptr), indices(nullptr) { filePath = ""; }

cherry::Object::~Object()
{
	// TODO: add back deletions
	delete[] vertices;

	// if not initialized, it causes an error if deleted.
	// since only the primitives use indicies, those call delete on their own.
	// if(indices != nullptr) // TODO: fix this
	// delete[] indices; 

	// delete bodies;
}


// gets the file path for the object file.
std::string cherry::Object::GetFilePath() const { return filePath; }

// returns the scene the object is part of.
std::string cherry::Object::GetScene() const { return scene; }

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

//// gets the color of the first vertex
//glm::vec4 cherry::Object::GetColor() const { return vertices->Color; }
//
//// sets colour based on range of 0 to 255. Alpha (a) still goes from 0 to 1.
//void cherry::Object::SetColor(int r, int g, int b, float a) { SetColor((float)r / 255.0F, (float)g / 255.0F, (float)b / 255.0F, a); }
//
//// sets the color for all vertices
//void cherry::Object::SetColor(float r, float g, float b, float a)
//{
//	// bounds checking for RGBA
//	r = (r < 0.0F) ? 0.0F : (r > 1.0F) ? 1.0F : r;
//	g = (g < 0.0F) ? 0.0F : (g > 1.0F) ? 1.0F : g;
//	b = (b < 0.0F) ? 0.0F : (b > 1.0F) ? 1.0F : b;
//	a = (a < 0.0F) ? 0.0F : (a > 1.0F) ? 1.0F : a;
//
//	for (int i = 0; i < verticesTotal; i++)
//		vertices[i].Color = glm::vec4(r, g, b, a);
//
//	// TODO: doing this causes the mesh to screw up for some reason.
//	bool wf = mesh->IsWireframe(); // copying over values
//	bool vis = mesh->IsVisible(); // copying over values
//
//	mesh = std::make_shared<Mesh>(vertices, verticesTotal, indices, indicesTotal); // creates the mesh
//	mesh->SetWireframe(wf);
//	mesh->SetVisible(vis);
//}
//
//// sets the color, keeping the alpha (a) value from the first vertex.
//void cherry::Object::SetColor(glm::vec3 color) { SetColor(color.x, color.y, color.z, vertices[0].Color.w); }
//
//// sets the color (RGBA [0-1])
//void cherry::Object::SetColor(glm::vec4 color) { SetColor(color.x, color.y, color.z, color.w); }

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
cherry::Material::Sptr& cherry::Object::GetMaterial() { return material; }

// returns if the object is visible
bool cherry::Object::IsVisible() const { return mesh->IsVisible(); }

// toggle's visibility on/off
void cherry::Object::SetVisible() { mesh->SetVisible(); }

// sets whether the object is visible.
void cherry::Object::SetVisible(bool visible) { mesh->SetVisible(visible); }

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
			case 4: // (x, y, z, w) (n/a) ('w' value is ignored)
				vertVec.push_back(Vertex{ {tempVecFlt[0], tempVecFlt[1], tempVecFlt[2]}, {1.0F, 1.0F, 1.0F, 1.0F}, {0.0F, 0.0F, 0.0F} });
				break;

			case 6: // (x, y, z, r, g, b)
				vertVec.push_back(Vertex{ {tempVecFlt[0], tempVecFlt[1], tempVecFlt[2]}, {tempVecFlt[3], tempVecFlt[4], tempVecFlt[5], 1.0F}, {0.0F, 0.0F, 0.0F} });
			case 7: // (x, y, z, w, r, g, b) (n/a) ('w' value is ignored)
				vertVec.push_back(Vertex{ {tempVecFlt[0], tempVecFlt[1], tempVecFlt[2]}, {tempVecFlt[4], tempVecFlt[5], tempVecFlt[6], 1.0F}, {0.0F, 0.0F, 0.0F} });
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

		// generates the material
		material = (dynamicObject) ? 
			Material::GenerateMtl(mtllib, nullptr, STATIC_VS, STATIC_FS) :
			Material::GenerateMtl(mtllib, nullptr, DYNAMIC_VS, DYNAMIC_FS);
	}

	return (safe = true); // returns whether the object was safely loaded.
}

// creates an entity with the provided m_Scene.
void cherry::Object::CreateEntity(std::string scene, cherry::Material::Sptr material)
{
	this->scene = scene; // saves the scene
	this->material = material; // saves the material.
	
							   // sets up the Update function for the entity. This gets automatically called.
	auto& ecs = GetRegistry(scene);
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

void cherry::Object::SetPosition(float x, float y, float z) { SetPosition(glm::vec3(x, y, z)); }

// sets the position
void cherry::Object::SetPosition(glm::vec3 newPos) { position = cherry::Vec3(newPos); }

// setting a new position
void cherry::Object::SetPosition(cherry::Vec3 newPos) { position = newPos; }



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
void cherry::Object::Translate(Vec3 translation) { position += translation; }

// translates the object
void cherry::Object::Translate(float x, float y, float z) { Translate(Vec3(x, y, z)); }



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

// default physics body size
cherry::Vec3 cherry::Object::GetPBodySize()
{
	return this->pBodySize;
}

// default physics body size
float cherry::Object::GetPBodyWidth()
{
	return this->GetPBodySize().GetX() / 2;
}

// default physics body size
float cherry::Object::GetPBodyHeight()
{
	return this->GetPBodySize().GetY() / 2;
}

// default physics body size
float cherry::Object::GetPBodyDepth()
{
	return this->GetPBodySize().GetZ() / 2;
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
		animations.GetCurrentAnimation()->isPlaying();
		animations.GetCurrentAnimation()->Update(deltaTime);
	}	

	// updating the physics bodies
	for (cherry::PhysicsBody* body : bodies)
		body->Update(deltaTime);

	SetRotationDegrees(GetRotationDegrees() + Vec3(30.0F, 10.0F, 5.0F) * deltaTime);
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

// destorys the object
void cherry::Object::Destroy()
{ 
	if (vertices == nullptr)
		vertices = new Vertex[0];

	// if (indices == nullptr)
		// indices = new uint32_t[0];
	delete this;
}