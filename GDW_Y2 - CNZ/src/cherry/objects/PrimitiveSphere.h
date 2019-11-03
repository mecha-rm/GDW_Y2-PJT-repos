#pragma once
#include "Primitive.h"

namespace cherry
{
	typedef class PrimitiveSphere : public cherry::Primitive
	{
	public:
		// segRows: the total amount of rows of segments (MUST be greater than 3).
		// segCols: the total amount of columns of segments (MUST be greater than 3).
		// add variable for starting angle and ending angle
		PrimitiveSphere(float radius = 1.0F, unsigned int segRows = 10, unsigned int segCols = 10);
		// PrimitiveSphere(float radius, float segments);

		// returns the radius
		float GetRadius() const;

	private:
		float radius;

	protected:
	} Sphere;
}

