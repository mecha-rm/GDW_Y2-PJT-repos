// Primitive - creates primitives for the engine
#pragma once

#include "Object.h"

namespace cherry
{
	// forward declare for functions
	// this is already forward declared in the Material class, but this is still here as a reminder.
	class LightList;

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

		// generate default material. This material does not support lighting.
		static cherry::Material::Sptr GenerateDefaultMaterial();

		// generates a generic material for the primitive.
		static cherry::Material::Sptr GenerateLightingMaterial();

		// generates a material using a primitive and gets values from the light list.
		static cherry::Material::Sptr GenerateLightingMaterial(cherry::LightList* ll);

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
