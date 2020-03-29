// TempTransformation and Update Behaviour - used for bringing sceneLists into world space and updating them.
#pragma once

#include <GLM/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp> 
#include <glm/gtx/quaternion.hpp>
#include <entt/entt.hpp>

// TempTransform and UpdateBehaviour were originally in the Game.cpp file for the Intro. to Computer Graphics framework.
// They were moved here so that they could be included by classes that need them.
namespace cherry
{
	struct TempTransform {
		glm::vec3 Position = glm::vec3(0.0f);
		glm::vec3 EulerRotation = glm::vec3(0.0f);
		glm::vec3 Scale = glm::vec3(1.0f);

		// does our TRS for us.
		glm::mat4 GetWorldTransform() const;
	};

	struct UpdateBehaviour {
		std::function<void(entt::entity e, float dt)> Function;
	};
}