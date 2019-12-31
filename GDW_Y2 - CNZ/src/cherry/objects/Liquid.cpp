#include "Liquid.h"
#include <string>

// the maximum amount of waves.
// NOTE: make sure this value aligns with the macro 'MAX_WAVES' in the 'water-shader.vs.glsl'
const int cherry::Liquid::MAX_WAVES = 8;

cherry::Liquid::Liquid(std::string scene, float size, float numSections, bool worldUVs)
	:Object()
{
	// creating the material
	Shader::Sptr liquidShader = std::make_shared<Shader>();
	liquidShader->Load("res/water-shader.vs.glsl", "res/water-shader.fs.glsl");
	
	material = std::make_shared<Material>(liquidShader);
	material->HasTransparency = true;

	mesh = Mesh::MakeSubdividedPlane(size, numSections, worldUVs);
	mesh->cullFaces = false;
	CreateEntity(scene, material); // creates the entity
}

void cherry::Liquid::SetEnabledWaves(int waveCount)
{
	waveCount = (waveCount < 0) ? 0 : (waveCount > MAX_WAVES) ? MAX_WAVES : waveCount;

	material->Set("a_EnabledWaves", waveCount);
}

void cherry::Liquid::SetGravity(float gravity)
{
	material->Set("a_Gravity", abs(gravity));
}

// sets a wave
void cherry::Liquid::SetWave(unsigned int waveNum, float xDir, float yDir, float steepness, float waveLength)
{
	if (waveNum > MAX_WAVES) // capping the wave number
		waveNum = MAX_WAVES;

	material->Set("a_Waves[" + std::to_string(waveNum) + "]", {xDir, yDir, steepness, waveLength});
}

// sets the alpha of the liquid (0 - 1)
void cherry::Liquid::SetAlpha(float a)
{
	// bounds checking
	a = (a < 0.0F) ? 0.0F : (a > 1.0F) ? 1.0F : a;

	material->Set("a_WaterAlpha", a);
}

// sets the colour of the liquid using a [0, 1] rgb scale
void cherry::Liquid::SetColor(float r, float g, float b)
{
	r = (r < 0.0F) ? 0.0F : (r > 1.0F) ? 1.0F : r;
	g = (g < 0.0F) ? 0.0F : (g > 1.0F) ? 1.0F : g;
	b = (b < 0.0F) ? 0.0F : (b > 1.0F) ? 1.0F : b;

	material->Set("a_WaterColor", { r, g, b });
}

// sets the colour of the liquid using a [0, 255] range.
void cherry::Liquid::SetColor(int r, int g, int b)
{
	SetColor((float)r / 255.0F, (float)g / 255.0F, (float)b / 255.0F);
}

// sets the liquid colour on a [0, 1] rgba scale
void cherry::Liquid::SetColor(float r, float g, float b, float a)
{
	SetAlpha(a);
	SetColor(r, g, b);
}

// sets liquid colour using a [0, 255] rgba scale
void cherry::Liquid::SetColor(int r, int g, int b, float a)
{
	SetColor(r, g, b);
	SetAlpha(a);
}

// sets the clarity of the liquid on a [0, 1] range
void cherry::Liquid::SetClarity(float clarity)
{
	clarity = (clarity < 0.0F) ? 0.0F : (clarity > 1.0F) ? 1.0F : clarity;
	
	material->Set("a_WaterClarity", clarity);
}

// the higher the power the higher the reflection 
void cherry::Liquid::SetFresnelPower(float fresnelPower)
{
	fresnelPower = (fresnelPower < 0.0F) ? 0.0F : (fresnelPower > 1.0F) ? 1.0F : fresnelPower;
	
	material->Set("a_FresnelPower", fresnelPower);
}

// sets the refraction index
void cherry::Liquid::SetRefractionIndex(float medium1, float medium2)
{
	material->Set("a_RefractionIndex", abs(medium1 / medium2));
}

// sets the environment (skybox) that the liquid is part of.
void cherry::Liquid::SetEnvironment(TextureCube::Sptr skybox)
{
	material->Set("s_Environment", skybox);
}
