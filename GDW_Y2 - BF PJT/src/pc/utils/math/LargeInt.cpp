#include "LargeInt.h"

util::math::LargeInt::LargeInt()
{
}

std::vector<int> util::math::LargeInt::minLength(std::vector<int> numA) // removes unneeded '0s' at the start of the vector that holds the number.
{
	std::vector <int> numB;
	int start = 0;

	for (int i = 0; numA.at(i) == 0 && i < numA.size(); i++) // finds where the number begins
		start++;

	for (int i = start; i < numA.size(); i++)
	{
		numB.push_back(numA.at(i));
	}

	return numB;
}

util::math::LargeInt util::math::LargeInt::add(LargeInt lInt) // Question 4: Member Function (Addition)
{
	// For adding together two vectors, the adding in columns method is used.
	// To explain it swiftly, each number that's in the same column is added together to yield what number will be in that same column in the end result of the whole calculation.
	// If the addition of all numbers in a column goes above 9, then what's in the '1s' column is placed in the aforementioned column in result.
	// Whatever's in the '10s' column is added to what the result of the following column (i.e. the column to the left) will be.
	// See MathisFun.com/"Addition in Columns" in my references section if my explanation wasn't clear enough.

	std::vector <int> num1 = lnum; // gets the values of this object's vector.
	std::vector <int> num2 = lInt.lnum; // used to get the vector from the passed object.
	std::vector <int> numsum; // the sum of the two vectors (num1 and num2)

	short int val1(0); // the value gotten from num1
	short int val2(0); // the value gotten from num2
	short int rem(0); // the remaining value that's added to the following column (i.e. the column to the left) if the current column's numbers surpass 9 when added together

	long unsigned int digits; // the total number of digits that the addition of the two vectors could have.
	(num1.size() > num2.size()) ? digits = num1.size() + 1: digits = num2.size() + 1; // the amount of digits needed will be either the length of the longest number, or one more than that.
	
	numsum = std::vector<int>(digits); // makes numsum have the amount of indexes necessary to hold the end result.

	for (int i = numsum.size() - 1; i >= 0; i--) // starting from the end of numsum so that the values are put in the proper order.
	{
		if (!num1.empty()) // gets the value at the top of the num1 stack, and deletes it from num1.
		{
			val1 = num1.back();
			num1.pop_back();
		}
		if (!num2.empty()) // gets the value on top of the num2 stack, and deletes it from num2.
		{
			val2 = num2.back();
			num2.pop_back();
		}
		
		numsum.at(i) = val1 + val2 + rem; // adds together the two values, and the remainder from the previous calculation.
		rem = 0; // sets remainder back to '0', since its value has been used.

		if (numsum.at(i) > 9) // only one digit should be stored in that index, so if the index's value surpasses '9', the other digit is added to the following index (i.e. the index to the left)
		{
			// the digit will not surpass '20' ('18' to be more specific), since we only adding together 2 vectors.
			numsum.at(i) = numsum.at(i) - 10;
			rem = 1;
		}

		// resetting varaibles
		val1 = 0;
		val2 = 0;
	}
	
	lnum = minLength(numsum); // removing unneeded zeroes.
	return *this;
}

