#include "CSV.h"

// constructor
CSV::CSV(std::string csvPath) 
{
	std::ifstream file = std::ifstream(csvPath);
	std::string line;

	while (std::getline(file, line)) {
		std::vector<std::string> row;
		std::stringstream iss(line);
		std::string val;

		while (std::getline(iss, val, ',')) {
			std::stringstream converter(val);
			std::string sval = converter.str();
			row.push_back(sval);
		}
		this->rows.push_back(row);
	}
}

// gets the row
std::string CSV::GetRow(int num, int num2) {
	if (num >= rows.size()) {
		std::cout << "row number/y-value too large" << std::endl;
	}
	else {
		if (num2 >= rows[num].size()) {
			std::cout << "col number/x-value too large" << std::endl;
			return "";
		}
	}

	return rows[num][num2];
}

// [] operator
std::vector<std::string> const& CSV::operator[](std::size_t index) const {
	return rows[index];
}

// size
std::size_t CSV::Size() const {
	return rows.size();
}
