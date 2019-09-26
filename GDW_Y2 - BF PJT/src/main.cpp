/*
 * Team: BONUS FRUIT
 * Developers:
	- Kennedy Adams	(100632983)
	- Jonah Griffin	(100702748)
	- Nathan Tuck	(100708651)
	- Spencer Tester (100653129)
	- Stephane Gagnon (100694227)
	- Roderick "R.J." Montague (100701758)
	- Ryan Burton (100707511)
 
 * Game: CODENAME: ZERO
 * Date: 09/26/2019
 * Description: file for GDW code. More items will be added from other course materials as needed.
 * References:
	* Learn OpenGL (https://learnopengl.com/)
	* GLFW (https://www.glfw.org/)
		* Window Guide
*/

#include "engine/Game.h"
// #include <Logging.h>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

// Object object = Object("C:/Users/steph/source/repos/GDW_Y2-PJT-repos/GDW_Y2 - BF PJT/src/cube.obj");
// const unsigned int numVerts = object.vertices.size();

int main() 
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); // checks for memory leaks once the program ends.
	long long allocPoint = 0;
	if (allocPoint)
	{
		_CrtSetBreakAlloc(allocPoint); // sets where you want to stop our program by assigning the allocation block index stopping point.
	}

	// stbi_image_free(data);
	// glfwSetWindowSize(window, 1000, 1000);

	// Logger::Init();

	Game* game = new Game();
	game->Run();
	delete game;

	// Logger::Uninitialize();

	return 0;
}

//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//#include "Engine/Shader.h"
//#include "Engine/stb_image.h"
//#include "Engine/Physics.h"
//#include <iostream>
//
//const unsigned int SCR_WIDTH = 800;
//const unsigned int SCR_HEIGHT = 600;
//
//const char* vertexShaderSource = "#version 420 core\n"
//"layout (location = 0) in vec3 aPos;\n"
//"layout (location = 1) in vec3 aColor;\n"
//"out vec3 ourColor;\n"
//"void main()\n"
//"{\n"
//"   gl_Position = vec4(aPos, 1.0);\n"
//"	ourColor = aColor;\n" // set ourColor to the input color we got from the vertex data
//"}\0";
//
//const char* fragmentShaderSource = "#version 420 core\n"
//"out vec4 FragColor;\n"
//"in vec3 ourColor;\n"
//"void main()\n"
//"{\n"
//"   FragColor = vec4(ourColor, 1.0f);\n" /*Colour of the Shape (R,G,B,A) */
//"}\n\0";
//
//// called when the window is resized so that the viewpoint also gets resized.
//// takes the GLFWwindow, its new width, and its new height.
//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//
//// used for processing user input in reference to the window
//void processInput(GLFWwindow*);
//
//// called when the window gets resized.
//void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
//	glViewport(0, 0, width, height); // creating the new window.
//}
//
//// processes user input in accordance with the game window.
//void processInput(GLFWwindow* window) {
//	// the window will close when the 'escape' key is pressed.
//	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//		glfwSetWindowShouldClose(window, true);
//}
//
//Object object = Object("C:/Users/steph/source/repos/GDW_Y2-PJT-repos/GDW_Y2 - BF PJT/src/cube.obj");
//const unsigned int numVerts = object.vertices.size();

