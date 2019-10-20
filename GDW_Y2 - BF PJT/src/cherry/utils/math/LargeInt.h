/*
 * Author: Roderick "R.J." Montague (100701758)
 * Date: 05/02/2019
 * Description: a project that contains various utility functions. Adapted fom Lab 6 of Introduction to Programming
*/

#pragma once
#include <vector>
#include <string>

namespace util
{
	namespace math
	{
		// Question 2: Class Declaration
		class LargeInt
		{
		public:
			LargeInt();

			// Question 3: Data Member Container
			std::vector<int> lnum;

			// Question 4: Member Functions
			LargeInt add(LargeInt); // adds vectors
			LargeInt subtract(LargeInt); // subtracts vectors

			std::string toString(); // returns the vector of this object as a string


		private:
			std::vector<int> minLength(std::vector<int> numA); // removes unneeded 'zeroes' from the vector, which makes it the minimum length needed to represent all values.

		protected:

		};
	}
}

