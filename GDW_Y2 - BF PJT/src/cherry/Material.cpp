// Material - used for adding materials to meshes
#include "Material.h"
#include "utils/Utils.h"

#include <fstream>

void cherry::Material::Apply() {

	// first value is the name, the second value is what we're actually setting.
	for (auto& kvp : myMat4s)
		myShader->SetUniform(kvp.first.c_str(), kvp.second);
	for (auto& kvp : myVec4s)
		myShader->SetUniform(kvp.first.c_str(), kvp.second);
	for (auto& kvp : myVec3s)
		myShader->SetUniform(kvp.first.c_str(), kvp.second);
	for (auto& kvp : myFloats)
		myShader->SetUniform(kvp.first.c_str(), kvp.second);
	for (auto& kvp : myInts)
		myShader->SetUniform(kvp.first.c_str(), kvp.second);

	// OLD [ REPLACED]
	//// binding the textures, and then sending hte slot it's bound to.
	//int slot = 0;
	//for (auto& kvp : myTextures) {
	//	kvp.second->Bind(slot);
	//	myShader->SetUniform(kvp.first.c_str(), slot);
	//	slot++;
	//}

	// updated in tutorial 09 of Intro. to Computer Graphics
	int slot = 0;
	for (auto& kvp : myTextures) {
		if (kvp.second.Sampler != nullptr)
			kvp.second.Sampler->Bind(slot);
		else
			TextureSampler::Unbind(slot);
		kvp.second.Texture->Bind(slot);
		myShader->SetUniform(kvp.first.c_str(), slot);
		slot++;
	}

	// updated in tutorial 10 of Intro. to Computer Graphics
	for (auto& kvp : myCubeMaps) {
		if (kvp.second.Sampler != nullptr)
			kvp.second.Sampler->Bind(slot);
		else
			TextureSampler::Unbind(slot);
		kvp.second.Texture->Bind(slot);
		myShader->SetUniform(kvp.first.c_str(), slot);
	}

	// mulitiplies everything by the source alpha so that transparent sceneLists don't blend with the clear colour.
	if (HasTransparency) {
		glEnable(GL_BLEND);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
		glDisable(GL_DEPTH_TEST);
	}
	else {
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
	}
}

