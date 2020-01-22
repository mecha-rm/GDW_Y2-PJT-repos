// used for creating liquids
#pragma once
#include "Object.h"

namespace cherry
{
	class Liquid : public Object
	{
	public:
		// the constructor for liquid; size and numsections must be greater than 1, otherwise they'll be set to 1.
		// this uses the water shader, with refers to 'water' being simplified to 'liquid'.
		Liquid(std::string scene, float size, float numSections, bool worldUVs = true);

		// gets the size of the liquid (length and width are the same for liquids).
		float GetSize() const;

		// gets the number of sections for the waves.
		float GetNumberOfSections() const;

		// gets the total amount of enabled waves
		int GetEnabledWaves() const;

		// sets the amount of waves the water has. Currently, the cap is 8 waves.
		// if the value exceeds MAX_WAVES, then it will be capped at MAX_WAVES.
		void SetEnabledWaves(int waveCount);

		// gets the gravity value for the
		float GetGravity() const;

		// sets the gravity for the waves; regular earth gravity is 9.8 (9.807 m/s^2 to be more percise)
		void SetGravity(float gravity);

		// gets the maximum amount of waves allowed.
		int GetMaximumWaveCount() const;

		/*
		 * sets a wave
		 * waveNum: the number of the wave being changed. See MAX_WAVES to check the maximum amount of waves available.
		 * xDir: the x-direction of the waves.
		 * yDir: the y-direction of the waves.
		 * steepness: the steepness of the wave. The sum of the steepnesses must not surpass 1.
		 * waveLength: the length of the wave.
		*/
		void SetWave(unsigned int waveNum, float xDir, float yDir, float steepness, float waveLength);

		// gets the alpha value of the wave
		float GetAlpha() const override;

		// sets the alpha value for the liquid (0 - 1)
		void SetAlpha(float a) override;

		// gets the colours as a GLM vector, which is in a [0, 1] range. It is in RGBA format.
		glm::vec4 GetColorGLM() const;

		// gets the colours as a cherry vector, which is in a [0, 1] range. It is in RGBA format.
		cherry::Vec4 GetColor() const;

		// sets the liquid colour [0,1] rgb scale
		void SetColor(float r, float g, float b);

		// sets the colour of the liquid using a [0, 255] range.
		void SetColor(int r, int g, int b);

		// sets the liquid colour on a [0, 1] rgba scale
		void SetColor(float r, float g, float b, float a);

		// sets the liquid colour on a [0, 255] rgba scale
		void SetColor(int r, int g, int b, float a);

		// returns the liquid's clarity.
		float GetClarity() const;

		// the higher the value, the higher the clarity ([0, 1] range)
		void SetClarity(float clarity);

		// gets the fresnal power
		float GetFresnalPower() const;

		// the higher the power the higher the reflection
		void SetFresnelPower(float fresnelPower);

		// gets the index of refraction
		float GetRefractionIndex();

		// index of refraction (1.0f / 1.34f for air to water)
		// medium1 / medium2
		void SetRefractionIndex(float medium1, float medium2);

		// returns the environment
		cherry::TextureCube::Sptr GetEnvironment() const;

		// sets the environment (skybox) the water part of.
		void SetEnvironment(TextureCube::Sptr skyboxTexture); // the environment (skybox) for the water.

		// update
		void Update(float deltaTime);

	private:
		float size; // size of wave
		float numSections; // number of sections

		static const int MAX_WAVES; // maximum amount of waves

		// number of enabled waves (all enabled by default)
		int enabledWaves = 8;

		float gravity = 9.807F; // wave gravity

		// save vertex count and wave count?
		// float size
		// float numSections

		cherry::Vec4 color{ 0.0F, 0.0F, 1.0F, 1.0F }; // wave colour
		
		float clarity = 1.0F; // wave clarity
		float fresnelPower = 1.0F; // reflection power
		float refractionIndex = 1.0F; // refraction (going from one medium to another)

		TextureCube::Sptr skyboxTexture; // skybox environment texture
	protected:

	};
}
