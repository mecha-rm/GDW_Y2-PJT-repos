// Used for making circles
#pragma once

#include "Primitive.h"

namespace cherry
{
	typedef class PrimitiveCircle : public cherry::Primitive
	{
	public:
		// takes a position, radius, and vertices. Vertices must be at least edges.
		// at least 3 dges must be provided.
		PrimitiveCircle(float radius = 1.0F, unsigned int edges = 10);

		// float getRadius();

	private:
	protected:

	} Circle;
}
