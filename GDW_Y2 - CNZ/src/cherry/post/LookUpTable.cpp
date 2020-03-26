/*
 * Name: Bonus Fruit
 * Date: 03/16/2020
 * Description: reads .CUBE files and makes a lookup table out of them.
 * References:
 */
#include "LookUpTable.h"

#include <iostream>
#include <fstream>
#include <vector>
#include "..\Game.h"

// splits a string into a vec3. The values are found by using the spaces as seperators.
glm::vec3 StringToVector3(const std::string& str)
{
	// saves the three values for the vec3.
	std::string vals[3];

	// this assumes there is no space before or after the provided values in the string.
	// it also assumes that the input is valid, and that the string is properly formatted.
	// it really should do error checking, but it was a rush job.
	vals[0] = str.substr(0, str.find_first_of(" ")); // value 1
	vals[1] = str.substr(str.find_first_of(" ") + 1, str.find_last_of(" ")); // value 2
	vals[2] = str.substr(str.find_last_of(" ") + 1); // value 3

	// returns the vector
	return glm::vec3(std::stof(vals[0]), std::stof(vals[1]), std::stof(vals[2]));
}

// checks to see if a string is a number.
// this function has been taken from a personal utilities project. Said project has also been implemented into my team's GDW game.
// so if any plagiarism flags are raised, that's why.
bool isNum(std::string str)
{
	// numbers 0 through 9. This isn't the most efficient way to check, but it's suitable for the asisgnment.
	std::string arr[10] = { "1","2","3","4","5","6","7","8","9","0" };

	// checks each index of the string for number validity.
	for (int x = 0; x < str.length(); x++) {
		for (int y = 0; y < 10; y++) {
			if (str.substr(x, 1) == arr[y]) 
			{
				break;
			}
			else if (y == 9) // invalid character
			{
				return false;
			}
		}
	}

	// string valid
	return true;
}

// constructor
cherry::LookUpTable::LookUpTable()
{
	// post processing layer
	
	// gets the window size
	glm::ivec2 windowSize = Game::GetRunningGame()->GetWindowSize();

	// shader
	shader = std::make_shared<Shader>();
	shader->Load(POST_VS, POST_LUT_FS);

	// frame buffer
	frameBuffer = std::make_shared<FrameBuffer>(windowSize.x, windowSize.y);

	// buffer color
	RenderBufferDesc bufferColor = RenderBufferDesc();
	bufferColor.ShaderReadable = true;
	bufferColor.Attachment = RenderTargetAttachment::Color0;
	bufferColor.Format = RenderTargetType::Color24; // loads with RGB

	// buffer depth
	RenderBufferDesc bufferDepth = RenderBufferDesc();
	bufferDepth.ShaderReadable = true; 
	bufferDepth.Attachment = RenderTargetAttachment::Depth;
	bufferDepth.Format = RenderTargetType::Depth24;

	// frame buffer
	frameBuffer->AddAttachment(bufferColor);
	frameBuffer->AddAttachment(bufferDepth);

	// makes the layer
	layer = std::make_shared<PostLayer>(shader, frameBuffer);
	
}

// constructor
cherry::LookUpTable::LookUpTable(const std::string& filePath)
	: LookUpTable()
{
	LoadCubeFile(filePath);
}

// destructor
cherry::LookUpTable::~LookUpTable()
{ 
	delete[] dataPoints; // deletes the data points
}

// returns the file path.
std::string cherry::LookUpTable::GetFilePath() const { return filePath; }

// returns the name of the lookup table
std::string cherry::LookUpTable::GetName() const { return name; }

// returns the description of the table
std::string cherry::LookUpTable::GetDescription() const { return description; }

// returns the 3D size of the LUT table, which is the amount of pixels along each axis.
int cherry::LookUpTable::GetLut3DSize() const { return lut3DSize; }

// returns domain minimum
glm::vec3 cherry::LookUpTable::GetDomainMinimum() const { return domainMin; }

// returns the domain maximum
glm::vec3 cherry::LookUpTable::GetDomainMaximum() const { return domainMax; }

// returns the amount of data points
int cherry::LookUpTable::GetDataPointCount() const { return dataPointsTotal; }

// returns the data points
glm::vec3* cherry::LookUpTable::GetDataPoints() const { return dataPoints; }

// returns the post processing layer.
cherry::PostLayer::Sptr cherry::LookUpTable::GetPostLayer() const
{
	return layer;
}