// loads from a .mtl file
bool cherry::Material::LoadMtl(std::string filePath, const TextureSampler::Sptr& sampler)
{
	std::ifstream file(filePath, std::ios::in);
	std::string line; // a single line form a file

	// TODO: maybe make this static so that you can share defaults?
	Texture2D::Sptr defaultAlbedo = Texture2D::LoadFromFile("res/images/default.png"); // default albedo

	// std::ifstream file(filePath, std::ios::in); // opens the file
	// file.open(filePath, std::ios::in); // opens file


	// file access failure check.
	if (!file)
	{
		throw std::runtime_error("Error opening .mtl file. Is it in the right location?");
		return false;
	}
	
	// only one light
	Set("a_LightCount", 1);

	// default textures
	Set("s_Albedos[0]", defaultAlbedo);
	Set("s_Albedos[1]", defaultAlbedo);
	Set("s_Albedos[2]", defaultAlbedo);

	// gets each line
	while (std::getline(file, line))
	{
		if (line.length() == 0) // if there was nothing on the line, then it is skipped.
			continue;

		// comment
		if (line.substr(0, line.find_first_of(" ")) == "#")
		{
			// nothing happens here, but comments would relate to the file that exported the .obj and .mtl, as well as the program used.
		}
		// material name
		else if (line.substr(0, line.find_first_of(" ")) == "newmtl")
		{
			name = line.substr(line.find_first_of(" ") + 1); // name of the material
		}

		// Phong Light Model
		// Our light shader just calculates the specular and diffuse colours using other values.
		// This is used instead of giving them their own dedicated colours.

		// ambient colour
		else if (line.substr(0, line.find_first_of(" ")) == "Ka")
		{
			std::vector<float> avec = util::splitString<float>(line.substr(line.find_first_of(" ") + 1));
			Set("a_AmbientColor[0]", { avec[0], avec[1], avec[2] });
		}
		// weight of ambient colour (ambient power)
		else if (line.substr(0, line.find_first_of(" ")) == "Na")
		{
			Set("a_AmbientPower[0]", util::convertString<float>(line.substr(line.find_first_of(" ") + 1)));
		}
		// diffuse colour
		else if (line.substr(0, line.find_first_of(" ")) == "Kd")
		{
			// since we calculate the diffuse colour based off the diffuse power and light colour, 'Kd' is not used.
			// vec3  diffuseOut = diffuseFactor * a_LightColor;
		}
		// weight of amblient colour (diffuse power)
		else if (line.substr(0, line.find_first_of(" ")) == "Nd")
		{
			// Since we calculate this based on the normals and light direction, 'Nd' is not used
			//  float diffuseFactor = max(dot(norm, toLight), 0);
		}
		// specular colour
		else if (line.substr(0, line.find_first_of(" ")) == "Ks")
		{
			// since we calculate specular based on the light colour, this isn't used.
			// specular calculation: vec3 specOut = specPower * a_LightColor;
		}
		// weight of specular colour (specular power)
		else if (line.substr(0, line.find_first_of(" ")) == "Ns")
		{
			Set("a_LightSpecPower[0]", util::convertString<float>(line.substr(line.find_first_of(" ") + 1)));
		}

		// TODO: change properties to look in 'res/sceneLists/' folder directly.
		// the engine doesn't support different textures for ambient, diffuse, and specular.
		// however, it does support multi-texturing with texture mixing, which is what's being used instead.

		// ambient map (i.e. texture). This is set to Albedo[0] for multi-texturing.
		else if (line.substr(0, line.find_first_of(" ")) == "map_Ka")
		{
			// gets the folder path if there is one.
			std::string tstr = (filePath.find("/") != std::string::npos) ? filePath.substr(0, filePath.find_last_of("/") + 1) : "";

			// getting the file.
			std::string file = tstr + line.substr(line.find_first_of(" ") + 1);

			// if the file exists and is accessible, then the texture is set.
			if(util::fileAccessible(file))
				Set("s_Albedos[0]", Texture2D::LoadFromFile(file));
		}
		// diffuse map (i.e. texture). This is set to Albedo[1] for multi-texturing.
		else if (line.substr(0, line.find_first_of(" ")) == "map_Kd")
		{
			// gets the folder path if there is one.
			std::string tstr = (filePath.find("/") != std::string::npos) ? filePath.substr(0, filePath.find_last_of("/") + 1) : "";

			// getting the file.
			std::string file = tstr + line.substr(line.find_first_of(" ") + 1);

			// if the file exists and is accessible, then the texture is set.
			if (util::fileAccessible(file))
				Set("s_Albedos[1]", Texture2D::LoadFromFile(file));
		}
		// specular map (i.e. texture). This is set to Albedo[2] for multi-texturing.
		else if (line.substr(0, line.find_first_of(" ")) == "map_Ks")
		{
			// gets the folder path if there is one.
			std::string tstr = (filePath.find("/") != std::string::npos) ? filePath.substr(0, filePath.find_last_of("/") + 1) : "";

			// getting the file.
			std::string file = tstr + line.substr(line.find_first_of(" ") + 1);

			// if the file exists and is accessible, then the texture is set.
			if(util::fileAccessible(file))
				Set("s_Albedos[2]", Texture2D::LoadFromFile(file));
		}
		
		// Index of Refraction (Optical Density)
		else if (line.substr(0, line.find_first_of(" ")) == "Ni")
		{
			// unused
		}
		// alpha (transparency) of object (1.0 by default)
		else if (line.substr(0, line.find_first_of(" ")) == "d")
		{
			// unused
		}
		// alpha (transparency) of material (1.0 by default) (inverted: Tr = 1 - d)
		else if (line.substr(0, line.find_first_of(" ")) == "Tr")
		{
			// unused
		}
		// illumination mode (0 - 10)
		else if (line.substr(0, line.find_first_of(" ")) == "illum")
		{
			/*
			 * Source: https://en.wikipedia.org/wiki/Wavefront_.obj_file#Material_template_library
			 * Each value refers to a different illumination mode. 
			 * The modes are listed below:
				0. Color On, Ambient Off
				1. Color On, Ambient On
				2. Highlight On
				3. Reflection On, Ray Trace On
				4. Transparency: glass on | Reflection: ray trace on
				5. Reflection: fresnel on, and ray trace on
				6. Transparency: refraction on | Reflection: fresnel off and ray trace on
				7. Transparency: refraction on | Reflection: fresnel on and ray trace on
				8. Reflection: On | Ray Trace: Off
				9. Transparency: glass on | Reflection: ray trace off
				10. Casts shadows onto invisible surfaces
			*/
		}
		
	}

	file.close(); // closing the file

	return true;
}

// creates a material using and MTL file and returns it.
cherry::Material::Sptr cherry::Material::GenerateMtl(std::string filePath, const TextureSampler::Sptr& sampler, std::string vs, std::string fs)
{
	// creates the material and gives it a shader
	Shader::Sptr shader = std::make_shared<cherry::Shader>();
	shader->Load(vs.c_str(), fs.c_str());

	cherry::Material::Sptr tempMat = std::make_shared<cherry::Material>(shader);
	
	tempMat->LoadMtl(filePath, sampler);

	return tempMat;
}

// parses a string to get all the values from it as data type (T).
template<typename T>
const std::vector<T> cherry::Material::parseStringForTemplate(std::string str, bool containsSymbol)
{
	// if the string is of length 0, then an empty vector is returned.
	if (str.length() == 0)
		return std::vector<T>();

	if (containsSymbol) // checks if the symbol is still in the string. If so, it is removed.
	{
		str.erase(0, str.find_first_of(" ")); // erases the start of the string, which contains the symbol.
	}

	// returns the string put into a vector
	return util::splitString<T>(str);
}
