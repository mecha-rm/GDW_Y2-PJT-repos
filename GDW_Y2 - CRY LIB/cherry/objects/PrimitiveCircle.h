// PrimitiveCircle - used for making circles
#pragma once

#include "Primitive.h"

namespace cherry
{
	typedef class PrimitiveCircle : public cherry::Primitive
	{
	public:
		// takes a position, radius, and vertices. Vertices must be at least edges.
		// at least 3 dges must be provided.
		PrimitiveCircle(float radius = 1.0F, unsigned int edges = 10, bool doubleSided = true, cherry::Vec4 color = { 1.0F, 1.0F, 1.0F, 1.0F });

		// gets the radius
		float GetRadius() const;

	private:
		float radius;
	protected:

	} Circle;
}

