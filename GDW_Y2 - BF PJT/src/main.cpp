/*
 * Team: BONUS FRUIT
 * Developers:
	- Kennedy Adams	(100632983)
	- Jonah Griffin	(100702748)
	- Nathan Tuck	(100708651)
	- Spencer Tester (100653129)
	- Stephane Gagnon (100694227)
	- Roderick “R.J.” Montague (100701758)
	- Ryan Burton (100707511)
 
 * Game: CODENAME-0
 * Date: 09/18/2019
 * Description: file for GDW code. More items will be added from other course materials as needed.
 * References:
	- Learn OpenGL (https://learnopengl.com/)
*/

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

int main()
{
	// Initialize GLFW
	if (glfwInit() == GLFW_FALSE) {
		std::cout << "Failed to initialize Glad" << std::endl;
		return 1;
	}
	// Create a new GLFW window
	// GLFWwindow * = (width, height, window_name, fullscreen_bool, share_between_monitors)
	GLFWwindow* window = glfwCreateWindow(300, 300, "Game Development Workshop (Year 2): BONUS FRUIT - CODENAME: ZERO", nullptr, nullptr);
	// We want GL commands to be executed for our window, so we make our window's context the current one
	glfwMakeContextCurrent(window);
	
	// Let glad know what function loader we are using (will call gl commands via glfw)
	if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0) {
		std::cout << "Failed to initialize Glad" << std::endl;
		return 2;
	}
	// Display our GPU and OpenGL version
	std::cout << glGetString(GL_RENDERER) << std::endl;
	std::cout << glGetString(GL_VERSION) << std::endl;

	// Run as long as the window is open
	while (!glfwWindowShouldClose(window)) {
		// Poll for events from windows (clicks, keypressed, closing, all that)
		glfwPollEvents();
		// Clear our screen every frame
		glClearColor(0.1f, 0.7f, 0.5f, 1.0f); // ClearColour(R, G, B, A) (greenish ~ original set)
		// glClearColor(0.9f, 0.2f, 0.1f, 1.0f); // ClearColour(R, G, B, A) 
		glClear(GL_COLOR_BUFFER_BIT);
		// Present our image to windows
		glfwSwapBuffers(window);
	}

	// std::cin.get(); // used to prevent the OpenGL window from closing immediately

	return 0;
}