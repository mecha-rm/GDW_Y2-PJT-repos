#include "Liquid.h"
#include <string>

// the maximum amount of waves.
// NOTE: make sure this value aligns with the macro 'MAX_WAVES' in the 'water-shader.vs.glsl'
const int cherry::Liquid::MAX_WAVES = 8;

// constructor
cherry::Liquid::Liquid(std::string scene, float size, float numSections, bool worldUVs)
	:Object()
{
	if (size <= 0) // minimum size
		size = 1;

	if (numSections < 0) // minimum amount of sections.
		numSections = 1.0F;

	this->size = size; // saving the wave size.
	this->numSections = numSections; // saving the number of wave sections.

	// creating the material
	Shader::Sptr liquidShader = std::make_shared<Shader>();
	liquidShader->Load("res/shaders/water-shader.vs.glsl", "res/shaders/water-shader.fs.glsl");
	safe = true;

	material = std::make_shared<Material>(liquidShader);
	material->HasTransparency = true;


	// the wave calculation doesn't save the vertices, so the calculation is done here.
	// the mesh body
	float start = -size / 2.0f;
	float step = size / numSections;

	// minimum values (first iteration of the loop)
	meshBodyMin.v.x = start;
	meshBodyMin.v.y = start;
	meshBodyMin.v.z = 0;

	// maximum values (final iteration of the loop)
	meshBodyMax.v.x = start + numSections * step;
	meshBodyMax.v.y = start + numSections * step;
	meshBodyMax.v.z = 0;

	mesh = Mesh::MakeSubdividedPlane(size, numSections, worldUVs);
	mesh->cullFaces = false;


	CreateEntity(scene, material); // creates the entity
}

// returns the size of the liquid.
float cherry::Liquid::GetSize() const { return size; }

// returns the number of sections for the wave.
float cherry::Liquid::GetNumberOfSections() const { return numSections; }

// gets the total amount of enabled waves.
int cherry::Liquid::GetEnabledWaves() const { return enabledWaves; }

void cherry::Liquid::SetEnabledWaves(int waveCount)
{
	waveCount = (waveCount < 0) ? 0 : (waveCount > MAX_WAVES) ? MAX_WAVES : waveCount;

	material->Set("a_EnabledWaves", waveCount);
	enabledWaves = waveCount;
}

// returns the gravity value
float cherry::Liquid::GetGravity() const { return gravity; }

// wave gravity
void cherry::Liquid::SetGravity(float gravity)
{
	material->Set("a_Gravity", abs(gravity));
	this->gravity = gravity;
}

// gets the maximum amount of waves allowed.
int cherry::Liquid::GetMaximumWaveCount() const { return MAX_WAVES; }

// sets a wave
void cherry::Liquid::SetWave(unsigned int waveNum, float xDir, float yDir, float steepness, float waveLength)
{
	if (waveNum >= MAX_WAVES) // if the index is out of range, then nothing happens.
		return;

	material->Set("a_Waves[" + std::to_string(waveNum) + "]", {xDir, yDir, steepness, waveLength});

}

// gets the alpha value of the wave.
float cherry::Liquid::GetAlpha() const { return color.v.w; }

// sets the alpha of the liquid (0 - 1)
void cherry::Liquid::SetAlpha(float a)
{
	// bounds checking
	a = (a < 0.0F) ? 0.0F : (a > 1.0F) ? 1.0F : a;

	material->Set("a_WaterAlpha", a);
	color.v.w = a; // saving the alpha value
	alpha = a; // the alpha variable in the object class. This isn't used for liquids, but is set regardless.
}

// gets the colour as a GLM vector.
glm::vec4 cherry::Liquid::GetColorGLM() const { return glm::vec4(color.v.x, color.v.y, color.v.z, color.v.w); }

// returns the color of the wave
cherry::Vec4 cherry::Liquid::GetColor() const { return color; }

// sets the colour of the liquid using a [0, 1] rgb scale
void cherry::Liquid::SetColor(float r, float g, float b)
{
	r = (r < 0.0F) ? 0.0F : (r > 1.0F) ? 1.0F : r;
	g = (g < 0.0F) ? 0.0F : (g > 1.0F) ? 1.0F : g;
	b = (b < 0.0F) ? 0.0F : (b > 1.0F) ? 1.0F : b;

	material->Set("a_WaterColor", { r, g, b });
	
	// saving the colour
	color.v.x = r;
	color.v.y = g;
	color.v.z = b;
}

// sets the colour of the liquid using a [0, 255] range.
void cherry::Liquid::SetColor(int r, int g, int b)
{
	SetColor((float)r / 255.0F, (float)g / 255.0F, (float)b / 255.0F);
}

// sets the liquid colour on a [0, 1] rgba scale
void cherry::Liquid::SetColor(float r, float g, float b, float a)
{
	SetColor(r, g, b); // setting the colour
	SetAlpha(a); // setting the alpha
}

// sets liquid colour using a [0, 255] rgba scale
void cherry::Liquid::SetColor(int r, int g, int b, float a)
{
	SetColor(r, g, b); // calls other function to convert the values to floats.
	SetAlpha(a);
}

// returns the liquid's clarity.
float cherry::Liquid::GetClarity() const { return clarity; }

// sets the clarity of the liquid on a [0, 1] range
void cherry::Liquid::SetClarity(float clarity)
{
	clarity = (clarity < 0.0F) ? 0.0F : (clarity > 1.0F) ? 1.0F : clarity;
	
	material->Set("a_WaterClarity", clarity);
	this->clarity = clarity;
}

// gets the fresnal power
float cherry::Liquid::GetFresnalPower() const { return fresnelPower; }

// the higher the power the higher the reflection 
void cherry::Liquid::SetFresnelPower(float fresnelPower)
{
	fresnelPower = (fresnelPower < 0.0F) ? 0.0F : (fresnelPower > 1.0F) ? 1.0F : fresnelPower;
	
	material->Set("a_FresnelPower", fresnelPower);
	this->fresnelPower = fresnelPower; // saving the fresnel power
}

// returns the refraction index
float cherry::Liquid::GetRefractionIndex() { return refractionIndex; }

// sets the refraction index
void cherry::Liquid::SetRefractionIndex(float medium1, float medium2)
{
	material->Set("a_RefractionIndex", abs(medium1 / medium2));
	refractionIndex = abs(medium1 / medium2);
}

// returns the texture cube.
cherry::TextureCube::Sptr cherry::Liquid::GetEnvironment() const { return skyboxTexture; }

// sets the environment (skybox) that the liquid is part of.
void cherry::Liquid::SetEnvironment(TextureCube::Sptr skyboxTexture)
{
	material->Set("s_Environment", skyboxTexture);
	this->skyboxTexture = skyboxTexture;
}

// update
void cherry::Liquid::Update(float deltaTime)
{
	Object::Update(deltaTime);
}
