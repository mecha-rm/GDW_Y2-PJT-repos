/*
* Date: 05/03/2020
* Description:
* References:
	* http://www.cplusplus.com/reference/random/
	* http://www.cplusplus.com/reference/limits/numeric_limits/
*/
#pragma once

namespace util
{
	namespace rand
	{
		// random intenger. Same as calling the standard rand() function.
		int randInt();

		// Returns a random number starting from 'lbound' upto but not including ubound.
		int randInt(int lBound, int uBound);

		// Returns a random number starting from 'lbound' upto ubound.
		// If 'includeUbound' is set to 'true', it's upto and including ubound; if false, it's upto but not including ubound. False by default.
		int randInt(int lBound, int uBound, const bool includeUBound);

		// generates a random decimal number with 6 decimal places.
		double randDecimal();

		// returns a random decimal with (n) decimal places.
		double randDecimal(int nPlaces);

		// generates a random decimal number from the lowerbound up-to but not including the upperbound
		double randDecimal(double lBound, double uBound);

		// generates a random decimal number from the lowerbound to the upperbound
		// if includeUBound is true, then the upper bound is included.
		double randDecimal(double lBound, double uBound, bool const includeUBound);

		// generates a random decimal with a lower bound, upper bound. The amount of decimal places used is based on the provided input.
		double randDecimal(double lBound, double uBound, const unsigned int nPlaces);

		// generates a random decimal with a lower bound, upper bound, and an amount of decimal places.
		double randDecimal(double lBound, double uBound, const unsigned int nPlaces, bool const includeUBound);



		// returns a random double from 0 to the maximum value an integer can represent, with 6 decimal point values.
		double randDouble();

		// random double with (n) deciaml places.
		double randDouble(const unsigned int nPlaces);

		// generates a random double number from the lowerbound up-to but not including the upperbound
		// same as randDecimal() function with the same parameters.
		double randDouble(double lBound, double uBound);

		// generates a random double number from the lowerbound to the upperbound.
		// same as randDecimal() function with the same parameters.
		double randDouble(double lBound, double uBound, bool const includeUBound);

		// generates a random double with a lower bound, upper bound. The amount of decimal places used is based on the provided input.
		// this is the same as randDecimal() function with the same parameters.
		double randDouble(double lBound, double uBound, const unsigned int nPlaces);

		// generates a random double with a lower bound, upper bound, and an amount of decimal places.
		double randDouble(double lBound, double uBound, const unsigned int nPlaces, bool const includeUBound);



		// Returns a random float from 1.7E-308 to 1.7E+308 (the maixmum value a float can represent).
		float randFloat();

		// returns a random float with (n) decimal places.
		float randFloat(int nPlaces);

		// generates a random float number from the lowerbound up-to but not including the upperbound
		// same as randDecimal() function with the same parameters.
		float randFloat(double lBound, double uBound);

		// generates a random float number from the lowerbound to the upperbound.
		// same as randDecimal() function with the same parameters.
		float randFloat(double lBound, double uBound, bool const includeUBound);

		// generates a random float with a lower bound, upper bound. The amount of decimal places used is based on the provided input.
		// this is the same as randDecimal() function with the same parameters.
		float randFloat(double lBound, double uBound, const unsigned int nPlaces);

		// generates a random decimal with a lower bound, upper bound, and an amount of decimal places.
		float randFloat(double lBound, double uBound, const unsigned int nPlaces, bool const includeUBound);

		// returns a random ASCII character. This can return 256 characters, which includes character '0', which is 'NULL'.
		char randAsciiChar();
	}
}