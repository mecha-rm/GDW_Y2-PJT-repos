/*
 * Name: Team BONUS FRUIT
 * Date: 07/13/2020
 * Description: manages scenes for the game.
 * References:
	* http://www.cplusplus.com/reference/unordered_map/unordered_map/erase/
	* https://en.cppreference.com/w/cpp/language/static_cast
	* https://en.cppreference.com/w/cpp/language/dynamic_cast
	* https://stackoverflow.com/questions/2253168/dynamic-cast-and-static-cast-in-c
*/

// SceneManager - Manages Scenes

#include "SceneManager.h"
#include<toolkit/Logging.h>


cherry::Scene* cherry::SceneManager::_CurrentScene = nullptr;
std::unordered_map<std::string, cherry::Scene*> cherry::SceneManager::_KnownScenes;
entt::registry cherry::SceneManager::Prefabs;

// returns current scene
cherry::Scene* cherry::SceneManager::Current() 
{
	return _CurrentScene;
}

// checks to see if the current scene exists
bool cherry::SceneManager::HasScene(const std::string& name)
{
	auto it = _KnownScenes.find(name);
	if (it != _KnownScenes.end()) {
		return true;
	}
	return false;
}

// gets the scene
cherry::Scene* cherry::SceneManager::Get(const std::string& name)
{
	if (!HasScene(name))
		return nullptr;
	else
		return _KnownScenes[name];
}

// sets the current scene
bool cherry::SceneManager::SetCurrentScene(const std::string& name) 
{
	auto it = _KnownScenes.find(name);
	
	// the scene exists.
	if (it != _KnownScenes.end()) 
	{
		if (_CurrentScene != it->second) 
		{
			// this would be null if it's the first time we're making the m_Scene. We don't want to close null.
			if (_CurrentScene != nullptr)
				_CurrentScene->OnClose(); // TODO: this causes crashes sometimes for unknown reasons. (3 successful closures, crashes afterwards)
			
			// if new instances are allowed, the scene being switched from is reset.
			if (_CurrentScene != nullptr)
			{
				// new instances are allowed.
				// NOTE: anything added to the scene outside of its constructor and OnOpen() is deleted.
				if (_CurrentScene->IsAllowingNewInstances())
				{
					Scene* remadeScene = _CurrentScene->GenerateNewInstance(); // gets a new scene
					DestroyScene(_CurrentScene); // destroys the current scene
					RegisterScene(remadeScene); // adds back in the remade scene.
				}
			}

			_CurrentScene = it->second;
			_CurrentScene->OnOpen();
		}
		return true;
	}
	return false;
}

// registers a new scene using a provided name.
void cherry::SceneManager::RegisterScene(const std::string& name)
{
	LOG_ASSERT(!HasScene(name), "A scene with that name already exists!");

 	Scene * scene = new Scene(name); // making the scene.
 
 	// we just reference it and put it in. Everything else is done internally.
 	_KnownScenes[name] = scene;
}


// registers a scene and uses its name as the array placement.
void cherry::SceneManager::RegisterScene(Scene* scene)
{
	if (scene == nullptr)
		return;

	const std::string name = scene->GetName();

	LOG_ASSERT(!HasScene(name), "A scene with that name already exists!");

	// we just reference it and put it in. Everything else is done internally.
	_KnownScenes[name] = scene;
}

cherry::SceneManager::SceneIterator cherry::SceneManager::Each() { return SceneIterator(); }

// removes a scene based on its name, and returns it.
cherry::Scene* cherry::SceneManager::RemoveScene(const std::string& name)
{
	// Removing from an un-ordered map.
	// http://www.cplusplus.com/reference/unordered_map/unordered_map/erase/
	
	// gets the scene.
	Scene* scene = Get(name);

	// scene exists, so it is removed form the list.
	if (scene != nullptr)
		_KnownScenes.erase(name);
	
	return scene;
}

// removes a scene using the scene object itself.
void cherry::SceneManager::RemoveScene(const Scene* scene)
{
	// scene does not exist.
	if (scene == nullptr)
		return;

	// the key and the scene name are one in the same.
	std::string key = scene->GetName();

	// removes the scene.
	_KnownScenes.erase(key);
}

// destroys a scene by name.
bool cherry::SceneManager::DestroyScene(const std::string& name)
{
	// gets the scene
	Scene* scene = Get(name);

	// if nullptr, return false.
	if (scene == nullptr)
		return false;

	// removes the scene, then deletes it.
	RemoveScene(scene);
	delete scene;

	return true;
}

// destroys the scene.
bool cherry::SceneManager::DestroyScene(Scene* scene)
{
	// scene doesn't exist.
	if (scene == nullptr)
		return false;

	// gives the scene's name to see if it will be destroyed.
	return DestroyScene(scene->GetName());
}

// interates over every m_Scene and deletes it.
void cherry::SceneManager::DestroyScenes() {
	for (auto& kvp : _KnownScenes) {
		delete kvp.second;
	}
	_KnownScenes.clear();
	_CurrentScene = nullptr;
}