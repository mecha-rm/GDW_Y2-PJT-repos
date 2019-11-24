#pragma once

#include <GLM/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp> 
#include <glm/gtx/quaternion.hpp>
#include <entt/entt.hpp>

namespace cherry
{
	struct TempTransform {
		glm::vec3 Position = glm::vec3(0.0f);
		glm::vec3 EulerRotation = glm::vec3(0.0f);
		glm::vec3 Scale = glm::vec3(1.0f);

		// does our TRS for us.
		glm::mat4 GetWorldTransform() const {
			return
				glm::translate(glm::mat4(1.0f), Position) *
				glm::mat4_cast(glm::quat(glm::radians(EulerRotation))) *
				glm::scale(glm::mat4(1.0f), Scale)
				;
		}
	};

	struct UpdateBehaviour {
		std::function<void(entt::entity e, float dt)> Function;
	};
}