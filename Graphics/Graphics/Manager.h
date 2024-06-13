#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <functional>
#include <iostream>

class Manager
{
public:

	Manager(GLFWwindow*& window, const int& width, const int& height, const char* title, bool cursor);
};