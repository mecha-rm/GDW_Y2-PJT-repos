#include "Light.h"
#include <fstream> // used for checking if the file exists.


// constructor
cherry::Light::Light(const std::string a_Scene, cherry::Vec3 a_LightPos, cherry::Vec3 a_LightColor, cherry::Vec3 a_AmbientColor, float a_AmbientPower, float a_LightSpecPower, float a_LightShininess, float a_LightAttenuation)
	: m_Scene(a_Scene)
{
	// setting the values
	SetLightPosition(a_LightPos);
	SetLightColor(a_LightColor);

	SetAmbientColor(a_AmbientColor);
	SetAmbientPower(a_AmbientPower);

	SetLightSpecularPower(a_LightSpecPower);
	SetLightShininess(a_LightShininess);
	SetLightAttenuation(m_LightAttenuation);
}

// constructor
cherry::Light::Light(const std::string a_Scene, glm::vec3 a_LightPos, glm::vec3 a_LightColor, glm::vec3 a_AmbientColor, float a_AmbientPower, float a_LightSpecPower, float a_LightShininess, float a_LightAttenuation)
	:Light(a_Scene, Vec3(a_LightPos), Vec3(a_LightColor), Vec3(a_AmbientColor), a_AmbientPower, a_LightSpecPower, a_LightShininess, a_LightAttenuation)
{}


// gets the m_Scene the light is in.
std::string cherry::Light::GetScene() const { return m_Scene; }

// gets the light position as a Vec3
cherry::Vec3 cherry::Light::GetLightPosition() const { return m_LightPos; }

// gets the light position as a GLM vector.
glm::vec3 cherry::Light::GetLightPositionGLM() const { return glm::vec3(m_LightPos.v.x, m_LightPos.v.y, m_LightPos.v.z); }

// sets the light position
void cherry::Light::SetLightPosition(float x, float y, float z) { m_LightPos = cherry::Vec3(x, y, z); }

// sets the light position
void cherry::Light::SetLightPosition(cherry::Vec3 pos) { m_LightPos = pos; }

// sets the light position
void cherry::Light::SetLightPosition(glm::vec3 pos) { m_LightPos = pos; }

// gets the light position (x-axis)
float cherry::Light::GetLightPositionX() const { return m_LightPos.v.x; }

// sets the light position (x-axis)
void cherry::Light::SetLightPositionX(float x) { m_LightPos.v.x = x; }

// gets the light position (y-axis)
float cherry::Light::GetLightPositionY() const { return m_LightPos.v.y; }

// set the light position (y-axis)
void cherry::Light::SetLightPositionY(float y) { m_LightPos.v.y = y; }

// get the light position (z-axis)
float cherry::Light::GetLightPositionZ() const { return m_LightPos.v.z; }

// sets the light position (z-axis)
void cherry::Light::SetLightPositionZ(float z) { m_LightPos.v.z = z; }



// gets the light colour in decimal form.
cherry::Vec3 cherry::Light::GetLightColor() const { return m_LightColor; }

// gets the light colour as a GLM vector.
glm::vec3 cherry::Light::GetLightColorGLM() const { return glm::vec3(m_LightColor.v.x, m_LightColor.v.y, m_LightColor.v.z); }

// sets the light colour.
void cherry::Light::SetLightColor(cherry::Vec3 clr) 
{ 
	// bounds checking
	m_LightColor.v.x = (clr.v.x > 1.0F) ? 1.0F : (clr.v.x < 0.0F) ? 0.0F : clr.v.x;
	m_LightColor.v.y = (clr.v.y > 1.0F) ? 1.0F : (clr.v.y < 0.0F) ? 0.0F : clr.v.y;
	m_LightColor.v.z = (clr.v.z > 1.0F) ? 1.0F : (clr.v.z < 0.0F) ? 0.0F : clr.v.z;
}

// sets the light colour
void cherry::Light::SetLightColor(glm::vec3 clr) { SetLightColor(cherry::Vec3(clr)); }

// gets the ambient colour.
cherry::Vec3 cherry::Light::GetAmbientColor() const { return m_AmbientColor; }

