// PrimitiveDiamond
#pragma once
#include "Primitive.h"

namespace cherry
{
	class PrimitiveDiamond : public Primitive
	{
	public:
		// creates a diamond
		// 'origin' determines where the centre of the diamond is. A value of 0.5 puts it halfway between the top and bottom.
		// 0.5F origin puts it perfectly between the top and bottom point. The origin is based on the BOTTOM of the diamond.
		// So < 0.5F puts the origin closer to the bottom, > 0.5F puts the origin closer to the top.
		PrimitiveDiamond(float radius = 0.5F, float height = 1.0F, unsigned int segments = 4, float origin = 0.5F, cherry::Vec4 color = cherry::Vec4(1.0F, 1.0F, 1.0F, 1.0F));

		// gets the radius
		float GetRadius() const;

		// gets the height
		float GetHeight() const;

	private:
		float radius; // radius
		
		float height; // height


	protected:
	};
}

