#pragma once
#include "Primitive.h"

namespace cherry
{
	typedef class PrimitiveCube : public cherry::Primitive
	{
	public:
		// default size of (1, 1, 1)
		PrimitiveCube();

		// sets a single side length for width, height, and depth
		PrimitiveCube(float sideLength);

		// sets the width, height, and depth
		PrimitiveCube(float width, float height, float depth);

		// gets the width
		float GetWidth() const;

		// gets the height
		float GetHeight() const;

		// gets the depth
		float GetDepth() const;


	private:
		float width; // width of box
		float height; // height of box
		float depth; // thickness of box
 	protected:
	} Cube;
}