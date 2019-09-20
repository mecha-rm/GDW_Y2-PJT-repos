/*
 * Author: Roderick "R.J." Montague
 * Date: 07/25/2019
 * Description: shader class for reading shaders from the disk, compiling them, linking them, and checking for errors.
	* HEADER FILE
 * Resources: learn OpenGL - Shaders/Our Own Shader Class (https://learnopengl.com/Getting-started/Shaders)
*/
#ifndef  SHADER_H
#define SHADER_H

#include "../..//external/glad/glad.h" // include glad to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	// the proper ID
	unsigned int ID;

	// constructor reads and builds the shader
	Shader(const GLchar * vertexPath, const GLchar * fragmentPath);
	
	// use/activate the shader
	void use();

	// utility uniform functions
	void setBool(const std::string &name, bool value) const;

	void setInt(const std::string &name, int value) const;

	void setFloat(const std::string &name, float value) const;

private:
	// utility function for checking shader compilation/linking errors
	void checkCompileErrors(unsigned int shader, std::string type);
};


#endif // ! SHADER_H

// Shader Use Example (DO NOT UNCOMMENT)
//Shader ourShader("path/to/shaders/shader.vs", "path/to/shaders/shader.fs");
//...
//while (...)
//{
//	ourShader.use();
//	ourShader.setFloat("someUniform", 1.0f);
//	DrawStuff();
//}

