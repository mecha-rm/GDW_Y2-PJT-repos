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
#include "..\animate\AnimationManager.h"
#include "..\animate\Path.h"
// #include "..\Physics.h"

namespace cherry
{
	class PhysicsBody;

	class Object
	{
	public:
		// the name and directory of the .obj file
		// if 'loadMtl' is set to true, then the object loads the texture, which is assumed to be in the section as the .obj file.
		// 'dynamicObj' determines if the object is static or dynamic. If it's dynamic, then the object has deformation animation.
		// remember to call CreateEntity() to add the entity to a scene
		Object(std::string filePath, bool loadMtl = false, bool dynamicObj = false);

		// loads an ob file using the filePath provided, and puts it in the provided scene.
		// Use the bool to load the mtl file as well, which is assumed to be of the same name and be in the same location.
		// this automatically calls CreateEntity() to add the entity into the scene.
		Object(std::string filePath, std::string scene, bool loadMtl = false, bool dynamicObj = false);

		// loads in the object with the designated scene, and an mtl file.
		// this automatically calls CreateEntity() to add the entity into the scene.
		Object(std::string filePath, std::string scene, std::string mtl, bool dynamicObj = false);

		// loads an obj file, and places it in the scene with the provided material
		// it can also load in an .mtl file after loading in the material, which is applied to be in the same location as the .obj file.
		// this automatically calls CreateEntity().
		Object(std::string filePath, std::string scene, Material::Sptr material, bool loadMtl = false, bool dynamicObj = false);

		// loads in an obj file, adds to the scene, applies the material, then loads in the mtl file.
		// adding the material and loading in the mtl file afterwards allows it to keep values that aren't changed by the mtl file.
		Object(std::string filePath, std::string scene, Material::Sptr material, std::string mtl, bool dynamicObj = false);



		// destructor
		virtual ~Object();

		// gets the file path of the requested object.
		// if there is no file path, it will return a string with the ("") character inside it.
		std::string GetFilePath() const;

		// gets the scene the object is part of.
		std::string GetScene() const;

		// gets the name of the object.
		std::string GetName() const;

		// sets the name of the object.
		void SetName(std::string newName);

		// gets the desc of the object.
		std::string GetDescription() const;

		// sets the desc of the object.
		void SetDescription(std::string newDesc);

		// returns 'true' if the file is safe to use, and 'false' if it isn't. If it's false, then something is wrong with the file.
		bool GetSafe() const;

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

		// gets the material for the object.
		Material::Sptr& GetMaterial();

		// returns whether or not the object is visible
		bool IsVisible() const;

		// toggle's visibility on/off
		void SetVisible();

		// sets whether the object is visible or not.
		void SetVisible(bool visible);

		// creates the entity with the provided m_Scene and material.
		void CreateEntity(std::string scene, cherry::Material::Sptr material);

		// gets the transformation from local space to world space.
		glm::mat4 GetWorldTransformation() const;

		
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



		// gets the rotation as a GLM vector
		glm::vec3 GetRotationGLM(bool inDegrees) const;

		// returns the rotation of the object. The boolean determines if it's returned in degrees or radians
		cherry::Vec3 GetRotation(bool inDegrees) const;

		// sets the rotation for the object. Bool'InDegrees' determines if the provided values are in degrees or radians.
		void SetRotation(glm::vec3 theta, bool inDegrees);

		// sets the rotation for the object. Bool'InDegrees' determines if the provided values are in degrees or radians.
		void SetRotation(cherry::Vec3 theta, bool inDegrees);

		// gets the rotation in degrees as a GLM vector.
		glm::vec3 GetRotationDegreesGLM() const;

		// gets the rotation in degrees, which is the storage default.
		cherry::Vec3 GetRotationDegrees() const;

		// sets the rotation in degrees
		void SetRotationDegrees(glm::vec3 theta);

		// sets the rotation in degrees
		void SetRotationDegrees(cherry::Vec3 theta);

		
		// gets the rotation in degrees as a GLM vector.
		glm::vec3 GetRotationRadiansGLM() const;

		// gets the rotation in radians
		cherry::Vec3 GetRotationRadians() const;

		// sets the rotation in radians
		void SetRotationRadians(glm::vec3 theta);

		// sets the rotation in degrees
		void SetRotationRadians(cherry::Vec3 theta);


		// gets the x-axis rotation in degrees
		float GetRotationXDegrees() const;

		// sets the x-axis rotation in degrees
		void SetRotationXDegrees(float degrees);

		// gets the x-axis rotation in radians
		float GetRotationXRadians() const;

		// gets the x-axis rotation in radians
		void SetRotationXRadians(float radians);


		// gets the y-axis rotation in degrees
		float GetRotationYDegrees() const;

		// sets the y-axis rotation in degrees
		void SetRotationYDegrees(float degrees);

		// gets the y-axis rotation in radians
		float GetRotationYRadians() const;

		// gets the y-axis rotation in radians
		void SetRotationYRadians(float radians);


		// gets the z-axis rotation in degrees
		float GetRotationZDegrees() const;

		// sets the z-axis rotation in degrees
		void SetRotationZDegrees(float degrees);

		// gets the z-axis rotation in radians
		float GetRotationZRadians() const;

