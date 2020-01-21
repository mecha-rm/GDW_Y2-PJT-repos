#include "Target.h"

cherry::Target::Target() : position()
{
}

// position
cherry::Target::Target(glm::vec3 position) : position(position)
{
}

// constructor
cherry::Target::Target(cherry::Vec3 position) : position(position)
{
}

// gets the target's position as a glm vector.
glm::vec3 cherry::Target::GetPositionGLM() const { return glm::vec3(position.v.x, position.v.y, position.v.z); }

// gets the position
cherry::Vec3 cherry::Target::GetPosition() const { return position; }

// sets the new position.
void cherry::Target::SetPosition(cherry::Vec3 newPos) { position = newPos; }

// sets the position
void cherry::Target::SetPosition(glm::vec3 newPos) { position = Vec3(newPos); }

// sets the position of the target.
void cherry::Target::SetPosition(float x, float y, float z) { position = Vec3(x, y, z); }

//// gets the position offset as a glm vector.
//glm::vec3 cherry::Target::GetPositionOffsetGLM() const { return glm::vec3(position.v.x, position.v.y, position.v.z); }
//
//// gets an offset of the target's position.
//cherry::Vec3 cherry::Target::GetPositionOffset() const { return offset; }
//
//// sets the offset of the position.
//void cherry::Target::SetPositionOffset(glm::vec3 newOffset) { offset = newOffset; }
//
//// sets the offset of the position.
//void cherry::Target::SetPositionOffset(cherry::Vec3 newOffset) { offset = glm::vec3(newOffset.v.x, newOffset.v.y, newOffset.v.z); }
//
//// sets the position offset.
//void cherry::Target::SetPositionOffset(float x, float y, float z) { offset = glm::vec3(x, y, z); }
//
//// compute follower position
//cherry::Vec3 cherry::Target::ComputeFollowerPositionGLM() const { return position + offset; }
//
//// compute follower position
//cherry::Vec3 cherry::Target::ComputeFollowerPosition() const { return position + offset; }

// translates the target.
void cherry::Target::Translate(glm::vec3 translate) { position += Vec3(translate); }

// translates the target.
void cherry::Target::Translate(cherry::Vec3 translation) { position += translation; }

// translates the target.
void cherry::Target::Translate(float x, float y, float z) { position += Vec3(x, y, z); }

