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
#include "..\post/PostLayer.h"
#include "..\textures/Texture3D.h"

namespace cherry
{
	// lookup table class
	typedef class LookUpTable
	{
	public:

		// TODO: allow for other file types

		// default constructor. Call 'LoadFileFunctions'.
		LookUpTable();

		// the file path to the lookup table
		// this currnetly only takes .CUBE files
		LookUpTable(const std::string& filePath);

		// destructor
		~LookUpTable();

		// returns the file path.
		std::string GetFilePath() const;

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

		// gets the post processing layer.
		cherry::PostLayer::Sptr GetPostLayer() const;

		// loads the lookup table
		void LoadCubeFile(const std::string& filePath);

		// loads a CSP file
		// void LoadCSPFile(const std::string& filePath); 

		// loads a a 3DL file
		// void Load3DLFile(const std::string& filePath);


		// generates a lookup table.
		static cherry::LookUpTable& GenerateLookUpTable(const std::string filePath);

	private:
		// file path
		std::string filePath = "";

		// the name of the file
		std::string name = "";

		// the description of the file
		std::string description = "";

		// the size of the LUT table; for CUBE files it's the same of each axis.
		int lut3DSize = 0;
		// int width = 0, height = 0, depth = 0;

		// the minimum and maximum domains
		glm::vec3 domainMin{};
		glm::vec3 domainMax{};

		// the amount of data points in the table
		int dataPointsTotal = 0;

		// the data points
		glm::vec3* dataPoints = nullptr;

		// 3d sampler
		Texture3D::Sptr lut;

		// post processing layer
		cherry::PostLayer::Sptr layer;

		// shader and frame buffer components
		Shader::Sptr shader;
		FrameBuffer::Sptr frameBuffer;

	protected:

	} Lut;
}