// gets the ambient colour as a GLM vector
glm::vec3 cherry::Light::GetAmbientColorGLM() const { return glm::vec3(m_AmbientColor.v.x, m_AmbientColor.v.y, m_AmbientColor.v.z); }

// sets the ambient colour
void cherry::Light::SetAmbientColor(float x, float y, float z) { m_AmbientColor = cherry::Vec3(x, y, z); }

// sets hte ambient colour
void cherry::Light::SetAmbientColor(cherry::Vec3 ambientClr) 
{ 
	// bounds checking
	m_AmbientColor.v.x = (ambientClr.v.x > 1.0F) ? 1.0F : (ambientClr.v.x < 0.0F) ? 0.0F : ambientClr.v.x;
	m_AmbientColor.v.y = (ambientClr.v.y > 1.0F) ? 1.0F : (ambientClr.v.y < 0.0F) ? 0.0F : ambientClr.v.y;
	m_AmbientColor.v.z = (ambientClr.v.z > 1.0F) ? 1.0F : (ambientClr.v.z < 0.0F) ? 0.0F : ambientClr.v.z;
}

// sets hte ambient color
void cherry::Light::SetAmbientColor(glm::vec3 ambientClr) { SetAmbientColor(cherry::Vec3(ambientClr)); }

// gets the ambient light power
float cherry::Light::GetAmbientPower() const { return m_AmbientPower; }

// sets the ambient power of the light
void cherry::Light::SetAmbientPower(float ambientPower) { m_AmbientPower = (ambientPower < 0.0F) ? 0.0F : ambientPower; }



// gets hte specular power
float cherry::Light::GetLightSpecularPower() const { return m_LightSpecPower; }

// sets hte light specular
void cherry::Light::SetLightSpecularPower(float specPower) 
{ 
	// TODO: check for limits of specular power.
	m_LightSpecPower = (specPower > 1.0F) ? 1.0F : (specPower < 0.0F) ? 0.0F : specPower;
}

// gets the light shininess
float cherry::Light::GetLightShininess() const { return m_LightShininess; }

// sets the light shininess
void cherry::Light::SetLightShininess(float shininess) 
{
	// this is an exponent, so its value usually ranges rom 2 to 256.
	m_LightShininess = (shininess > 256.0F) ? 256.0F : (shininess < 2.0F) ? 2.0F : shininess;
}

// gets the light attenuation
float cherry::Light::GetLightAttenuation() const { return m_LightAttenuation; }

// sets the light attenuation
void cherry::Light::SetLightAttenuation(float attenuation) 
{ 
	// TODO: check to see if the bounds are correct
	// bounds checking
	m_LightAttenuation = (attenuation > 1.0F) ? 1.0F : (attenuation < 0.0F) ? 0.0F : attenuation;

}

// generates shader with default shader values
cherry::Material::Sptr cherry::Light::GenerateMaterial(const TextureSampler::Sptr& sampler) const { return GenerateMaterial(STATIC_VS, STATIC_FS, sampler); }

// generates a material with the current light values
cherry::Material::Sptr cherry::Light::GenerateMaterial(std::string vs, std::string fs, const TextureSampler::Sptr& sampler) const { return GenerateMaterial(vs, fs, "res/images/default.png", 1.0F); }

// generates a material using a material lib file.
cherry::Material::Sptr cherry::Light::GenerateMaterial(std::string vs, std::string fs, std::string mtllib, const TextureSampler::Sptr& sampler)
{
	// the material
	Material::Sptr material = GenerateMaterial(vs, fs);

	// loads in the material
	material->LoadMtl(mtllib);

	return material;
}

// generates a material with one texture
cherry::Material::Sptr cherry::Light::GenerateMaterial(std::string vs, std::string fs, std::string texturePath, float weight, const TextureSampler::Sptr& sampler) const
{
	return GenerateMaterial(vs, fs, texturePath, weight, "", 0.0F, "", 0.0F, sampler);
}

// generates a material with two textures
cherry::Material::Sptr cherry::Light::GenerateMaterial(std::string vs, std::string fs, std::string txt0, float wgt0, std::string txt1, float wgt1, const TextureSampler::Sptr& sampler) const
{
	return GenerateMaterial(vs, fs, txt0, wgt0, txt1, wgt1, "", 0.0F, sampler);
}

