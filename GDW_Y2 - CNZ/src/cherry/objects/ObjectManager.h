#pragma once

// #include "Object.h"
#include "Primitives.h"
#include "Image.h"
#include "Liquid.h"
#include "Terrain.h"

namespace cherry
{
	class ObjectList;

	class ObjectManager
	{
	public:
		ObjectManager() = default;
		
		// destructor
		~ObjectManager();

		// checks if a list for a scene exists. If so, a 'true' is returned.
		static bool SceneObjectListExists(std::string sceneName);

		// gets the object list for the scene by index. A nullptr is returned if the scene doesn't have a list.
		static cherry::ObjectList* GetSceneObjectListByIndex(unsigned int index);

		// gets the object list for the scene. A nullptr is returned if the scene doesn't have a list.
		static cherry::ObjectList* GetSceneObjectListByName(std::string sceneName);

		// adds an object using the provided scene. If successful, a true is returned.
		// if a list for the scene already exists, then a new list isn't made, and a false is returned.
		static bool CreateSceneObjectList(std::string scene);

		// adds an object to the cooresponding scene object list. If successful, 'true' is passed.
		// only call this function if the object has already been given a scene.
		// if the scene doesn't exist, or the object is already part of that scene, a false is passed.
		// However, if addSceneList is set to 'true', a list for that scene is created, and 'true' is returned.
		static bool AddObjectToSceneObjectList(cherry::Object * obj, bool addSceneList = true);

		// removes an object from its scene object list.
		// if 'true' is returned, then the object was removed successfully. 
		// If false, the object either wasn't in the list, or the list didn't exist.
		static bool RemoveObjectFromSceneObjectList(cherry::Object* obj);

		// delets an object from its scene object list.
		// if a 'false' is returned, then the object was null, or the list didn't exist.
		static bool DeleteObjectFromSceneObjectList(cherry::Object* obj);

		// deletes a scene object list by using its index.
		static bool DestroySceneObjectListByIndex(unsigned int index);

		// deletes an object list via finding its pointer.
		static bool DestroySceneObjectListByPointer(cherry::ObjectList * objList);
		
		// deletes a scene list via the name of the scene.
		static bool DestroySceneObjectListByName(std::string sceneName);

		// standard operators (taken out because free access isn't needed)
		// reading
		// const cherry::ObjectList & operator[](const int index) const;
		// 
		// // editing
		// cherry::ObjectList & operator[](const int index);

		// updates the object when a window child is added or removed.
		static void UpdateWindowChild(cherry::Object* object);

	private:

		// a vector that contains a series of pointer vectors that all contain teir list of sceneLists
		static std::vector<cherry::ObjectList *> objectLists;

	protected:

	};

	// a class for holding a list of sceneLists
	class ObjectList 
	{
	public:
		ObjectList(std::string scene); // the scene the sceneLists are all in.

		// destructor
		~ObjectList();

		// returns the scene the object list is for.
		std::string GetSceneName() const;

		// gets the total amount of objects
		int GetObjectCount() const;

		// gets the objects as a vector
		std::vector<cherry::Object*> & GetObjects();

		// finds and returns an object via its index. Returns a nullptr if no object is found.
		cherry::Object* GetObjectByIndex(unsigned int index);

		// TODO: add 'get object index'?
		
		// gets the first object found with the provided name.
		cherry::Object* GetObjectByName(std::string name);


		// adds an object to the list
		bool AddObject(cherry::Object * obj);

		// removes an object from the list, and returns it. This is based on the index of the object.
		// this does not delete the object from memory. To do so, use DeleteObject.
		cherry::Object * RemoveObjectByIndex(unsigned int index);

		// removes the object from the list by searching for the pointer to said object. Said object is then returned.
		// if an object that isn't in the list was passed, then a nullptr is returned.
		// to delete the object from memory, use DeleteObject.
		cherry::Object* RemoveObjectByPointer(cherry::Object* obj);

		// removes the first object found with provided name. This object is returned if found, and a nullptr is returned if not.
		// to delete the object from memory, use DeleteObject.
		cherry::Object* RemoveObjectByName(std::string name);

		// deletes an object from memory based on a provided index.
		bool DeleteObjectByIndex(unsigned int index);

		// deletes an object from the list based on a provided pointer.
		bool DeleteObjectByPointer(cherry::Object* obj);

		// deletes an object based on its name.
		bool DeleteObjectByName(std::string name);

		// standard operators
		// reading; please user proper functions if adding or removing an object.
		const cherry::Object& operator[](const int index) const;

		// editing; please user proper functions if adding or removing an object.
		cherry::Object& operator[](const int index);

		// gets the size of the list, which is the same as calling GetObjectCount().
		size_t Size() const;

		// grabs an object fom the list.
		cherry::Object& At(const int index) const;

		// called so that the object is remembered as a child of the window.
		void RememberWindowChild(cherry::Object* object);

		// called so that the child is removed from the window child list.
		// this does not remove the window child from the overall object list.
		void ForgetWindowChild(cherry::Object* object);

		// called when the window is being resized for window children.
		void OnWindowResize(int newWidth, int newHeight);

		// updates all sceneLists in the list.
		void Update(float deltaTime);

		// if 'true', collision is checked between objects.
		// bool checkCollision = true;

		// this should REALLY be private, but there's no time to change that.
		std::vector<cherry::Object*> objects; // the vector of sceneLists

	private:
		std::string scene = ""; // the scene te object is in. TODO: make consts?

		std::vector<cherry::Object*> windowChildren;

	protected:
	};
	
}