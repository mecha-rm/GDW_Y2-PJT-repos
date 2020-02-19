// Shader (Source) - renders meshes to the screen
#include "Shader.h"
#include <toolkit/Logging.h>
#include <fstream>

// Reads the entire contents of a file
char* readFile(const char* filename) {
	// Declare and open the file stream
	std::ifstream file;
	file.open(filename, std::ios::binary);

	// Only read if the file is open
	if (file.is_open()) {
		// Get the starting location in the file
		uint64_t fileSize = file.tellg();

		// Seek to the end
		file.seekg(0, std::ios::end);
		// Calculate the file size from end to beginning
		fileSize = (uint64_t)file.tellg() - fileSize;
		// Seek back to the beginning of the file
		file.seekg(0, std::ios::beg);

		// Allocate space for our entire file, +1 byte at the end for null terminator
		char* result = new char[fileSize + 1];

		// Read the entire file to our memory
		file.read(result, fileSize);

		// Make our text null-terminated
		result[fileSize] = '\0';

		// Close the file before returning
		file.close();
		return result;
	}
	// Otherwise, we failed to open our file, throw a runtime error
	else {
		throw std::runtime_error("We cannot open the file!");
	}
}

// constructor
cherry::Shader::Shader() { myShaderHandle = glCreateProgram(); }

// destructor
cherry::Shader::~Shader()  { glDeleteProgram(myShaderHandle); }

// loads the shaders
void cherry::Shader::Load(const char* vsFile, const char* fsFile)
{
	// Load in our shaders (read the source)
	char* vs_source = readFile(vsFile);
	char* fs_source = readFile(fsFile);

	// Compile our program
	Compile(vs_source, fs_source);

	// saving the shader file paths
	vertexShader = std::string(vsFile);
	fragmentShader = std::string(fsFile);

	// Clean up our memory
	delete[] fs_source;
	delete[] vs_source;
}

// mat3 ver.
void cherry::Shader::SetUniform(const char* name, const glm::mat3& value) {
	GLint loc = glGetUniformLocation(myShaderHandle, name);
	if (loc != -1) {
		glProgramUniformMatrix3fv(myShaderHandle, loc, 1, false, &value[0][0]);
	}
}

// mat4 ver.
// gets the first float in the array so that OpenGl knows to grab the values, and where to do so.
void cherry::Shader::SetUniform(const char* name, const glm::mat4& value) {
	GLint loc = glGetUniformLocation(myShaderHandle, name);
	if (loc != -1) {
		glProgramUniformMatrix4fv(myShaderHandle, loc, 1, false, &value[0][0]);
	}
}

// vec2 (added for the engine specifically) 
void cherry::Shader::SetUniform(const char* name, const glm::vec2& value)
{
	GLint loc = glGetUniformLocation(myShaderHandle, name);
	if (loc != -1) {
		glProgramUniform2fv(myShaderHandle, loc, 1, &value[0]);
	}
}

// vec3 ver.
void cherry::Shader::SetUniform(const char* name, const glm::vec3& value) {
	GLint loc = glGetUniformLocation(myShaderHandle, name);
	if (loc != -1) {
		glProgramUniform3fv(myShaderHandle, loc, 1, &value[0]);
	}
}

// vec4 ver.
void cherry::Shader::SetUniform(const char* name, const glm::vec4& value)
{
	GLint loc = glGetUniformLocation(myShaderHandle, name);
	if (loc != -1) {
		glProgramUniform4fv(myShaderHandle, loc, 1, &value[0]);
	}
}

// float ver.
void cherry::Shader::SetUniform(const char* name, const float& value) {
	GLint loc = glGetUniformLocation(myShaderHandle, name);
	if (loc != -1) {
		glProgramUniform1fv(myShaderHandle, loc, 1, &value);
	}
}

// int ver.
void cherry::Shader::SetUniform(const char* name, const int& value) {
	GLint loc = glGetUniformLocation(myShaderHandle, name);
	if (loc != -1) {
		glProgramUniform1iv(myShaderHandle, loc, 1, &value);
	}
}

// uses glUseProgram to use the shader for drawing.
void cherry::Shader::Bind()
{
	glUseProgram(myShaderHandle);
}

// gets the vertex shader.
const char* cherry::Shader::GetVertexShader() const { return vertexShader.c_str(); }

// returns the file name for the fragment shader
const char* cherry::Shader::GetFragmentShader() const { return fragmentShader.c_str(); }

// compiling the bits of our shader and checking for errors.
GLuint cherry::Shader::__CompileShaderPart(const char* source, GLenum type) {
	GLuint result = glCreateShader(type);

	// Load in our shader source and compile it
	glShaderSource(result, 1, &source, NULL);
	glCompileShader(result);

	// Check our compile status
	GLint compileStatus = 0;
	glGetShaderiv(result, GL_COMPILE_STATUS, &compileStatus); // GL_COMPILE_STATUS returns (1) if it's good, (0) if there's a problem.

	// If we failed to compile
	if (compileStatus == GL_FALSE) {
		// Get the size of the error log
		GLint logSize = 0;

		glGetShaderiv(result, GL_INFO_LOG_LENGTH, &logSize);
		// Create a new character buffer for the log
		char* log = new char[logSize];
		// Get the log
		glGetShaderInfoLog(result, logSize, &logSize, log);

		// Dump error log
		LOG_ERROR("Failed to compile shader part:\n{}", log);

		// Clean up our log memory
		delete[] log;

		// Delete the broken shader result
		glDeleteShader(result);

		// Throw a runtime exception
		throw std::runtime_error("Failed to compile shader part!");
	}
	else {
		LOG_TRACE("Shader part has been compiled!");
	}
	// Return the compiled shader part
	return result;
}

// compiling the shader itself.
void cherry::Shader::Compile(const char* vs_source, const char* fs_source) {
	// Compile our two shader programs
	GLuint vs = __CompileShaderPart(vs_source, GL_VERTEX_SHADER);
	GLuint fs = __CompileShaderPart(fs_source, GL_FRAGMENT_SHADER);

	// Attach our two shaders
	glAttachShader(myShaderHandle, vs);
	glAttachShader(myShaderHandle, fs);

	// Perform linking
	glLinkProgram(myShaderHandle);

	// Remove shader parts to save space
	glDetachShader(myShaderHandle, vs);
	glDeleteShader(vs);
	glDetachShader(myShaderHandle, fs);
	glDeleteShader(fs);

	// Get whether the link was successful
	GLint success = 0;
	glGetProgramiv(myShaderHandle, GL_LINK_STATUS, &success);
	// If not, we need to grab the log and throw an exception
	if (success == GL_FALSE) {
		// Get the length of the log
		GLint length = 0;
		glGetProgramiv(myShaderHandle, GL_INFO_LOG_LENGTH, &length);
		if (length > 0) {
			// Read the log from OpenGL
			char* log = new char[length];
			glGetProgramInfoLog(myShaderHandle, length, &length, log);
			LOG_ERROR("Shader failed to link:\n{}", log);
			delete[] log;
		}
		else {
			LOG_ERROR("Shader failed to link for an unknown reason!");
		}
		// Delete the partial program
		glDeleteProgram(myShaderHandle);

		// Throw a runtime exception
		throw std::runtime_error("Failed to link shader program!");
	}
	else {
		LOG_TRACE("Shader has been linked");
	}
}