/*
 * Date: 05/02/2020
 * Description: generates random numbers.
 * References:
	* http://www.cplusplus.com/reference/cstdlib/rand/
	* http://www.cplusplus.com/reference/cmath/trunc/
	* http://www.cplusplus.com/reference/string/string/substr/
	* https://en.cppreference.com/w/cpp/language/types
	* http://www.cplusplus.com/reference/iomanip/setprecision/
	* https://stackoverflow.com/questions/16605967/set-precision-of-stdto-string-when-converting-floating-point-values
	* https://docs.microsoft.com/en-us/cpp/c-language/cpp-integer-limits?view=vs-2019
 */

// randomizer functions
#include "Random.h"

#include <random>
#include <string>
#include "math/LargeInt.h"
#include "Utils.h"

// returns a random integer.
int util::rand::randInt() { return std::rand(); }

// random integer from lower bound to upper bound (does not include upper bound).
int util::rand::randInt(int lBound, int uBound)
{
	return randInt(lBound, uBound, false);
}

// generates a random integer
int util::rand::randInt(int lBound, int uBound, const bool includeUBound)
{
	int randInt(0); // random intger
	int tempInt(0); // temporary value.

	// If the upper and lower bounds on the same, it returns the lBound
	if (lBound == uBound)
	{
		return lBound;
	}
	// If the lower bound is greater than the upper bound, they are swapped
	else if (lBound > uBound)
	{
		tempInt = lBound;
		lBound = uBound;
		uBound = tempInt;
	}

	// gets the difference between the lower bound and upper bound.
	// it then adds the result to the lower bound so that the offset is relative to it.
	tempInt = (includeUBound) ? (abs(uBound - lBound) + 1) : abs(uBound - lBound);
	
	randInt = std::rand() % tempInt;
	
	// offset based on lower bound.
	randInt += lBound;

	// return random integer.
	return randInt;
}

// generates a random decimal value 
double util::rand::randDecimal()
{
	return randDecimal(6);
}

// returns a random decimal.
double util::rand::randDecimal(int nPlaces)
{
	// random number based on the highest value a double can represent.
	double val = std::rand() % std::numeric_limits<int>::max();

	// generates a random number with 6 decimal points
	for (int i = 1; i <= nPlaces; i++)
	{
		val = val + ((double)(std::rand() % 9) / pow(10.0, i));
	}

	return val;
}

// generates a random decimal from the lower bound up to but not including the upper bound.
double util::rand::randDecimal(double lBound, double uBound)
{
	return randDecimal(lBound, uBound, false);
}

// generates a random double.
double util::rand::randDecimal(double lBound, double uBound, const bool includeUBound)
{
	// saves the amount of decimal places, which is based on how many decimal places the provided numbers have.
	int nPlaces = 0;

	// converts the numbers to strings
	// std::string lBoundStr = std::to_string(lBound), uBoundStr = std::to_string(uBound);
	std::string lBoundStr = util::doubleToString(lBound), uBoundStr = util::doubleToString(uBound);

	// this defaults to 6 digits of double percision, regardless of how many there are for lb and ub.
	// to fix this, the doubleToString function part of the util namespace is used.

	// finds the decimal place, and counts the amount of digits after it.
	// it is saved for the lower bound and upper bound.
	int lnPlaces = 0, unPlaces = 0;
	int index = 0;

	// if there is a decimal point (i.e. the value is not a whole number)
	if(lBoundStr.find(".") != std::string::npos)
		lnPlaces = lBoundStr.substr(lBoundStr.find(".")).length() - 1;
	
	// if there is a decimal point (i.e. the value is not a whole number)
	if (uBoundStr.find(".") != std::string::npos)
		unPlaces = uBoundStr.substr(uBoundStr.find(".")).length() - 1;
	
	// gets the highest of the two values
	nPlaces = std::max(lnPlaces, unPlaces);

	return randDecimal(lBound, uBound, nPlaces, includeUBound);
}

