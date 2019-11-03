#pragma once
#include "Primitive.h"

namespace cherry
{
	class PrimitiveCapsule : public cherry::Primitive
	{
	public:
		/*
		 * radius: radius of the body
		 * Height: the height of the cylinder. This includes the caps
		 * segments: the amount of segments that make up the height of the capsule
		 * capSegments: the amount of segments for the caps. These are the horizontal segments. The verticle segmentsare just the cylSegments.
			* MUST BE AN EVEN NUMBER
			* amount of segments per cap

		*/
		PrimitiveCapsule(float radius = 0.5F, float height = 1.0F, unsigned int cylSegments = 8, unsigned int capSements = 8);

		// gets the radius of the capsule.
		float GetRadius() const;

		// gets the height
		float GetHeight() const;

	private:
		float radius;
		float height;

	protected:
	};
}