// generates a material using a provided texture and weights
cherry::Material::Sptr cherry::Light::GenerateMaterial(std::string vs, std::string fs, std::string txt0, float wgt0, std::string txt1, float wgt1, std::string txt2, float wgt2, const TextureSampler::Sptr& sampler) const
{
	// the m_Scene material
	Material::Sptr material; // the material
	Shader::Sptr phong = std::make_shared<Shader>();
	std::ifstream file; // used for checking if the image file exists.
	glm::vec3 texWeights; // the weights of all of the textures.

	// used to make the albedo // TODO: fix shaders
	phong->Load(vs.c_str(), fs.c_str()); // the shader
	material = std::make_shared<Material>(phong); // loads in the shader.

	material->Set("a_LightCount", 1);
	material->Set("a_LightPos[0]", glm::vec3(m_LightPos.v.x, m_LightPos.v.y, m_LightPos.v.z));
	material->Set("a_LightColor[0]", glm::vec3(m_LightColor.v.x, m_LightColor.v.y, m_LightColor.v.z));
	material->Set("a_AmbientColor[0]", glm::vec3(m_AmbientColor.v.x, m_AmbientColor.v.y, m_AmbientColor.v.z));
	material->Set("a_AmbientPower[0]", m_AmbientPower); // change this to change the main lighting power (originally value of 0.1F)
	material->Set("a_LightSpecPower[0]", m_LightSpecPower);
	material->Set("a_LightShininess[0]", m_LightShininess);
	material->Set("a_LightAttenuation[0]", m_LightAttenuation);

	// getting the weights of the textures.
	texWeights.x = (wgt0 < 0.0F) ? 0.0F : (wgt0 > 1.0F) ? 1.0F : wgt0;
	texWeights.y = (wgt1 < 0.0F) ? 0.0F : (wgt1 > 1.0F) ? 1.0F : wgt1;
	texWeights.z = (wgt2 < 0.0F) ? 0.0F : (wgt2 > 1.0F) ? 1.0F : wgt2;


	// goes through each texture and sees if it the files can be found.
	for (int i = 0; i < 3; i++)
	{
		// opening the image file to check for available reading
		if(i == 0)
		{
			// if there is no texture, a default is provided
			if (txt0 == "")
				txt0 = "res/images/default.png";

			file.open(txt0, std::ios::in);
		}
			
		else if (i == 1)
		{
			// if there is no texture, a default is provided
			if (txt1 == "")
				txt1 = "res/images/default.png";
			file.open(txt1, std::ios::in);
		}
		else if (i == 2)
		{
			// if there is no texture, a default is provided
			if (txt2 == "")
				txt2 = "res/images/default.png";
			file.open(txt2, std::ios::in);
		}

		if (!file) // file read failure
		{
//#ifndef _DEBUG
//			throw std::runtime_error("Error. Texture image file could not be found.");
//#endif // !_DEBUG

			// sets to default texture
			if(i == 0)
				txt0 = "res/images/default.png";
			else if(i == 1)
				txt1 = "res/images/default.png";
			else if (i == 2)
				txt2 = "res/images/default.png";

		}

		file.close();
	}
	
	file.close();

	// applies the albedo values
	material->Set("s_Albedos[0]", Texture2D::LoadFromFile(txt0), sampler);
	material->Set("s_Albedos[1]", Texture2D::LoadFromFile(txt1), sampler);
	material->Set("s_Albedos[2]", Texture2D::LoadFromFile(txt2), sampler);
	
	
	return material;
}

// to_string function
std::string cherry::Light::ToString() const
{
	return "Scene: " + m_Scene + " | Light Position: " + m_LightPos.ToString() + " | Light Color: " + m_LightColor.ToString() +
		" | Ambient Color: " + m_AmbientColor.ToString() + " | Ambient Power: " + std::to_string(m_AmbientPower) +
		" | Light Specular Power: " + std::to_string(m_LightSpecPower) + " | Light Shininess: " + std::to_string(m_LightShininess) +
		" | Light Attenuation: " + std::to_string(m_LightAttenuation);
}
