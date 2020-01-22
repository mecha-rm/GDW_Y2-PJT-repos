#pragma once

#include "Object.h"

namespace cherry
{
	class Terrain : public Object
	{
	public:
		// heightMap: the file path for the height map
		// if 'worldUvs' is set to 'true', then each face of the terrain will have its own heightmap, rather than using one overall heightmap.
		// NOTE: terrains do not support lighting.
		Terrain(std::string scene, std::string heightMap, float size, float numSections, bool worldUVs = false);


		// gets the height map
		const std::string& GetHeightMap() const;

		// sets the height map; returns 'true' if switched successfully.
		bool SetHeightMap(const std::string newHeightMap);

		// gets the minimum height of the height map
		float GetMinimumHeight() const;

		// sets the minimum height of the height map
		void SetMinimumHeight(float hgtMin);

		// gets the maximum height of the heightmap.
		float GetMaximumHeight() const;

		// sets the highest point of the heightmap.
		void SetMaximumHeight(float hgtMax);

		// returns 'true' if the texture was changed successfully.
		// if the index is out of bounds, then nothing happens, and 'false' is returned.
		// if the file cannot be found, then 'false' is once again returned.
		// check TEXTURES_MAX to see the maximum amount of textures allowed.
		bool SetTexture(unsigned int index, std::string filePath);

		// Texture1
		// Texture2
		//Texture 3

		// update
		void Update(float deltaTime);

		// maximum amount of textures
		static const int TEXTURES_MAX;

	private:
		// the height map
		std::string heightMap = "";

		

		std::string textures[3]{ "", "", "" };

		// the size of the terrain
		int size = 0;

		// the number of sections the terrain has.
		int numSections = 0;

		// the minimum and maximum height of the height max.
		float heightMin = 0.0F;
		float heightMax = 1.0F;

		// the sampler used for the terrain.
		// TextureSampler::Sptr sampler;

	protected:


	};
}