// loads a cube file.
void cherry::LookUpTable::LoadCubeFile(const std::string& filePath)
{
	// period can be found.
	if (filePath.find(".") != std::string::npos)
	{
		// if the file extension isn't a .CUBE file.
		if (!util::equalsIgnoreCase(filePath.substr(filePath.find(".")), ".CUBE"))
		{
			std::cout << "Incorrect file extension detected. LUT creation failure" << std::endl;
			return;
		}
	}
	else
	{
		std::cout << "Could not find file extension. LUT creation failure." << std::endl;
		return;
	}

	// opens the file
	std::ifstream file(filePath, std::ios::in); // opens hte file
	std::string line = ""; // gets a line from the file
	std::vector<glm::vec3> vals; // gets data point values from the file

	if (!file) // if the file failed to open.
	{
		std::cout << "File failed to load." << std::endl;
		file.close();
		return;
	}

	// it should check for file type validity, but there is no time for that.

	// saving the file path
	this->filePath = filePath;

	// goes through each line
	while (std::getline(file, line))
	{
		if (line.length() == 0)
			continue;

		// file description
		if (line.substr(0, line.find_first_of(" ")) == "#Created by:")
		{
			description = line.substr(1); // removes the # and takes in the rest
		}
		// the title of the file
		else if (line.substr(0, line.find_first_of(" ")) == "TITLE")
		{
			name = line.substr(line.find_first_of(" ") + 1);
			name = name.substr(1, name.size() - 2); // removes quote (") symbols
		}
		// the lut size
		else if (line.substr(0, line.find_first_of(" ")) == "LUT_3D_SIZE")
		{
			lut3DSize = std::stoi(line.substr(line.find_first_of(" ") + 1));
		}
		// data domain - domain minimum
		else if (line.substr(0, line.find_first_of(" ")) == "DOMAIN_MIN")
		{
			domainMin = StringToVector3(line.substr(line.find_first_of(" ") + 1));
		}
		// data domain - data maximum
		else if (line.substr(0, line.find_first_of(" ")) == "DOMAIN_MAX")
		{
			domainMax = StringToVector3(line.substr(line.find_first_of(" ") + 1));
		}
		// if the first value is a number, then the line is a data point.
		else if (isNum(line.substr(0, 1)))
		{
			vals.push_back(StringToVector3(line));
		}
	}

	file.close();

	// saves the data points.
	// for the files in this project, the data size is 32,768 (2 ^ (32 / 2 - 1) = 2 ^ 15  = 32,768)
	dataPointsTotal = vals.size();
	
	delete[] dataPoints; // deleting the current data
	dataPoints = new glm::vec3[dataPointsTotal];

	for (int i = 0; i < dataPointsTotal; i++)
	{
		dataPoints[i] = vals[i];
	}
	// copying in the data
	// memcpy(dataPoints, vals.data(), sizeof(glm::vec3) * dataPointsTotal);

	// getting the texture  
	lut = Texture3D::LoadFromLookUpTable(*this);
	shader->SetUniform("a_Lut", lut); // TODO: add function to take in this variable. 
}

// loads a CSP file
// void cherry::LookUpTable::LoadCSPFile(const std::string& filePath)
// {
// 	// opens the file
// 	std::ifstream file(filePath, std::ios::in); // opens hte file
// 	std::string line = ""; // gets a line from the file
// 	std::vector<glm::vec3> vals; // gets data point values from the file
// 
// 	if (!file) // if the file failed to open.
// 	{
// 		std::cout << "File failed to load." << std::endl;
// 		file.close();
// 		return;
// 	}
// 
// 	// it should check for file type validity, but there is no time for that.
// 
// 	// saving the file path
// 	this->filePath = filePath;
// 	
// 	// default values
// 	domainMin = glm::vec3();
// 	domainMax = glm::vec3(1.0F);
// 
// 	// goes through each line
// 	while (std::getline(file, line))
// 	{
// 		if (line.length() == 0)
// 			continue;
// 
// 		// file description
// 		if (line.substr(0, line.find_first_of(" ")) == "#Created by:")
// 		{
// 			description = line.substr(1); // removes the # and takes in the rest
// 		}
// 		// the title of the file
// 		else if (line.substr(0, line.find_first_of(" ")) == "TITLE")
// 		{
// 			name = line.substr(line.find_first_of(" ") + 1);
// 			name = name.substr(1, name.size() - 2);
// 		}
// 		// the lut size
// 		else if (line.substr(0, line.find_first_of(" ")) == "LUT_3D_SIZE")
// 		{
// 			lut3DSize = std::stoi(line.substr(line.find_first_of(" ") + 1));
// 		}
// 		// data domain - domain minimum
// 		else if (line.substr(0, line.find_first_of(" ")) == "DOMAIN_MIN")
// 		{
// 			domainMin = StringToVector3(line.substr(line.find_first_of(" ") + 1));
// 		}
// 		// data domain - data maximum
// 		else if (line.substr(0, line.find_first_of(" ")) == "DOMAIN_MAX")
// 		{
// 			domainMax = StringToVector3(line.substr(line.find_first_of(" ") + 1));
// 		}
// 		// if the first value is a number, then the line is a data point.
// 		else if (isNum(line.substr(0, 1)))
// 		{
// 			// splits the string
// 			std::vector<std::string> temp = util::splitString<std::string>(line);
// 
// 			switch (temp.size())
// 			{
// 			case 2: // domain
// 
// 				break;
// 			case 3: // colour value or size
// 				if (util::isInt(temp[0])) // if it's an integer, then its the 3D lut size.
// 				{
// 					width = util::convertString<int>(temp[0]);
// 					height = util::convertString<int>(temp[0]);
// 					depth = util::convertString<int>(temp[0]);
// 				}
// 				break;
// 			}
// 
// 			vals.push_back(StringToVector3(line));
// 		}
// 	}
// 
// 	file.close();
// 
// 	// saves the data points.
// 	// for the files in this project, the data size is 32,768 (2 ^ (32 / 2 - 1) = 2 ^ 15  = 32,768)
// 	dataPointsTotal = vals.size();
// 
// 	delete[] dataPoints; // deleting the current data
// 	dataPoints = new glm::vec3[dataPointsTotal];
// 
// 	for (int i = 0; i < dataPointsTotal; i++)
// 	{
// 		dataPoints[i] = vals[i];
// 	}
// 	// copying in the data
// 	// memcpy(dataPoints, vals.data(), sizeof(glm::vec3) * dataPointsTotal);
// 
// 	// getting the texture  
// 	lut = Texture3D::LoadFromLookUpTable(*this);
// 	shader->SetUniform("a_Lut", lut); // TODO: add function to take in this variable.
// }


// generates a lookup table.
cherry::LookUpTable& cherry::LookUpTable::GenerateLookUpTable(const std::string filePath)
{
	LookUpTable temp(filePath);
	return temp;
}