//int main() {
//	// Initialize GLFW
//	if (glfwInit() == GLFW_FALSE) {
//		std::cout << "Failed to initialize Glad" << std::endl;
//		return 1;
//	}
//
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//#ifdef __APPLE__
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // required for MAC OS X
//#endif // __APPLE__
//
//	// Create a new GLFW window
//	// GLFWwindow * = (width, height, window_name, fullscreen_bool, share_between_monitors)
//	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Game Development Workshop (Year 2): BONUS FRUIT - CODENAME: ZERO", NULL, NULL);
//	if (window == NULL) {
//		std::cout << "Failed to create GLFW window." << std::endl;
//		glfwTerminate();
//		return -1;
//	}
//
//	// We want GL commands to be executed for our window, so we make our window's context the current one
//	glfwMakeContextCurrent(window);
//
//	// Let glad know what function loader we are using (will call gl commands via glfw)
//	if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0) {
//		std::cout << "Failed to initialize Glad" << std::endl;
//		return 2;
//	}
//	// Display our GPU and OpenGL version
//	std::cout << glGetString(GL_RENDERER) << std::endl;
//	std::cout << glGetString(GL_VERSION) << std::endl;
//
//	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
//
//	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//
//	Shader ourShader("3.3.shader.vs", "3.3.shader.fs");
//
//	// Textures - Vertices (4 X 4) - additional changes need to be madet to tell OpenGL about the extra information.
//	float vertices[126];
//	//= {
//	//	// positions		// colours			  // texture coords
//	//	 0.6f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
//	//	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
//	//	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
//	//	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
//	//};
//
//	for (int i = 0; i < object.vertices.size(); i += 8) {
//		vertices[i] = object.vertices[i].getX();
//		vertices[i + 1] = object.vertices[i].getY();
//		vertices[i + 2] = object.vertices[i].getZ();
//	}
//
//	// Controls what vertices to draw
//	unsigned int indices[18];
//	//	= {
//	//	0, 1, 3, // first triangle
//	//	1, 2, 3 // second triangle
//	//};
//
//	for (int i = 0; i < object.vertexIndices.size(); i++) {
//		indices[i] = object.vertexIndices[i];
//	}
//
//	// texture
//	float texCoords[] = {
//	0.0f, 0.0f, // lower-left corner
//	1.0f, 0.0f, // lower-right corner
//	0.5f, 1.0f // top-center corner
//	};
//
//	float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f }; // border colour
//	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//	// Compiling the Shader
//	unsigned int vertexShader; // gets shader ID
//	vertexShader = glCreateShader(GL_VERTEX_SHADER); // type of shader
//
//	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
//	glCompileShader(vertexShader);
//
//	int success; // checking for a successful shader
//	char infoLog[512];
//	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
//
//	if (!success) {
//		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
//		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
//	}
//
//	// Fragment Shader
//	unsigned int fragmentShader;
//
//	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
//	glCompileShader(fragmentShader);
//
//	// check for shader compile errors
//	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
//	if (!success) {
//		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
//		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
//	}
//
//	// Shader Program ~ link shaders
//	unsigned int shaderProgram;
//	shaderProgram = glCreateProgram(); // creates a program and returns an ID reference to newly created program object.
//
//	// attaching the shaders to the program
//	glAttachShader(shaderProgram, vertexShader);
//	glAttachShader(shaderProgram, fragmentShader);
//	glLinkProgram(shaderProgram);
//
//	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
//	if (!success) {
//		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
//		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
//	}
//
//	glUseProgram(shaderProgram); // uses a program
//	glDeleteShader(vertexShader); // deletes shader
//	glDeleteShader(fragmentShader); // deletes shader
//
//	glUseProgram(shaderProgram);
//	// 3. now draw the object
//
//	unsigned int VBO;
//	glGenBuffers(1, &VBO);
//
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//
//	// generating a Vertex Array Object
//	unsigned int VAO;
//	glGenVertexArrays(1, &VAO);
//
//
//	// SIZE AND RATIO
//	// We bind the VAO with the preferred settings before drawing the object.
//	// ...:: Initialization code (done once (unless your object frequently changes)) :: ..
//	// 1. bind Vertex Array Object
//	glBindVertexArray(VAO);
//
//	// 2. copy our vertices array in a buffer for OpenGl to use
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//	// 3. then set our vertex attributes pointers
//	// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // NON-RAINBOW
//	// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // RAINBOW
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(0);
//
//	// color attribute
//	// glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // RAINBOW
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
//	glEnableVertexAttribArray(1);
//
//	// Textures ~ Notifying OpenGl of the new vertex format
//	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
//	glEnableVertexAttribArray(2);
//
//	// [...]
//
//	// Element Buffer
//	unsigned int EBO;
//	glGenBuffers(1, &EBO);
//
//	// Buffer Binding
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//
//	//// setting the vertex attributes 
//	//// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//	//glEnableVertexAttribArray(0);
//
//	// [...]
//	int nrAttributes;
//	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
//	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
//
//	// texture generation process
//	unsigned int texture;
//	glGenTextures(1, &texture);
//	// binding the objects so any subsequent texture commands will configure the currently bound texture.
//	glBindTexture(GL_TEXTURE_2D, texture);
//	// set the texture wrapping/filtering options (on the currently bound texture object)
//	// texture wrapping parameters
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	// texture filtering parameters
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//	// TEXTURE CREATION ~ load and generate the texture (mipmaps)
//	int width, height, nrChannels;
//	// takes in the image file, its width, its height, and the number of color channels.
//	unsigned char* data = stbi_load("./resources/textures/container.jpg", &width, &height, &nrChannels, 0);
//
//	if (data) {
//		// generating the texture using the previously loaded image data.
//		/*
//		 * Arg1: texture target
//		 * Arg2: mipmap level we want to set a texture for. Base level is 0, and if we wanted more, we'd set it manually or use glGenerateMipmap after texture generation.
//		 * Arg3: storage format (example image only uses RGB values)
//		 * Arg4/Arg5: width/height
//		 * Arg6: "legacy stuff" (always should be 0)
//		 * Arg7: format (image is RGB, so that's the format used)
//		 * Arg8: data type (char bytes)
//		 * Arg9: actual image data
//		*/
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
//		glGenerateMipmap(GL_TEXTURE_2D);
//	}
//	else {
//		std::cout << "Failed to load texture" << std::endl;
//	}
//
//	stbi_image_free(data);
//
//	// glfwSetWindowSize(window, 1000, 1000);
//	// Run as long as the window is open
//	while (!glfwWindowShouldClose(window)) {
//		processInput(window); // input: checks for user input every iteration.
//
//		// render commands
//		glClearColor(0.2F, 0.3F, 0.3F, 1.0F); // this will configure the colour that's used to clear the window
//		glClear(GL_COLOR_BUFFER_BIT); // (RGBA) clears the buffer to get rid of any artifacts from the previous iteration
//
//
//		// ..::Drawing code (in render loop)
//		// 4. draw the object (drawing code)
//		// glUseProgram(shaderProgram);
//
//		// Added from Shaders step
//		// float timeValue = glfwGetTime();
//		// float greenValue = (sin(timeValue) / 2.0F) + 0.5F;
//		// int vertexColourLocation = glGetUniformLocation(shaderProgram, "ourColor");
//		// glUseProgram(shaderProgram);
//
//		// ourShader.use();
//		// glUniform4f(vertexColourLocation, 0.0F, greenValue, 0.0F, 1.0F);
//		// std::cout << greenValue << std::endl;
//
//		glBindTexture(GL_TEXTURE_2D, texture); // added texture
//		ourShader.use();
//		glBindVertexArray(VAO);
//		// glDrawElements brought back in for Texturing
//		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // (mode, number of elments (vertices), indice type, and offset)
//		glDrawArrays(GL_TRIANGLES, 0, 3); // used for triangles
//		// glBindVertexArray(0);
//		// drawTriangle();
//
//		// swaps the buffers so that the rendered graphics are only displayed when they're complete, and done so instantaneously.
//		glfwSwapBuffers(window); // swaps the colour buffer (a buffer that contains colour values for each pixel in a GLFW window) that has been used to draw for this interation.
//		glfwPollEvents(); // checks for triggered events (keyboard, mouse, etc.), and calls the desigated callback methods
//	}
//
//	// de-allocate all resources once they're no longer needed.
//	glDeleteVertexArrays(1, &VAO);
//	glDeleteBuffers(1, &VBO);
//	// glDeleteBuffers(1, &EBO)
//
//	glfwTerminate();
//	return 0;
//}