#include "ObjectManager.h"
#include "..\utils\Utils.h"
#include "..\PhysicsBody.h"

// object manager

// destructor
cherry::ObjectManager::~ObjectManager()
{
	// deleting the data in the lists
	for (cherry::ObjectList* list : objectLists)
	{
		delete list;
	}

	objectLists.clear();
}

// checks to see if a scene object list already exists.
bool cherry::ObjectManager::SceneObjectListExists(std::string sceneName) const
{
	// checks to see if a scene exist.
	for (ObjectList* list : objectLists)
	{
		if (list->GetSceneName() == sceneName)
			return true;
	}

	return false;
}

// gets the object list by index
cherry::ObjectList* cherry::ObjectManager::GetSceneObjectListByIndex(unsigned int index) const
{
	if (index >= objectLists.size()) // index out of bounds
		return nullptr;

	return objectLists[index];
}

// gets list of objects for a provided scene. A nullptr is returned if the scene doesn't have a list.
cherry::ObjectList* cherry::ObjectManager::GetSceneObjectListByName(std::string sceneName) const
{
	// goes through the sceneLists to get the right object list.
	for (cherry::ObjectList* objList : objectLists)
	{
		if (objList->GetSceneName() == sceneName) // if the objectList for the provided scene has been found.
			return objList;
	}
	return nullptr;
}

// add scene object list. 
bool cherry::ObjectManager::AddSceneObjectList(std::string scene)
{
	for (cherry::ObjectList* objList : objectLists)
	{
		if (objList->GetSceneName() == scene) // a list for this scene already exists.
			return false;
	}

	// adds an object to the list.
	ObjectList* list = new ObjectList(scene);
	objectLists.push_back(list);

	return true;
}

// adds object to the scene object list.
bool cherry::ObjectManager::AddObjectToSceneObjectList(Object* obj, bool addSceneList)
{
	if (obj == nullptr) // nullptr of an object
		return false;

	std::string scene = obj->GetScene();

	// goes thorugh the object lists to see if an appropriate scene exists.
	for (cherry::ObjectList* objList : objectLists)
	{
		if (objList->GetSceneName() == obj->GetScene()) // if the object is part of a scene that already has a list.
		{
			return objList->AddObject(obj); // adds the object to the list.
		}
	}

	if (addSceneList) // if true, a new scene list is created.
	{
		ObjectList* objList = new ObjectList(obj->GetScene());
		objList->AddObject(obj);
		objectLists.push_back(objList);
		return true;
	}
	else
	{
		return false;
	}
}

// deletes a scene object list by using its index.
bool cherry::ObjectManager::DeleteSceneObjectListByIndex(unsigned int index)
{
	ObjectList* objList = GetSceneObjectListByIndex(index);

	if (objList != nullptr) // if the object isn't a nullptr
	{
		util::removeFromVector(objectLists, objList);
		delete objList;

		return true;
	}
	else // if the object is a nullptr
	{
		return false;
	}
}

// deletes a scene object, which is found via a pointer
bool cherry::ObjectManager::DeleteSceneObjectListByPointer(cherry::ObjectList* obj)
{
	if (util::removeFromVector(objectLists, obj)) // in list
	{
		delete obj;
		return true;
	}
	else // was not in list
	{
		return false;
	}
}

// deletes a scene object via finding the name of the scene
bool cherry::ObjectManager::DeleteSceneObjectListByName(std::string sceneName)
{
	ObjectList* obj = GetSceneObjectListByName(sceneName);

	return DeleteSceneObjectListByPointer(obj);
}

// reading of an index
const cherry::ObjectList & cherry::ObjectManager::operator[](const int index) const { return *objectLists[index]; }

// editing of an index
cherry::ObjectList & cherry::ObjectManager::operator[](const int index) { return *objectLists[index]; }

// OBJECT LIST
// constructor
cherry::ObjectList::ObjectList(std::string scene)
	: scene(scene)
{
}

// destructor
cherry::ObjectList::~ObjectList()
{
	// deletes all sceneLists
	for (cherry::Object* obj : objects)
		delete obj;

	objects.clear();
}

// returns the scene for the object list.
std::string cherry::ObjectList::GetSceneName() const { return scene; }

// gets the total amount of objects
int cherry::ObjectList::GetObjectCount() const { return objects.size(); }

// gets the objects as a vector
std::vector<cherry::Object*>& cherry::ObjectList::GetObjects() { return objects; }

// gets the object at the provided index.
cherry::Object* cherry::ObjectList::GetObjectByIndex(unsigned int index)
{
	if (index >= objects.size()) // out of bounds
		return nullptr;
	
	return objects.at(index);
}

