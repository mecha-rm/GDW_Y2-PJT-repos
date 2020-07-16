// SceneManager - Manages Scenes
#pragma once
#include "Scene.h"
#include <unordered_map>


namespace cherry
{
	class SceneManager {
	public:
		struct SceneIterator {
			typedef std::unordered_map<std::string, Scene*>::iterator iterator;
			typedef std::unordered_map<std::string, Scene*>::const_iterator const_iterator;
			iterator begin() { return SceneManager::_KnownScenes.begin(); }
			iterator end() { return SceneManager::_KnownScenes.end(); }
			const_iterator begin() const { return SceneManager::_KnownScenes.begin(); }
			const_iterator end() const { return SceneManager::_KnownScenes.end(); }
		};

		// gets the current scene.
		static Scene* Current();

		// sets the current m_Scene
		static bool SetCurrentScene(const std::string& name);

		// returns 'true' if the m_Scene manager has the given m_Scene
		static bool HasScene(const std::string& name);

		// gets the m_Scene
		static Scene* Get(const std::string& name);

		// registered a scene of the provided name.
		static void RegisterScene(const std::string& name);

		// registers the provided scene.
		static void RegisterScene(Scene* scene);

		// returns a SceneIterator
		static SceneIterator Each();

		// removes a scene based on its name. The scene is then returned (nullptr returned otherwise).
		static Scene* RemoveScene(const std::string& name);

		// removes a scene based on its scene object. The scene is then returned (nullptr returned otherwise).
		static void RemoveScene(const Scene* scene);

		// destroys a scene based on its name. A true is returned if it's successful.
		static bool DestroyScene(const std::string& name);

		// destroys a scene based on its scene object. A true is returned if it's successful.
		// note that if the scene is NOT in the scene manager's list, it will NOT be destroyed.
		static bool DestroyScene(Scene* scene);

		// destorys all scenes
		static void DestroyScenes();

		static entt::registry Prefabs;
	private:
		static Scene* _CurrentScene;
		static std::unordered_map<std::string, Scene*> _KnownScenes;
	};

	// We can make some macros to shorten our calls
#define CurrentScene() SceneManager::Current()
#define CurrentRegistry() SceneManager::Current()->Registry()
#define GetRegistry(name) SceneManager::Get(name)->Registry()
}