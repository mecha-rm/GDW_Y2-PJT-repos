// class used for construction of level map
#pragma once
#include <string>
#include <vector>
#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>

// Basically we're reconstructing the CSV into an object.
class CSV 
{
public:
	// constructor
	CSV() = default;

	// constructor
	CSV(std::string csvPath);

	// get row 
	std::string GetRow(int num, int num2);

	std::vector<std::string> const& operator[](std::size_t index) const;
	
	// size of CSV
	std::size_t Size() const;

private:
	// rows[y][x]
	std::vector<std::vector<std::string>> rows;
};