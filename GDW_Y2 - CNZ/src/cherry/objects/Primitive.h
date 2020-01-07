// Primitive - creates primitives for the engine
#pragma once

#include "Object.h"

namespace cherry
{
	// all primitives inherit from this
	// TODO: make abstract class
	class Primitive : public cherry::Object
	{
	public:
		Primitive();

		~Primitive();

		// gets the base color of the primitive.
		// cherry::Vec4 GetBaseColor() const;

		// gets the base color of the primitive.
		// glm::vec4 GetBaseColor() const;

		// would this work?
		// virtual std::string ToString() = 0;

	private:
		// cherry::Vec4 baseColor; // the base color of the object.


	protected:

		// calculates the normals for the primitive.
		// ONLY CALL THIS IF THE VERTICES AND INDICES ARE SET.
		void CalculateNormals();

		// changes the direction of the normals. Only call this if vertices are set.
		void InvertNormals();

		cherry::Vec4 baseColor{ 1.0F, 1.0F, 1.0F, 1.0F }; // the base color of the object.
	};
}
