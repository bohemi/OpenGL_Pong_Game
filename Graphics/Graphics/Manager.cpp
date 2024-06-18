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
		std::cout << "Failed to open GLFW window\n";
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Make the window's context current
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW\n";
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

void Manager::HandleAudio(AudioManager& audio, FMOD::System* system, FMOD::Sound* sound, bool& batSound, bool& wallSound)
{
	if (batSound)
	{
		audio.PlaySound("MyFolder/Audio/BatHit.wav", system, sound, FMOD_LOOP_OFF);
		batSound = false;
	}
	else if (wallSound)
	{
		audio.PlaySound("MyFolder/Audio/WallHit.ogg", system, sound, FMOD_LOOP_OFF);
		wallSound = false;
	}
}

void Manager::RestartGame(glm::vec3& ballPos, glm::vec2& ballScale, glm::vec3& batPosRight,
	glm::vec2& batScaleRight, float& ballSpeedVertical, float& ballSpeedHorizontal, bool& up)
{
	if (ballPos.x > 100.0f || ballPos.x < 0.0f)
	{
		ballPos = glm::vec3(50.0f, 50.0f, 0.0f);
		ballSpeedVertical = 0.0f;
		ballSpeedHorizontal = 0.7f;
		up = true;
	}
	// after restart ball will gain speed after hitting right bat
	if (ballPos.x + ballScale.x / 2.0f >= batPosRight.x - batScaleRight.x / 2.0f && up)
	{
		ballSpeedVertical = 0.7f;
		up = false;
	}
}

void Manager::KeyEvents(GLFWwindow *&window)
{
	//Quit
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// press
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		leftBatUp = true;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		leftBatDown = true;

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		rightBatUp = true;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		rightBatDown = true;

	// release
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE)
		leftBatUp = false;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE)
		leftBatDown = false;

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE)
		rightBatUp = false;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE)
		rightBatDown = false;
}
