#pragma once

// #include "Object.h"
#include "Primitives.h"
#include "Image.h"
#include "Liquid.h"

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
		bool SceneObjectListExists(std::string sceneName) const;

		// gets the object list for the scene by index. A nullptr is returned if the scene doesn't have a list.
		cherry::ObjectList* GetSceneObjectListByIndex(unsigned int index) const;

		// gets the object list for the scene. A nullptr is returned if the scene doesn't have a list.
		cherry::ObjectList* GetSceneObjectListByName(std::string sceneName) const;

		// adds an object using the provided scene. If successful, a true is returned.
		// if a list for the scene already exists, then a new list isn't made, and a false is returned.
		bool AddSceneObjectList(std::string scene);

		// adds an object to the cooresponding scene object list. If successful, 'true' is passed.
		// only call this function if the object has already been given a scene.
		// if the scene doesn't exist, or the object is already part of that scene, a false is passed.
		// However, if addSceneList is set to 'true', a list for that scene is created, and 'true' is returned.
		bool AddObjectToSceneObjectList(Object * obj, bool addSceneList = true);

		// AddObject(scene)

		// deletes a scene object list by using its index.
		bool DeleteSceneObjectListByIndex(unsigned int index);

		// deletes an object list via finding its pointer.
		bool DeleteSceneObjectListByPointer(cherry::ObjectList * obj);
		
		// deletes a scene list via the name of the scene.
		bool DeleteSceneObjectListByName(std::string sceneName);

		// standard operators
		// reading
		const cherry::ObjectList & operator[](const int index) const;

		// editing
		cherry::ObjectList & operator[](const int index);

	private:

		// a vector that contains a series of pointer vectors that all contain teir list of sceneLists
		std::vector<cherry::ObjectList *> objectLists;

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
		// reading
		// const cherry::Object& operator[](const int index) const;

		// editing
		// cherry::Object& operator[](const int index);

		// updates all sceneLists in the list.
		void Update(float deltaTime);

		std::vector<cherry::Object*> objects; // the vector of sceneLists

	private:
		std::string scene = ""; // the scene te object is in. TODO: make consts?

	protected:
	};
	
}