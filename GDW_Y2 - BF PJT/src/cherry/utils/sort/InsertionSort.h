/*
 * Date: 04/24/2020
 * Description: insertion sort.
	- takes a value from the unsorted list, and puts it in its proper place in the sorted list.
	- this is modelled after the code framework provided by the professor of Algorithms and Data Structures.
 * Resources:
	- https://en.cppreference.com/w/cpp/language/function_template
*/

#pragma once

namespace util
{
	namespace sort
	{
		// TODO: expand on comments regarding code.

		// vector version
		template<typename T, typename A>
		void insertionSort(std::vector<T, A>& list)
		{
			// the size of the list.
			const int SIZE = list.size();

			// iterates through the sorted list.
			for (int i = 1, j; i < SIZE; i++)
			{
				T temp = list[i];
				
				for (j = i; j > 0 && temp < list[j - 1]; j--)
				{
					list[j] = list[j - 1];
				}

				list[j] = temp;
			}
		}

		// vector version
		template<typename T>
		void insertionSort(T* list, const int SIZE)
		{
			// iterates through the sorted list.
			for (int i = 1, j; i < SIZE; i++)
			{
				T temp = list[i];

				for (j = i; j > 0 && temp < list[j - 1]; j--)
				{
					list[j] = list[j - 1];
				}

				list[j] = temp;
			}
		}

	}
}