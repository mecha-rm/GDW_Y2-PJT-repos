#include "ObjectManager.h"

#include "..\utils\Utils.h"
#include "..\physics/PhysicsBody.h"
#include "..\Game.h"

// object manager
std::vector<cherry::ObjectList*> cherry::ObjectManager::objectLists = std::vector<cherry::ObjectList*>();

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
bool cherry::ObjectManager::SceneObjectListExists(std::string sceneName)
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
cherry::ObjectList* cherry::ObjectManager::GetSceneObjectListByIndex(unsigned int index)
{
	if (index >= objectLists.size()) // index out of bounds
		return nullptr;

	return objectLists[index];
}

// gets list of objects for a provided scene. A nullptr is returned if the scene doesn't have a list.
cherry::ObjectList* cherry::ObjectManager::GetSceneObjectListByName(std::string sceneName)
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
bool cherry::ObjectManager::CreateSceneObjectList(std::string scene)
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
bool cherry::ObjectManager::AddObjectToSceneObjectList(cherry::Object* obj, bool addSceneList)
{
	if (obj == nullptr) // nullptr of an object
		return false;

	std::string scene = obj->GetSceneName();

	// goes thorugh the object lists to see if an appropriate scene exists.
	for (cherry::ObjectList* objList : objectLists)
	{
		if (objList->GetSceneName() == obj->GetSceneName()) // if the object is part of a scene that already has a list.
		{
			return objList->AddObject(obj); // adds the object to the list.
		}
	}

	if (addSceneList) // if true, a new scene list is created.
	{
		ObjectList* objList = new ObjectList(obj->GetSceneName());
		objList->AddObject(obj);
		objectLists.push_back(objList);
		return true;
	}
	else
	{
		return false;
	}
}

// removes an object from a scene object list.
bool cherry::ObjectManager::RemoveObjectFromSceneObjectList(cherry::Object* obj)
{
	if (obj == nullptr) // no object passed.
		return false;

	ObjectList* objList = GetSceneObjectListByName(obj->GetSceneName());

	if (objList == nullptr) // no list exists
		return false;
	
	// if the object isn't returned then it was never in hte list.
	if (objList->RemoveObjectByPointer(obj) != nullptr)
		return true;
	else
		return false;
}

// deletes an object from its scene object list.
bool cherry::ObjectManager::DeleteObjectFromSceneObjectList(cherry::Object* obj)
{
	if (obj == nullptr) // no object passed.
		return false;

	ObjectList* objList = GetSceneObjectListByName(obj->GetSceneName()); // gets the object list.

	if (objList == nullptr) // no list exists
		return false;

	return objList->DeleteObjectByPointer(obj);
}

// deletes a scene object list by using its index.
bool cherry::ObjectManager::DestroySceneObjectListByIndex(unsigned int index)
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
bool cherry::ObjectManager::DestroySceneObjectListByPointer(cherry::ObjectList* objList)
{
	if (util::removeFromVector(objectLists, objList)) // in list
	{
		delete objList;
		return true;
	}
	else // was not in list
	{
		return false;
	}
}

// deletes a scene object via finding the name of the scene
bool cherry::ObjectManager::DestroySceneObjectListByName(std::string sceneName)
{
	ObjectList* obj = GetSceneObjectListByName(sceneName);

	return DestroySceneObjectListByPointer(obj);
}

// reading
// const cherry::ObjectList& cherry::ObjectManager::operator[](const int index) const
// {
// 	return *objectLists.at(index);
// }
// 
// // editing
// cherry::ObjectList& cherry::ObjectManager::operator[](const int index)
// {
// 	return *objectLists.at(index);
// }

// updates the object manager to inform it of a window chil being added or removed.
void cherry::ObjectManager::UpdateWindowChild(cherry::Object* object)
{
	if (object == nullptr) // safety check
		return;
	
	// gets the list
	ObjectList * list = ObjectManager::GetSceneObjectListByName(object->GetSceneName()); // gets its object list.
	
	if (list == nullptr)
		return;

	// object is a window child
	if (object->IsWindowChild())
	{
		list->RememberWindowChild(object);
	}
	else // object is not a window child
	{
		list->ForgetWindowChild(object);
	}
}

// reading of an index
// const cherry::ObjectList & cherry::ObjectManager::operator[](const int index) const { return *objectLists[index]; }

// editing of an index
// cherry::ObjectList & cherry::ObjectManager::operator[](const int index) { return *objectLists[index]; }

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
	windowChildren.clear(); // has objects that are also in objects vector.
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

	// adds the object to the vector.
	bool added = util::addToVector(objects, obj);

	// if the object was added, and is a window child.
	if (added == true && obj->IsWindowChild()) // remembering the child
		util::addToVector(windowChildren, obj);
	

	return added;
}

