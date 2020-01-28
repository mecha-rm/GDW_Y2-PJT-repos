// Primitive Cube - creates a quadrilateral object
#pragma once
#include "Primitive.h"

namespace cherry
{
	// PrimitiveCube
	typedef class PrimitiveCube : public cherry::Primitive
	{
	public:
		// default size of (1, 1, 1)
		PrimitiveCube();

		// sets a single side length for width, height, and depth
		// colour ranges from 0 to 1 for RGBA
		PrimitiveCube(float sideLength, cherry::Vec4 color = Vec4(1.0F, 1.0F, 1.0F, 1.0F));

		// sets the width, height, and depth
		PrimitiveCube(float width, float height, float depth, cherry::Vec4 color = Vec4(1.0F, 1.0F, 1.0F, 1.0F));

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