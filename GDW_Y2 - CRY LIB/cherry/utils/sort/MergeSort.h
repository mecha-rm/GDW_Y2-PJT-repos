/*
 * Date: 05/18/2020
 * Description: merge sort
	* splits the arrays into individual pieces, then merges the pieces back together in the right order.
 * Resources:
	* http://www.cplusplus.com/reference/vector/vector/insert/
	* http://www.cplusplus.com/reference/vector/vector/
*/
#pragma once

#include <queue>

// TODO: the merge sort runs but it doesn't actually change the list.
namespace util
{

	namespace sort
	{
		// FORWARD DECLARES //

		// merges the two sections of the vector list.
		template<typename T, typename A>
		void merge(std::vector<T, A>&, int, int);

		// merges the two sections of the array list.
		template<typename T>
		void merge(T* list, int, int);

		template<typename T, typename A>
		void mergeSort(std::vector<T, A>&);

		template<typename T, typename A>
		void mergeSort(std::vector<T, A>&, int, int);

		template<typename T>
		void mergeSort(T* list, const unsigned int SIZE);

		template<typename T>
		void mergeSort(T*, int, int);

		// FUNCTIONS //
		
		// merge sort (vector)
		template<typename T, typename A>
		void merge(std::vector<T, A>& list, int first, int last)
		{
			// finds the mid point
			int mid = (first + last) / 2;

			// if the list is out of order, so the two lists need to swap places.
			// [mid + 1, last] + [first, mid]
			if (list[mid] > list[mid + 1])
			{
				// queue used to merge hte lists.
				std::queue<T> q;

				// pushes the values for the second half of the list onto the queue.
				for (int i = mid + 1; i <= last; i++)
				{
					q.push(list[i]);
				}

				// pushes the values for the second half of the list onto the queue.
				for (int i = first; i <= mid; i++)
				{
					q.push(list[i]);
				}

				// puts the values into the vector.
				int index = first;
				while (!q.empty()) // while there are still values in the queue.
				{
					list[index] = q.front();
					q.pop();
				}
			}
			else // if the list is already in order, then nothing needs to happen.
			{
				return;
			}

		}

		// merge sort (array)
		template<typename T>
		void merge(T* list, int first, int last)
		{
			// finds the mid point
			int mid = (first + last) / 2;

			// if the list is out of order, so the two lists need to swap places.
			// [mid + 1, last] + [first, mid]
			if (list[mid] > list[mid + 1])
			{
				// queue used to merge hte lists.
				std::queue<T> q;

				// pushes the values for the second half of the list onto the queue.
				for (int i = mid + 1; i <= last; i++)
				{
					q.push(list[i]);
				}

				// pushes the values for the second half of the list onto the queue.
				for (int i = first; i <= mid; i++)
				{
					q.push(list[i]);
				}

				// puts the values into the vector.
				int index = first;
				while (!q.empty()) // while there are still values in the queue.
				{
					list[index] = q.front();
					q.pop();
				}
			}
			else // if the list is already in order, then nothing needs to happen.
			{
				return;
			}

		}

		// merge sort (vector)
		template<typename T, typename A>
		void mergeSort(std::vector<T, A>& list)
		{
			mergeSort(list, 0, list.size() - 1);
		}


		// vector version - mergeSort(list, starting index, ending index)
		template<typename T, typename A>
		void mergeSort(std::vector<T, A>& list, int first, int last)
		{
			// if (first != last)
			if (list[first] < list[last])
			{
				int mid = (first + last) / 2;

				mergeSort(list, first, mid);
				mergeSort(list, mid + 1, last);
				merge(list, first, last);
			}
		}

		// merge sort - array version
		template<typename T>
		void mergeSort(T* list, const unsigned int SIZE)
		{
			mergeSort(list, 0, SIZE - 1);
		}

		// merge sort - array version - mergeSort(list, starting index, ending index)
		template<typename T>
		void mergeSort(T* list, int first, int last)
		{
			// if (first != last)
			if (list[first] < list[last])
			{
				int mid = (first + last) / 2;

				mergeSort(list, first, mid);
				mergeSort(list, mid + 1, last);
				merge(list, first, last);
			}
		}

	}
}