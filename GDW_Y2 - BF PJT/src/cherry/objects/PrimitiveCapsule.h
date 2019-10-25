#pragma once
#include "Primitive.h"

namespace cherry
{
	class PrimitiveCapsule : public cherry::Primitive
	{
	public:
		/*
		 * cylRadius: the radius of the cylinder
		 * Height: the height of the cylinder. This does NOT include the caps.
		 * segments: the amount of segments that make up the height of the capsule
		 * capRadius: the radius of the cap
		 * capSegments: the amount of segments for the caps. These are the horizontal segments. The verticle segmentsare just the cylSegments.
			* MUST BE AN EVEN NUMBER
			* amount of segments per cap

		*/
		PrimitiveCapsule(float cylRadius, float height, unsigned int cylSegments, float capRadius, unsigned int capSements);
	private:
	protected:
	};
}
