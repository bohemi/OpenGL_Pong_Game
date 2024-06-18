#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "AudioManager.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <functional>
#include <iostream>

class Manager
{
public:

	Manager(GLFWwindow*& window, const int& width, const int& height, const char* title, bool cursor);

	void HandleAudio(AudioManager& audio, FMOD::System* system, FMOD::Sound* sound, bool& batSound, bool& wallSound);

	void KeyEvents(GLFWwindow*& window);

	void RestartGame(glm::vec3& ballPos, glm::vec2& ballScale, glm::vec3& batPosRight,
		glm::vec2& batScaleRight, float& ballSpeedVertical, float& ballSpeedHorizontal, bool& up);

	// keys
	bool leftBatUp = false;
	bool leftBatDown = false;
	bool rightBatUp = false;
	bool rightBatDown = false;

};