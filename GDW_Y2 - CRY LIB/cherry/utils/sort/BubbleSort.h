/*
 * Date: 04/24/2020
 * Description: bubble sort.
	- bubble sort works by going through the whole list and swapping two adjacent values if they are out of order.
	- when a pass is made where no values are moved, then the list is sorted.
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
		// sorts the vector.
		template<typename T, typename A>
		void bubbleSort(std::vector<T, A>& list)
		{
			// the size of the array.
			const int SIZE = list.size();

			// sorts through the list.
			for (int iter = 0; iter < SIZE - 1; iter++)
			{
				// if 'sorted' becomes false, then that means some values were moved.
				// if that's the case, then the bubble sort can't end early.
				bool sorted = true;

				// sorts the elements.
				for (int i = SIZE - 1; i > iter; i--)
				{
					if (list[i] < list[i - 1])
					{
						T temp = list[i];
						
						list[i] = list[i - 1];
						list[i - 1] = temp;

						sorted = false;
					}
				}

				// the list is sorted, so the bubble sort breaks early.
				if (sorted)
					break;
			}

			return;
		}

		// sorts the array. Variable 'SIZE' is the size of the array (i.e. the number of elements).
		template<typename T>
		void bubbleSort(T * list, const int SIZE)
		{
			// if the list is empty.
			if (list == nullptr)
				return;

			// sorts through the list.
			for (int iter = 0; iter < SIZE - 1; iter++)
			{
				// if 'sorted' becomes false, then that means some values were moved.
				// if that's the case, then the bubble sort can't end early.
				bool sorted = true;

				// sorts the elements.
				for (int i = SIZE - 1; i > iter; i--)
				{
					if (list[i] < list[i - 1])
					{
						T temp = list[i];

						list[i] = list[i - 1];
						list[i - 1] = temp;

						sorted = false;
					}
				}

				// the list is sorted, so the bubble sort breaks early.
				if (sorted)
					break;
			}
		}


	}
}