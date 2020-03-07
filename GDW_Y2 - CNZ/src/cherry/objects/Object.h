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
#include "..\Target.h"


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

		// copy constructor.
		Object(const cherry::Object&);

		// destructor
		virtual ~Object();

		// gets the file path of the requested object.
		// if there is no file path, it will return a string with the ("") character inside it.
		virtual const std::string & GetFilePath() const;

		// gets the scene the object is part of.
		std::string GetSceneName() const;

		// sets a new scene for the object to be part of. This removes the object from the list its currently part of.
		// if the scene doesn't exist, then a new one is created.
		void SetScene(std::string newScene);

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
		const Material::Sptr& GetMaterial() const;

		// gets the alpha value of the object.
		virtual float GetAlpha() const;

		// sets the alpha value of the mesh. It goes from the range [0, 1].
		// if the alpha value is set to 1.0, transparency is turned off. 
		// Some items need transparency, so if that's the case this and the getter function should be overloaded.
		virtual void SetAlpha(float a);

		// returns whether or not the object is visible
		bool IsVisible() const;

		// toggle's visibility on/off
		void SetVisible();

		// sets whether the object is visible or not.
		void SetVisible(bool visible);

		// TODO: save in the Mesh class whether to draw in perspective or othographic.
		// returns 'true' if the object is drawn in perspective mode.
		// by default, the object is drawn in perspective mode.
		bool IsPerspectiveObject() const;

		// sets if the object should be drawn in perspective mode.
		void SetPerspectiveObject(bool perspective);

		// returns 'true' if the object is drawn in orthographic mode.
		bool IsOrthographicObject() const;

		// sets if the object should be drawn in orthographic mode.
		void SetOrthographicObject(bool orthographic);

		// if 'true', the screen position of the mesh is fixed regardless of the camera position or orientation.
		// the mesh itself can still be moved, but it will be uneffected by the movement in the camera.
		bool IsWindowChild() const;

		// if 'true', the object will be a child of the screen, keeping the same porportional size and position to the screen.
		void SetWindowChild(bool windowChild);

		// returns 'true' if this object is post processed.
		bool GetPostProcess() const;

		// sets whether this object is post processed or not.
		// if the mesh hasn't been made yet, nothing happens.
		void SetPostProcess(bool postProcess);

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

		// gets the x-position
		float GetPositionX() const;

		// sets the x-position.
		void SetPositionX(float x);

		// gets the y-position
		float GetPositionY() const;

		// sets the y-position.
		void SetPositionY(float y);

		// gets the z-position
		float GetPositionZ() const;

		// sets the z-position.
		void SetPositionZ(float z);

		// TODO: fix the position so that ti goes in the right direction
		/*
		 * sets the object's position via the window size. This is meant for the overlay objects.
		 * Variables:
		 *	screenPos: the new position. It is treated as a percentage of the screen size, juding it as a [0, 1] range.
		 *	camOrigin: the origin of the camera in reference to the screen.
			* the overlay camera's origin is the centre of the screen. However, screen space treats the bottom-left corner as the origin.
			* the camOrigin is used to offset the providied posiition so that it is in the expected screen space location.
			* the camOrigin can be left as it's default since the overlay camera won't be moved.
		*/
		void SetPositionByWindowSize(const cherry::Vec2 windowPos, const cherry::Vec2 camOrigin = Vec2(0.5F, 0.5F));

		// sets the position using the size of the window
		void SetPositionByWindowSize(const float x, const float y, const cherry::Vec2 camOrigin = Vec2(0.5F, 0.5F));

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




		// Transformation Functions
		// translates the object
		void Translate(cherry::Vec3 translation);

		// translates the object by the provided values.
		void Translate(float x, float y, float z);

		// rotates in the order of x-y-z.
		void Rotate(cherry::Vec3 theta, bool inDegrees);

		// rotates in the order of x-y-z.
		void Rotate(float x, float y, float z, bool inDegrees);

		// rotate x-axis
		void RotateX(float x, bool inDegrees);

		// rotate y-axis
		void RotateY(float y, bool inDegrees);

		// rotate z-axis
		void RotateZ(float z, bool inDegrees);

		// pushes forward on the y and z axis in a direction determined by the x-rotation.
		// 'fromY'determines whether the rotation starts from y = 0 or z = 0.
		//	* true: (y, 0) is considered a rotation of 0. 
		//	* false: (0, z) is considered a rotation of 0.
		void ForwardX(float scalar, bool fromY);

		// pushes forward on the x and z axis in a direction determined by the y-rotation.
		// 'fromX'determines whether the rotation starts from x = 0 or z = 0.
		//	* true: (x, 0) is considered a rotation of 0. 
		//	* false: (0, z) is considered a rotation of 0.
		void ForwardY(float scalar, bool fromX);

		// pushes forward on the x and y axis in a direction determined by the z-rotation.
		// 'fromX'determines whether the rotation starts from y = 0 or x = 0.
		//	* true: (x, 0) is considered a rotation of 0. 
		//	* false: (0, y) is considered a rotation of 0.
		void ForwardZ(float scalar, bool fromX);



		// gets the parent object.
		// const cherry::Object* GetParent() const;

		// sets the object for the parent.
		// void SetParent(const cherry::Object * newParent);

		// removes the parent by making it a nullptr.
		// void RemoveParent();

		// adds a child to the object.
		// bool AddChild(cherry::Object* child);

		// removes a child from the object.
		// bool RemoveChid(cherry::Object * child);

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

		// TODO: add function for following a target.

		// if 'true' is passed, the object follows the path, if it exists.
		void UsePath(bool follow);

		// gets the object as a target. This target gets updated each frame to match up with the current position.
		const std::shared_ptr<cherry::Target>& GetObjectAsTarget() const;

		// returns 'true' if the object is following a target.
		// void FollowTarget(bool tgt);

		// get mesh body maximum.
		const cherry::Vec3 & GetMeshBodyMaximum() const;

		// calculates the upper limits of the mesh body
		static cherry::Vec3 CalculateMeshBodyMaximum(const Vertex * vertices, const unsigned int VERTEX_COUNT);

		// get the mesh body minimum.
		const cherry::Vec3 & GetMeshBodyMinimum() const;

		// calculates the lower limits of the mesh body
		static cherry::Vec3 CalculateMeshBodyMinimum(const Vertex* vertices, const unsigned int VERTEX_COUNT);

		// updates the object
		virtual void Update(float deltaTime);

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

		// a target to be followed by the object.
		std::shared_ptr<cherry::Target> target = std::make_shared<cherry::Target>();

		// the offset from the target's position.
		cherry::Vec3 targetOffset{};

		// if 'true', the target is followed.
		bool followTarget = false;

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


		// parent objects
		// TODO: make an object be capable of having multiple parents
		// const cherry::Object* parent = nullptr;

		// child objects
		// std::vector<Object*> children;

		// the string for the file path
		std::string filePath = "";

		// transformation into world coordinate space.
		glm::mat4 worldTransform;

		// a vector of physics bodies
		std::vector<cherry::PhysicsBody*> bodies;

		// becomes 'true' when an object intersects something.
		bool intersection = false;

		// saves the rotation on the x, y, and z axis in DEGREES.
		cherry::Vec3 rotation = { 0.0F, 0.0F, 0.0F };

		// the object as a target.
		std::shared_ptr<cherry::Target> leaderTarget = std::make_shared<cherry::Target>();

	protected:
		// constructor used for default primitives
		Object();

		// calculates the limits of the mesh (i.e. the highest and lowest vertex positions). This does not account for animations.
		// only call this once the vertices have all been found and accounted for.
		void CalculateMeshBody();

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

		// mesh body size
		cherry::Vec3 meshBodyMin; // minimum vertex positions

		cherry::Vec3 meshBodyMax; // maximum vertex positions

		// the color of the model.
		// cherry::Vec4 color;

		// the material of the object.
		Material::Sptr material;

		// the alpha value for the mesh.
		float alpha = 1.0F;

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

