// Material - used for applying materials to meshes
#pragma once
#include <GLM/glm.hpp>
#include <unordered_map>
#include <memory>
 
#include "Shader.h"
#include "textures/Texture2D.h"
#include "textures/TextureCube.h" // used for waves
#include "UtilsCRY.h"

/*
Represents settings for a shader
*/

// shader macros moved to UtilsCRY

namespace cherry
{
	class Material {
	public:
		bool HasTransparency; // states if a material has transparency
		typedef std::shared_ptr<Material> Sptr;

		// all sceneLists are opaque by default.
		Material(const Shader::Sptr& shader) :HasTransparency(false) { myShader = shader; }
		
		virtual ~Material() = default;

		const Shader::Sptr& GetShader() const { return myShader; }

		// sets the shader for the material
		void SetShader(Shader::Sptr shader) { myShader = shader; }

		// applies all uniforms to the shader.
		virtual void Apply();

		void Set(const std::string& name, const glm::mat4& value) { myMat4s[name] = value; }
		void Set(const std::string& name, const glm::vec4& value) { myVec4s[name] = value; }
		void Set(const std::string& name, const glm::vec3& value) { myVec3s[name] = value; }
		void Set(const std::string& name, const float& value) { myFloats[name] = value; }
		void Set(const std::string& name, const int& value) { myInts[name] = value; }

		// now sets sampler sceneLists per texture.
		void Set(const std::string& name, const Texture2D::Sptr& value,
			const TextureSampler::Sptr& sampler = nullptr) {
			myTextures[name] = { value, sampler };
		}

		void Set(const std::string& name, const TextureCube::Sptr& value, const TextureSampler::Sptr& sampler = nullptr) {
			myCubeMaps[name] = { value, sampler };
		}

		// loads a material from a MaterialTemplateLibrary (.mtl) file.
		// this replaces the applicable values in this material with the ones from the .mtl file
		// make sure the material has values already, since the .mtl file doesn't override every value for the Material class.
		// You can also pass the sampler the material.
		bool LoadMtl(std::string filePath, const TextureSampler::Sptr& sampler = nullptr);

		// generates a material using a .mtl file, and returns said material. Make sure to set the lighting information.
		// pass the sampler if you so choose. This also takes in a vertex shader and fragment shader
		static cherry::Material::Sptr GenerateMtl(std::string filePath, const TextureSampler::Sptr& sampler = nullptr, std::string vs = STATIC_VS, std::string fs = STATIC_FS);


	private:

		// TODO: put this function in its own dedicated file.
		// parses the line, gets the values as data type T, and stores them in a vector.
		// containsSymbol: tells the function if the string passed still contains the symbol at the start. If so, it is removed before the parsing begins.
		// *** the symbol at the start is what's used to determine what the values in a given line of a .obj are for.
		template<typename T>
		const std::vector<T> parseStringForTemplate(std::string str, bool containsSymbol = true);
		
		std::string name = ""; // name of material
		std::string textureName = ""; // file for the texture

	protected:

		struct Sampler2DInfo {
			Texture2D::Sptr Texture;
			TextureSampler::Sptr Sampler;
		};

		struct SamplerCubeInfo {
			TextureCube::Sptr Texture;
			TextureSampler::Sptr Sampler;
		};

		std::unordered_map<std::string, SamplerCubeInfo> myCubeMaps;

		Shader::Sptr myShader;

		// everything we can put into shader we put into a mat4. This is really bad for memory though.
		std::unordered_map<std::string, glm::mat4> myMat4s;
		std::unordered_map<std::string, glm::vec4> myVec4s;
		std::unordered_map<std::string, glm::vec3> myVec3s;
		std::unordered_map<std::string, glm::vec2> myVec2s;
		std::unordered_map<std::string, float> myFloats;
		std::unordered_map<std::string, int> myInts;

		std::unordered_map<std::string, Sampler2DInfo> myTextures; // changed to use the struct.
	};
}