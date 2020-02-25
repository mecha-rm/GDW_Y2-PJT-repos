// Shader (Header) - renders meshes to the screen
#pragma once
#include <glad/glad.h>
#include <GLM/glm.hpp>
#include <memory>

#include "UtilsCRY.h"

namespace cherry
{
	class Shader {
	public:
		GraphicsClass(Shader);

		// typedef std::shared_ptr<Shader> Sptr;
		Shader();
		~Shader();
		void Compile(const char* vs_source, const char* fs_source);

		// Loads a shader program from 2 files. vsFile is the path to the vertex shader, and fsFile is the path to the fragment shader
		void Load(const char* vsFile, const char* fsFile);

		// mat3
		void SetUniform(const char* name, const glm::mat3& value);
		
		// mat4
		void SetUniform(const char* name, const glm::mat4& value);

		// vec2 (added for the engine specifically)
		void SetUniform(const char* name, const glm::vec2& value);

		// vec3
		void SetUniform(const char* name, const glm::vec3& value);
		
		// vec4
		void SetUniform(const char* name, const glm::vec4& value);
		


		// float
		void SetUniform(const char* name, const float& value);

		// int
		// sending integers to our shader; you're setting a slot, rather than a texture ID.
		void SetUniform(const char* name, const int& value);

		void Bind();

		// returns the path and name of the vertex shader file being used.
		const char* GetVertexShader() const;

		// returns the path and name of the fragment shader file being used.
		const char* GetFragmentShader() const;
	private:
		GLuint __CompileShaderPart(const char* source, GLenum type);

		GLuint myShaderHandle;

		std::string vertexShader; // vertex shader file

		std::string fragmentShader; // fragment shader file
	};
}