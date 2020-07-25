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

	// string to int
	int stringToInt(const std::string& str);

	// string to short
	short stringToShort(const std::string& str);

	// string to long
	long stringToLong(const std::string& str);

	// string to double
	double stringToDouble(const std::string& str);

	// string to int
	float stringToFloat(const std::string& str);

	// converts a double to a string
	std::string doubleToString(double value);

	// converts a float to a string
	std::string floatToString(float value);

	// returns a string of size 'LENGTH' with zeros filling in the rest of the string's size.
	// if the length provided is less than the length of the number provided, the number is truncated.
	std::string zeroFill(int num, const unsigned int LENGTH);
	
	// TODO: zero front fill and back fill for decimal numbers.

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
	
	// adds an element to a pointer vector if it isn't already in there. 
	template<typename T>
	bool vectorContainsElement(std::vector<T*>& vector, T* element)
	{
		// goes through each index to see if the item is in it.
		for (int i = 0; i < vector.size(); i++)
		{
			if (vector[i] == element)
				return true;
		}

		return false;
	}
	
	// Share Pointer Version
	// adds an element to a pointer vector if it isn't already in there. 
	template<typename T>
	bool vectorContainsElement(std::vector<std::shared_ptr<T>>& vector, std::shared_ptr<T> element)
	{
		// goes through each index to see if the item is in it.
		for (int i = 0; i < vector.size(); i++)
		{
			if (vector[i] == element)
				return true;
		}

		return false;
	}

	// adds an element to a pointer vector if it isn't already in there. 
	template<typename T>
	bool addToVector(std::vector<T*>& vector, T* val) // Because of the current C++ version, the definition is placed here.
	{
		for (T* item : vector) // if the vector already contains the pointer, it is not added.
		{
			if (item == val)
				return false;
		}

		vector.push_back(val);
		return true;
	}

	// SHARED POINTER VERSION
	// adds an element to a pointer vector if it isn't already in there. 
	template<typename T>
	bool addToVector(std::vector<std::shared_ptr<T>>& vector, std::shared_ptr<T> val) // Because of the current C++ version, the definition is placed here.
	{
		for (std::shared_ptr<T> item : vector) // if the vector already contains the pointer, it is not added.
		{
			if (item == val)
				return false;
		}

		vector.push_back(val);
		return true;
	}
	
	// STANDARD POINTER VERSION
	// removes an element from a vector if it is present.
	template<typename T>
	bool removeFromVector(std::vector<T*>& vector, T* val) // Because of the current C++ version, the definition is placed here.
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

	// SHARED POINTER VERSION
	// removes an element from a vector if it is present.
	template<typename T>
	bool removeFromVector(std::vector<std::shared_ptr<T>>& vector, std::shared_ptr<T> val) // Because of the current C++ version, the definition is placed here.
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

}

#endif // !UTILITIES_H