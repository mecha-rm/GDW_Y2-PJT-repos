// Miscellaneous utility functions
#ifndef UTILITIES_H
#define UTILITIES_H

#include "math/Matrix.h"
#include <string>
#include <sstream>

namespace util
{
	// Standard Utilities
	// converts a whole string to lowercase
	std::string toLower(std::string);

	// converts a whole string to uppercase
	std::string toUpper(std::string);

	// capitalizes a string, making all but the first character lowercase.
	std::string captialize(std::string);

	// checks if two strings are equal, ignoring case diffrences.
	bool equalsIgnoreCase(std::string, std::string);

	// replaces every instance of a set of characters in a string with another set of characters.
	// oldSubstr: the string segment that's being removed
	// newSubstr: the string that will be put in its place.
	// ignoreCase: if false, then the function is case sensitive. If true, then the function ignores cases when looking for oldSubstr.
	std::string replaceSubstring(std::string str, std::string oldSubstr, std::string newSubstr, bool ignoreCase = false);

	// splits the string into a vector of the provided data type using spaces.
	template<typename T>
	const std::vector<T> splitString(std::string str)
	{
		std::stringstream ss; // the string stream.
		std::vector<T> vec; // the vector used for the vertex.
		T var; // used to store the item from the string.


		// if the string is of length 0, then an empty vector is returned.
		if (str.length() == 0)
			return std::vector<T>();

		ss.str(str); // stores the string in the stream

		while (ss >> var) // while the string stream isn't empty
		{
			// if the conversion failed, the string stream moves onto the next item.
			if (ss.bad())
				continue;

			vec.push_back(var); // saves in the vector
		}

		return vec; // returns the vector
	}


	// checks to see if a string is an integer; this is improved from isNum.
	bool isInt(std::string str);

	// checks to see if a string is a decimal, which would be a float or double. However, it is validated under the assumption it will become a double.
	bool isDecimal(std::string str);

	// checks if a string is a number. IsInt or IsDecimal should be used instead.
	bool isNum(std::string str);

	// convets the string to the provided data type. Do note that the value might be truncated if the conversion doesn't make logical sense.
	template<typename T>
	T convertString(const std::string & str)
	{
		// TODO: do error checking for this function.
		std::stringstream ss; // the string stream.
		T val; // used to store the item from the string.

		ss.str(str); // stores the string in the stream

		ss >> val;

		return val;
	}

	// returns 'true' if a file can be opened successsfully, and false if it cannot.
	bool fileAccessible(const std::string & filePath);

	// returns the minimum between two values.
	// the less than operator (<) must be overloaded for this function to work.
	template<typename T>
	T min(T val1, T val2) { return (val1 < val2) ? val1 : val2; }

	// returns the maximum between two values.
	// the greater than operator (>) must be overloaded for this function to work.
	template<typename T>
	T max(T val1, T val2) { return (val1 > val2) ? val1 : val2; }

	// caps 'val' using lowerBound and upperBound. The (<) and (>) operators must be overloaded for this function. 
	template<typename T>
	T clamp(T val, T lowerBound, T upperBound) { return (val < lowerBound) ? lowerBound : (val > upperBound) ? upperBound : val; };

	template<typename T>
	bool addToVector(std::vector<T*>& vector, T* val) // adds an element to a pointer vector if it isn't already in there. Because this is a template, the definition is placed here.
	{
		for (T* item : vector) // if the vector already contains the pointer, it is not added.
		{
			if (item == val)
				return false;
		}

		vector.push_back(val);
		return true;
	}


	template<typename T>
	bool removeFromVector(std::vector<T*>& vector, T* val) // removes an element from a vector if it is present. This is placed in the header because it is a template function.
	{
		for (int i = 0; i < vector.size(); i++)
		{
			if (vector.at(i) == val) // if the pointer has been found, it is removed.
			{
				vector.erase(vector.begin() + i);
				return true;
			}
		}

		return false;
	}


	// Mathematic Utilities
	// http://www.cplusplus.com/reference/random/
	// http://www.cplusplus.com/reference/limits/numeric_limits/
	// Returns a random number starting form 'lbound' upto ubound.
	// If 'includeUbound' is set to 'true', it's upto and including ubound; if false, it's upto but not including ubound. False by default.
	int randInt(int lBound, int uBound, bool includeUBound = false);

	// Returns a random float from 1.7E-308 to 1.7E+308
	float randFloat();

	// returns a random float. If a 'true' is used, it is up-to and including the upper bound. Not complete.
	// 1.7E-38 to 1.7E+38
	// float randFloat(unsigned int dPlaces, float lBound, float uBound, bool includeUBound = false);
	float randFloat(unsigned int dPlaces);

	// returns a random double
	double randDouble();

	// returns a random double. If 'true' is used for includeUBound, the upper bound is included. Not complete.
	// 1.7E-308 to 1.7E+308
	double randDouble(unsigned int dPlaces);

	double randDecimal(unsigned int dPlaces);

	// returns a random ASCII character. This can return 256 characters, which includes character '0', which is 'NULL'.
	char randAsciiChar();
}

#endif // !UTILITIES_H