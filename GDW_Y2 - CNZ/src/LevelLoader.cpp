#include "LevelLoader.h"

Level::Level(std::string legendPath, std::string levelPath) {
	if (!loadLegend(legendPath)) { // wasn't valid
		std::cout << "Legend CSV is NOT VALID. Exiting." << std::endl;
		exit(1);
	}

	else {
		std::cout << "Legend valid." << std::endl;

		if (!loadLevel(levelPath)) {
			std::cout << "Level CSV is NOT VALID. Exiting." << std::endl;
			exit(1);
		}

		else {
			std::cout << "Level valid." << std::endl;
		}
	}
}

bool Level::loadLegend(std::string legendPath) {
	std::vector<std::string> names;
	std::vector<std::string> symbols;
	CSV legendCSV = CSV(legendPath);
	bool csvIsValid = true;

	//// Legend format: CSV[y][x], [0][0] should contain "Legend", [0][1] "Name", [0][2] "Symbol".
	// check to make sure format is valid
	if (legendCSV.getRow(0, 0) != "Legend" || legendCSV.getRow(0, 1) != "Name" || legendCSV.getRow(0, 2) != "Symbol") {
		csvIsValid = false;
	}

	if (csvIsValid) {
		int numRows = legendCSV.size();

		// because the first row and the first column do not contain data we care about, we need to skip them. (they are used for ensuring valid format)
		for (int i = 1; i < numRows; i++) { // arrays start at 0, so it has to be while < numRows, and we want to skip first row so int i = 1;
			names.push_back(legendCSV.getRow(i, 1)); // since first col is always empty, names are in col 1 not 0.
			symbols.push_back(legendCSV.getRow(i, 2)); // sumbols come after name col, so col 2.
		}
	}

	else {
		return false; // shit ain't valid, yo!
	}

	// sort names and symbols into legend map.
	for (int i = 0; i < names.size(); i++) {
		legend[names[i]] = symbols[i];
	}

	return true;
}

bool Level::loadLevel(std::string levelPath) {
	CSV levelCSV = CSV(levelPath);
	std::vector<int> mapSize;

	sceneName = levelCSV.getRow(0, 0);
	mapSize = getMapSize(levelCSV);

	if (mapSize.size() != 2) {
		return false;
	}

	else {
		this->map = getMap(levelCSV);
		this->properties = getMapProperties(levelCSV);
		return true;
	}
}

std::vector<int> Level::getMapSize(CSV level) {
	std::vector<int> wh;
	std::string temp = level.getRow(0, 1); // this should contain a string with two ints, formatted as: int,int

	int commaIndex = temp.find('x');
	std::string widthStr = temp.substr(0, commaIndex);
	std::string heightStr = temp.substr(commaIndex + 1, temp.length());

	int width, height;

	try {
		width = std::stoi(widthStr);
		height = std::stoi(heightStr);
	}

	catch (const std::invalid_argument & ia) {
		std::cout << "couldn't convert str to int" << std::endl;
		return wh;
	}

	wh.push_back(width);
	wh.push_back(height);
	return wh;
}


/*
[[0,1,2,3,4],
 [5,6,7,8,9]]
*/


std::vector<std::vector<std::string>> Level::getMap(CSV level) {
	std::vector<std::vector<std::string>> tempMap;
	std::vector<int> size = getMapSize(level);

	for (int y = 1; y <= size[1]; y++) { // row iteration
		std::vector<std::string> row;
		for (int x = 1; x <= size[0]; x++) { // column iterator
			row.push_back(level.getRow(y, x));
		}
		tempMap.push_back(row);
	}

	return tempMap;
}
