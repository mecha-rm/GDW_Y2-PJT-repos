// SHADER CLASS (HEADER)
#pragma once
#include <glad/glad.h>
#include <GLM/glm.hpp>

#include <memory>

namespace cherry
{
	class Shader {
	public:
		typedef std::shared_ptr<Shader> Sptr;

		// constructor
		Shader();

		// destructor
		~Shader();

		// Compiles shaders
		void Compile(const char* vs_source, const char* fs_source);

		// Loads a shader program from 2 files. vsFile is the path to the vertex shader, and fsFile is the path to the fragment shader
		void Load(const char* vsFile, const char* fsFile);

		void SetUniform(const char* name, const glm::mat4& value);

		void Bind();

	private:
		GLuint __CompileShaderPart(const char* source, GLenum type);

		GLuint myShaderHandle;
	};
}