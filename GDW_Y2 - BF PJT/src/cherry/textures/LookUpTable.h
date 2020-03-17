/*
 * Name: Roderick "R.J." Montague (100701758)
 * Date: 02/04/2020
 * Description: opens files and creates lookup tables from them.
 * References:
 */
#pragma once

#include <string>
#include <GLM/vec3.hpp>
#include "..\Material.h"

namespace cherry
{
	// lookup table class
	class LookUpTable
	{
	public:
		// TODO: allow for other file types
		// the file path to the lookup table
		// this only takes .CUBE files
		LookUpTable(const std::string& filePath);

		// destructor
		~LookUpTable();

		// returns the name of the lookup table
		std::string GetName() const;

		// gets the description of the lookup table
		std::string GetDescription() const;

		// returns the lookup table's 3D size, which is the amount of pixels along each individual axis
		int GetLut3DSize() const;

		// returns the domain minimum
		glm::vec3 GetDomainMinimum() const;

		// returns the domain maximum
		glm::vec3 GetDomainMaximum() const;

		// returns the amount of data points
		int GetDataPointCount() const;

		// returns the data points
		glm::vec3* GetDataPoints() const;

	private:
		// loads the lookup table
		void LoadLookUpTable(const std::string& filePath);

		// file path
		std::string filePath = "";

		// the name of the file
		std::string name = "";

		// the description of the file
		std::string description = "";

		// the size of the LUT table
		int lut3DSize = 0;

		// the minimum and maximum domains
		glm::vec3 domainMin{};
		glm::vec3 domainMax{};

		// the amount of data points in the table
		int dataPointsTotal = 0;

		// the data points
		glm::vec3* dataPoints = nullptr;

	protected:

	};
}