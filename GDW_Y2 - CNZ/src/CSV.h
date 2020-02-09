#pragma once
#include <string>
#include <vector>
#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>

// Basically we're reconstructing the CSV into an object.
class CSV {
public:
	CSV() = default;

	CSV(std::string csvPath);

	std::string GetRow(int num, int num2);
	std::vector<std::string> const& operator[](std::size_t index) const;
	std::size_t Size() const;

private:
	// rows[y][x]
	std::vector<std::vector<std::string>> rows;
};