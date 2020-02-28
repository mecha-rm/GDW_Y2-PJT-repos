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

		static SceneIterator Each();

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