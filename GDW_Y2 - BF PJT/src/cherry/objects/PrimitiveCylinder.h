// PrimitiveCylinder
#pragma once
#include "Primitive.h"
namespace cherry
{
	typedef class PrimitiveCylinder : public cherry::Primitive
	{
	public:
		// creates a cylinder
		// segments: the amount of segments for the depth of the cylinder
		PrimitiveCylinder(float radius = 1.0F, float height = 1.0F, unsigned int segments = 10);

		// gets the radius
		float GetRadius() const;

		// gets the height
		float GetHeight() const;

	private:
		float radius;
		float height;

	protected:
	
	} Cylinder;
}

