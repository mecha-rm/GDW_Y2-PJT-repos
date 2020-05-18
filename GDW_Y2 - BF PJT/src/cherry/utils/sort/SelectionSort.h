/*
 * Date: 04/24/2020
 * Description: selection sort.
	- selection sort has the program grab the value at the start of the unsorted list.
	- it then searches the unsorted list for the smallest value.
	- it then swaps the selected value with the smallest value in the list.
	- this keeps going until there are no more unsorted items.
	- this is modelled after the code framework provided by the professor of Algorithms and Data Structures.
 * Resources:
	- https://en.cppreference.com/w/cpp/language/function_template
*/

#pragma once

#include <vector>
#include <array>

namespace util
{
	namespace sort
	{
		// vector version
		template<typename T, typename A>
		void selectionSort(std::vector<T, A>& list)
		{
			// the size of the list.
			const int SIZE = list.size();

			// the iteration of the list.
			for (int iter = 0; iter < SIZE - 1; iter++) 
			{
				// gets the smallest index.
				int smallestIndex = iter;

				// goes through each index, finding hte smallest one.
				for (int i = iter + 1; i < SIZE; i++)
				{
					if (list[i] < list[smallestIndex])
						smallestIndex = i;
				}

				// swaps values.
				if (iter != smallestIndex)
				{
					T temp = list[iter];

					list[iter] = list[smallestIndex];
					list[smallestIndex] = temp;
				}
			}
		}

		// array version
		template<typename T>
		void selectionSort(T* list, const int SIZE)
		{
			// the iteration of the list.
			for (int iter = 0; iter < SIZE - 1; iter++)
			{
				// gets the smallest index.
				int smallestIndex = iter;

				// goes through each index, finding hte smallest one.
				for (int i = iter + 1; i < SIZE; i++)
				{
					if (list[i] < list[smallestIndex])
						smallestIndex = i;
				}

				// swaps values.
				if (iter != smallestIndex)
				{
					T temp = list[iter];

					list[iter] = list[smallestIndex];
					list[smallestIndex] = temp;
				}
			}
		}
	}
}