// gets an object using its name.
cherry::Object* cherry::ObjectList::GetObjectByName(std::string name)
{
	for (Object* obj : objects) // finds object by name
	{
		if (obj->GetName() == name)
			return obj;
	}

	return nullptr;
}

// adds an object to the list.
bool cherry::ObjectList::AddObject(cherry::Object* obj)
{
	if (obj == nullptr)
		return false;

	return util::addToVector(objects, obj); // adds the object to the vector.
}

// removes an object from the list based on if it is the correct index.
cherry::Object* cherry::ObjectList::RemoveObjectByIndex(unsigned int index)
{
	if (index >= objects.size())
		return nullptr;

	// gets a pointer to the object, removes it, and then returns said object.
	cherry::Object* obj = objects[index];
	util::removeFromVector(objects, obj);

	return obj;
}

// removes the object based on a pointer to said object.
cherry::Object* cherry::ObjectList::RemoveObjectByPointer(cherry::Object* obj)
{
	if (obj == nullptr)
		return nullptr;

	if (util::removeFromVector(objects, obj)) // if the object was found and removed
	{
		return obj;
	}
	else // object wasn't in the list.
	{
		return nullptr;
	}
}

// removes an object by its name.
cherry::Object* cherry::ObjectList::RemoveObjectByName(std::string name)
{
	for (Object* obj : objects) // searches for object with a given name.
	{
		if (obj->GetName() == name) // object with proper name found.
		{
			return RemoveObjectByPointer(obj);
		}
	}

	// no object with said name found.
	return nullptr;
}

// deletes an object from memory via its index in the list.
bool cherry::ObjectList::DeleteObjectByIndex(unsigned int index)
{
	cherry::Object* obj = RemoveObjectByIndex(index);

	if (obj == nullptr) // requested object wasn't found
	{
		return false;
	}
	else // requested object was found
	{
		delete obj;
		return true;
	}
}

// deletes an object, which is found via its pointer.
bool cherry::ObjectList::DeleteObjectByPointer(cherry::Object* obj)
{
	if (obj == nullptr)
		return false;

	cherry::Object* objPtr = RemoveObjectByPointer(obj); // removes the object, and saves it

	if (objPtr == nullptr) // requested object wasn't found
	{
		return false;
	}
	else // requested object was found
	{
		delete objPtr;
		return true;
	}
}

// deletes an object by name.
bool cherry::ObjectList::DeleteObjectByName(std::string name)
{
	cherry::Object* obj = RemoveObjectByName(name); // removes the object, and saves a reference for deletion.

	if (obj == nullptr) // requested object wasn't found
	{
		return false;
	}
	else // requested object was found
	{
		delete obj;
		return true;
	}
}

// reading ~ gets an object from the object list
// const cherry::Object& cherry::ObjectList::operator[](const int index) const { return *(objects.at(index)); }

// editing ~ gets an object from the object list
// cherry::Object& cherry::ObjectList::operator[](const int index) { return *(objects.at(index)); }

// updates all sceneLists in the list
void cherry::ObjectList::Update(float deltaTime)
{
	// updates all objects.
	for (Object* obj : objects)
	{
		obj->Update(deltaTime);
		obj->SetIntersection(false);
	}

	// collision calculations
mainLoop:
	for (cherry::Object* obj1 : objects) // object 1
	{
		if (obj1 == nullptr)
			continue;
		if (obj1->GetIntersection() == true) // already colliding with something.
			continue;

		for (cherry::Object* obj2 : objects) // object 2
		{
			if (obj1 == obj2 || obj2 == nullptr) // if the two sceneLists are the same.
				continue;

			if (obj2->GetIntersection() == true) // if the object is already intersecting with something.
				continue;

			// gets the vectors from both sceneLists
			std::vector<cherry::PhysicsBody*> pbods1 = obj1->GetPhysicsBodies();
			std::vector<cherry::PhysicsBody*> pbods2 = obj2->GetPhysicsBodies();

			// goes through each collision body
			for (cherry::PhysicsBody* pb1 : pbods1)
			{
				for (cherry::PhysicsBody* pb2 : pbods2)
				{
					bool col = PhysicsBody::Collision(pb1, pb2);

					if (col == true) // if collision has occurred.
					{
						obj1->SetIntersection(true);
						// obj1->setColor(255, 0, 0);
						obj2->SetIntersection(true);
						// obj2->setColor(255, 0, 0);
						// std::cout << "Hit!" << std::endl;

						goto mainLoop; // goes back to the main loop
					}
				}
			}
		}
	}
}
