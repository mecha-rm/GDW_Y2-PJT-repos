#include "CSV.h"

CSV::CSV(std::string csvPath) {
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

std::string CSV::getRow(int num, int num2) {
	if (num >= rows.size()) {
		std::cout << "y value too big" << std::endl;
	}
	else {
		if (num2 >= rows[num].size()) {
			std::cout << "x value too big" << std::endl;
		}
	}

	return rows[num][num2];
}

std::vector<std::string> const& CSV::operator[](std::size_t index) const {
	return rows[index];
}

std::size_t CSV::size() const {
	return rows.size();
}
