#pragma once
#include "Primitive.h"

namespace cherry
{
	class PrimitiveCone : public Primitive
	{
	public:
		// creates a cone.
		PrimitiveCone(float radius = 0.5F, float height = 1.0F, unsigned int segments = 10);
		
		// gets the radius
		float GetRadius() const;

		// gets the height
		float GetHeight() const;
	
	private:
		float radius;
		float height;

	protected:
	};
}
