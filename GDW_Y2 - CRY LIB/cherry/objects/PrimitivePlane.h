// PrimitivePlane
#pragma once
#include "Primitive.h"

namespace cherry
{
	typedef class PrimitivePlane : public cherry::Primitive
	{
	public:
		PrimitivePlane(float width = 1.0F, float height = 1.0F, bool doubleSided = true, cherry::Vec4 color = cherry::Vec4(1.0F, 1.0F, 1.0F, 1.0F));

		// gets the width of the plane.
		float GetWidth() const;

		// gets the height of the plane.
		float GetHeight() const;

	private:
		float width; // width
		float height; // height

	protected:

	} Plane;
}
