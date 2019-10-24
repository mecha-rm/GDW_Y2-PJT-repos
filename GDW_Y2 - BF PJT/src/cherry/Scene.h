#pragma once

#include <entt/entt.hpp>

namespace cherry
{
	class Scene {
	public:
		Scene() = default;

		virtual ~Scene() = default;

		virtual void OnOpen() {};

		virtual void OnClose() {};

		entt::registry& Registry() { return myRegistry; }

		const std::string& GetName() const { return myName; }

		void SetName(const std::string& name) { myName = name; }
	private:
		entt::registry myRegistry;
		std::string myName;
	};
}