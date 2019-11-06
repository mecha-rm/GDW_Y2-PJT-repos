// Default Primitives for the Engine
#pragma once

#include "Object.h"

namespace cherry
{
	// TODO: make abstract class
	class Primitive : public cherry::Object
	{
	public:
		Primitive();

		// would this work?
		// virtual std::string ToString() = 0;

	private:

	protected:

		// calculates the normals for the primitive.
		// ONLY CALL THIS IF THE VERTICES AND INDICES ARE SET.
		void calculateNormals();

	};
}
