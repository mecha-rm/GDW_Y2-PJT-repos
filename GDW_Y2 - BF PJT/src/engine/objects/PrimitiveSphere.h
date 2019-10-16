#pragma once
#include "Primitive.h"

typedef class PrimitiveSphere : public Primitive
{
public:
	// segRows: the total amount of rows of segments (MUST be greater than 3).
	// segCols: the total amount of columns of segments (MUST be greater than 3).
	// add variable for starting angle and ending angle
	PrimitiveSphere(float radius, unsigned int segRows, unsigned int segCols);
	// PrimitiveSphere(float radius, float segments);
private:
protected:
} Sphere;

