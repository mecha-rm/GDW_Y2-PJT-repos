#include "Color.h"

// converts RGB from int to decimal.
util::math::Vec3 util::RGBIntToDecimal(int r, int g, int b)
{
	util::math::Vec4 tempVec = RGBAIntToDecimal(r, g, b, 0.0F);
	return util::math::Vec3(tempVec.x, tempVec.y, tempVec.z); // returns the values (excluding the alpha channel)
}

// converts RGB from decimal to int.
util::math::Vec3 util::RGBDecimalToInt(float r, float g, float b)
{
	util::math::Vec4 tempVec = RGBADecimalToInt(r, g, b, 0.0F);
	return util::math::Vec3(tempVec.x, tempVec.y, tempVec.z); // returns the values (excluding the alpha channel)
}

// converts RGB values (0.0F to 1.0F) into int form (out of RGB).
util::math::Vec3 util::RGBDecimalToInt(util::math::Vec3 colour) { return RGBDecimalToInt(colour.x, colour.y, colour.z); }

// converts RGBA int to decimal.
util::math::Vec4 util::RGBAIntToDecimal(int r, int g, int b, float a)
{
	// making sure the colour values are within [0, 255].
	r = (r > 255) ? 255 : (r < 0) ? 0 : r;
	g = (g > 255) ? 255 : (g < 0) ? 0 : g;
	b = (b > 255) ? 255 : (b < 0) ? 0 : b;

	// making sure the alpha value is within [0.0F, 1.0F]
	a = (a > 1.0F) ? 1.0F : (a < 0.0F) ? 0.0F : a;

	return util::math::Vec4((float)r / 255.0F, (float)g / 255.0F, (float)b / 255.0F, a);
}

// converts decimal RGB to int RGB (alpha value not withstanding).
// The values returned are still floats, but are now whole numbers that range from 0 to 255.
util::math::Vec4 util::RGBADecimalToInt(float r, float g, float b, float a)
{
	// making sure the colour values and alpha are less than or equal to 1.0F, as well as greater than or equal to 0.0F.
	r = (r > 1.0F) ? 1.0F : (r < 0.0F) ? 0.0F : r;
	g = (g > 1.0F) ? 1.0F : (g < 0.0F) ? 0.0F : g;
	b = (b > 1.0F) ? 1.0F : (b < 0.0F) ? 0.0F : b;
	a = (a > 1.0F) ? 1.0F : (a < 0.0F) ? 0.0F : a;

	return util::math::Vec4(round(r * 255.0F), round(g * 255.0F), round(b * 255.0F), a); // reutrns hte values.
}

// converts decimal RGB to int RGB (alpha value not withstanding).
util::math::Vec4 util::RGBADecimalToInt(util::math::Vec4 colour) { return RGBADecimalToInt(colour.x, colour.y, colour.z, colour.w); }


