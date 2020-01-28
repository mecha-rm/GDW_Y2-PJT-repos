// Scene (Header) - holds scene information
#pragma once

#include <entt/entt.hpp>

#include "TextureCube.h"
#include "Shader.h"
#include "Mesh.h"

namespace cherry
{
	class Scene {
	public:
		TextureCube::Sptr Skybox;
		Shader::Sptr SkyboxShader;
		Mesh::Sptr SkyboxMesh;

		Scene() = default;

		// EX: creates a scene, providing it its name.
		Scene(std::string name) : myName(name) {}

		virtual ~Scene() = default;

		virtual void OnOpen() {};

		virtual void OnClose() {};

		// returns the main/primary registry
		entt::registry& Registry() { return myRegistry; }

		const std::string& GetName() const { return myName; }

		void SetName(const std::string& name) { myName = name; }
	private:
		entt::registry myRegistry; // registry
		std::string myName;
	};
}