util::math::LargeInt util::math::LargeInt::subtract(LargeInt lInt) // Question 4: Subtraction Function
{
	// This function returns '0' if the number would be negative.

	// This function is modeled after the subtraction regrouping method to do its calculations.
	// Subtraction works in a similar matter to addition, being that you subtract each number in the same column from one another.
	// If the calculation goes below '0', then you need to borrow a '1' from the leftward column in order to bring up the value in the current column by '10'.
	// See MathisFun.com/"Subtraction by Grouping" in my references section if the way I explainded it wasn't clear enough.

	std::vector <int> num1 = lnum; // gets the values of this object's vector (lnum).
	std::vector <int> num2 = lInt.lnum; // used to get the vector from the passed object (lInt)
	std::vector <int> numsum; // saves the difference of the two vectors (result)

	short int val1(0); // the value gotten from num1
	short int val2(0); // the value gotten from num2
	int endIndex(0); // gets the index of the last value in num1
	int borrowIndex(-1); // saves the index of the number that's having a '1' taken from it to add to the number in the rightward column.

	(num1.size() > num2.size()) ? numsum = std::vector<int>(num1.size()) : numsum = std::vector<int>(num2.size()); // setting numsum's size based on the maximum amount of digits it could possibly need to hold.

	if (std::stoi(lInt.toString()) >= std::stoi(this->toString())) // if the value of num2's vector is greater than the value of num1's vector, then a '0' is set as lnum's value since the calculation would result in a negative number.
	{
		lnum = std::vector <int> (1, 0);
		return *this;
	}

	// If the conditions checked above yielded a false boolean, then the end result must be positive (i.e. >= 0)
	for (int i = numsum.size() - 1; i >= 0; i--) // subtracting the values, starting from the last digit.
	{ 
		if (!num1.empty()) // gets the last value in the num1 vector, and deletes it.
		{
			val1 = num1.back();
			num1.pop_back();
		}
		if (!num2.empty()) // gets the last value in the num2 vector, and deletes it.
		{
			val2 = num2.back();
			num2.pop_back();
		}

		if (val2 > val1) // if the value being subtracted from is less than what it's been subtracted by (i.e. if the calculation would result in a negative number), a borrowing of values occurs.
		{
			num1.push_back(val1); // pushes the value back onto the num1 stack, since it needs to be changed.
			endIndex = num1.size() - 1; // gets the index of the last value in num1.
			
			if (endIndex > 0) // an out of bounds exception shouldn't be thrown regardless, but this is just a safeguard to make sure that it doesn't happen.
			{
				if (num1.at(endIndex - 1) > 0) // if the number in the following column (i.e. leftward column) is greater than '0', it is subtracted by '1', and '10' is added to the number in the current column.
				{
					num1.at(endIndex - 1)--; // subtracts from the number in the column to the left
					num1.at(endIndex) += 10; // adding '10' to the value stored at this location since it's about to be popped off

					val1 = num1.back(); // saves the new value to val1
					num1.pop_back(); // removes the value from the num1 stack once again
				}
				else 
				{
					// if the number directly to the left of the current number (i.e. endIndex - 1) is '0', then this number needs to have '10' added to it first in order for a '1' to be borrowed from it.
					// To do this, the '10' must come from the value next to this number (endIndex - 2).

					borrowIndex = -1;

					// keeps going until a column that has a borrowable value is found.
					for (int j = endIndex - 1; j >= 0; j--)
					{
						if (num1.at(j) > 0) // if a column that can have its value borrowed has been found (i.e. the column's value can be subtracted by '1' without being taken below 0), its index is saved.
						{
							borrowIndex = j;
							break;
						}
					}

					if (borrowIndex == -1) // if no number that can be decreased by '1' without being brought below '0' is found, then a '0' is returned as the result of the whole calculation.
					{
						lnum = std::vector <int>(1, 0);
						return *this;
					}

					while (borrowIndex < endIndex) // loop continues until borrowIndex is back to the last index of num1. 
					{
						// subtracts '1' from the current index, and adds '10' to the value in the following index (i.e. rightward column).
						num1.at(borrowIndex)--;
						num1.at(borrowIndex + 1) += 10;
						borrowIndex++;
					}

					val1 = num1.back(); // giving 'val1' the new value.
					num1.pop_back(); // removing that value from the num1 vector.
				}
			}
		}

		numsum.at(i) = val1 - val2; // gets the difference between the two values, and puts them in numsum.
		
		// resetting variables
		val1 = 0;
		val2 = 0;
	}

	lnum = minLength(numsum); // removing unneeded zeroes.
	return *this;
}

std::string util::math::LargeInt::toString() // returning the vector stored in this object as a string
{
	std::string vecstr = "";
	for (int i = 0; i < lnum.size(); i++)
		vecstr += std::to_string(lnum.at(i));

	return vecstr;
}