#include "Manager.h"

Manager::Manager(GLFWwindow*& window, const int& width, const int& height, const char* title, bool cursor)
{
	glfwInit();
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		exit(EXIT_FAILURE);
	}

	// Set GLFW to use the core profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a windowed mode window and its OpenGL context
	window = glfwCreateWindow(width, height, title, NULL, NULL);

	if (!window) {
		std::cerr << "Failed to open GLFW window" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Make the window's context current
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}

	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* win, int width, int height)
		{
			glViewport(0, 0, width, height);
			std::cout << "Window Resized\n";
		});

	// GLFW capture mouse
	glfwSetInputMode(window, GLFW_CURSOR, cursor ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}