		// gets the z-axis rotation in radians
		void SetRotationZRadians(float radians);


		// gets the scale of the object as a GLM vector
		glm::vec3 GetScaleGLM() const;

		// gets the scale of the object
		cherry::Vec3 GetScale() const;

		// sets the scale on all axes with a single value
		void SetScale(float scl);
		
		// sets the scale on all axes with a single value
		void SetScale(float scaleX, float scaleY, float scaleZ);

		// sets the scale with an individual scale per axis
		void SetScale(glm::vec3 newScale);

		// sets the scale with an individual scale per axis
		void SetScale(cherry::Vec3 newScale);		


		// gets the scale on the x-axis
		float GetScaleX() const;

		// sets the scale on the x-axis
		void SetScaleX(float scaleX);

		// gets the scale on the y-axis
		float GetScaleY() const;

		// sets the scale on the y-axis
		void SetScaleY(float scaleY);

		// gets the scale on the z-axis
		float GetScaleZ() const;

		// sets the scale on the z-axis
		void SetScaleZ(float scaleZ);


		// translates the object
		void Translate(Vec3 translation);

		// translates the object by the provided values.
		void Translate(float x, float y, float z);


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


		// ANIMATION //

		// if the object is dynamic, then the object deforms.
		bool IsDynamicObject() const;

		// if the object is static, it doesn't deform.
		bool IsStaticObject() const;
		
		// gets the animation manager for the object
		cherry::AnimationManager& GetAnimationManager();

		// adds an animation to the object. This comes the object for the animation if it isn't already.
		bool AddAnimation(Animation * anime, bool current = false);

		// gets an animation based on a given index.
		// if there is no animation at this index, an empty object is returned.
		cherry::Animation * GetAnimation(unsigned int index);

		// gets the current animation
		cherry::Animation * GetCurrentAnimation();

		// sets the current animation
		void SetCurrentAnimation(unsigned int index);

		/// PATH ///
		// gets the path that the object is locked to.
		cherry::Path GetPath() const;

		// sets the path the object follows. Set to 'nullptr' if you want to remove the path reference from the object.
		// To delete the path from memory, use DeletePath().
		void SetPath(cherry::Path newPath);

		// sets the path for the object. If 'attachPath' is true, then the object starts moving via this path.
		void SetPath(cherry::Path newPath, bool attachPath);

		// clears all nodes from the path.
		void ClearPath();

		// if 'true' is passed, the object follows the path, if it exists.
		void UsePath(bool follow);

		// TODO: remove this?
		// default physics body size
		cherry::Vec3 GetPBodySize();

		// default physics body size
		float GetPBodyWidth();

		// default physics body size
		float GetPBodyHeight();

		// default physics body size
		float GetPBodyDepth();


		// updates the object
		void Update(float deltaTime);

		// toString
		virtual std::string ToString() const;

		// the maximum amount of vertices one object can have. This doesn't get used.
		const static unsigned int VERTICES_MAX;

		// the maximum amount of indices one object can have. This doesn't get used.
		const static unsigned int INDICES_MAX;

		// the path the object follows
		// std::shared_ptr <Path> path = nullptr;
		Path path = Path();

		// following the path
		bool followPath = false;

	private:
		// void setMesh(Mesh::sptr);

		// called to load the object
		bool LoadObject(bool loadMtl = false);

		// parses the line, gets the values as data type T, and stores them in a vector.
		// containsSymbol: tells the function if the string passed still contains the symbol at the start. If so, it is removed before the parsing begins.
		// *** the symbol at the start is what's used to determine what the values in a given line of a .obj are for.
		template<typename T>
		const std::vector<T> parseStringForTemplate(std::string str, bool containsSymbol = true);

		// template<typename T>
		// void CalculateNormals(std::vector<);

		std::string scene = "";

		// saves whether an object is static or dynamic. If it's dynamic, that means there's mesh deformation.
		bool dynamicObject = false;

		// parent object
		// Object * parent = nullptr;

		// std::vector<Object*> children;

		// the string for the file path
		std::string filePath = "";

		// transformation into world coordinate space.
		glm::mat4 worldTransform;

		// a vector of physics bodies
		std::vector<cherry::PhysicsBody*> bodies;

		// becomes 'true' when an object intersects something.
		bool intersection = false;

		// used for object transformations
		// entt::registry ecs;
		// the entity for the object
		// entt::entity entity;

		// saves the rotation on the x, y, and z axis in DEGREES.
		cherry::Vec3 rotation = { 0.0F, 0.0F, 0.0F };

		// default body size
		cherry::Vec3 pBodySize;

	protected:
		// constructor used for default primitives
		Object();

		// object name
		std::string name = "";

		// object description
		std::string description = "";

		// true if the file is safe to read from, false otherwise.
		bool safe = false;

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

		// the position of the object.
		cherry::Vec3 position = { 0.0F, 0.0F, 0.0F };

		// the scale of the object
		cherry::Vec3 scale = { 1.0F, 1.0F, 1.0F };

		// the animation
		// TODO: repalce with an animation manager
		cherry::AnimationManager animations = cherry::AnimationManager();
		// cherry::Animation animate = Animation();
	};
}

