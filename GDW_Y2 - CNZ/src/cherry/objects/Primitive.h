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

		// copy constructor.
		// Primitive(const cherry::Primitive&);

		~Primitive();

		// gets the base color of the primitive.
		cherry::Vec4 GetColor() const;

		// would this work?
		// virtual std::string ToString() = 0;

	private:
		// cherry::Vec4 baseColor; // the base color of the object.

		// maybe add a primitive object counter.
	protected:

		// calculates the normals for the primitive.
		// ONLY CALL THIS IF THE VERTICES AND INDICES ARE SET.
		void CalculateNormals();

		// changes the direction of the normals. Only call this if vertices are set.
		void InvertNormals();

		// colour
		cherry::Vec4 color{ 1.0F, 1.0F, 1.0F, 1.0F }; // the base color of the object.
	};
}
