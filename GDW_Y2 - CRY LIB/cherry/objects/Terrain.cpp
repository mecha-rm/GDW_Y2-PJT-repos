#include "Terrain.h"
#include <fstream>

const int cherry::Terrain::TEXTURES_MAX = 3;

cherry::Terrain::Terrain(std::string scene, std::string heightMap, float size, float numSections, bool worldUVs)
{
	if (size <= 0) // minimum size
		size = 1;

	if (numSections < 0) // minimum amount of sections.
		numSections = 1.0F;

	this->size = size;
	this->numSections = numSections;

	std::ifstream file(heightMap, std::ios::in); // opens the file
	// file.open(filePath, std::ios::in); // opens file

	// file access failure check.
	if (!file)
	{
		safe = false; // file cannot be used
		// #ifndef _DEBUG
		throw std::runtime_error("Error opening file. Functions for this object should not be used.");
		// #endif // !DEBUG
		file.close();
		return;
	}
	else // if file opening was successful, it is safe to read from.
	{
		safe = true;
	}

	// closes the file since it was only opened for this check.
	file.close();
	this->heightMap = heightMap;

	mesh = Mesh::MakeSubdividedPlane(size, numSections, worldUVs);

	Shader::Sptr terrainShader = std::make_shared<Shader>();
	terrainShader->Load("res/shaders/terrain.vs.glsl", "res/shaders/terrain.fs.glsl");
	safe = true;

	material = std::make_shared<Material>(terrainShader);
	material->Set("a_HeightMin", heightMin);
	material->Set("a_HeightMax", heightMax);
	material->Set("a_TextureSampler", Texture2D::LoadFromFile(heightMap));

	textures[0] = heightMap;
	textures[1] = heightMap;
	textures[2] = heightMap;
	Texture2D::Sptr albedo = Texture2D::LoadFromFile(heightMap);

	material->Set("s_Albedos[0]", albedo); // sand: on level, and below the water (originally 'dirt')
	material->Set("s_Albedos[1]", albedo); // grass: medium terrain height
	material->Set("s_Albedos[2]", albedo); // rocks: highest terrain (originally 'snow')

	CreateEntity(scene, material); 
}

// returns the height map.
const std::string& cherry::Terrain::GetHeightMap() const { return heightMap; }

// sets a new height map
bool cherry::Terrain::SetHeightMap(std::string hm)
{
	std::ifstream file(hm, std::ios::in); // opens the file

	if (!file)
	{
		// #ifndef _DEBUG
		throw std::runtime_error("Error opening file. File cannot be used.");
		// #endif // !DEBUG
		file.close();
		return false;
	}

	heightMap = hm;
	material->Set("a_TextureSampler", Texture2D::LoadFromFile(hm));
	return true;
}

// gets the minimm height of the height map
float cherry::Terrain::GetMinimumHeight() const { return heightMin; }

// sets the minimum height of the height map
void cherry::Terrain::SetMinimumHeight(float hgtMin) 
{ 
	heightMin = hgtMin; 
	material->Set("a_HeightMin", heightMin);
}

// gets the maximum height of the height map.
float cherry::Terrain::GetMaximumHeight() const { return heightMax; }

// sets the maximum height of the height map.
void cherry::Terrain::SetMaximumHeight(float hgtMax) 
{ 
	heightMax = hgtMax; 
	material->Set("a_HeightMax", heightMax);
}

// sets a texture based on the provided index.
bool cherry::Terrain::SetTexture(unsigned int index, std::string filePath)
{
	if (index > TEXTURES_MAX)
		return false;

	std::ifstream file(filePath, std::ios::in); // opens the file

	if (!file)
	{
		// #ifndef _DEBUG
		throw std::runtime_error("Error opening file. File cannot be used.");
		// #endif // !DEBUG
		file.close();
		return false;
	}
	file.close();

	textures[index] = filePath;
	material->Set("s_Albedos[" + std::to_string(index) + "]", Texture2D::LoadFromFile(filePath));
	return true;
}

// updates the object.
void cherry::Terrain::Update(float deltaTime)
{
	Object::Update(deltaTime);
}
