/*
 * Name: Roderick "R.J." Montague
 * Student Number: 100701758
 * Date: 03/23/2019
 * References:
*/

#ifndef ARRAY_H
#define ARRAY_H

#include <iostream>
#include <array>
#include <iterator> // this was included for returning an interator, though it is no longer used.

namespace util
{
	// Question 1: MyArray.h file
	template<typename T>
	class Array
	{
	public:

		// Question 3: constructor - takes the size of the array, and makes the array.
		Array(const unsigned int size) : arr(new T[size]), SIZE(size) {}

		~Array() { delete arr; }

		// Question 2: size() function.
		const unsigned int size() const { return SIZE; }

		T * operator[](const unsigned int index) // Question 2: operator[] function.
		{
			try
			{
				if (index >= SIZE) // throws an error if this is the case.
					throw index;
				else
					return &arr[index];
			}
			catch (...)
			{
				std::cout << "'Out of bounds' exception thrown." << std::endl;
			}
		}


		// Question 2: Begin() Function.
		T * Begin() const { return &arr[0]; }

		// Question 2: End() Function. *edited to use tri-catch block
		T * End() const
		{
			try
			{
				if (SIZE - 1 < 0)
					throw "No elements within array; returning nullptr.";

			}
			catch (const char * ex)
			{
				std::cout << ex << std::endl;
				return nullptr;
			}
			catch (...)
			{
				std::cout << "Error encountered. Returning nullptr.";
				return nullptr;
			}

			return &arr[SIZE - 1];
		}

		

	private:
		T * const arr = nullptr; // Question 3: dynamic array that values are stored in.

		const unsigned int SIZE; // the size of the array.

	protected:
	};
}

#endif // !ARRAY_H