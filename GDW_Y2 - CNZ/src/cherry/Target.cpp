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

// gets the position
cherry::Vec3 cherry::Target::GetPosition() const { return position; }
