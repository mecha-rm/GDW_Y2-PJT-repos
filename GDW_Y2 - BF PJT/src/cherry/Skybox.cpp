#include "Skybox.h"
#include "scenes/Scene.h"
#include <fstream>

// default constructor
cherry::Skybox::Skybox()
{
	// default 'white' skybox
	files[0] = "res/images/default.jpg";
	files[1] = "res/images/default.jpg";
	files[2] = "res/images/default.jpg";
	files[3] = "res/images/default.jpg";
	files[4] = "res/images/default.jpg";
	files[5] = "res/images/default.jpg";
}

// sets the images for the skybox.
cherry::Skybox::Skybox(std::string left, std::string right, std::string down, std::string up, std::string front, std::string back)
{
	SetLeft(left);
	SetRight(right);
	SetDown(down);
	SetUp(up);
	SetFront(front);
	SetBack(back);
}

// gets the image for the left of the skybox.
std::string cherry::Skybox::GetLeft() const { return files[0]; }

// sets the image for the leftside of the cubemmap
bool cherry::Skybox::SetLeft(std::string filePath) { return SetFile(0, filePath); }

// gets the rightside of the skybox.
std::string cherry::Skybox::GetRight() const { return files[1]; }

// sets the rightside of the cubemap
bool cherry::Skybox::SetRight(std::string filePath) { return SetFile(1, filePath); }

// gets the image for the up of the skybox.
std::string cherry::Skybox::GetUp() const { return files[2]; }

// sets the top of the cubemap
bool cherry::Skybox::SetUp(std::string filePath) { return SetFile(2, filePath); }

// gets the image for the down of the skybox.
std::string cherry::Skybox::GetDown() const { return files[3]; }

// sets the bottom of a cubemap
bool cherry::Skybox::SetDown(std::string filePath) { return SetFile(3, filePath); }

// gets the image for the front of the skybox.
std::string cherry::Skybox::GetFront() const { return files[4]; }

// front of the cubemap
bool cherry::Skybox::SetFront(std::string filePath) { return SetFile(4, filePath); }

// gets the image for the back of the skybox.
std::string cherry::Skybox::GetBack() const { return files[5]; }

// back of the cubemap
bool cherry::Skybox::SetBack(std::string filePath) { return SetFile(5, filePath); }

// sets a file based on the provided index.
bool cherry::Skybox::SetFile(unsigned int index, std::string filePath)
{
	std::ifstream file(filePath, std::ios::in); // opening for file checking

	if (!file) // image not found
	{
		// #ifndef _DEBUG
		throw std::runtime_error("Image not found.");
		// #endif // !DEBUG
		file.close();
		return false;
	}

	file.close();
	files[index] = filePath;

	return true;
}

// generates the texture cube and returns it
cherry::TextureCube::Sptr cherry::Skybox::GenerateTextureCube() const { return TextureCube::LoadFromFiles(files); }

cherry::Shader::Sptr cherry::Skybox::GenerateShader() const
{
	Shader::Sptr shader = std::make_shared<Shader>();
	shader->Load("res/shaders/cubemap.vs.glsl", "res/shaders/cubemap.fs.glsl");
	return shader;
}

// generates and returns the skybox's mesh
cherry::Mesh::Sptr cherry::Skybox::GenerateMesh() const { return Mesh::MakeInvertedCube(); }

// adds a skybox to a scene
void cherry::Skybox::AddSkyboxToScene(cherry::Scene * scene) const
{
	// adds the shader, mesh, and textured cube.
	scene->SkyboxShader = GenerateShader();
	scene->SkyboxMesh = GenerateMesh();
	scene->Skybox = GenerateTextureCube();
}


