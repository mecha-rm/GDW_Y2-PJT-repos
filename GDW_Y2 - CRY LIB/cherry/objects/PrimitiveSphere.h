// PrimitiveSphere
#pragma once
#include "Primitive.h"

namespace cherry
{
	// TODO: create ICOSPHERE
	// class PrimitiveSphere

	typedef class PrimitiveUVSphere : public cherry::Primitive
	{
	public:
		// segRows: the total amount of rows of segments (MUST be greater than 3).
		// segCols: the total amount of columns of segments (MUST be greater than 3).
		// add variable for starting angle and ending angle
		// colour is the base vertex color
		PrimitiveUVSphere(float radius = 1.0F, unsigned int segRows = 10, unsigned int segCols = 10, Vec4 color = Vec4(1.0F, 1.0F, 1.0F, 1.0F));
		// PrimitiveUVSphere(float radius, float segments);

		// returns the radius
		float GetRadius() const;

	private:
		float radius;

	protected:
	} UVSphere;

	//typedef class PrimitiveIcoSphere : public cherry::Primitive
	//{
	//public:
	//private:
	//protected:

	//};

}