// creates a random decimal with (n) amount of decimal places. Does not include upper bound.
double util::rand::randDecimal(double lBound, double uBound, const unsigned int nPlaces)
{
	return randDecimal(lBound, uBound, nPlaces, false);
}

// rand decimal with varying decimal places
double util::rand::randDecimal(double lBound, double uBound, const unsigned int nPlaces, bool const includeUBound)
{
	// due to the way this is designed, it's weighted towards the lower end of the randomization bounds.
	// if the values are the same.
	if (lBound == uBound)
		return uBound;

	// the result of the randomization.
	double result = 0.0;

	// L for 'lower', and U for 'upper' (in terms of lower bound and upper bound
	// the pre decimal seperator values (whole numbers)
	double prePntL = trunc(lBound), prePntU = trunc(uBound);

	// the post decimal sperator values (fraction numbers)
	double postPntL = lBound - prePntL, postPntU = uBound - prePntU;

	// basically it multiplies both numbers by some multiple of 10 in order to move all decimal values to the left.
	// a regular rand() operation is done to generate a random value from this result.
	// after that, it divides the result by the number of number places shifted over so that it's back within the bounds.
	// as such, the amount of decimal places is limited to the amount for the provided numbers.

	// randomization
	{
		// the multiple of 10 being applied.
		int tenMult = pow(10, nPlaces);

		// the lower bound and upper bound multiplied by the ten multiple.
		int lbX = lBound * tenMult;
		int ubX = uBound * tenMult;

		// makes them whole numbers and generates a random value.
		result = randInt(lbX, ubX, includeUBound);

		// puts back within the bounds of the decimal numbers.
		result /= tenMult;
	}

	// returns the result.
	return result;
}

// returns a random double.
double util::rand::randDouble() { return randDecimal(); }

// generates a random double.
double util::rand::randDouble(const unsigned int nPlaces)
{
	return randDecimal(nPlaces);
}

// generates random double value [lBound, uBound)
double util::rand::randDouble(double lBound, double uBound)
{
	return randDecimal(lBound, uBound);
}

// generates a random double between lBound and uBound. If includeUBound is true, the upperbound is included.
double util::rand::randDouble(double lBound, double uBound, bool const includeUBound)
{
	return randDecimal(lBound, uBound, includeUBound);
}

// random double with a given amount of decimal places. The upperbounds are not included.
double util::rand::randDouble(double lBound, double uBound, const unsigned int nPlaces)
{
	return randDecimal(lBound, uBound, nPlaces);
}

// generates a random double with (n) amount of decimal places within the bounds. Optional to include the upper bound.
double util::rand::randDouble(double lBound, double uBound, const unsigned int nPlaces, bool const includeUBound)
{
	return randDecimal(lBound, uBound, nPlaces, includeUBound);
}

// returns a random float.
float util::rand::randFloat() 
{ 
	return (float)randDecimal();
}

// generates a random float.
float util::rand::randFloat(int nPlaces)
{
	return (float)randDecimal(nPlaces);
}

// random float number [lBound, uBound)
float util::rand::randFloat(double lBound, double uBound)
{
	return (float)randDecimal(lBound, uBound);
}

// random float; [lBound, uBound] or [lBound, uBound) depending on includeUBound.
float util::rand::randFloat(double lBound, double uBound, bool const includeUBound)
{
	return (float)randDecimal(lBound, uBound, includeUBound);
}

// generates a random float [lBound, uBound) with nPlaces.
float util::rand::randFloat(double lBound, double uBound, const unsigned int nPlaces)
{
	return (float)randDecimal(lBound, uBound, nPlaces);
}

// generates a random float; [lBound, uBound) or [lBound, uBound] based on includeUBound.
// nPlaces determines how many decimal places are used.
float util::rand::randFloat(double lBound, double uBound, const unsigned int nPlaces, bool const includeUBound)
{
	return (float)randDecimal(lBound, uBound, nPlaces, includeUBound);
}

// Returns a random ASCII char
char util::rand::randAsciiChar() { return (char)(std::rand() % 256); }
