// Physics (Header) - currently does nothing
// This was a file that was deleted, but kept having its .obj file added back when the project gets rebuilt.
// This would cause compile errors, and require that the file be added back in.
// For ease of development, this file has been kept, even though it serves no purpose at this time.
#pragma once

#include "PhysicsBody.h"
#include "..\utils/math/Collision.h"

namespace cherry
{
	static float gravity = 9.807F;
}