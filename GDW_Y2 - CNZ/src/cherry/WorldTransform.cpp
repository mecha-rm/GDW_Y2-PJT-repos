#include "WorldTransform.h"

glm::mat4 cherry::TempTransform::GetWorldTransform() const
{
	return
		glm::translate(glm::mat4(1.0f), Position) *
		glm::mat4_cast(glm::quat(glm::radians(EulerRotation))) *
		glm::scale(glm::mat4(1.0f), Scale)
		;
}
