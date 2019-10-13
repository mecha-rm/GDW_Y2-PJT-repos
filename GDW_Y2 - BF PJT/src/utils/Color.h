#ifndef COLOUR_H
#define COLOUR_H

#include "math/Vector.h"

namespace util
{

	// converts RGB values (out of 255) into decimal form.
	// in decimal form, 255 is considered 1.0, and 0 is considered 0.0;
	util::math::Vec3 RGBIntToDecimal(int r, int g, int b);

	// converts RGB values (0.0F to 1.0F) into int form (out of RGB).
	util::math::Vec3 RGBDecimalToInt(float r, float g, float b);

	// converts RGB values (0.0F to 1.0F) into int form (out of RGB).
	util::math::Vec3 RGBDecimalToInt(util::math::Vec3 colour);

	// converts RGB values (out of 255) plus an alpha value (which ranges from 0 to 1) into decimal form.
	// in decimal form, 255 is considered 1.0, and 0 is considered 0.0;
	util::math::Vec4 RGBAIntToDecimal(int r, int g, int b, float a);

	// converts decimal RGB to int RGB (alpha value not withstanding).
	// The values returned are still floats, but are now whole numbers that range from 0 to 255.
	util::math::Vec4 RGBADecimalToInt(float r, float g, float b, float a);

	// converts decimal RGB to int RGB (alpha value not withstanding).
	// The values returned are still floats, but are now whole numbers that range from 0 to 255.
	util::math::Vec4 RGBADecimalToInt(util::math::Vec4 colour);
	
}

#endif // !COLOUR_H