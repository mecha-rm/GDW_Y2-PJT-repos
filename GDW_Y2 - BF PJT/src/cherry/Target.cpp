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

// sets the new position.
void cherry::Target::SetPosition(cherry::Vec3 newPos) { position = newPos; }

// translates the target.
void cherry::Target::Translate(cherry::Vec3 translation)
{
	position += translation;
}

