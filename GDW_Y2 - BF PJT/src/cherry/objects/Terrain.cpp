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
	this->worldUVs = worldUVs;

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

	// doesn't keep track of vertices and indicies, so you to re-create the mesh.
	// since we don't use the vert and index count for anything, it's fine to not track them for now.
	// getting vertex total and index total
	// int numEdgeVerts = numSections + 1;
	// verticesTotal = numEdgeVerts * numEdgeVerts;
	// indicesTotal = numSections * numSections * 6;

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

	material->Set("s_Albedos[0]", albedo);
	material->Set("s_Albedos[1]", albedo);
	material->Set("s_Albedos[2]", albedo);

	// default weights
	material->Set("a_Weights[0]", weights[0]);
	material->Set("a_Weights[1]", weights[1]);
	material->Set("a_Weights[2]", weights[2]);

	CreateEntity(scene, material); 
}

// copy constructor (does NOT use object constructor)
cherry::Terrain::Terrain(const cherry::Terrain& terrain) // : Object(terrain)
{
	// from object copy constructor.
	{
		name = terrain.GetName();
		description = terrain.GetDescription();

		path = terrain.GetPath();
		followPath = terrain.followPath;

		position = terrain.GetPosition();
		scale = terrain.GetScale();
		SetRotationDegrees(terrain.GetRotationDegrees());
	}

	// heightmap
	heightMap = terrain.heightMap;

	// textures
	textures[0] = terrain.textures[0];
	textures[1] = terrain.textures[1];
	textures[2] = terrain.textures[2];

	// size, sections, and worldUVs
	size = terrain.size;
	numSections = terrain.numSections;
	worldUVs = terrain.worldUVs;

	// the minimum and maximum height.
	heightMin = terrain.heightMin;
	heightMax = terrain.heightMax;

	// weights
	weights[0] = terrain.weights[0];
	weights[1] = terrain.weights[1];
	weights[2] = terrain.weights[2];

	// creating the object.
	mesh = Mesh::MakeSubdividedPlane(size, numSections, worldUVs);

	// new terrain shader
	Shader::Sptr terrainShader = std::make_shared<Shader>();
	terrainShader->Load("res/shaders/terrain.vs.glsl", "res/shaders/terrain.fs.glsl");
	safe = true;

	// uniforms
	material = std::make_shared<Material>(terrainShader);
	material->Set("a_HeightMin", heightMin);
	material->Set("a_HeightMax", heightMax);
	material->Set("a_TextureSampler", Texture2D::LoadFromFile(heightMap));

	// if there is no texture, the heightmap is used. If there is a texture, that is used.
	if (textures[0] == "") // texture 0
		material->Set("s_Albedos[0]", Texture2D::LoadFromFile(heightMap));
	else
		material->Set("s_Albedos[0]", Texture2D::LoadFromFile(textures[0]));

	if (textures[1] == "") // texture 1
		material->Set("s_Albedos[1]", Texture2D::LoadFromFile(heightMap));
	else
		material->Set("s_Albedos[1]", Texture2D::LoadFromFile(textures[1]));

	if (textures[2] == "") // texture 2
		material->Set("s_Albedos[2]", Texture2D::LoadFromFile(heightMap));
	else
		material->Set("s_Albedos[2]", Texture2D::LoadFromFile(textures[2]));

	// weights
	material->Set("a_Weights[0]", weights[0]);
	material->Set("a_Weights[1]", weights[1]);
	material->Set("a_Weights[2]", weights[2]);

	// creates the entity
	CreateEntity(terrain.GetSceneName(), material);
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
bool cherry::Terrain::SetTexture(int index, std::string filePath)
{
	if (index < 0 || index >= TEXTURES_MAX)
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

// Texture 0
bool cherry::Terrain::SetTexture0(std::string filePath)
{
	return SetTexture(0, filePath);
}

// Texture 1
bool cherry::Terrain::SetTexture1(std::string filePath)
{
	return SetTexture(1, filePath);
}

// Texture 2
bool cherry::Terrain::SetTexture2(std::string filePath)
{
	return SetTexture(2, filePath);
}

// get the texture path.
std::string cherry::Terrain::GetTextureFilePath(int index)
{
	index = std::clamp(index, 0, TEXTURES_MAX - 1);

	return textures[index];
}

// gets the texture 0 file path.
std::string cherry::Terrain::GetTexture0FilePath() const
{
	return textures[0];
}

// gets the texture 1 file path.
std::string cherry::Terrain::GetTexture1FilePath() const
{
	return textures[1];
}

// gets the texture 2 file path.
std::string cherry::Terrain::GetTexture2FilePath() const
{
	return textures[2];
}

// returns texture weight.
float cherry::Terrain::GetTextureWeight(int index) const
{
	index = std::clamp(index, 0, TEXTURES_MAX - 1);
	return weights[index];
}

// sets a texture weight.
void cherry::Terrain::SetTextureWeight(int index, float amnt)
{
	// clamps the amount.
	index = std::clamp(index, 0, TEXTURES_MAX - 1);
	amnt = std::clamp(amnt, 0.0F, 1.0F);

	// saves the weight
	weights[index] = amnt;
	material->Set("a_Weights[" + std::to_string(index) + "]", amnt);
}

// returns weight 0.
float cherry::Terrain::GetTextureWeight0() const
{
	return weights[0];
}

// sets texture weight 0
void cherry::Terrain::SetTextureWeight0(float amnt)
{
	SetTextureWeight(0, amnt);
}

// returns weight 1
float cherry::Terrain::GetTextureWeight1() const
{
	return weights[1];
}

// sets texture weight 1
void cherry::Terrain::SetTextureWeight1(float amnt)
{
	SetTextureWeight(1, amnt);
}

// returns weight 2
float cherry::Terrain::GetTextureWeight2() const
{
	return weights[2];
}

// sets texture weight 2
void cherry::Terrain::SetTextureWeight2(float amnt)
{
	SetTextureWeight(2, amnt);
}

// updates the object.
void cherry::Terrain::Update(float deltaTime)
{
	Object::Update(deltaTime);
}
