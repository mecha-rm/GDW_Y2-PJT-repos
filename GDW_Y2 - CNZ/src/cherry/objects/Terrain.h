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
		bool SetTexture(int index, std::string filePath);

		// sets Texture 0; returns 'true' if successful.
		bool SetTexture0(std::string filePath);

		// Texture 1; returns 'true' if successful.
		bool SetTexture1(std::string filePath);

		// Texture 2; returns 'true' if successful.
		bool SetTexture2(std::string filePath);

		// gets the texture path. 
		// The index is clamped, so an index that's less than 0, returns index 0, and an index greater than 32 returns 2.
		std::string GetTextureFilePath(int index);

		// gets texture 0 file path.
		std::string GetTexture0FilePath() const;

		// gets texture 1 file path.
		std::string GetTexture1FilePath() const;

		// gets texture 2 file path.
		std::string GetTexture2FilePath() const;

		// gets the texture weight at the given index.
		// the index value is clamped.
		float GetTextureWeight(int index) const;

		// sets the weight of a given texture. The weight can only be set between 0 and 1.
		// the index is clamped so that it stays within the bounds.
		// if the total amount of weights exceeds 1, the weights are set as a percentage of the total weight.
		void SetTextureWeight(int index, float amnt);

		// returns weight 0.
		float GetTextureWeight0() const;

		// sets weight 0 for texture 0.
		void SetTextureWeight0(float amnt);

		// returns weight 1.
		float GetTextureWeight1() const;

		// sets weight 1 for texture 1.
		void SetTextureWeight1(float amnt);

		// returns weight 2.
		float GetTextureWeight2() const;

		// sets weight 2 for texture 2.
		void SetTextureWeight2(float amnt);


		// update
		void Update(float deltaTime);

		// maximum amount of textures
		static const int TEXTURES_MAX;

	private:
		// the height map
		std::string heightMap = "";
		
		// the three textures.
		std::string textures[3]{ "", "", "" };

		// the size of the terrain
		int size = 0;

		// the number of sections the terrain has.
		int numSections = 0;

		// the minimum and maximum height of the height max.
		float heightMin = 0.0F, heightMax = 1.0F;

		// thE default weights for the textures.
		float weights[3] { 0.3F, 0.3F, 0.4F };

	protected:


	};
}