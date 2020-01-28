#include "Utils.h"
#include "math/LargeInt.h"
#include "math/Quaternion.h"

#include <cmath>
#include <math.h>
#include <sstream>
#include <fstream>

/// STRING AND GENERAL FUNCTIONS //////////////////////////////////////////////////////
std::string util::toLower(std::string str)
{
	// returns the string if its of length zero.
	if (str.length() == 0)
		return str;

	std::string strL; // temporary string
	strL.assign(str); // copies original string

	for (int i = 0; i < strL.length(); i++)
	{
		strL.at(i) = tolower(strL.at(i)); // converts each 'char' to its lowercase version if applicable
	}

	return strL;
}

// converts a whole string to uppercase
std::string util::toUpper(std::string str)
{
	// returns the string if its of length zero.
	if (str.length() == 0)
		return str;

	std::string strL; // temporary string
	strL.assign(str); // copies original string

	for (int i = 0; i < strL.length(); i++)
	{
		strL.at(i) = toupper(strL.at(i)); // converts each 'char' to its uppercase version if applicable
	}

	return strL;
}

// capitalizes a string.
std::string util::captialize(std::string str)
{
	// returns the string if its of length zero.
	if (str.length() == 0)
		return str;

	std::string strL; // temporary string
	strL.assign(str); // copies original string

	str = toLower(str); // makes the string all lowercase
	str.at(0) = toupper(str.at(0)); // capitalizes the first letter

	return strL;
}

// checks if two strings are equal, ignoring case diffrences.
bool util::equalsIgnoreCase(std::string str1, std::string str2)
{
	// making both strings lowercase
	str1 = toLower(str1);
	str2 = toLower(str2);
	return str1 == str2; // returning whether the two are equal
}

// replaces every instance of a set of characters in a string with another set of characters.
std::string util::replaceSubstring(std::string str, std::string oldSubstr, std::string newSubstr, bool ignoreCase)
{
	unsigned int index; // the index of the most recently found oldSubstr

	// first set is if ignoreCase is false, second set is if ignoreCase is true.
	while ((ignoreCase == false && str.find(oldSubstr) != std::string::npos) || 
		   (ignoreCase == true && util::toLower(str).find(util::toLower(oldSubstr)) != std::string::npos))
	{
		// checks if case sensitivity was requested.
		index = (ignoreCase) ? index = util::toLower(str).find(util::toLower(oldSubstr)) : index = str.find(oldSubstr);

		str.replace(index, oldSubstr.length(), newSubstr); // replaces the old string with the new string.
	}

	return str;
}

// checks if a string is an integer.
bool util::isInt(std::string str)
{
	int strInt; // gets the string as an int.
	std::stringstream ss(str); // creates a string stream for converting the string to an integer.

	ss >> strInt; // puts the string stream into the integer. If non-int values exist, then the int is truncated.

	return std::to_string(strInt) == str; // returns 'true' if all values were carried over from the string.
}

// checks if a string is a decimal number (specifically a double).
bool util::isDecimal(std::string str)
{
	double strDbl; // gets the string as an float
	std::stringstream ss(str); // creates a string stream for converting the string to an integer.

	ss >> strDbl; // puts the string stream into the double. If non-int values exist, then the doubleis truncated.

	return std::to_string(strDbl) == str; // returns 'true' if all values were carried over from the string.
}

// checks to see if a string is a number. IsInt or IsDecimal should be used instead.
bool util::isNum(std::string str)
{
	std::string arr[10] = { "1","2","3","4","5","6","7","8","9","0" };

	// Checks each index of the stirng, checking if it's a whole, numeric value.
	for (int x = 0; x < str.length(); x++) {
		for (int y = 0; y < 10; y++) {
			if (str.substr(x, 1) == arr[y]) {
				break;
			}
			else if (y == 9) {
				return false;
			}
		}
	}
	return true;
}

// returns 'true' if the file can be opened and read from.
bool util::fileAccessible(const std::string& filePath)
{
	std::ifstream file(filePath, std::ios::in); // the file
	bool found;

	// if !file is true, then the file couldn't be opened. If it is false, then the file can be accessed.
	found = !file;
	file.close();

	// returns the opposite as 'found' since it's showing if the file does exist.
	return !found;
}




/// MATH /////////////////////////////////////////////////////////////////////////////
// generates a random integer
int util::randInt(int lBound, int uBound, bool includeUBound)
{
	int tempInt(0);
	int randInt(0);
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

	// If the upperbound is positive, the values can be used as is.
	// If the upperbound and lowerbound are both negative, then a randomizer is run to get their absoluted range, then another randomizer is run to make it positive or negative.

	randInt = (uBound > 0) ? rand() % (uBound - lBound + includeUBound) + lBound : (abs(uBound) - abs(lBound) + includeUBound) + abs(lBound);

	// If the number must be negative, it is multiplied by -1 to make it so.
	if (lBound < 0 && uBound < 0)
		randInt *= -1;
	// randInt = rand() % (uBound - lBound + includeUBound) + lBound;

	return randInt;
}

// returns a random float from float max to float min; not complete
float util::randFloat() { return (float)randDecimal(std::to_string(std::numeric_limits<float>::max()).length()); }

// returns a random float. If a 'true' is used, it is up-to and including the upper bound.
// 1.7E-38 to 1.7E+38
float util::randFloat(unsigned int dPlaces) { return (float)randDecimal(dPlaces); }

// returns a random double
double util::randDouble() { return randDecimal(std::to_string(std::numeric_limits<double>::max()).length()); }

// returns a random double. If 'true' is used for includeUBound, the upper bound is included.
// 1.7E-308 to 1.7E+308
double util::randDouble(unsigned int dPlaces) { return randDecimal(dPlaces); }

// returns a random decimal
double util::randDecimal(unsigned int dPlaces)
{
	// minimum six floating points.
	// maximum value (Float): 340282346638528859811704183484516925440.000000
	std::string str = ""; // gets the largeInt as a string
	math::LargeInt lg; // used to help generat the random float

	double dbl(0.0F); // saves and returns the double.

	if (dPlaces == 0) // if there are no decimal places requested, then a 0 is returned.
		return 0;

	// if (dPlaces > digits_max + abs(digits_min)) // if the digits requested go beyond the limit, it's truncated
		// dPlaces = digits_max + abs(digits_min);

	for (int i = 0; i < dPlaces; i++) // generates a number from 0 to 9, and inserts it into the large int.
		lg.lnum.push_back(rand() % 10);

	str = lg.toString(); // gets the large int as a string.

	if (str.length() == 0) // if the string is of length 0, then a 0 is returned.
		return 0.0F;

	str.at(rand() % (str.length() - 1) + 1) = '.'; // inserts a decimal point
	dbl = std::stof(str); // gets the string as a float

	return dbl;
}

// Returns a random ASCII char
char util::randAsciiChar() { return (char)(rand() % 256); }

