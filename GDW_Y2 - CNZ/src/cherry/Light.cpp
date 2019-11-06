#include "Light.h"
#include <fstream> // used for checking if the file exists.


// constructor
cherry::Light::Light(const std::string a_Scene, cherry::Vec3 a_LightPos, cherry::Vec3 a_LightColor, cherry::Vec3 a_AmbientColor, float a_AmbientPower, float a_LightSpecPower, float a_LightShininess, float a_LightAttenuation)
	: m_Scene(a_Scene), m_LightPos(a_LightPos), m_LightColor(a_LightColor), m_AmbientColor(a_AmbientColor), m_AmbientPower(a_AmbientPower), m_LightSpecPower(a_LightSpecPower), m_LightShininess(a_LightShininess), m_LightAttenuation(a_LightAttenuation)
{}

// constructor
cherry::Light::Light(const std::string a_Scene, glm::vec3 a_LightPos, glm::vec3 a_LightColor, glm::vec3 a_AmbientColor, float a_AmbientPower, float a_LightSpecPower, float a_LightShininess, float a_LightAttenuation)
	: m_Scene(a_Scene), m_LightPos(a_LightPos), m_LightColor(a_LightColor), m_AmbientColor(a_AmbientColor), m_AmbientPower(a_AmbientPower), m_LightSpecPower(a_LightSpecPower), m_LightShininess(a_LightShininess), m_LightAttenuation(a_LightAttenuation)
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

// gets hte specular power
float cherry::Light::GetLightSpecularPower() const { return m_LightSpecPower; }

// sets hte light specular
void cherry::Light::SetLightSpecularPower(float specPower) 
{ 
	// TODO: check for limits of specular power.
	m_LightSpecPower = (specPower > 1.0F) ? 1.0F : (specPower < 0.0F) ? 0.0F : specPower;
}

// gets the light shininess
float cherry::Light::GetLightShiniess() const { return m_LightShininess; }

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

// generates a material with the current light values
cherry::Material::Sptr cherry::Light::GenerateMaterial() const { return GenerateMaterial("res/blank.png"); }

// generates a material with the current light values, and a texture.
cherry::Material::Sptr cherry::Light::GenerateMaterial(std::string texturePath) const
{
	// the m_Scene material
	Material::Sptr material; // the material
	Shader::Sptr phong = std::make_shared<Shader>();
	std::ifstream file; // used for checking if the image file exists.

	// used to make the albedo
	phong->Load("res/lighting.vs.glsl", "res/blinn-phong.fs.glsl"); // the shader
	material = std::make_shared<Material>(phong); // loads in the shader.

	material->Set("a_LightPos", glm::vec3(m_LightPos.v.x, m_LightPos.v.y, m_LightPos.v.z));
	material->Set("a_LightColor", glm::vec3(m_LightColor.v.x, m_LightColor.v.y, m_LightColor.v.z));
	material->Set("a_AmbientColor", glm::vec3(m_AmbientColor.v.x, m_AmbientColor.v.y, m_AmbientColor.v.z));
	material->Set("a_AmbientPower", m_AmbientPower); // change this to change the main lighting power (originally value of 0.1F)
	material->Set("a_LightSpecPower", m_LightSpecPower);
	material->Set("a_LightShininess", m_LightShininess);
	material->Set("a_LightAttenuation", m_LightAttenuation);
	
	// if the texture path is not blank.
	if (texturePath != "")
	{
		// opens the file for reading to check if it exists.
		file.open(texturePath, std::ios::in);

		if (!file) // file read failure
		{
			// throw std::runtime_error("Error. Texture image file could not be found.");
			texturePath = "res/blank.png"; // blanks out the texture.
		}

		file.close(); // closes the file
	}

	// Texture2D::Sptr albedo = Texture2D::LoadFromFile("color-grid.png");
	// material->Set("s_Albedo", albedo);

	// applies the albedo
	if(texturePath != "")
		material->Set("s_Albedo", Texture2D::LoadFromFile(texturePath));

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