// removes an object from the list based on if it is the correct index.
cherry::Object* cherry::ObjectList::RemoveObjectByIndex(unsigned int index)
{
	if (index >= objects.size())
		return nullptr;

	// gets a pointer to the object, removes it, and then returns said object.
	cherry::Object* obj = objects[index];

	if (obj != nullptr)
	{
		util::removeFromVector(objects, obj);

		if (obj->IsWindowChild()) // if the object is a window child, it is removed.
			util::removeFromVector(windowChildren, obj);
	}

	return obj;
}

// removes the object based on a pointer to said object.
cherry::Object* cherry::ObjectList::RemoveObjectByPointer(cherry::Object* obj)
{
	if (obj == nullptr)
		return nullptr;

	if (util::removeFromVector(objects, obj)) // if the object was found and removed
	{
		if (obj->IsWindowChild()) // if the object is a window child, it is removed.
			util::removeFromVector(windowChildren, obj);

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
const cherry::Object& cherry::ObjectList::operator[](const int index) const { return *objects[index]; }

// editing ~ gets an object from the object list
cherry::Object& cherry::ObjectList::operator[](const int index) { return *objects[index]; }

// gets the size of the object list.
size_t cherry::ObjectList::Size() const { return objects.size(); }

// grabs an object from the lsit.
cherry::Object& cherry::ObjectList::At(const int index) const { return *objects.at(index); }

// called so that the list remembers this is a window child.
void cherry::ObjectList::RememberWindowChild(cherry::Object* object)
{
	util::addToVector(windowChildren, object);
}


// forgets the child is a child to the window
void cherry::ObjectList::ForgetWindowChild(cherry::Object* object)
{
	util::removeFromVector(windowChildren, object);
}

// called when the window is resized for window children.
void cherry::ObjectList::OnWindowResize(int newWidth, int newHeight)
{
	glm::ivec2 windowSize = Game::GetRunningGame()->GetWindowSize(); // the window size hasn't been changed yet
	glm::vec2 scale{ (float)newWidth / (float)windowSize.x, (float)newHeight / (float)windowSize.y}; // new scale

	for (Object* windowChild : windowChildren)
	{
		// object's current position
		glm::vec3 currPos = windowChild->GetPositionGLM();

		// gets the (t) value from the original window size
		// inverseLerp: (pos_want - pos_start) / (pos_end - pos_start)
		glm::vec2 t(
			(currPos.x) / (windowSize.x),
			(currPos.y) / (windowSize.y)
		);

		// windowChild->SetPosition(tempPos.v.x * scale.x, tempPos.v.y * scale.y, tempPos.v.z);
		// uses lerp to repostion the object.
		windowChild->SetPosition(
			glm::mix(0.0F, (float)newWidth, t.x), 
			glm::mix(0.0F, (float)newHeight, t.y),
			currPos.z
		);
		windowChild->SetScale(windowChild->GetScale() * ((scale.x + scale.y) / 2.0F));
	}
}

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
//mainLoop:
//	for (cherry::Object* obj1 : objects) // object 1
//	{
//		if (obj1 == nullptr)
//			continue;
//		if (obj1->GetIntersection() == true) // already colliding with something.
//			continue;
//
//		for (cherry::Object* obj2 : objects) // object 2
//		{
//			if (obj1 == obj2 || obj2 == nullptr) // if the two sceneLists are the same.
//				continue;
//
//			if (obj2->GetIntersection() == true) // if the object is already intersecting with something.
//				continue;
//
//			// gets the vectors from both sceneLists
//			std::vector<cherry::PhysicsBody*> pbods1 = obj1->GetPhysicsBodies();
//			std::vector<cherry::PhysicsBody*> pbods2 = obj2->GetPhysicsBodies();
//
//			// goes through each collision body
//			for (cherry::PhysicsBody* pb1 : pbods1)
//			{
//				for (cherry::PhysicsBody* pb2 : pbods2)
//				{
//					bool col = PhysicsBody::Collision(pb1, pb2);
//
//					if (col == true) // if collision has occurred.
//					{
//						obj1->SetIntersection(true);
//						// obj1->setColor(255, 0, 0);
//						obj2->SetIntersection(true);
//						// obj2->setColor(255, 0, 0);
//						// std::cout << "Hit!" << std::endl;
//
//						goto mainLoop; // goes back to the main loop
//					}
//				}
//			}
//		}
//	}
}
