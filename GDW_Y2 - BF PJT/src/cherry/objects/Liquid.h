// used for creating liquids
#pragma once
#include "Object.h"

namespace cherry
{
	class Liquid : public Object
	{
	public:
		// the constructor for liquid
		// this uses the water shader, with refers to 'water' being simplified to 'liquid'.
		Liquid(std::string scene, float size, float numSections, bool worldUVs = true);

		// sets the amount of waves the water has.
		// if the value exceeds MAX_WAVES, then it will be capped at MAX_WAVES.
		void SetEnabledWaves(int waveCount);

		// sets the gravity for the waves; regular earth gravity is 9.8 (9.807 m/s^2 to be more percise)
		void SetGravity(float gravity);

		/*
		 * sets a wave
		 * waveNum: the number of the wave being changed. See MAX_WAVES to check the maximum amount of waves available.
		 * xDir: the x-direction of the waves.
		 * yDir: the y-direction of the waves.
		 * steepness: the steepness of the wave. The sum of the steepnesses must not surpass 1.
		 * waveLength: the length of the wave.
		*/
		void SetWave(unsigned int waveNum, float xDir, float yDir, float steepness, float waveLength);

		// sets the alpha value for the liquid (0 - 1)
		void SetAlpha(float a);

		// sets the liquid colour [0,1] rgb scale
		void SetColor(float r, float g, float b);

		// sets the colour of the liquid using a [0, 255] range.
		void SetColor(int r, int g, int b);

		// sets the liquid colour on a [0, 1] rgba scale
		void SetColor(float r, float g, float b, float a);

		// sets the liquid colour on a [0, 255] rgba scale
		void SetColor(int r, int g, int b, float a);

		// the higher the value, the higher the clarity ([0, 1] range)
		void SetClarity(float clarity);

		// the higher the power the higher the reflection
		void SetFresnelPower(float fresnelPower);

		// index of refraction (1.0f / 1.34f for air to water)
		// medium1 / medium2
		void SetRefractionIndex(float medium1, float medium2);

		// sets the environment (skybox) the water part of.
		void SetEnvironment(TextureCube::Sptr skybox); // the environment (skybox) for the water.
	private:
		static const int MAX_WAVES;



	protected:

	};
}
